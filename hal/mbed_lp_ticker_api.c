/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

#if DEVICE_LPTICKER

void lp_ticker_set_interrupt_wrapper(timestamp_t timestamp);

static ticker_event_queue_t events = { 0 };

static ticker_irq_handler_type irq_handler = ticker_irq_handler;

static const ticker_interface_t lp_interface = {
    .init = lp_ticker_init,
    .read = lp_ticker_read,
    .disable_interrupt = lp_ticker_disable_interrupt,
    .clear_interrupt = lp_ticker_clear_interrupt,
#if LPTICKER_DELAY_TICKS > 0
    .set_interrupt = lp_ticker_set_interrupt_wrapper,
#else
    .set_interrupt = lp_ticker_set_interrupt,
#endif
    .fire_interrupt = lp_ticker_fire_interrupt,
    .get_info = lp_ticker_get_info,
};

static const ticker_data_t lp_data = {
    .interface = &lp_interface,
    .queue = &events,
};

const ticker_data_t* get_lp_ticker_data(void)
{
    return &lp_data;
}

ticker_irq_handler_type set_lp_ticker_irq_handler(ticker_irq_handler_type ticker_irq_handler)
{
    ticker_irq_handler_type prev_irq_handler = irq_handler;

    irq_handler = ticker_irq_handler;

    return prev_irq_handler;
}

void lp_ticker_irq_handler(void)
{
    if (irq_handler) {
        irq_handler(&lp_data);
    }
}

#endif
