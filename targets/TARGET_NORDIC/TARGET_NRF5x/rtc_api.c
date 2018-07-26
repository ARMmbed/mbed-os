/*
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "rtc_api.h"

#if DEVICE_RTC

#include "common_rtc.h"
#include "nrf_rtc.h"
#include "app_util_platform.h"

static time_t m_time_base;

void rtc_init(void)
{
    common_rtc_init();
}

void rtc_free(void)
{
    // A common counter is used for RTC, lp_ticker and us_ticker, so it can't be
    // disabled here, but this does not cause any extra cost. Besides, currently
    // this function is not used by RTC API in mbed-drivers.
}

int rtc_isenabled(void)
{
    return m_common_rtc_enabled;
}

static uint32_t rtc_seconds_get(void)
{
    // Convert current counter value to seconds.
    uint32_t seconds = nrf_rtc_counter_get(COMMON_RTC_INSTANCE) / RTC_INPUT_FREQ;
    // Add proper amount of seconds for each registered overflow of the counter.
    uint32_t seconds_per_overflow = (1uL << RTC_COUNTER_BITS) / RTC_INPUT_FREQ;
    return (seconds + (m_common_rtc_overflows * seconds_per_overflow));
}

time_t rtc_read(void)
{
    return m_time_base + rtc_seconds_get();
}

void rtc_write(time_t t)
{
    uint32_t seconds;
    do {
        seconds = rtc_seconds_get();
        m_time_base = t - seconds;
    // If the number of seconds indicated by the counter changed during the
    // update of the time base, just repeat the update, now using the new
    // number of seconds.
    } while (seconds != rtc_seconds_get());
}

#endif // DEVICE_RTC
