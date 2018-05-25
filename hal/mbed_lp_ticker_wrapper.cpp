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
#include "hal/lp_ticker_api.h"

#if DEVICE_LPTICKER && (LPTICKER_DELAY_TICKS > 0)

#include "Timeout.h"
#include "mbed_critical.h"

static const timestamp_t min_delta = LPTICKER_DELAY_TICKS;

static bool init = false;
static bool pending = false;
static bool timeout_pending = false;
static timestamp_t last_set_interrupt = 0;
static timestamp_t last_request = 0;
static timestamp_t next = 0;

static timestamp_t mask;
static timestamp_t reschedule_us;

// Do not use SingletonPtr since this must be initialized in a critical section
static mbed::Timeout *timeout;
static uint64_t timeout_data[sizeof(mbed::Timeout) / 8];

/**
 * Initialize variables
 */
static void init_local()
{
    MBED_ASSERT(core_util_in_critical_section());

    const ticker_info_t* info = lp_ticker_get_info();
    if (info->bits >= 32) {
        mask = 0xffffffff;
    } else {
        mask = ((uint64_t)1 << info->bits) - 1;
    }

    // Round us_per_tick up
    timestamp_t us_per_tick = (1000000 + info->frequency - 1) / info->frequency;

    // Add 1 tick to the min delta for the case where the clock transitions after you read it
    // Add 4 microseconds to round up the micro second ticker time (which has a frequency of at least 250KHz - 4us period)
    reschedule_us = (min_delta + 1) * us_per_tick + 4;

    timeout = new (timeout_data) mbed::Timeout();
}

/**
 * Call lp_ticker_set_interrupt with a value that is guaranteed to fire
 *
 * Assumptions
 * -Only one low power clock tick can pass from the last read (last_read)
 * -The closest an interrupt can fire is max_delta + 1
 *
 * @param last_read The last value read from lp_ticker_read
 * @param timestamp The timestamp to trigger the interrupt at
 */
static void set_interrupt_safe(timestamp_t last_read, timestamp_t timestamp)
{
    MBED_ASSERT(core_util_in_critical_section());
    uint32_t delta = (timestamp - last_read) & mask;
    if (delta < min_delta + 2) {
        timestamp = (last_read + min_delta + 2) & mask;
    }
    lp_ticker_set_interrupt(timestamp);
}

/**
 * Set the low power ticker match time when hardware is ready
 *
 * This event is scheduled to set the lp timer after the previous write
 * has taken effect and it is safe to write a new value without blocking.
 * If the time has already passed then this function fires and interrupt
 * immediately.
 */
static void set_interrupt_later()
{
    core_util_critical_section_enter();

    timestamp_t current = lp_ticker_read();
    if (_ticker_match_interval_passed(last_request, current, next)) {
        lp_ticker_fire_interrupt();
    } else {
        set_interrupt_safe(current, next);
        last_set_interrupt = lp_ticker_read();
    }
    timeout_pending = false;

    core_util_critical_section_exit();
}

/**
 * Wrapper around lp_ticker_set_interrupt to prevent blocking
 *
 * Problems this function is solving:
 * 1. Interrupt may not fire if set earlier than LPTICKER_DELAY_TICKS low power clock cycles
 * 2. Setting the interrupt back-to-back will block
 *
 * This wrapper function prevents lp_ticker_set_interrupt from being called
 * back-to-back and blocking while the first write is in progress. This function
 * avoids that problem by scheduling a timeout event if the lp ticker is in the
 * middle of a write operation.
 *
 * @param timestamp Time to call ticker irq
 * @note this is a utility function and it's not required part of HAL implementation
 */
extern "C" void lp_ticker_set_interrupt_wrapper(timestamp_t timestamp)
{
    core_util_critical_section_enter();

    if (!init) {
        init_local();
        init = true;
    }

    timestamp_t current = lp_ticker_read();
    if (pending) {
        // Check if pending should be cleared
        if (((current - last_set_interrupt) & mask) >= min_delta) {
            pending = false;
        }
    }

    if (pending || timeout_pending) {
        next = timestamp;
        last_request = current;
        if (!timeout_pending) {
            timeout->attach_us(set_interrupt_later, reschedule_us);
            timeout_pending = true;
        }
    } else {
        // Schedule immediately if nothing is pending
        set_interrupt_safe(current, timestamp);
        last_set_interrupt = lp_ticker_read();
        pending = true;
    }

    core_util_critical_section_exit();
}

#endif
