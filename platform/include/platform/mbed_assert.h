/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_ASSERT_H
#define MBED_ASSERT_H

#include <assert.h>
#include "platform/mbed_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_Assert Assert macros
 * @{
 */

/** Internal mbed assert function which is invoked when MBED_ASSERT macro fails.
 *  This function is active only if NDEBUG is not defined prior to including this
 *  assert header file.
 *  In case of MBED_ASSERT failing condition, error() is called with the assertation message.
 *  @param expr Expression to be checked.
 *  @param file File where assertation failed.
 *  @param line Failing assertation line number.
 */
MBED_NORETURN void mbed_assert_internal(const char *expr, const char *file, int line);

#ifdef __cplusplus
}
#endif

/** MBED_ASSERT
 *  Declare runtime assertions: results in runtime error if condition is false
 *
 *  @note
 *  Use of MBED_ASSERT is limited to Debug and Develop builds.
 *
 *  @code
 *
 *  int Configure(serial_t *obj) {
 *      MBED_ASSERT(obj);
 *  }
 *  @endcode
 */
#ifdef NDEBUG
#define MBED_ASSERT(expr) ((void)0)

#else
#define MBED_ASSERT(expr)                                \
do {                                                     \
    if (!(expr)) {                                       \
        mbed_assert_internal(#expr, __FILE__, __LINE__); \
    }                                                    \
} while (0)
#endif

// ARM Compiler 6 currently fails to define `static_assert` in assert.h; correct for this
#if !defined __cplusplus && !defined static_assert
#define static_assert _Static_assert
#endif

/** MBED_STATIC_ASSERT
 *  Declare compile-time assertions, results in compile-time error if condition is false
 *
 *  The assertion acts as a declaration that can be placed at file scope, in a
 *  code block (except after a label), or as a member of a C++ class/struct/union.
 *
 *  @code
 *  MBED_STATIC_ASSERT(MBED_MAJOR_VERSION  >= 6,
 *          "The mbed-os library must be at least version 6.0.0");
 *
 *  int main() {
 *      MBED_STATIC_ASSERT(sizeof(int) >= sizeof(char),
 *              "An int must be larger than a char");
 *  }
 *  @endcode
 *
 *  @deprecated This feature is now no longer necessary with the minimum
 *  supported language versions. It will be removed in a forthcoming release.
 *  Use `static_assert` instead. For C this is provided by `<assert.h>`, and
 *  for C++ it is a built-in keyword.
 */
#if defined(__cplusplus)
#define MBED_STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#else
#define MBED_STATIC_ASSERT(expr, msg) _Static_assert(expr, msg)
#endif

/** MBED_STRUCT_STATIC_ASSERT
 *  Declare compile-time assertions, results in compile-time error if condition is false
 *
 *  Previous supported compiler languages would not allow static_assert to be
 *  used within a struct or a class. This is no longer the case. This macro
 *  exists for backwards compatibility.
 *
 *  @code
 *  struct thing {
 *      MBED_STRUCT_STATIC_ASSERT(2 + 2 == 4,
 *              "Hopefully the universe is mathematically consistent");
 *  };
 *  @endcode
 *
 *  @deprecated This feature is now no longer necessary with the minimum
 *  supported language versions. It will be removed in a forthcoming release.
 *  Use `static_assert` instead. For C this is provided by `<assert.h>`, and
 *  for C++ it is a built-in keyword.
 */
#define MBED_STRUCT_STATIC_ASSERT(expr, message) MBED_STATIC_ASSERT(expr, message)

#endif

/**@}*/

/**@}*/

