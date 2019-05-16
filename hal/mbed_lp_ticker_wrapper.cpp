/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "hal/mbed_lp_ticker_wrapper.h"

#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)

#include "hal/LowPowerTickerWrapper.h"
#include "platform/mbed_critical.h"

// Do not use SingletonPtr since this must be initialized in a critical section
static LowPowerTickerWrapper *ticker_wrapper;
static uint64_t ticker_wrapper_data[(sizeof(LowPowerTickerWrapper) + 7) / 8];
static bool init = false;

static void lp_ticker_wrapper_init()
{
    ticker_wrapper->init();
}

static uint32_t lp_ticker_wrapper_read()
{
    return ticker_wrapper->read();
}

static void lp_ticker_wrapper_set_interrupt(timestamp_t timestamp)
{
    ticker_wrapper->set_interrupt(timestamp);
}

static void lp_ticker_wrapper_disable_interrupt()
{
    ticker_wrapper->disable_interrupt();
}

static void lp_ticker_wrapper_clear_interrupt()
{
    ticker_wrapper->clear_interrupt();
}

static void lp_ticker_wrapper_fire_interrupt()
{
    ticker_wrapper->fire_interrupt();
}

static const ticker_info_t *lp_ticker_wrapper_get_info()
{
    return ticker_wrapper->get_info();
}

static void lp_ticker_wrapper_free()
{
    ticker_wrapper->free();
}

static const ticker_interface_t lp_interface = {
    lp_ticker_wrapper_init,
    lp_ticker_wrapper_read,
    lp_ticker_wrapper_disable_interrupt,
    lp_ticker_wrapper_clear_interrupt,
    lp_ticker_wrapper_set_interrupt,
    lp_ticker_wrapper_fire_interrupt,
    lp_ticker_wrapper_free,
    lp_ticker_wrapper_get_info,
    true
};

void lp_ticker_wrapper_irq_handler(ticker_irq_handler_type handler)
{
    core_util_critical_section_enter();

    if (!init) {
        // Force ticker to initialize
        get_lp_ticker_data();
    }

    ticker_wrapper->irq_handler(handler);

    core_util_critical_section_exit();
}

const ticker_data_t *get_lp_ticker_wrapper_data(const ticker_data_t *data)
{
    core_util_critical_section_enter();

    if (!init) {
        ticker_wrapper = new (ticker_wrapper_data) LowPowerTickerWrapper(data, &lp_interface, LPTICKER_DELAY_TICKS, LPTICKER_DELAY_TICKS);
        init = true;
    }

    core_util_critical_section_exit();

    return &ticker_wrapper->data;
}

void lp_ticker_wrapper_suspend()
{
    if (!init) {
        // Force ticker to initialize
        get_lp_ticker_data();
    }

    ticker_wrapper->suspend();
}

void lp_ticker_wrapper_resume()
{
    if (!init) {
        // Force ticker to initialize
        get_lp_ticker_data();
    }

    ticker_wrapper->resume();
}

#endif
