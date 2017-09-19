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

/** \addtogroup hal_lp_ticker_tests
 *  @{
 */

#ifndef LP_TICKER_API_TESTS_H
#define LP_TICKER_API_TESTS_H

#include "device.h"


#ifdef __cplusplus
extern "C" {
#endif


/** Test that the ticker has the right frequency and number of bits
 *
 */
void lp_ticker_info_test(void);

/** Test that the ticker operates in deep sleep mode
 *
 */
void lp_ticker_deepsleep_test(void);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/**@}*/
