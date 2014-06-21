#ifndef FLOATPARSE_H
#define FLOATPARSE_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Contains prototypes of ecvt()-like float to string conversion
 *        functions
 */

#include <printf/fp_float_def.h>

/**
 * @brief Print mantissa of floating-point value (of type float) to string
 *
 * Outputs first \p precision digits of floating-point number's mantissa into
 * string buffer pointer by \p buf. If value needs rounding, it is done
 * (i.e. 129999999999... is written as 130000 with precision = 6)
 * Returns sign and decimal exponent of supplied value in \p rsign and
 * \p rdec_exp, respectively. Also returns "speciality tag"(#fp_specialval_t)
 * in \p rspec.
 *
 * Function writes only mantissa' digits, removing leading zeros (if possible),
 * and adding trailing zeros if necessary (if required precision is too high).
 * @note
 * Function does not write nor decimal point, nor exponent value into buffer!
 *
 * Decimal exponent \p dec_exp is equal to position of decimal point
 * relative to the MSD (most significant digit), i.e.:
 * - dec_exp == 0: decimal point is located exactly before MSD
 * - dec_exp < 0: decimal point is abs(dec_exp) digits before MSD
 * - dec_exp > 0: decimal point is dec_exp digits after MSD
 *
 * Examples:
 * - 123.7 is printed as "123699995875" with precision of 12,
 *   dec_exp is 3, sign is 0
 * - 123.7 is printed as "123700" with precision of 6, dec_exp is 3, sign is 0
 * - -123e-25 is printed as "1229999959" with precision of 10, dec_exp is -22,
 *   sign is 1
 * - -123e25 is printed as "123000" with precision of 6, dec_exp is 28, sign is
 *   1
 *
 * Algorithm used for rounding always places '0' as a first digit in buffer to
 * use it as most significant digit placeholder in case rounding introduces
 * overflow (i.e. 999999.. must be written as 1000 with precision = 4, but
 * 998999.. would be written as 9990 with same precision).
 * Therefore, function returns pointer to MSD that can only be either buf
 * (rounding resulted in overflow and first '0' became '1') or buf + 1
 * (no overflow during rounding, first '0' was kept '0' and it is treated
 * as leading zero that must be removed).
 *
 * @param value floating-point value to parse
 * @param buf pointer to output buffer of length (\p precision + 1) at least
 * @param[out] rsign sign of value is stored at this place
 *             0 if positive value, 1 if negative
 * @param[out] rdec_exp decimal exponent is stored at this place
 * @param[out] rspec value "speciality tag" is placed here
 * @param precision exact number of digits to print
 * @return pointer to MSD (most significant digit) in supplied buffer
 */
char* fp_parse_float(float value, char *buf, int *rsign, int *rdec_exp,
		     fp_specialval_t *rspec, int precision);

/**
 * @brief Print mantissa of floating-point value (of type double) to string
 *
 * Behaves identically to fp_parse_float(), except that parses double, not float
 *
 * @param value floating-point value to parse
 * @param buf pointer to output buffer of length (\p precision + 1) at least
 * @param[out] rsign sign of value is stored at this place
 *             0 if positive value, 1 if negative
 * @param[out] rdec_exp decimal exponent is stored at this place
 * @param[out] rspec value "speciality tag" is placed here
 * @param precision exact number of digits to print
 * @return pointer to MSD (most significant digit) in supplied buffer
 */
char* fp_parse_double(double value, char *buf, int *rsign, int *rdec_exp,
		      fp_specialval_t *spec, int precision);

#endif // FLOATPARSE_H
