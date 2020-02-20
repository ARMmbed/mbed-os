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

/** \addtogroup hal_ticker_tests */
/** @{*/

#ifndef TICKER_API_TEST_FREQ_H
#define TICKER_API_TEST_FREQ_H

#include "device.h"


#ifdef __cplusplus
extern "C" {
#endif

/** Test that the ticker is operating at the frequency it specifies.
 *
 * Given ticker is available.
 * When ticker specifies its frequency.
 * Then the specified frequency is valid.
 */
void ticker_frequency_test(void);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
