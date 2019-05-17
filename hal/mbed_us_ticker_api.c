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

#include <stddef.h>
#include "platform/mbed_atomic.h"
#include "hal/us_ticker_api.h"

#if DEVICE_USTICKER

static ticker_event_queue_t events = { 0 };

static ticker_irq_handler_type irq_handler = ticker_irq_handler;

#if MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT

// If we are initializing at boot, we want the timer to be
// always-on, so we block any attempt to free it. We do need
// to pass through init(), as that needs to reset pending
// interrupts.
static void block_us_ticker_free()
{
}

#else // MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT

bool _us_ticker_initialized;

// If we are not initializing at boot, we want to track
// whether the timer has been initialized. This permits
// a fast path for wait_us.
static void note_us_ticker_init()
{
    us_ticker_init();
    core_util_atomic_store_bool(&_us_ticker_initialized, true);
}

static void note_us_ticker_free()
{
    core_util_atomic_store_bool(&_us_ticker_initialized, false);
    us_ticker_free();
}

#endif // MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT

static const ticker_interface_t us_interface = {
#if MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT
    .init = us_ticker_init,
#else
    .init = note_us_ticker_init,
#endif
    .read = us_ticker_read,
    .disable_interrupt = us_ticker_disable_interrupt,
    .clear_interrupt = us_ticker_clear_interrupt,
    .set_interrupt = us_ticker_set_interrupt,
    .fire_interrupt = us_ticker_fire_interrupt,
    .get_info = us_ticker_get_info,
#if MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT
    .free = block_us_ticker_free,
#else
    .free = note_us_ticker_free,
#endif
    .runs_in_deep_sleep = false,
};

static const ticker_data_t us_data = {
    .interface = &us_interface,
    .queue = &events
};

const ticker_data_t *get_us_ticker_data(void)
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

#else

const ticker_data_t *get_us_ticker_data(void)
{
    return NULL;
}

#endif  // DEVICE_USTICKER
