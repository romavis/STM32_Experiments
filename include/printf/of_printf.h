#ifndef OF_PRINTF_H
#define OF_PRINTF_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Provides access to core printf functionality with output specified
 *        through special output function pointer
 */

#include <stddef.h>
#include <stdarg.h>

/**
 * @brief Text output function pointer type
 *
 * @param arg function-specific argument that must be maintained across calls
 *	      i.e. pointer to address of char output buffer
 * @param str pointer to string to output
 * @param len defined from following (NOTE: len is unsigned!):
 *            function should print characters up to MIN(len, strlen(str)),
 *            where strlen(str) is number of characters in str up to the first
 *            encountered '\0' character, not including the latter
 */
typedef void (*outpf_ptr) (void *arg, const char *str, size_t len);

/**
 * @brief Core printf function
 *
 * Parses format string and prints formatted text in chunks using supplied
 * output function pointer
 *
 * Supported type specifiers:
 * - % - outputs percent symbol
 * - %%c - char
 * - %%s - string
 * - %%p - prints pointer in nice hexadecimal notation
 * - %%e, %%E - prints floating point number in scientific notation
 * - %%o - integer in base-8
 * - %%X, %%x - integer in base-16
 * - %%d, %%i - signed integer in base-10
 * - %%u - unsigned integer in base-10
 *
 * Supported modifiers (flags):
 * - + - always print sign for numbers
 * - {space} - same as '+' but print ' ' instead of '+'
 * - # - print number base specifier (0x for hex, 0b for bin, 0 for oct)
 * - 0 - pad to specified field width with zeros
 *
 * Supported features:
 * - field width
 * - precision for numbers and string
 * - size qualifier (%e - float, %le - double, %hhd - char, %lld - long long,
 *   etc.
 * @note
 * - only right alignment supported
 * - # modifier affects only integers output
 * - no %%f, %%g and %%a support (yet)
 *
 * @param outpf output function pointer
 * @param outpf_arg argument that is passed to output function in each call
 * @param fmt format string
 * @param args variable length argument list (va_list type)
 */
void of_vprintf(outpf_ptr outpf, void *outpf_arg, const char *fmt,
		va_list args);

/**
 * @brief Wrapper around of_printf() that accepts usual "..."-style variable
 *        length arguments list
 * @param outpf output function pointer
 * @param outpf_arg argument that is passed to output function in each call
 * @param fmt format string
 */
void of_printf(outpf_ptr outpf, void *outpf_arg, const char *fmt, ...);

#endif // OF_PRINTF_H
