/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
  * Low-power elapsed time measure and interval timer in micro-secundum,
  * servicing \ref lp_ticker_api.h, using CMSDK Timer1 \ref CMSDK_TIMER1_DEV.
  */

#include <limits.h>

#include "cmsdk_ticker.h"
#include "lp_ticker_api.h"
#include "platform_devices.h"

#if DEVICE_LPTICKER
/**
 * \brief Calculate clocks to us
 *
 * \param[in] tick Number of clock ticks
 *
 * \return Number of usec, relative to the timer frequency,
 *         that a given ammount of ticks equates to.
 */
 static uint32_t convert_tick_to_us(uint32_t tick)
 {
     return (tick / (SystemCoreClock / SEC_TO_USEC_MULTIPLIER));
 }

/**
 * \brief Calculate us to clock ticks
 *
 * \param[in] us Time to convert to clock ticks
 *
 * \return Number of clock ticks relative to the timer frequency,
 *         that a given period of usec equates to.
 */
 static uint32_t convert_us_to_tick(uint32_t us)
 {
     return (us * (SystemCoreClock / SEC_TO_USEC_MULTIPLIER));
 }

static const struct tick_cfg_t cfg =
{
    .timer_driver = &CMSDK_TIMER1_DEV,
    .irq_n = TIMER1_IRQn,
    .interval_callback = &lp_ticker_irq_handler,
    .convert_tick_to_time = &convert_tick_to_us,
    .convert_time_to_tick = &convert_us_to_tick
};

static struct tick_data_t data =
{
    .is_initialized = false,
    .cumulated_time = 0,
    .max_interval_time = 0,
    .reload_time = 0,
    .interval_callback_enabled = false,
    .previous_cumulated_time = 0,
    .previous_elapsed = 0
};

static struct tick_drv_data_t timer_data =
{
    .cfg = &cfg,
    .data = &data
};

void lp_ticker_init(void)
{
    cmsdk_ticker_init(&timer_data);
}

uint32_t lp_ticker_read()
{
    return cmsdk_ticker_read(&timer_data);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    cmsdk_ticker_set_interrupt(&timer_data, timestamp);
}

void lp_ticker_disable_interrupt(void)
{
    cmsdk_ticker_disable_interrupt(&timer_data);
}

void lp_ticker_clear_interrupt(void)
{
    cmsdk_ticker_clear_interrupt(&timer_data);
}

void lp_ticker_fire_interrupt(void)
{
    cmsdk_ticker_fire_interrupt(&timer_data);
}

void TIMER1_IRQHandler(void)
{
    cmsdk_ticker_irq_handler(&timer_data);
}

#endif
