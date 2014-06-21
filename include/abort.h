#ifndef ABORT_H
#define ABORT_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Provide abort() function needed by GCC
 */

/**
 * @brief Function to handle software abort
 * 
 * This function can be called by some stuff from libgcc, as well as user code.
 * Should never return and maybe print debug messages
 */
void abort(void);

#endif // ABORT_H
