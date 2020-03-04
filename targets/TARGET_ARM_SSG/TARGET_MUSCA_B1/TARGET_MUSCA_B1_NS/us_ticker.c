/* mbed Microcontroller Library
 * Copyright (c) 2017-2019 Arm Limited
 *
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

/**
 * Supports the High-resolution Ticker for mbed by implementing
 * \ref us_ticker_api.h, using a CMSDK Timer \ref timer_cmsdk_dev_t.
 */

#include "device.h"
#include "mbed_critical.h"
#include "timer_cmsdk_drv.h"
#include "us_ticker_api.h"

static uint64_t total_ticks = 0;
/* Stores the last reload value, or the last tick value read when a read API
 * call occurs from the upper layer, needed to keep total_ticks
 * accumulated properly.
 */
static uint32_t previous_ticks = 0;

static void restart_timer(uint32_t new_reload)
{
    timer_cmsdk_disable(&USEC_TIMER_DEV);
    timer_cmsdk_set_reload_value(&USEC_TIMER_DEV,
                                 new_reload);
    timer_cmsdk_reset(&USEC_TIMER_DEV);
    timer_cmsdk_clear_interrupt(&USEC_TIMER_DEV);
    timer_cmsdk_enable_interrupt(&USEC_TIMER_DEV);
    timer_cmsdk_enable(&USEC_TIMER_DEV);
}

static void update_ticker(void)
{
    if (timer_cmsdk_is_interrupt_active(&USEC_TIMER_DEV)) {
        total_ticks += previous_ticks;
        previous_ticks = TIMER_CMSDK_MAX_RELOAD;
        restart_timer(previous_ticks);
    } else {
        uint32_t tick = timer_cmsdk_get_current_value(&USEC_TIMER_DEV);

        if (tick < previous_ticks) {
            uint32_t delta = previous_ticks - tick;
            total_ticks += delta;
            previous_ticks = tick;
        }
    }
}

void us_ticker_init(void)
{
    timer_cmsdk_init(&USEC_TIMER_DEV);
    previous_ticks = TIMER_CMSDK_MAX_RELOAD;
    NVIC_EnableIRQ(USEC_INTERVAL_IRQ);
    restart_timer(previous_ticks);
}

void us_ticker_free(void)
{
    timer_cmsdk_disable(&USEC_TIMER_DEV);
}

uint32_t us_ticker_read(void)
{
    core_util_critical_section_enter();
    update_ticker();
    core_util_critical_section_exit();

    return (uint32_t)(total_ticks >> USEC_REPORTED_SHIFT);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t reload = (timestamp - us_ticker_read()) << USEC_REPORTED_SHIFT;
    previous_ticks = reload;
    restart_timer(previous_ticks);
}

void us_ticker_disable_interrupt(void)
{
    timer_cmsdk_disable_interrupt(&USEC_TIMER_DEV);
}

void us_ticker_clear_interrupt(void)
{
    timer_cmsdk_clear_interrupt(&USEC_TIMER_DEV);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(USEC_INTERVAL_IRQ);
}

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        USEC_REPORTED_FREQ_HZ,
        USEC_REPORTED_BITS
    };
    return &info;
}

#ifndef usec_interval_irq_handler
#error "usec_interval_irq_handler should be defined, check device_cfg.h!"
#endif
void usec_interval_irq_handler(void)
{
    update_ticker();
    us_ticker_irq_handler();
}
