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

/** \addtogroup hal_specification */
/** @{*/

#ifndef MBED_RTC_API_TESTS_H
#define MBED_RTC_API_TESTS_H

#include "device.h"

#if DEVICE_RTC

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup ha_specificationl_rtc RTC
 * RTC specification and tests
 *
 * To run the RTC hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-rtc*
 *
 * # Defined behavior
 * * The function ::rtc_init is safe to call repeatedly - Verified by test ::rtc_init_test
 * * RTC accuracy is at least 10% - Not verified
 * * Init/free doesn't stop RTC from counting - Verified by test ::rtc_persist_test
 * * Software reset doesn't stop RTC from counting - Verified by ::rtc_reset_test
 * * Sleep modes don't stop RTC from counting - Verified by ::rtc_sleep_test
 * * Shutdown mode doesn't stop RTC from counting - Not verified
 *
 * # Undefined behavior
 * * Calling any function other than ::rtc_init before the initialization of the RTC
 *
 * # Potential bugs
 * * Incorrect overflow handling - Verified by ::rtc_range_test
 * * Glitches due to ripple counter - Verified by ::rtc_glitch_test
 *
 * @{
 */

/** Test that ::rtc_init can be called multiple times
 *
 */
void rtc_init_test(void);

/** Test that the RTC keeps counting in the various sleep modes
 *
 */
void rtc_sleep_test(void);

/** Test that the RTC keeps counting even after ::rtc_free has been called
 *
 */
void rtc_persist_test(void);

/** Test time does not glitch backwards due to an incorrectly implemented ripple counter driver
 *
 */
void rtc_glitch_test(void);

/** Test that the RTC correctly handles large time values
 *
 */
void rtc_range_test(void);

/** Test that the RTC does not stop counting after a software reset
 *
 */
void rtc_reset_test(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/**@}*/
