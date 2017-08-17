
/** \addtogroup hal */
/** @{*/
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
#ifndef MBED_RTC_API_H
#define MBED_RTC_API_H

#include "device.h"

#if DEVICE_RTC

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_rtc RTC hal functions
 *
 * The RTC hal provides a low level interface to the Real Time Counter (RTC) of a
 * target.
 *
 * @see hal_rtc_tests
 *
 * @{
 */

/** Initialize the RTC peripheral
 *
 * Powerup the RTC in perpetration for access. This function must be called
 * before any other RTC functions ares called. This does not change the state
 * of the RTC. It just enables access to it.
 *
 * @note This function is safe to call repeatedly - Tested by ::rtc_init_test
 *
 * Psuedo Code:
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
 * Powerdown the RTC after in preparation for sleep, powerdown or reset.
 * After this function is called no other RTC functions should be called
 * except for ::rtc_init.
 *
 * @note This function does not stop the RTC from counting - Tested by ::rtc_persist_test
 *
 * Psuedo Code:
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
 * Psuedo Code:
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
 * Psuedo Code:
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

#endif

/** @}*/
