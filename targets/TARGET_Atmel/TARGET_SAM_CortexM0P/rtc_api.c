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
#include "system.h"

#include "rtc_count.h"

#if !defined(RTC_CLOCK_SOURCE)
#  warning  RTC_CLOCK_SOURCE is not defined, assuming RTC_CLOCK_SELECTION_ULP1K.
#  define RTC_CLOCK_SOURCE RTC_CLOCK_SELECTION_ULP1K
#endif

/* Global RTC instance*/
static struct rtc_module rtc_instance;

static int rtc_inited = 0;

/* Extern variables */
extern uint8_t g_sys_init;

/** Initialize the RTC
 *
 * Initialize the RTC with default time
 * @param[void] void
 */
void rtc_init(void)
{
    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }

    struct rtc_count_config config_rtc_count;

    rtc_count_get_config_defaults(&config_rtc_count);

    config_rtc_count.prescaler           = RTC_COUNT_PRESCALER_DIV_1024;
    config_rtc_count.mode                = RTC_COUNT_MODE_32BIT;
#ifdef FEATURE_RTC_CONTINUOUSLY_UPDATED
    config_rtc_count.continuously_update = true;
#endif

    rtc_count_init(&rtc_instance, RTC, &config_rtc_count);

    rtc_count_enable(&rtc_instance);
    rtc_inited = 1;
}

/** Frees the RTC
 *
 * @param[void] void
 */
void rtc_free(void)
{
    if (rtc_inited) {
        /* Disable the RTC module */
        rtc_count_disable(&rtc_instance);
        /* Disable the RTC clock */
#if (SAMD21) || (SAMR21)
        system_gclk_chan_disable(RTC_GCLK_ID);
#elif (SAML21)
        system_gclk_chan_disable(RTC_CLOCK_SOURCE);
#endif
        rtc_inited = 0;
    }
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
    return (time_t)rtc_count_get_count(&rtc_instance);
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

    uint32_t count_value = (uint32_t)t;
    rtc_count_set_count(&rtc_instance, count_value);
}
