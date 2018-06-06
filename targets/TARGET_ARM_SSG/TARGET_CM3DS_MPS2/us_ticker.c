/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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
 * Elapsed time measure and interval timer in micro-secundum,
 * servicing \ref us_ticker_api.h, using CMSDK Timer0 \ref CMSDK_TIMER0_DEV.
 */

#include "cmsdk_ticker.h"
#include "us_ticker_api.h"
#include "platform_devices.h"

/*
 * The CMSDK Ticker counts on 32 bits.
 */
#define CMSDK_TICKER_COUNTER_BITS 32U

/**
 * \brief Pass-through function to make the US ticker HAL only work in the tick
 *        domain. This function is needed by the CMSDK Ticker layer.
 *
 * \param[in] tick Number of clock ticks
 *
 * \return The number of ticks given.
 */
static uint32_t convert_tick_to_us(uint32_t tick)
{
    /* Work only in the tick domain. */
    return tick;
}

/**
 * \brief Pass-through function to make the US ticker HAL only work in the tick
 *        domain. This function is needed by the CMSDK Ticker layer.
 *
 * \param[in] us Number of us
 *
 * \return The number of us given.
 */
static uint32_t convert_us_to_tick(uint32_t us)
{
    /* Work only in the tick domain. */
    return us;
}

static const struct tick_cfg_t cfg =
{
    .timer_driver = &CMSDK_TIMER0_DEV,
    .irq_n = TIMER0_IRQn,
    .interval_callback = &us_ticker_irq_handler,
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

void us_ticker_init(void)
{
    cmsdk_ticker_init(&timer_data);
}

uint32_t us_ticker_read()
{
    return cmsdk_ticker_read(&timer_data);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    cmsdk_ticker_set_interrupt(&timer_data, timestamp);
}

void us_ticker_disable_interrupt(void)
{
    cmsdk_ticker_disable_interrupt(&timer_data);
}

void us_ticker_clear_interrupt(void)
{
    cmsdk_ticker_clear_interrupt(&timer_data);
}

void us_ticker_fire_interrupt(void)
{
    cmsdk_ticker_fire_interrupt(&timer_data);
}

void TIMER0_IRQHandler(void)
{
    cmsdk_ticker_irq_handler(&timer_data);
}

const ticker_info_t* us_ticker_get_info(void)
{
    static ticker_info_t us_ticker_info = {
        .bits = CMSDK_TICKER_COUNTER_BITS
    };

    /*
     * SystemCoreClock is not a constant so it cannot be used to initialize the
     * ticker_info_t structure.
     */
    us_ticker_info.frequency = SystemCoreClock;

    return &us_ticker_info;
}
