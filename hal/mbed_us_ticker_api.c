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
#include "hal/us_ticker_api.h"

static ticker_event_queue_t events = { 0 };

static ticker_irq_handler_type irq_handler = ticker_irq_handler;

static const ticker_interface_t us_interface = {
    .init = us_ticker_init,
    .read = us_ticker_read,
    .disable_interrupt = us_ticker_disable_interrupt,
    .clear_interrupt = us_ticker_clear_interrupt,
    .set_interrupt = us_ticker_set_interrupt,
    .fire_interrupt = us_ticker_fire_interrupt,
    .get_info = us_ticker_get_info,
};

static const ticker_data_t us_data = {
    .interface = &us_interface,
    .queue = &events
};

const ticker_data_t* get_us_ticker_data(void)
{
    return &us_data;
}

ticker_irq_handler_type set_us_ticker_irq_handler(ticker_irq_handler_type ticker_irq_handler)
{
    ticker_irq_handler_type prev_irq_handler = irq_handler;

    irq_handler = ticker_irq_handler;

    return prev_irq_handler;
}

void us_ticker_irq_handler(void)
{
    if (irq_handler) {
        irq_handler(&us_data);
    }
}
