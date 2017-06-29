/*
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

#include "rtc_api.h"
#include "device.h"
#include "cmsis.h"

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
    CMSDK_RTC->RTCCR |= (1 << CMSDK_RTC_ENABLE_Pos);
}

/**
 * \brief Deinitialize the RTC peripheral
 *        According to DDI0224B_RTC_PL031_TRM.pdf chapter 3.3.4 there is
 *        no reason to implement.
 */
void rtc_free(void)
{
    /* Not supported */
}

/**
 * \brief Get the RTC enable status
 *
 * \return 0 disabled, 1 enabled
 */
int rtc_isenabled(void)
{
    return (CMSDK_RTC->RTCCR & CMSDK_RTC_ENABLE_Msk);
}

/**
 * \brief Get the current time from the RTC peripheral
 *
 * \return The current time in seconds
 */
time_t rtc_read(void)
{
    return (time_t)CMSDK_RTC->RTCDR;
}

/**
 * \brief Set the current time to the RTC peripheral, write it to LOAD register
 *
 * \param[in] t The current time to be set in seconds
 */

void rtc_write(time_t t)
{
    CMSDK_RTC->RTCLR = (uint32_t)t;
}
/**@}*/

