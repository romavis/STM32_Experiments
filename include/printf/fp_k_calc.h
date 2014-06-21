#ifndef FLOAT_K_CALC_H
#define FLOAT_K_CALC_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Contains prototypes of functions used to calculate K values
 *
 * Also defines inlines used instead of table-access functions to get K
 * at runtime by direct calculation
 */

#include <stdint.h>

#include <printf/fp_float_def.h>

/**
 * @brief Calculate 64-bit K value based on known 2-base exponent
 * @param exp binary exponent to calculate K for
 * @return fixed point 64-bit K value (64 bit fractional part), 0 < K < 1
 *         (<i>Read note below!</i>)
 *
 * @note If exp equals 0, then K must be 1. However, given chosen K storage
 *       format (0 bit integer + 64 bit fractional fixed-point) it is
 *       not possible to represent that value of K.
 *       Function returns zero in such case
 */
uint64_t fp_calc_k_64b(int exp);

/**
 * @brief Calculate 32-bit K value based on known 2-base exponent
 * @param exp binary exponent to calculate K for
 * @return fixed point 32-bit K value (32 bit fractional part), 0 < K < 1
 *         (<i>Read note below!</i>)
 *
 * @note If exp equals 0, then K must be 1. However, given chosen K storage
 *       format (0 bit integer + 32 bit fractional fixed-point) it is
 *       not possible to represent that value of K.
 *       Function returns zero in such case
 */
uint32_t fp_calc_k_32b(int exp);


static inline uint64_t fp_calc_k_d(int exp_rnd)
{
	return fp_calc_k_64b(exp_rnd);
}

static inline uint32_t fp_calc_k_f(int exp_rnd)
{
	return fp_calc_k_32b(exp_rnd);
}

#endif // FLOAT_K_CALC_H
