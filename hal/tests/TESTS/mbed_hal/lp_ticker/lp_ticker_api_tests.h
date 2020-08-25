/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

/** \addtogroup hal_lp_ticker_tests
 *  @{
 */

#ifndef LP_TICKER_API_TESTS_H
#define LP_TICKER_API_TESTS_H

#include "device.h"


#ifdef __cplusplus
extern "C" {
#endif

/** Test that the ticker has the correct frequency and number of bits.
 *
 * Given ticker is available.
 * When ticker information data is obtained.
 * Then collected data indicates that ticker frequency is between 4KHz and 64KHz and the counter is at least 12 bits wide.
 */
void lp_ticker_info_test(void);

/** Test that the ticker continues operating in deep sleep mode.
 *
 * Given ticker is available.
 * When ticker has interrupt set and board enters deep-sleep mode.
 * Then ticker continues operating.
 */
void lp_ticker_deepsleep_test(void);

/** Test that the ticker does not glitch backwards due to an incorrectly implemented ripple counter driver.
 *
 * Given ticker is available.
 * When ticker is enabled and counts.
 * Then ticker does not glitch backwards due to an incorrectly implemented ripple counter driver.
 */
void lp_ticker_glitch_test(void);
/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
