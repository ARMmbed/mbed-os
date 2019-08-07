/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

// This implementation of the wait functions will be compiled only
// if the RTOS is present. Note that we still use these old
// bare metal versions of wait and wait_ms rather than using
// thread_sleep_for for backwards compatibility. People should
// be prompted to shift via their deprecation.
#ifdef MBED_CONF_RTOS_PRESENT

#include "platform/mbed_wait_api.h"
#include "hal/us_ticker_api.h"
#include "rtos/ThisThread.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_power_mgmt.h"
#include "platform/mbed_error.h"

void wait(float s)
{
    if ((s >= 0.01f)  && core_util_are_interrupts_enabled()) {
        rtos::ThisThread::sleep_for(s * 1000.0f);
        return;
    }

    uint32_t us = (s * 1000000.0f);
    const ticker_data_t *const ticker = get_us_ticker_data();
    uint32_t start = ticker_read(ticker);
    if ((us >= 1000) && core_util_are_interrupts_enabled()) {
        // Use the RTOS to wait for millisecond delays if possible
        sleep_manager_lock_deep_sleep();
        rtos::ThisThread::sleep_for((uint32_t)us / 1000);
        sleep_manager_unlock_deep_sleep();
    }
    // Use busy waiting for sub-millisecond delays, or for the whole
    // interval if interrupts are not enabled
    while ((ticker_read(ticker) - start) < (uint32_t)us);
}

/*  The actual time delay may be up to one timer tick less - 1 msec */
void wait_ms(int ms)
{
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
#if defined(MBED_TRAP_ERRORS_ENABLED) && MBED_TRAP_ERRORS_ENABLED
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_INVALID_OPERATION),
                   "Deprecated behavior: milli-sec delay should not be used in interrupt.\n");
#else
        wait_us(ms * 1000);
#endif
    } else {
        rtos::ThisThread::sleep_for(ms);
    }
}

#endif // #if MBED_CONF_RTOS_PRESENT
