/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "mbed_assert.h"
#include "rtc_api.h"
#include "cmsis.h"
#include "sysclk.h"
#include "rtc.h"

static int rtc_inited = 0;

extern uint8_t g_sys_init;

/** Initialize the RTC
 *
 * Initialize the RTC with default time
 * @param[void] void
 */
void rtc_init(void)
{
    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }

    rtc_inited = 1;
}

/** Frees the RTC
 *
 * @param[void] void
 */
void rtc_free(void)
{
}

/** Checks whether RTC is enabled or not
 *
 * To check whether RTC module is enabled or not
 * @param[void] void
 * @return      Non zero if RTC is already enabled, else zero
 */
int rtc_isenabled(void)
{
    return rtc_inited;
}

/** Reads the RTC value
 *
 * Reads and return the current time in RTC
 * @param[void] void
 * @return      the current value in RTC
 */
time_t rtc_read(void)
{
    if (!rtc_inited) {
        /* Return invalid time for now! */
        return 0;
    }
}

/** Write the RTC value
 *
 * Update the time value in RTC
 * @param[in] t The time value to be written
 * @return      void
 */
void rtc_write(time_t t)
{
    if (!rtc_inited) {
        /* Initialize the RTC is not yet initialized */
        rtc_init();
    }
}