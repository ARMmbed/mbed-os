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

/** \addtogroup hal_rtc_tests
 *  @{
 */

#ifndef MBED_RTC_TEST_H
#define MBED_RTC_TEST_H

#if DEVICE_RTC

#ifdef __cplusplus
extern "C" {
#endif

/** Test that ::rtc_init can be called multiple times.
 *
 *  Given board provides RTC.
 *  When ::rtc_init is called multiple times.
 *  Then ::rtc_init are successfully performed (no exception is generated).
 *
 */
void rtc_init_test(void);

/** Test that the RTC keeps counting in the various sleep modes.
 *
 * Given board provides RTC.
 * When system enters sleep/deep-sleep mode.
 * RTC keeps counting.
 *
 */
void rtc_sleep_test(void);

/** Test that the RTC keeps counting even after ::rtc_free has been called.
 *
 * Given board provides RTC.
 * When ::rtc_free has been called.
 * RTC keeps counting.
 *
 */
void rtc_persist_test(void);

/** Test time does not glitch backwards due to an incorrectly implemented ripple counter driver.
 *
 * Given board provides RTC.
 * When RTC is enabled and counts.
 * Then time does not glitch backwards due to an incorrectly implemented ripple counter driver.
 *
 */
void rtc_glitch_test(void);

/** Test that the RTC correctly handles large time values.
 *
 * Given board provides RTC.
 * When RTC is enabled and counts.
 * The RTC correctly handles different time values.
 */
void rtc_range_test(void);

/** Test that the RTC accuracy is at least 10%.
 *
 *  Given platform provides Real Time Clock.
 *  When delay is performed based on RTC (10 sec delay).
 *  Then the delay time measured using high frequency timer indicate that RTC accuracy is at least 10%.
 *
 */
void rtc_accuracy_test(void);

/** Test that ::rtc_write/::rtc_read functions provides availability to set/get RTC time.
 *
 *  Given platform provides Real Time Clock.
 *  When an example RTC time is set by means of rtc_write function and rtc_read is performed immediately after this operation.
 *  Then rtc_read function returns time which has been set.
 *
 */
void rtc_write_read_test(void);

/** Test that ::rtc_isenabled function returns 1 if the RTC is counting and the time has been set, 0 otherwise
 *
 *  NOTE: RTC is counting when it has been initialised by means of rtc_init().
 *        RTC time is set by means of rtc_write() function.
 *        RTC must be initialised before rtc_isenabled() function can be called.
 *
 *  Given platform provides Real Time Clock.
 *  When rtc_isenabled() function is called.
 *  Then the result is 1 if RTC time has been set, otherwise the result is 0.
 *
 */
void rtc_enabled_test(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
