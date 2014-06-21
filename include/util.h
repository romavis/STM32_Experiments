#ifndef UTIL_H
#define UTIL_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Utility functions and macros
 */

#define REG_WRITE(reg, mask, val)\
	((reg) = ((reg) & ~(mask)) | (val))

#define REG_WRITE_SH(reg, shift, mask, val)\
	REG_WRITE(reg, (mask) << (shift), (val) << (shift))

#define ARRAY_LEN(x) (sizeof(x) / sizeof(__typeof__(*(x))))

#endif // UTIL_H
