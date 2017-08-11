/** \addtogroup hal_rtc_tests
 *  @{
 */
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
#ifndef MBED_RTC_TEST_H
#define MBED_RTC_TEST_H

#if DEVICE_RTC

#ifdef __cplusplus
extern "C" {
#endif

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

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
