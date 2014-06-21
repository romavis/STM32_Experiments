#ifndef FP_K_H
#define FP_K_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Contains substitution macros that are used to get K value
 *
 * K values, needed by conversion routines, can be obtained in 2 ways:
 * - from precomputed cache(table), if it was included
 * - by calculating them directly at runtime
 *
 * It is up to user to choose one of these ways: tables significantly reduce
 * number of ticks required to perform float-to-string conversion, but require
 * significant amount of storage
 */

#include <printf/fp_k_table.h>
#include <printf/fp_k_calc.h>

/** Conversion approximation: D = -e * log10(2) */
#define FP_E_TO_D(e) ((-((int) e) * 315653) >> 20)

#if defined(__DOXYGEN__)
/**
 * Use table to store K values for float-to-string conversion
 */
#define CONFIG_PRINTF_FLOAT_TABLE

/**
 * Use table to store K values for double-to-string conversion
 */
#define CONFIG_PRINTF_DOUBLE_TABLE

/**
 * Macro to obtain K value for floats
 */
#define FP_GET_K_F

/**
 * Macro to obtain K value for double
 */
#define FP_GET_K_D

#else // defined(__DOXYGEN__)

#ifdef CONFIG_PRINTF_FLOAT_KTABLE
#define FP_GET_K_F fp_table_k_f
#else
#define FP_GET_K_F fp_calc_k_f
#endif // CONFIG_PRINTF_FLOAT_KTABLE

#ifdef CONFIG_PRINTF_DOUBLE_KTABLE
#define FP_GET_K_D fp_table_k_d
#else
#define FP_GET_K_D fp_calc_k_d
#endif // CONFIG_PRINTF_DOUBLE_KTABLE

#endif // defined(__DOXYGEN__)

#endif // FP_K_H
