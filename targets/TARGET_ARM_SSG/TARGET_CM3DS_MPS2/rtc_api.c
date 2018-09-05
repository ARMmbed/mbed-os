/*
 * Copyright (c) 2018 Arm Limited
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

#include "rtc_api.h"
#include "platform_devices.h"
#include "rtc_pl031_drv.h"

static uint32_t is_enabled = 0;

/**
 * \defgroup hal_rtc RTC hal functions
 * @{
 */

/**
 * \brief Initialize the RTC peripheral.
 *        The RTC starts counting from 0x0 at reset and increments
 *        with a frequency of 1hz (clock source: CLK1HZ).
 *        The current value can be read through the DATA register
 */
void rtc_init(void)
{
    rtc_pl031_init(&RTC_PL031_DEV);
    rtc_pl031_dev_enable(&RTC_PL031_DEV);
    is_enabled = 1;
}

/**
 * \brief Deinitialize the RTC peripheral
 *        According to DDI0224B_RTC_PL031_TRM.pdf chapter 3.3.4 there is
 *        no reason to implement.
 */
void rtc_free(void)
{
    is_enabled = 0;
}

/**
 * \brief Get the RTC enable status
 *
 * \return 0 disabled, 1 enabled
 */
int rtc_isenabled(void)
{
    return is_enabled;
}

/**
 * \brief Get the current time from the RTC peripheral
 *
 *        Sysclock and RTC clock may not be in sync which can cause reading
 *        out metastable values. It's usually prevented by adding a loop,
 *        however PL031 has a syncronisation block to prevent this, therefore
 *        no additional loop needed.
 *
 * \return The current time in seconds
 */
time_t rtc_read(void)
{
    uint32_t val;
    rtc_pl031_read_current_time(&RTC_PL031_DEV, &val);
    return (time_t)val;
}

/**
 * \brief Set the current time to the RTC peripheral, write it to LOAD register
 *
 * \param[in] t The current time to be set in seconds
 */

void rtc_write(time_t t)
{
    rtc_pl031_write_current_time(&RTC_PL031_DEV, (uint32_t)t);
}
/**@}*/

