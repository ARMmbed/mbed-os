/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

/** \addtogroup hal_sleep_tests */
/** @{*/

#ifndef MBED_SLEEP_API_TESTS_H
#define MBED_SLEEP_API_TESTS_H

#include "device.h"

#if DEVICE_SLEEP

#ifdef __cplusplus
extern "C" {
#endif

/** High frequency ticker interrupt can wake up from sleep (locked deep-sleep).
 *
 *  Given is an environment with high frequency ticker.
 *  When the board enters sleep mode.
 *  Then the board can be wake up from the sleep by high frequency ticker interrupt and
 *  wake-up time should be less than 10 us.
 */
void sleep_usticker_test();

/** Low power ticker interrupt to wake up from deep-sleep (unlocked deep-sleep).
 *
 *  Given is an environment with low power ticker.
 *  When the board enters deep-sleep mode.
 *  Then the board can be wake up from the sleep by low power ticker interrupt and
 *  wake-up time should be less than 10 ms.
 *
 */
void deepsleep_lpticker_test();

/** High speed clocks are turned off in deep-sleep (unlocked deep-sleep)
 *
 *  Given is an environment with high frequency ticker.
 *  When the board enters deep-sleep mode.
 *  Then high frequency ticker does not count while the board is in the deep-sleep mode.
 *
 */
void deepsleep_high_speed_clocks_turned_off_test();

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
