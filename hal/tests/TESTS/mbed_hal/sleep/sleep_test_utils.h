/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

/**
 * @addtogroup hal_sleep
 * @{
 * @defgroup hal_sleep_test_util Tests
 * Tests of the sleep HAL.
 * @{
 */

#ifndef MBED_SLEEP_TEST_UTILS_H
#define MBED_SLEEP_TEST_UTILS_H

#include "hal/ticker_api.h"
#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"

/* To prevent a loss of Greentea data, the serial buffers have to be flushed
 * before the UART peripheral shutdown. The UART shutdown happens when the
 * device is entering the deepsleep mode or performing a reset.
 *
 * With the current API, it is not possible to check if the hardware buffers
 * are empty. However, it is possible to determine the time required for the
 * buffers to flush.
 *
 * Assuming the biggest Tx FIFO of 128 bytes (as for CY8CPROTO_062_4343W)
 * and a default UART config (9600, 8N1), flushing the Tx FIFO wold take:
 * (1 start_bit + 8 data_bits + 1 stop_bit) * 128 * 1000 / 9600 = 133.3 ms.
 * To be on the safe side, set the wait time to 150 ms.
 */
#define SERIAL_FLUSH_TIME_MS 150

#define US_PER_S 1000000

unsigned int ticks_to_us(unsigned int ticks, unsigned int freq)
{
    return (unsigned int)((unsigned long long) ticks * US_PER_S / freq);
}

unsigned int us_to_ticks(unsigned int us, unsigned int freq)
{
    return (unsigned int)((unsigned long long) us * freq / US_PER_S);
}

unsigned int overflow_protect(unsigned int timestamp, unsigned int ticker_width)
{
    unsigned int counter_mask = ((1 << ticker_width) - 1);

    return (timestamp & counter_mask);
}

bool compare_timestamps(unsigned int delta_ticks, unsigned int ticker_width, unsigned int expected, unsigned int actual)
{
    const unsigned int counter_mask = ((1 << ticker_width) - 1);

    const unsigned int lower_bound = ((expected - delta_ticks) & counter_mask);
    const unsigned int upper_bound = ((expected + delta_ticks) & counter_mask);

    if (lower_bound < upper_bound) {
        if (actual >= lower_bound && actual <= upper_bound) {
            return true;
        } else {
            return false;
        }
    } else {
        if ((actual >= lower_bound && actual <= counter_mask) || (actual >= 0 && actual <= upper_bound)) {
            return true;
        } else {
            return false;
        }
    }
}

void busy_wait_ms(int ms)
{
    const ticker_info_t *info = us_ticker_get_info();
    uint32_t mask = (1 << info->bits) - 1;
    int delay = (int)((uint64_t) ms * info->frequency / 1000);

    uint32_t prev = us_ticker_read();
    while (delay > 0) {
        uint32_t next = us_ticker_read();
        delay -= (next - prev) & mask;
        prev = next;
    }
}

void us_ticker_isr(const ticker_data_t *const ticker_data)
{
    us_ticker_clear_interrupt();
}

#if DEVICE_LPTICKER
void lp_ticker_isr(const ticker_data_t *const ticker_data)
{
    lp_ticker_clear_interrupt();
}
#endif

#endif

/** @}*/
/** @}*/
