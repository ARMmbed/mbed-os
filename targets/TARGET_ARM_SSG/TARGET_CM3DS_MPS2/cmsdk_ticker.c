/*
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file cmsdk_ticker.c
 * Two abstracted functionalities for CMSDK APB Timers
 *   1. Measure elapsed time
 *   2. Timer interval interrupt
 *
 *   Passed \ref tick_drv_data_t should be initialized by the caller
 *   for using these services accordingly.
 *   See details \ref tick_cfg_t and \ref tick_data_t.
 */

#include "cmsdk_ticker.h"

void cmsdk_ticker_init(const struct tick_drv_data_t* timer_data)
{
    if (!timer_data->data->is_initialized) {
        timer_cmsdk_init(timer_data->cfg->timer_driver);
        timer_cmsdk_set_reload_value(timer_data->cfg->timer_driver,
                                     TIMER_CMSDK_MAX_RELOAD);
        timer_cmsdk_enable_interrupt(timer_data->cfg->timer_driver);
        NVIC_EnableIRQ(timer_data->cfg->irq_n);

        timer_data->data->max_interval_time =
                timer_data->cfg->convert_tick_to_time(TIMER_CMSDK_MAX_RELOAD);
        timer_data->data->reload_time = timer_data->data->max_interval_time;
        timer_data->data->is_initialized = true;
        timer_cmsdk_enable(timer_data->cfg->timer_driver);
    }
}

uint32_t cmsdk_ticker_read(const struct tick_drv_data_t* timer_data)
{
    uint32_t current_elapsed = 0;

    if (!timer_data->data->is_initialized) {
        cmsdk_ticker_init(timer_data);
    }
    current_elapsed = timer_cmsdk_get_elapsed_value(timer_data->cfg->timer_driver);
    /*
     * If for the same reload cycle (ie. cumulated_time is the same) the
     * current elapsed time is lower than the previous one, it means that the
     * timer has wrapped around without the system logging it. To ensure that
     * we are always returning an increasing time in those condition, we return
     * the time perviously read.
     */
    if ((timer_data->data->previous_cumulated_time ==
         timer_data->data->cumulated_time) &&
        (current_elapsed < timer_data->data->previous_elapsed)) {
        current_elapsed = timer_data->data->previous_elapsed;
    }

    timer_data->data->previous_elapsed = current_elapsed;
    timer_data->data->previous_cumulated_time =
                                               timer_data->data->cumulated_time;

    return (timer_data->data->cumulated_time +
            timer_data->cfg->convert_tick_to_time(current_elapsed));
}

void cmsdk_ticker_set_interrupt(const struct tick_drv_data_t* timer_data,
                                uint32_t timestamp)
{
    uint32_t interval = 0;
    uint32_t interval_reload_tick = 0;

    /* Stop before read to avoid race condition with IRQ. */
    timer_cmsdk_disable(timer_data->cfg->timer_driver);
    uint32_t current_time = cmsdk_ticker_read(timer_data);

    timer_data->data->interval_callback_enabled = true;

    /*
     * We always assume that the event is in the future, even if this
     * substraction underflows it is still corect.
     */
    interval = (timestamp - current_time);

    if (interval >= timer_data->data->max_interval_time) {
        /* Event will be in the future but the time is too big: set max */
        interval_reload_tick = TIMER_CMSDK_MAX_RELOAD;
        timer_data->data->reload_time = timer_data->data->max_interval_time;
    } else {
        /* Event will be in the future in a time that can be set */
        interval_reload_tick =
        timer_data->cfg->convert_time_to_tick(interval);
        timer_data->data->reload_time = interval;
    }

    /* Store the current elapsed time, before reset the timer */
    timer_data->data->cumulated_time = current_time;
    /* Reset the timer with new reload value */
    timer_cmsdk_set_reload_value(timer_data->cfg->timer_driver,
                                 interval_reload_tick);
    timer_cmsdk_reset(timer_data->cfg->timer_driver);

    timer_cmsdk_enable(timer_data->cfg->timer_driver);
}

void cmsdk_ticker_disable_interrupt(const struct tick_drv_data_t* timer_data)
{
    if (!timer_data->data->is_initialized) {
        cmsdk_ticker_init(timer_data);
    }

    timer_data->data->interval_callback_enabled = false;

    /* Stop before read to avoid race condition with IRQ. */
    timer_cmsdk_disable(timer_data->cfg->timer_driver);
    /* If interval interrupt is disabled, restore the default max interval,
     * but save the current elapsed time before changing the timer. */
    timer_data->data->cumulated_time = cmsdk_ticker_read(timer_data);
    /* Reset the timer with default reload value */
    timer_cmsdk_set_reload_value(timer_data->cfg->timer_driver,
                                 TIMER_CMSDK_MAX_RELOAD);
    timer_data->data->reload_time = timer_data->data->max_interval_time;

    timer_cmsdk_reset(timer_data->cfg->timer_driver);
    timer_cmsdk_enable(timer_data->cfg->timer_driver);
}

void cmsdk_ticker_clear_interrupt(const struct tick_drv_data_t* timer_data)
{
    timer_cmsdk_clear_interrupt(timer_data->cfg->timer_driver);
}

void cmsdk_ticker_fire_interrupt(const struct tick_drv_data_t* timer_data)
{
    NVIC_SetPendingIRQ(timer_data->cfg->irq_n);
}

void cmsdk_ticker_irq_handler(const struct tick_drv_data_t* timer_data)
{
    uint32_t reload_val = 0;
    /* If timer's internal interrupt status is not active, then not overflow,
     * but explicit interrupt request was fired by cmsdk_ticker_fire_interrupt.
     */
    if (timer_cmsdk_is_interrupt_active(timer_data->cfg->timer_driver)) {
    /* 1. Calculate cumulated time by overflow */
        timer_cmsdk_clear_interrupt(timer_data->cfg->timer_driver);
        reload_val = timer_cmsdk_get_reload_value(timer_data->cfg->timer_driver);
        timer_data->data->cumulated_time +=
                timer_data->cfg->convert_tick_to_time(reload_val);
    }

    /* 2. Call mbed interval interrupt handler if it's required */
    if (timer_data->data->interval_callback_enabled) {
        cmsdk_ticker_disable_interrupt(timer_data);
        timer_data->cfg->interval_callback();
    }
}
