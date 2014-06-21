#ifndef ALLOCA_H
#define ALLOCA_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief alloca() __builtin wrapper
 */

#define alloca(size) __builtin_alloca(size)

#endif // ALLOCA_H
