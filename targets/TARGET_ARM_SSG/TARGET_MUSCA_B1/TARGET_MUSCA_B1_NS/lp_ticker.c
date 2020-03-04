/* mbed Microcontroller Library
 * Copyright (c) 2019 Arm Limited
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
 * Low-power elapsed time measure and interval ticker,
 * using General Purpose Timer \ref timer_gp_dev_t.
 */

#include "cmsis.h"
#include "device_cfg.h"
#include "timer_gp_drv.h"
#include "lp_ticker_api.h"

/* Check if the module configuration is right. These definitions
   are usually located in device_cfg.h */
#ifndef GP_TIMER_DEV
#error "Configuration macro GP_TIMER_DEV is undefined!"
#endif

void lp_ticker_init(void)
{
    timer_gp_init(&GP_TIMER_DEV);
    NVIC_EnableIRQ(GP_TIMER_ALARM0_IRQ);
    timer_gp_interrupt_disable(&GP_TIMER_DEV, GP_TIMER_ALARM_NR);
}

void lp_ticker_free(void)
{
   /* This function should stop the ticker from counting, but GP Timer cannot
    * be stopped.
    */
    timer_gp_interrupt_disable(&GP_TIMER_DEV, GP_TIMER_ALARM_NR);
}

uint32_t lp_ticker_read(void)
{
    return timer_gp_get_counter(&GP_TIMER_DEV);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    timer_gp_set_alarm_value(&GP_TIMER_DEV, GP_TIMER_ALARM_NR, (uint32_t)timestamp);
    timer_gp_interrupt_enable(&GP_TIMER_DEV, GP_TIMER_ALARM_NR);
}

void lp_ticker_disable_interrupt(void)
{
    timer_gp_interrupt_disable(&GP_TIMER_DEV, GP_TIMER_ALARM_NR);
}

void lp_ticker_clear_interrupt(void)
{
    timer_gp_interrupt_clear(&GP_TIMER_DEV, GP_TIMER_ALARM_NR);
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(GP_TIMER_ALARM0_IRQ);
}

const ticker_info_t* lp_ticker_get_info(void)
{
    static const ticker_info_t info = {
        GP_TIMER_FREQ_HZ,
        GP_TIMER_BIT_WIDTH
    };
    return &info;
}

#ifndef GP_TIMER_IRQ0_HANDLER
#error "GP_TIMER_IRQ0_HANDLER should be defined, check device_cfg.h!"
#endif
void GP_TIMER_IRQ0_HANDLER(void)
{
    lp_ticker_irq_handler();
}
