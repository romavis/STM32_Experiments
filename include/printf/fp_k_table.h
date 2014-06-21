#ifndef FLOAT_K_TABLE_H
#define FLOAT_K_TABLE_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Contains inlines used to get K from precomputed tables
 */

#include <stdint.h>

#include <printf/fp_float_def.h>

#define FP_REXP_MIN_F ((-EXPMID_F + 2) & ~0x3)
#define FP_REXP_MIN_D ((-EXPMID_D + 2) & ~0x3)
#define FP_REXP_MAX_F (( EXPMID_F + 1) & ~0x3)
#define FP_REXP_MAX_D (( EXPMID_D + 1) & ~0x3)

/**
 * Convert exponent value to index
 */
#define FP_EXP_TO_IDX(exp_rnd, type) (((exp_rnd) - FP_REXP_MIN_##type) >> 2)

/**
 * Convert table index to exponent value
 */
#define FP_IDX_TO_EXP(idx, type) (((idx) << 2) + FP_REXP_MIN_##type)

/**
 * Precomputed K table size for floats
 */
#define FP_TABLE_SZ_F (FP_EXP_TO_IDX(FP_REXP_MAX_F, F) + 1)

/**
 * Precomputed K table size for double
 */
#define FP_TABLE_SZ_D (FP_EXP_TO_IDX(FP_REXP_MAX_D, D) + 1)

#if defined(CONFIG_PRINTF_DOUBLE_KTABLE) || defined(__DOXYGEN__)
/**
 * Double K table address provided by linker
 */
extern uint64_t _fp_k_table_d_start;
static inline uint64_t fp_table_k_d(int exp_rnd)
{
	return (&_fp_k_table_d_start)[FP_EXP_TO_IDX(exp_rnd, D)];
}
#endif

#if defined(CONFIG_PRINTF_FLOAT_KTABLE) || defined(__DOXYGEN__)
/**
 * Floats K table address provided by linker
 */
extern uint32_t _fp_k_table_f_start;
static inline uint32_t fp_table_k_f(int exp_rnd)
{
	return (&_fp_k_table_f_start)[FP_EXP_TO_IDX(exp_rnd, F)];
}
#endif

#endif // FLOAT_K_TABLE_H
