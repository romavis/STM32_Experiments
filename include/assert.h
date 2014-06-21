#ifndef ASSERT_H
#define ASSERT_H

/**
 * @file
 * @author romavis (Roman Dobrodiy)
 * @date April, 2014
 *
 * @brief Provide functionality similar to libc assert.h
 */

#if defined(CONFIG_DEBUG_ASSERT) || defined(__DOXYGEN__)
/**
 * @brief Expression assertion macro
 *
 * @param EXPR expression to evaluate
 * 
 * If EXPR doesn't hold, macro calls special function assert_failed() to
 * process assertion failure
 */
#define assert(EXPR) (if(!(EXPR)) assert_failed("EXPR",\
						__func__, __FILE__, __LINE__\))
#else
#define assert(EXPR)
#endif /* CONFIG_DEBUG_ASSERT */

/**
 * @brief Function to handle assertion failure
 * @param expr failed assertion expression written to the string
 * @param function function name, in which failure occured
 * @param file file name, in which failure occured
 * @param line line number, on which failure occured
 * 
 * Normally this function should print debug message and restart task,
 * halt or reboot system. I don't know exactly, however :-)
 */
void assert_failed(const char *expr,
		   const char *function,
		   const char *file,
		   int line);

#endif // ASSERT_H
