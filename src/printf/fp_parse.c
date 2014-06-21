/*
 * Copyright (C) 2014 Roman Dobrodiy <ztcoils _d@g_ gmail.com>
 * 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

#include <stdint.h>
#include <alloca.h>

#include <assert.h>
#include <printf/fp_float_def.h>
#include <printf/fp_k.h>

void fp_puts_padded(char *buf, const char *str, char pad_char, int precision)
{
	int char_cnt = 0;
	while(*str != '\0' && char_cnt < precision) {
		*buf++ = *str++;
		char_cnt++;
	}
	while(char_cnt < precision) {
		*buf++ = pad_char;
		char_cnt++;
	}
}

/* buf should be of size precision+1 at least */

char* fp_parse_float(float value, char *buf, int *rsign, int *rdec_exp,
		     fp_specialval_t *rspec, int precision)
{
	union fp_float_t var;
	int bin_exp;
	int bin_exp_rnd;
	int dec_exp;
	uint32_t fract;

	char *buf_ptr = buf;

	var.val = value;
	fract = var.p.fraction;
	bin_exp = var.p.exponent;

	*rdec_exp = dec_exp = 0;
	*rsign = var.p.sign;

	/*
	 * Test for specials
	 */
	if(bin_exp == EXPMAX_F) {
		if(fract) {
			/* It's NaN */
			*rspec = FPS_NAN;
			fp_puts_padded(buf, "NaN", '#', precision);
			return buf;
		} else {
			/* It's infinity */
			*rspec = FPS_INF;
			fp_puts_padded(buf, "Inf", '#', precision);
			return buf;
		}
	} else if(!bin_exp && !fract) {
		/* It's zero */
		*rspec = FPS_ZERO;
		fp_puts_padded(buf, "0", '0', precision);
		return buf;
	}

	/*
	 * Test whether the number is normalized
	 * and account for implicit 1 in front of mantissa (normalized)
	 * or increase exponent as required by standard (denormalized)
	 */
	if(bin_exp) {
		fract|= (1ULL<<MANTLEN_F);
		*rspec = FPS_NORMAL;
	} else {
		bin_exp++;
		*rspec = FPS_DENORMALIZED;
	}

	bin_exp-= EXPMID_F - 1;
	bin_exp_rnd = bin_exp & ~0x3;

	fract<<=4;
	dec_exp = -FP_E_TO_D(bin_exp_rnd);
	if(bin_exp_rnd != 0) {
		uint32_t k;
		k = FP_GET_K_F(bin_exp_rnd);

		uint64_t mk;
		mk = (uint64_t) fract * k;
		fract = mk >> 32;
		fract+= (uint32_t) mk >= 0x80000000;
	}
	fract<<= bin_exp - bin_exp_rnd;

	/* Remove leading zeros */
	dec_exp++;
	while(!(uint32_t) (fract >> 28)) {
		dec_exp--;
		fract*= 10;
	}
	/* Reserve place for rounding.. */
	*buf_ptr++ = '0';
	/* Extract digits */
	while(fract && (buf_ptr - buf - 1) < precision) {
		*buf_ptr++ = (uint32_t) (fract >> 28) + '0';
		fract&= 0x0FFFFFFF;
		fract*= 10;
	}
	/* Pad with zeros */
	while((buf_ptr - buf - 1) < precision)
		*buf_ptr++ = '0';
	/* Round */
	if((uint32_t) (fract >> 28) >= 5) {
		char *buf_rnd = buf_ptr - 1;
		(*buf_rnd)++;
		while(*buf_rnd > '9') {
			*buf_rnd = '0';
			(*--buf_rnd)++;
		}
	}
	if(*buf == '0')
		buf++;
	else
		dec_exp++;

	*rdec_exp = dec_exp;

	return buf;

}

char* fp_parse_double(double value, char *buf, int *rsign, int *rdec_exp,
		      fp_specialval_t *rspec, int precision)
{
	union fp_double_t var;
	int bin_exp;
	int bin_exp_rnd;
	int dec_exp;
	uint64_t fract;

	char *buf_ptr = buf;

	var.val = value;
	fract = var.p.fraction;
	bin_exp = var.p.exponent;

	*rdec_exp = dec_exp = 0;
	*rsign = var.p.sign;

	/*
	 * Test for specials
	 */
	if(bin_exp == EXPMAX_D) {
		if(fract) {
			/* It's NaN */
			*rspec = FPS_NAN;
			fp_puts_padded(buf, "NaN", '#', precision);
			return buf;
		} else {
			/* It's infinity */
			*rspec = FPS_INF;
			fp_puts_padded(buf, "Inf", '#', precision);
			return buf;
		}
	} else if(!bin_exp && !fract) {
		/* It's zero */
		*rspec = FPS_ZERO;
		fp_puts_padded(buf, "0", '0', precision);
		return buf;
	}

	/*
	 * Test whether the number is normalized
	 * and account for implicit 1 in front of mantissa (normalized)
	 * or increase exponent as required by standard (denormalized)
	 */
	if(bin_exp) {
		fract|= (1ULL<<MANTLEN_D);
		*rspec = FPS_NORMAL;
	} else {
		bin_exp++;
		*rspec = FPS_DENORMALIZED;
	}

	bin_exp-= EXPMID_D - 1;
	bin_exp_rnd = bin_exp & ~0x3;

	fract<<=7;
	dec_exp = -FP_E_TO_D(bin_exp_rnd);
	if(bin_exp_rnd != 0) {
		uint64_t k;
		k = FP_GET_K_D(bin_exp_rnd);

		/* 64x64 => 128 bit multiplication (fract * k) */
		uint32_t lf, lk, hf, hk;
		uint64_t lflk, hflk, hklf;
		uint64_t hsum;

		lf = fract;
		hf = fract >> 32;
		lk = k;
		hk = k >> 32;

		fract = (uint64_t) hf * hk;
		lflk = (uint64_t) lf * lk;
		hflk = (uint64_t) hf * lk;
		hklf = (uint64_t) hk * lf;

		uint32_t lflk_h, lflk_l;
		lflk_h = lflk >> 32;
		lflk_l = lflk;

		lflk_h+= lflk_l >= 0x80000000;	/* Rounding.. */
		fract+= lflk_h < (lflk >> 32);

		/* Avoid overflow: zero 2MSB of hflk, hklf and transfer these to hfhk */

		fract+= (hflk >> 32) & 0xC0000000;
		fract+= (hklf >> 32) & 0xC0000000;
		hflk&= 0x3FFFFFFFFFFFFFFFULL;
		hklf&= 0x3FFFFFFFFFFFFFFFULL;

		hsum = hflk + hklf + lflk_h;
		fract+= hsum >> 32;
		fract+= (uint32_t) hsum >= 0x80000000;
	}
	fract<<= bin_exp - bin_exp_rnd;

	/* Remove leading zeros */
	dec_exp++;
	while(!(uint32_t) (fract >> 60)) {
		dec_exp--;
		fract*= 10;
	}
	/* Reserve place for rounding.. */
	*buf_ptr++ = '0';
	/* Extract digits */
	while(fract && (buf_ptr - buf - 1) < precision) {
		*buf_ptr++ = (uint32_t) (fract >> 60) + '0';
		fract&= 0x0FFFFFFFFFFFFFFFULL;
		fract*= 10;
	}
	/* Pad with zeros */
	while((buf_ptr - buf - 1) < precision)
		*buf_ptr++ = '0';
	/* Round */
	if((uint32_t) (fract >> 60) >= 5) {
		char *buf_rnd = buf_ptr - 1;
		(*buf_rnd)++;
		while(*buf_rnd > '9') {
			*buf_rnd = '0';
			(*--buf_rnd)++;
		}
	}
	if(*buf == '0')
		buf++;
	else
		dec_exp++;

	*rdec_exp = dec_exp;

	return buf;
}
