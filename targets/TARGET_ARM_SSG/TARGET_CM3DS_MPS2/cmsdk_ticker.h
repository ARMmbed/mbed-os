/*
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
 *
 */

/**
 * \file cmsdk_ticker.h
 * CMSDK Ticker implements the functionalities of mbed tickers:
 * 1. Elapsed time measurement
 * 2. Interval interrupt request
 *
 * This ticker service is based on CMSDK APB Timers, abstracting
 * the HAL logic, the timer driver and interrupt number.
 * These parameters should be passed to the functions by
 * an initialized \ref tick_drv_data_t pointer.
 */

#ifndef CMSDK_TICKER_H
#define CMSDK_TICKER_H

#include <stdbool.h>

#include "CM3DS.h"
#include "timer_cmsdk_drv.h"

#define SEC_TO_USEC_MULTIPLIER    1000000U

/**
 * brief Encapsulating struct for config data \ref tick_cfg_t and
 *       the current status \ref tick_data_t.
 */
struct tick_drv_data_t {
    const struct tick_cfg_t* const cfg;
    struct tick_data_t* const data;
};

/**
 * brief Configuration data of the CMSDK ticker
 */
struct tick_cfg_t {
    /** Pointer to the used CMSDK Timer's device structure */
    struct timer_cmsdk_dev_t* const timer_driver;
    /** IRQ number of the used CMSDK Timer */
    const IRQn_Type irq_n;
    /** Interval callback of mbed*/
    void (*const interval_callback)();
    /** Function pointers to call for conversions of clock ticks and defined
     *  time unit.
     *  These conversions define the unit of the measured time.
     */
    uint32_t (*const convert_tick_to_time)(uint32_t tick);
    uint32_t (*const convert_time_to_tick)(uint32_t time);
};

/**
 * brief Current state data of the CMSDK ticker
 */
struct tick_data_t {
    /** True if initialized the ticker, false otherwise */
    bool is_initialized;
    /** Measured elapsed time in the defined unit by
     *  \ref convert_tick_to_time and \ref convert_time_to_tick */
    uint32_t cumulated_time;
    /** Max interval time possible to set, in the defined unit by
     *  \ref convert_tick_to_time and \ref convert_time_to_tick */
    uint32_t max_interval_time;
    /** Current reload time in the defined unit by
     *  \ref convert_tick_to_time and \ref convert_time_to_tick */
    uint32_t reload_time;
    /** Interval IRQ callback is requested */
    bool interval_callback_enabled;
    /** Previous cumulated time calculated for this ticker. Used in the
     *  cmsdk_ticker_read function to detect that the timer has wrapped. */
    uint32_t previous_cumulated_time;
    /** Previous elapsed value for this ticker. Used in the
     *  cmsdk_ticker_read function to detect that the timer has wrapped. */
    uint32_t previous_elapsed;
};

/**
 * \brief Init the CMSDK Ticker
 *
 * \param[in] timer_data Pointer to the used CMSDK Timer's device structure
 */
void cmsdk_ticker_init(const struct tick_drv_data_t* timer_data);

/**
 * \brief Read elapsed time by CMSDK Ticker
 *
 * \param[in] timer_data Pointer to the used CMSDK Timer's device structure
 *
 * \return Elapsed time in the unit defined by \ref convert_tick_to_time
 */

uint32_t cmsdk_ticker_read(const struct tick_drv_data_t* timer_data);

/**
 * \brief Request interval interrupt by time stamp \ref timestamp_t
 *
 * \param[in] timer_data Pointer to the used CMSDK Timer's device structure
 * \param[in] timestamp Absolute time \ref timestamp_t value when the interval
 *                     is requested. Unit of the timestamp is defined by
 *                     \ref convert_tick_to_time and \ref convert_time_to_tick
 */
void cmsdk_ticker_set_interrupt(const struct tick_drv_data_t* timer_data,
                                uint32_t timestamp);

/**
 * \brief Disable interval interrupt
 *
 * \param[in] timer_data Pointer to the used CMSDK Timer's device structure
 */
void cmsdk_ticker_disable_interrupt(const struct tick_drv_data_t* timer_data);

/**
 * \brief Clear interval interrupt
 *
 * \param[in] timer_data Pointer to the used CMSDK Timer's device structure
 */
void cmsdk_ticker_clear_interrupt(const struct tick_drv_data_t* timer_data);

/**
 * \brief Set pending interrupt that should be fired right away.
 *
 * \param[in] timer_data Pointer to the used CMSDK Timer's device structure
 */
void cmsdk_ticker_fire_interrupt(const struct tick_drv_data_t* timer_data);

/**
 * \brief Interrupt handler of the given CMSDK Timer
 *
 * \warning This function may be called from multiple interrupt handlers,
 *          so extra care must be taken for re-entrancy!
 *
 * \param[in] timer_data Pointer to the used CMSDK Timer's device structure
 */
void cmsdk_ticker_irq_handler(const struct tick_drv_data_t* timer_data);
#endif
