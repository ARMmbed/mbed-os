/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

/** \addtogroup hal_us_ticker_tests */
/** @{*/

#ifndef US_TICKER_API_TESTS_H
#define US_TICKER_API_TESTS_H

#include "device.h"


#ifdef __cplusplus
extern "C" {
#endif

/** Test that the ticker has the correct frequency and number of bits.
 *
 * Given ticker is available.
 * When ticker information data is obtained.
 * Then ticker information indicate that frequency between 250KHz and 8MHz and the counter is at least 16 bits wide.
 */
void us_ticker_info_test(void);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
