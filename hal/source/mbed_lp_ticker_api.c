/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#include "hal/lp_ticker_api.h"
#include "hal/mbed_lp_ticker_wrapper.h"

#if DEVICE_LPTICKER

static ticker_event_queue_t events = { 0 };

static ticker_irq_handler_type irq_handler = ticker_irq_handler;

static const ticker_interface_t lp_interface = {
    .init = lp_ticker_init,
    .read = lp_ticker_read,
    .disable_interrupt = lp_ticker_disable_interrupt,
    .clear_interrupt = lp_ticker_clear_interrupt,
    .set_interrupt = lp_ticker_set_interrupt,
    .fire_interrupt = lp_ticker_fire_interrupt,
    .get_info = lp_ticker_get_info,
    .free = lp_ticker_free,
    .runs_in_deep_sleep = true,
};

static const ticker_data_t lp_data = {
    .interface = &lp_interface,
    .queue = &events,
};

const ticker_data_t *get_lp_ticker_data(void)
{
#if LPTICKER_DELAY_TICKS > 0
    return get_lp_ticker_wrapper_data(&lp_data);
#else
    return &lp_data;
#endif
}

ticker_irq_handler_type set_lp_ticker_irq_handler(ticker_irq_handler_type ticker_irq_handler)
{
    ticker_irq_handler_type prev_irq_handler = irq_handler;

    irq_handler = ticker_irq_handler;

    return prev_irq_handler;
}

void lp_ticker_irq_handler(void)
{
#if LPTICKER_DELAY_TICKS > 0
    lp_ticker_wrapper_irq_handler(irq_handler);
#else
    if (irq_handler) {
        irq_handler(&lp_data);
    }
#endif
}

#endif
