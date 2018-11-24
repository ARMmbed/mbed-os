/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 Arm Limited
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
 * Supports the Low Power Ticker for mbed by implementing \ref lp_ticker_api.h,
 * using both timers in a CMSDK Dual Timer \ref dualtimer_cmsdk_dev_t.
 */

#include "dualtimer_cmsdk_drv.h"
#include "lp_ticker_api.h"
#include "platform_devices.h"

static uint32_t last_read = 0;

/* Initializes both timers in the Dualtimer:
 *      -Timer1 as the free running timer
 *      -Timer2 as the interval timer
 * Both timers are set:
 *      -32 bit size
 *      -256 Clk divisor
 *      -Maximum reload value
 */
void lp_ticker_init(void)
{
    dualtimer_cmsdk_init(&LP_TIMER_DEV);
    dualtimer_cmsdk_set_size_both_timers(&LP_TIMER_DEV,
                                         DUALTIMER_CMSDK_SIZE_32BIT);
    dualtimer_cmsdk_set_prescale_both_timers(&LP_TIMER_DEV,
                                        DUALTIMER_CMSDK_CLOCK_DIV256);
    dualtimer_cmsdk_disable_both_timers(&LP_TIMER_DEV);
    dualtimer_cmsdk_set_reload_both_timers(&LP_TIMER_DEV,
                                 TIMER_CMSDK_MAX_RELOAD);
    dualtimer_cmsdk_reset_both_timers(&LP_TIMER_DEV);
    NVIC_EnableIRQ(LP_INTERVAL_IRQ);
    dualtimer_cmsdk_enable_both_timers(&LP_TIMER_DEV);
}

void lp_ticker_free(void)
{
    dualtimer_cmsdk_free(&LP_TIMER_DEV);
}

uint32_t lp_ticker_read()
{
    uint32_t tick = dualtimer_cmsdk_get_elapsed_value_timer1(&LP_TIMER_DEV);
    last_read = tick >> LP_REPORTED_SHIFT;
    return last_read;
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t reload = (timestamp - last_read) << LP_REPORTED_SHIFT;
    dualtimer_cmsdk_disable_timer2(&LP_TIMER_DEV);
    dualtimer_cmsdk_set_reload_timer2(&LP_TIMER_DEV, reload);
    dualtimer_cmsdk_reset_timer2(&LP_TIMER_DEV);
    dualtimer_cmsdk_enable_interrupt_timer2(&LP_TIMER_DEV);
    dualtimer_cmsdk_enable_timer2(&LP_TIMER_DEV);
}

void lp_ticker_disable_interrupt(void)
{
    dualtimer_cmsdk_disable_interrupt_timer2(&LP_TIMER_DEV);
}

void lp_ticker_clear_interrupt(void)
{
    dualtimer_cmsdk_clear_interrupt_timer2(&LP_TIMER_DEV);
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(LP_INTERVAL_IRQ);
}

const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        LP_REPORTED_FREQ_HZ,
        LP_REPORTED_BITS
    };
    return &info;
}

#ifndef lp_interval_irq_handler
#error "lp_interval_irq_handler should be defined, check device_cfg.h!"
#endif
/* According to mbed's specification, the free running timer shouldn't fire
 * interrupt, since the upper layer polls that and handles the overflow,
 * by cumulating the read values.
 */
void lp_interval_irq_handler(void)
{
    lp_ticker_disable_interrupt();
    lp_ticker_irq_handler();
}
