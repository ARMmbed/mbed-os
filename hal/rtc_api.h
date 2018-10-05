/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

/** \addtogroup hal */
/** @{*/

#ifndef MBED_RTC_API_H
#define MBED_RTC_API_H

#include "device.h"

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_rtc RTC hal
 *
 * The RTC hal provides a low level interface to the Real Time Counter (RTC) of a
 * target.
 *
 * # Defined behaviour
 * * The function ::rtc_init is safe to call repeatedly - Verified by test ::rtc_init_test.
 * * RTC accuracy is at least 10% - Verified by test ::rtc_accuracy_test.
 * * Init/free doesn't stop RTC from counting - Verified by test ::rtc_persist_test.
 * * Software reset doesn't stop RTC from counting - Verified by test ::rtc_reset_test.
 * * Sleep modes don't stop RTC from counting - Verified by test ::rtc_sleep_test.
 * * Shutdown mode doesn't stop RTC from counting - Not verified.
 * * The functions ::rtc_write/::rtc_read provides availability to set/get RTC time
 * - Verified by test ::rtc_write_read_test.
 * * The functions ::rtc_isenabled returns 1 if the RTC is counting and the time has been set,
 * 0 otherwise - Verified by test ::rtc_enabled_test.
 *
 * # Undefined behaviour
 * * Calling any function other than ::rtc_init before the initialisation of the RTC
 *
 * # Potential bugs
 * * Incorrect overflow handling - Verified by ::rtc_range_test
 * * Glitches due to ripple counter - Verified by ::rtc_glitch_test
 *
 * @see hal_rtc_tests
 *
 * @{
 */

/**
 * \defgroup hal_rtc_tests RTC hal tests
 * The RTC test validate proper implementation of the RTC hal.
 *
 * To run the RTC hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-rtc*
 */


/** Initialize the RTC peripheral
 *
 * Powerup the RTC in perpetration for access. This function must be called
 * before any other RTC functions ares called. This does not change the state
 * of the RTC. It just enables access to it.
 *
 * @note This function is safe to call repeatedly - Tested by ::rtc_init_test
 *
 * Example Implementation Pseudo Code:
 * @code
 * void rtc_init()
 * {
 *     // Enable clock gate so processor can read RTC registers
 *     POWER_CTRL |= POWER_CTRL_RTC_Msk;
 *
 *     // See if the RTC is already setup
 *     if (!(RTC_STATUS & RTC_STATUS_COUNTING_Msk)) {
 *
 *         // Setup the RTC clock source
 *         RTC_CTRL |= RTC_CTRL_CLK32_Msk;
 *     }
 * }
 * @endcode
 */
void rtc_init(void);

/** Deinitialize RTC
 *
 * Powerdown the RTC in preparation for sleep, powerdown or reset. That should only
 * affect the CPU domain and not the time keeping logic.
 * After this function is called no other RTC functions should be called
 * except for ::rtc_init.
 *
 * @note This function does not stop the RTC from counting - Tested by ::rtc_persist_test
 *
 * Example Implementation Pseudo Code:
 * @code
 * void rtc_free()
 * {
 *     // Disable clock gate since processor no longer needs to read RTC registers
 *     POWER_CTRL &= ~POWER_CTRL_RTC_Msk;
 * }
 * @endcode
 */
void rtc_free(void);

/** Check if the RTC has the time set and is counting
 *
 * @retval 0 The time reported by the RTC is not valid
 * @retval 1 The time has been set the RTC is counting
 *
 * Example Implementation Pseudo Code:
 * @code
 * int rtc_isenabled()
 * {
 *     if (RTC_STATUS & RTC_STATUS_COUNTING_Msk) {
 *         return 1;
 *     } else {
 *         return 0;
 *     }
 * }
 * @endcode
 */
int rtc_isenabled(void);

/** Get the current time from the RTC peripheral
 *
 * @return The current time in seconds
 *
 * @note Some RTCs are not synchronized with the main clock. If
 * this is the case with your RTC then you must read the RTC time
 * in a loop to prevent reading the wrong time due to a glitch.
 * The test ::rtc_glitch_test is intended to catch this bug.
 *
 * Example implementation for an unsynchronized ripple counter:
 * @code
 * time_t rtc_read()
 * {
 *     uint32_t val;
 *     uint32_t last_val;
 *
 *     // Loop until the same value is read twice
 *     val = RTC_SECONDS;
 *     do {
 *         last_val = val;
 *         val = RTC_SECONDS;
 *     } while (last_val != val);
 *
 *     return (time_t)val;
 * }
 * @endcode
 */
time_t rtc_read(void);

/** Write the current time in seconds to the RTC peripheral
 *
 * @param t The current time to be set in seconds.
 *
 * Example Implementation Pseudo Code:
 * @code
 * void rtc_write(time_t t)
 * {
 *     RTC_SECONDS = t;
 * }
 * @endcode
 */
void rtc_write(time_t t);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
