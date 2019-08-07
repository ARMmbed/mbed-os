/** \ingroup mbed-os-public */
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_preprocessor preprocessor macros
 * @{
 */

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
#ifndef MBED_PREPROCESSOR_H
#define MBED_PREPROCESSOR_H


/** MBED_CONCAT
 *  Concatenate tokens together
 *
 *  @note
 *  Expands tokens before concatenation
 *
 *  @code
 *  // Creates a unique label based on the line number
 *  int MBED_CONCAT(UNIQUE_LABEL_, __LINE__) = 1;
 *  @endcode
 */
#define MBED_CONCAT(a, b) MBED_CONCAT_(a, b)
#define MBED_CONCAT_(a, b) a##b

/** MBED_STRINGIFY
 *  Converts tokens into strings
 *
 *  @note
 *  Expands tokens before stringification
 *
 *  @code
 *  // Creates a string based on the parameters
 *  const char *c = MBED_STRINGIFY(This is a ridiculous way to create a string)
 *  @endcode
 */
#define MBED_STRINGIFY(a) MBED_STRINGIFY_(a)
#define MBED_STRINGIFY_(a) #a

/** MBED_STRLEN
 *  Reports string token length
 *
 *  @note
 *  Expands tokens before calculating length
 *
 *  @code
 *  // Get string length
 *  const int len = MBED_STRLEN("Get the length")
 *  @endcode
 */
#define MBED_STRLEN(a) MBED_STRLEN_(a)
#define MBED_STRLEN_(a) (sizeof(a) - 1)

/** MBED_COUNT_VA_ARGS(...)
 *  Reports number of tokens passed
 *
 *  @note
 *  Token limit is 16
 *
 *  @code
 *  // Get number of arguments
 *  const int count = MBED_COUNT_VA_ARGS("Address 0x%x, Data[0] = %d Data[1] = %d", 0x20001234, 10, 20)
 *  @endcode
 */
#define MBED_COUNT_VA_ARGS(...) GET_NTH_ARG_(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define GET_NTH_ARG_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, N, ...) N

#endif

/** @}*/
/** @}*/
