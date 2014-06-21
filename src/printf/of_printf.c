/*
 * Core printf()-like functionality
 * 
 * Almost completely rewritten by Roman Dobrodiy (C) 2014
 * 
 * This code is based on a file that contains the following:

 Copyright (C) 2002 Michael Ringgaard. All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 
 1. Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
 3. Neither the name of the project nor the names of its contributors
	may be used to endorse or promote products derived from this software
	without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.
*/

#include <stdarg.h>
#include <stddef.h>
#include <alloca.h>
#include <stdint.h>

#include <printf/of_printf.h>
#include <printf/fp_parse.h>

/* Flags that affect output */

#define ZEROPAD		(1<<0)	/* Pad with zero on left */
#define SIGNED		(1<<1)	/* Unsigned/signed long */
#define SIGN_PLUS	(1<<2)	/* When num > 0 show '+' in place of sign */
#define SIGN_SPACE	(1<<3)	/* When num > 0 show ' ' in place of sign */
#define UPPERCASE	(1<<5)	/* 'ABCDEF' */
#define BASE_PREP	(1<<6)	/* Prepend with '0x', '0b', '0' */

#define is_digit(c) ((c) >= '0' && (c) <= '9')
#define MIN(a,b) ((a)<(b) ? (a) : (b))
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define ABS(x) (x < 0 ? -x : x)

static char *lower_digits = "0123456789abcdef";
static char *upper_digits = "0123456789ABCDEF";

/*
 * skip_atoi()
 *
 * Read unsigned from string, advancing pointer to string
 */

static int skip_atoi(const char **s)
{
	int i = 0;
	while (is_digit(**s)) i = i*10 + *((*s)++) - '0';
	
	return i;
}

/*
 * strnlen()
 *
 * Naive implementation of classic strnlen()
 */

static size_t strnlen(const char *s, size_t count)
{
	const char *sc;
	for (sc = s; *sc != '\0' && count--; ++sc);
	return sc - s;
}

/*
 * print_padded()
 *
 * Outputs buffer with padding up to the field_width (spaces or zeros),
 *  depending on the supplied flags argument
 *
 * -Space padding: inserted in front, or (TODO) after supplied buffer
 * -Zero padding: inserted character pointed by zeroinsp
 *
 * NOTE: zeroinsp must point to the character in buffer or right behind
 *       the last character of buffer
 */
static void print_padded(outpf_ptr outpf, void *outpf_arg,
			 char *buf, char *zeroinsp, int len,
			 int flags, int field_width)
{
	if(len < 0)
		len = strnlen(buf, (size_t) -1);

	field_width-= len;
	if(field_width > 0) {
		char *padb, *padp, padc;
		padb = alloca(field_width);
		padc = (flags & ZEROPAD) ? '0' : ' ';

		padp = padb + field_width - 1;
		while(padp >= padb)
			*padp-- = padc;

		if(flags & ZEROPAD) {
			/* Zero padding */
			int pplen = zeroinsp - buf;
			if(pplen > 0)
				outpf(outpf_arg, buf, zeroinsp - buf);
			outpf(outpf_arg, padb, field_width);
			if(len - pplen > 0)
				outpf(outpf_arg, zeroinsp, len - pplen);

		} else {
			/* Space padding */
			outpf(outpf_arg, padb, field_width);
			outpf(outpf_arg, buf, len);
		}
	} else {
		outpf(outpf_arg, buf, len);
	}
}

/*
 * print_number()
 *
 * Prints number using given outpf
 *
 * -base: base that number should be displayed in (allowed: 2, 8, 10, 16)
 * -type: type qualifier for supplied number
 * -flags: output modifiers, as usual
 * -field_width: field width
 * -precision: minimal number of digits that must be printed
 * -arg: va_list to read number from (oh, shi..)
 */

#define INT_T_INT	0
#define	INT_T_CHAR	1
#define	INT_T_SHORT	2
#define	INT_T_LONG	3
#define	INT_T_LONGLONG	4
#define	INT_T_SIZE	5
#define	INT_T_PTR	6

#define INT_DIGIT_CNT(type) {(sizeof(type)), /* sizeof  */\
	(sizeof(type)*8),		     /* base  2 */\
	((sizeof(type)*8 + 2)/3),	     /* base  8 */\
	((sizeof(type)*8 + 2.32)/3.32),	     /* base 10 */\
	(sizeof(type)*2)}		     /* base 16 */

static const int int_digit_cnt[][5] = {
	[INT_T_INT] = INT_DIGIT_CNT(int),
	[INT_T_CHAR] = INT_DIGIT_CNT(char),
	[INT_T_SHORT] = INT_DIGIT_CNT(short),
	[INT_T_LONG] = INT_DIGIT_CNT(long),
	[INT_T_LONGLONG] = INT_DIGIT_CNT(long long),
	[INT_T_SIZE] = INT_DIGIT_CNT(size_t),
	[INT_T_PTR] = INT_DIGIT_CNT(uintptr_t)
};

#define INT_DIGIT_EXTRACT(num, buf, base, dig){\
	if ((num) == 0) {\
	*(buf)-- = '0';\
	} else {\
	while ((num) != 0) {\
	*(buf)-- = (dig)[(num) % (base)];\
	(num)/= (base);\
	}}}

static void print_number(outpf_ptr outpf, void *outpf_arg,
			 int base, int type,
			 int flags, int field_width, int precision,
			 va_list *arg)
{
	char sign;
	char *buf, *buf_end, *buf_zp;
	char *dig;
	size_t buf_size;

	switch(base) {
	/* Here we account either for sign or for base specifier placeholders */
	case 2:
		buf_size = int_digit_cnt[type][1] + 2;
		break;
	case 8:
		buf_size = int_digit_cnt[type][2] + 2;
		break;
	case 10:
		buf_size = int_digit_cnt[type][3] + 1;
		break;
	case 16:
		buf_size = int_digit_cnt[type][4] + 2;
		break;
	default:
		return;
	}
	buf_size = MAX((int) buf_size, precision + 2);
	buf = alloca(buf_size);
	buf = buf_end = buf + buf_size - 1;

	/* Sign makes difference only for base-10 output */
	if(base != 10)
		flags &= ~SIGNED;
	sign = 0;
	if (flags & SIGNED) {
		if (flags & SIGN_SPACE) {
			sign = ' ';
		} else if (flags & SIGN_PLUS) {
			sign = '+';
		}
	}
	/* Extract digits from number */
	if (flags & UPPERCASE)
		dig = upper_digits;
	else
		dig = lower_digits;

	switch(type) {
	case INT_T_INT:
	case INT_T_CHAR:
	case INT_T_SHORT:
		{
			/* int or smaller: handle as int with proper conversion */
			unsigned num = va_arg(*arg, unsigned);
			switch(type) {
			case 1:
				num = (flags & SIGNED) ?
					      (char) num : (unsigned char) num;
				break;
			case 2:
				num = (flags & SIGNED) ?
					      (short) num : (unsigned short) num;
				break;
			}
			if(flags & SIGNED && (int) num < 0) {
				sign = '-';
				num = -(int) num;
			}
			INT_DIGIT_EXTRACT(num, buf, base, dig);
		}
		break;
	case INT_T_LONG:
		{
			unsigned long num = va_arg(*arg, unsigned long);
			if(flags & SIGNED && (long) num < 0) {
				sign = '-';
				num = -(long) num;
			}
			INT_DIGIT_EXTRACT(num, buf, base, dig);
		}
		break;
	case INT_T_LONGLONG:
		{
			unsigned long long num = va_arg(*arg,
							unsigned long long);
			if(flags & SIGNED && (long long) num < 0) {
				sign = '-';
				num = -(long long) num;
			}
			INT_DIGIT_EXTRACT(num, buf, base, dig);
		}
		break;
	case INT_T_SIZE:
		{
			size_t num = va_arg(*arg, size_t);
			INT_DIGIT_EXTRACT(num, buf, base, dig);
		}
		break;
	case INT_T_PTR:
		{
			uintptr_t num = va_arg(*arg, uintptr_t);
			INT_DIGIT_EXTRACT(num, buf, base, dig);
		}
		break;
	default:
		outpf(outpf_arg, "#TYPE%", -1);
		return;
	}

	buf_zp = buf;

	/* Pad for precision now.. */
	while((buf_end - buf) < precision)
		*buf-- = '0';

	/* Insert sign */
	if (sign)
		*buf-- = sign;

	/* Insert base specifier */
	if (flags & BASE_PREP)
	{
		switch(base)
		{
		case 2:
			*buf-- = 'b';
			*buf-- = '0';
			break;
		case 16:
			*buf-- = 'x';
			*buf-- = '0';
			break;
		case 8:
			*buf-- = 'o';
			*buf-- = '0';
			break;
		}
	}

	print_padded(outpf, outpf_arg, buf+1, buf_zp+1, buf_end - buf,
		     flags, field_width);
}

/*
 * No alignment supported, only space padding to field_width
 */
static void print_char(outpf_ptr outpf, void *outpf_arg,
		       int ch, int field_width)
{
	char *buf,  *buf_end;
	size_t buf_size = MAX(field_width, 1);

	buf = alloca(buf_size);
	buf = buf_end = buf + buf_size - 1;

	*buf-- = (char) ch;

	/* Space padding */
	while((buf_end - buf) < field_width)
		*buf-- = ' ';

	outpf(outpf_arg, buf+1, buf_end - buf);
}

/*
 * No alignment supported, padding and length limiting supported
 * NOTE: if padding is required, it is done via separate output function call
 */
static void print_str(outpf_ptr outpf, void *outpf_arg,
		      const char *str, int field_width, int precision)
{
	if(field_width > 0) {
		size_t strl = strnlen(str, precision);
		if((int) strl < field_width) {
			size_t bufsize = field_width - strl;
			char *buf = alloca(bufsize);

			for(unsigned i=0; i < bufsize; ++i)
				buf[i] = ' ';

			outpf(outpf_arg, buf, bufsize);
		}
	}
	outpf(outpf_arg, str, precision);
}

#define FLT_T_FLOAT 0
#define FLT_T_DOUBLE 1

static void print_flt_e(outpf_ptr outpf, void *outpf_arg,
			double val, int type,
			int flags, int field_width, int precision)
{
	int exp;
	int sign;
	fp_specialval_t special;

	char csign;

	char *buf_head, *buf_tail, *buf_zip;
	size_t buf_len;

	if(precision < 0)
		precision = 6;

	/* Allocate buffer on stack with enough space */
	buf_len = precision + 8;
	buf_head = alloca(buf_len);

	/* Reserve space for sign and point */
	buf_head+=2;

	/* Format float into decimal representation using correct type */
	switch(type) {
	case FLT_T_FLOAT:
			buf_head = fp_parse_float(val, buf_head, &sign, &exp,
						  &special, precision + 1);
		break;
	case FLT_T_DOUBLE:
			buf_head = fp_parse_double(val, buf_head, &sign, &exp,
						   &special, precision + 1);
		break;
	}

	buf_tail = buf_head + precision + 1;
	/* Choose sign character */
	csign = 0;
	if(sign)
		csign = '-';
	else if(flags & SIGN_SPACE)
		csign = ' ';
	else if(flags & SIGN_PLUS)
		csign = '+';

	if(FP_IS_SPECIAL(special)) {
		flags &= ~ZEROPAD;
		if(csign)
			*(--buf_head) = csign;
		print_padded(outpf, outpf_arg, buf_head, buf_head + 1,
			     buf_tail - buf_head, flags, field_width);
		return;
	}

	/* Put decimal point after first digit of mantissa */
	*(buf_head - 1) = *buf_head;
	*buf_head-- = '.';
	buf_zip = buf_head;
	if(csign)
		*(--buf_head) = csign;

	/* Correct exponent (we print mantissa in 1.23 form, not .123) */
	/* If number is zero, exponent should be zero, right? */
	if(special != FPS_ZERO)
		exp--;

	/* Print 'E' or 'e' */
	*buf_tail++ = flags & UPPERCASE ? 'E' : 'e';

	/* Print exponent sign */
	csign = '+';
	if(exp < 0) {
		csign = '-';
		exp = -exp;
	}
	*buf_tail++ = csign;

	/* Exponent can't be greater than 1000 anyway (by absolute value) */
	int t = 100;
	while(t) {
		*buf_tail++ = exp / t + '0';
		exp%= t;
		t/= 10;
	}

	print_padded(outpf, outpf_arg, buf_head, buf_zip, buf_tail - buf_head,
		     flags, field_width);
}

void of_vprintf(outpf_ptr outpf, void *outpf_arg, const char *fmt, va_list args)
{
	int base;
	char *str;

	int flags;

	int field_width;
	int precision;
	int sz_qualifier;

	const char *fmt_start;

	/*
	 * Note on size qualifier:
	 * 0 means "default size" (int)
	 * >0 means "larger size" (1 for long, 2 for long long)
	 * <0 means "smaller size" (-1 for short, -2 for char)
	 */

	/* Cycle... */
	while(*fmt != '\0') {
		fmt_start = fmt;
		/* Iterate up to % or end of format string */
		while(*fmt != '\0' && *fmt != '%')
			fmt++;

		/* Output part of format string between formatters */
		if(fmt - fmt_start)
			outpf(outpf_arg, fmt_start, fmt - fmt_start);

		/* Process further only if fmt points to formatter */
		if(*fmt != '%')
			continue;

		flags = 0;
		field_width = -1;
		precision = -1;
		sz_qualifier = 0;
		base = 10;

		/* Read flags */
		while(1) {
			++fmt;	/* This also skips first '%' */
			switch (*fmt)
			{
			case '+':
				flags |= SIGN_PLUS;
				continue;
			case ' ':
				flags |= SIGN_SPACE;
				continue;
			case '#':
				flags |= BASE_PREP;
				continue;
			case '0':
				flags |= ZEROPAD;
				continue;
			}
			break;
		}

		/* Get field width */
		if (is_digit(*fmt)) {
			field_width = skip_atoi(&fmt);
		} else if (*fmt == '*') {
			++fmt;
			field_width = va_arg(args, int);
			field_width = MAX(field_width, 0);
		}

		/* Get precision */
		if (*fmt == '.') {
			++fmt;
			if (is_digit(*fmt)) {
				precision = skip_atoi(&fmt);
			} else if (*fmt == '*') {
				++fmt;
				precision = va_arg(args, int);
				precision = MAX(precision, 0);
			}
		}

		/* Get size qualifier */
		while(*fmt == 'l' || *fmt == 'h') {
			if(*fmt == 'l')
				sz_qualifier++;
			else
				sz_qualifier--;
			++fmt;
		}

		/* Parse type specifier itself.. */
		switch (*fmt)
		{
		/* If by accident it was incomplete formatter at the end of string.. */
		case '\0':
			continue;

		case 'c':
			++fmt;
			print_char(outpf, outpf_arg, va_arg(args, int), field_width);
			continue;

		case '%':
			++fmt;
			outpf(outpf_arg, "%", -1);
			continue;

		case 's':
			++fmt;
			str = va_arg(args, char *);
			if (!str) {
				str = "<NULL>";
				precision = 0;
			}
			print_str(outpf, outpf_arg, str, field_width, precision);
			continue;

		case 'p':
			++fmt;
			if (precision == -1)
				precision = 2 * sizeof(void *);
			print_number(outpf, outpf_arg, 16, INT_T_PTR,
				     flags, field_width, precision, &args);
			continue;

		case 'E':
			flags |= UPPERCASE;
		case 'e':
			++fmt;
			print_flt_e(outpf, outpf_arg, va_arg(args, double),
				    sz_qualifier > 0 ? FLT_T_DOUBLE:FLT_T_FLOAT,
				    flags, field_width, precision);
			continue;

			/* Integer number formats - set up the flags and "break" */
		case 'o':
			base = 8;
			break;

		case 'X':
			flags |= UPPERCASE;
		case 'x':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGNED;
			break;

		case 'u':
			break;

		default:
			/* Specifier oops */
			++fmt;
			outpf(outpf_arg, "#FORM%", -1);
			continue;
		}

		++fmt;

		/* Here output of integers takes place */
		int type;
		switch(sz_qualifier)
		{
		case -2:
			type = INT_T_CHAR;
			break;
		case -1:
			type = INT_T_SHORT;
			break;
		case 1:
			type = INT_T_LONG;
			break;
		case 2:
			type = INT_T_LONGLONG;
			break;
		default:
			type = INT_T_INT;
		}
		print_number(outpf, outpf_arg, base, type,
			     flags, field_width, precision, &args);
	}
	return;
}

void of_printf(outpf_ptr outpf, void *outpf_arg, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	of_vprintf(outpf, outpf_arg, fmt, args);
	va_end(args);
}
