/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "device.h"

#if DEVICE_LOWPOWERTIMER

#include "ticker_api.h"
#include "lp_ticker_api.h"
#include "rtc_api.h"
#include "rtc_api_hal.h"

static uint8_t lp_ticker_inited = 0;
static uint8_t lp_ticker_reconf_presc = 0;

void lp_ticker_init() {
    if (lp_ticker_inited) return;
    lp_ticker_inited = 1;

    rtc_init();
    rtc_set_irq_handler((uint32_t) lp_ticker_irq_handler);
}

uint32_t lp_ticker_read() {
    uint32_t sub_secs, milis;
    time_t time;

    lp_ticker_init();

    time = rtc_read();
    sub_secs = rtc_read_subseconds();
    milis = 1000 - (sub_secs * 1000 / rtc_ticker_get_synch_presc());

    return (time * 1000000) + (milis * 1000);
}

void lp_ticker_set_interrupt(timestamp_t timestamp) {
    uint32_t sub_secs, delta, milis;
    time_t secs;
    struct tm *timeinfo;

    // Reconfigure RTC prescalers whenever the timestamp is below 30ms
    if (!lp_ticker_reconf_presc && timestamp < 30000) {
        rtc_reconfigure_prescalers();
        lp_ticker_reconf_presc = 1;
    }

    milis = (timestamp % 1000000) / 1000;

    secs = rtc_read();
    delta = ((timestamp / 1000000) - secs);

    secs += delta;
    sub_secs = (rtc_ticker_get_synch_presc() * (1000 - milis)) / 1000;
    timeinfo = localtime(&secs);

    rtc_set_alarm(timeinfo, sub_secs);
}

void lp_ticker_disable_interrupt() {
    lp_ticker_reconf_presc = 0;
    rtc_ticker_disable_irq();
}

void lp_ticker_clear_interrupt() {
}

#endif
