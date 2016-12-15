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

void lp_ticker_init(void)
{
    if (lp_ticker_inited) return;
    lp_ticker_inited = 1;

    rtc_init();
    rtc_set_irq_handler((uint32_t) lp_ticker_irq_handler);
}

uint32_t lp_ticker_read(void)
{
    uint32_t usecs = 0;
    time_t time = 0;

    lp_ticker_init();

    do {
        time = rtc_read();
        usecs = rtc_read_subseconds();
    } while (time != rtc_read());

    return (time * 1000000) + usecs;
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta;

    delta = timestamp - lp_ticker_read();
    rtc_set_wake_up_timer(delta);
}

void lp_ticker_disable_interrupt(void)
{
    rtc_deactivate_wake_up_timer();
}

void lp_ticker_clear_interrupt(void)
{

}

#endif
