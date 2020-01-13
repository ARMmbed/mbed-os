/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "mbed_assert.h"
#include "device.h"

#if DEVICE_RTC
#include "rtc_api.h"
#include "mbed_mktime.h"
extern long long get_counter(void);
time_t g_base_timeval = 0;

void rtc_init(void)
{

}

void rtc_free(void)
{

}

time_t rtc_read(void)
{
    time_t t;
    long long cnt;
    cnt = get_counter(); //return aliva countvalue in 40bits
    t = cnt / 32768;    //calculate as seconds
    t += g_base_timeval; //accumulate base time(e.g., NTP) with counter elapsed.

    return t;
}

void rtc_write(time_t t)
{
    g_base_timeval = t;
}

static uint8_t rtc_hex8_to_dec(uint8_t hex_val)
{
    uint32_t calc_data;

    return (uint8_t)calc_data;
}

static uint16_t rtc_hex16_to_dec(uint16_t hex_val)
{
    uint32_t calc_data;
    return (uint16_t)calc_data;
}

int rtc_isenabled(void)
{
    return 1;
}
#endif /* DEVICE_RTC */
