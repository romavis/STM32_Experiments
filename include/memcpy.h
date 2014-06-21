#ifndef MEMCPY_H
#define MEMCPY_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Provide classical memcpy() function
 */

#include <stddef.h>

/**
 * @brief Classic memcpy()
 * @param dest pointer to destination buffer
 * @param src pointer to source buffer
 * @param count number of bytes to copy
 * 
 * @return returns dest
 */
void* memcpy(void *dest, void *src, size_t count);

#endif // MEMCPY_H
