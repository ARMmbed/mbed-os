#ifndef ASSERT_H
#define ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

/** This function is active only if NDEBUG is not defined prior to including this
 *  assert header file.
 *  Internal mbed assert function which is invoked when MBED_ASSERT macro failes.
 *  In case of MBED_ASSERT failing condition, the assertation message is printed
 *  to stderr and mbed_die() is called.
 *  @param expr Expresion to be checked.
 *  @param file File where assertation failed.
 *  @param line Failing assertation line number.
 */
void mbed_assert_internal(const char *expr, const char *file, int line);

#ifdef __cplusplus
}
#endif

#ifdef NDEBUG
#define MBED_ASSERT(expr) ((void)0)

#else
#define MBED_ASSERT(expr)                                \
do {                                                     \
    if (!(expr)) {                                         \
        mbed_assert_internal(#expr, __FILE__, __LINE__); \
    }                                                    \
} while (0)
#endif

#endif
