
/** \ingroup mbed-os-public */
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_rtc_time rtc_time functions
 * @{
 */
/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Timeval definition for non GCC_ARM toolchains */
#if !defined(__GNUC__) || defined(__CC_ARM) || defined(__clang__)
struct timeval {
    time_t tv_sec;
    int32_t tv_usec;
};
#endif

/** Implementation of the C time.h functions
 *
 * Provides mechanisms to set and read the current time, based
 * on the microcontroller Real-Time Clock (RTC), plus some
 * standard C manipulation and formatting functions.
 *
 * Example:
 * @code
 * #include "mbed.h"
 *
 * int main() {
 *     set_time(1256729737);  // Set RTC time to Wed, 28 Oct 2009 11:35:37
 *
 *     while (true) {
 *         time_t seconds = time(NULL);
 *
 *         printf("Time as seconds since January 1, 1970 = %u\n", (unsigned int)seconds);
 *
 *         printf("Time as a basic string = %s", ctime(&seconds));
 *
 *         char buffer[32];
 *         strftime(buffer, 32, "%I:%M %p\n", localtime(&seconds));
 *         printf("Time as a custom formatted string = %s", buffer);
 *
 *         wait(1);
 *     }
 * }
 * @endcode
 */

/** Set the current time
 *
 * Initializes and sets the time of the microcontroller Real-Time Clock (RTC)
 * to the time represented by the number of seconds since January 1, 1970
 * (the UNIX timestamp).
 *
 * @param t Number of seconds since January 1, 1970 (the UNIX timestamp)
 *
 * @note Synchronization level: Thread safe
 *
 * Example:
 * @code
 * #include "mbed.h"
 *
 * int main() {
 *     set_time(1256729737); // Set time to Wed, 28 Oct 2009 11:35:37
 * }
 * @endcode
 */
void set_time(time_t t);

/** Attach an external RTC to be used for the C time functions
 *
 * @note Synchronization level: Thread safe
 *
 * @param read_rtc pointer to function which returns current UNIX timestamp
 * @param write_rtc pointer to function which sets current UNIX timestamp, can be NULL
 * @param init_rtc pointer to function which initializes RTC, can be NULL
 * @param isenabled_rtc pointer to function which returns if the RTC is enabled, can be NULL
 */
void attach_rtc(time_t (*read_rtc)(void), void (*write_rtc)(time_t), void (*init_rtc)(void), int (*isenabled_rtc)(void));

/** Standard lib retarget, get time since Epoch
 *
 * @param tv    Structure containing time_t seconds and useconds_t microseconds. Due to
 *              separate target specific RTC implementations only the seconds component is used.
 * @param tz    DEPRECATED IN THE STANDARD: This parameter is left in for legacy code. It is
 *              not used.
 * @return      0 on success, -1 on a failure.
 * @note Synchronization level: Thread safe
 *
 */
int gettimeofday(struct timeval *tv, void *tz);

/** Standard lib retarget, set time since Epoch
 *
 * @param tv    Structure containing time_t seconds and useconds_t microseconds. Due to
 *              separate target specific RTC implementations only the seconds component is used.
 * @param tz    DEPRECATED IN THE STANDARD: This parameter is left in for legacy code. It is
 *              not used.
 * @return      Time in seconds on success, -1 on a failure.
 * @note Synchronization level: Thread safe
 *
 */
int settimeofday(const struct timeval *tv, const struct timezone *tz);

#ifdef __cplusplus
}
#endif

/** @}*/
/** @}*/
