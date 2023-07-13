/* mbed Microcontroller Library
 * Copyright (c) 2023, Arm Limited and affiliates.
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

#include "us_ticker_api.h"
#include "mbed_critical.h"
#include "hardware/timer.h"

#define US_TICKER_COUNTER_BITS        32u
#define US_TICKER_FREQ                1000000

/* us ticker is driven by 1MHz clock and counter length is 32 bits */
const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        US_TICKER_FREQ,
        US_TICKER_COUNTER_BITS
    };
    return &info;
}

static const uint8_t alarm_num = 0;
static const uint8_t irq_num = TIMER_IRQ_0; // IRQ corresponding to the above alarm

static bool us_ticker_inited = false;

static void us_ticker_irq_handler_internal() {
    timer_hw->intr = 1u << alarm_num; // Clear the alarm ISR bit
    us_ticker_irq_handler();
}

void us_ticker_init(void)
{
    if(us_ticker_inited) {
        /* calling init again should cancel current interrupt */
        us_ticker_disable_interrupt();
    }
    else {
        // Note: We don't use the RPi Pico alarm callback mechanism, because it isn't compatible
        // with manually firing the interrupt.  So, we attach a function but then immediately replace
        // the driver IRQ handler with our own.
        hardware_alarm_claim(alarm_num);
        hardware_alarm_set_callback(alarm_num, (hardware_alarm_callback_t)us_ticker_irq_handler_internal);

        irq_remove_handler(irq_num, irq_get_vtable_handler(irq_num));
        irq_set_exclusive_handler(irq_num, &us_ticker_irq_handler_internal);

        us_ticker_inited = true;
    }

}

uint32_t (us_ticker_read)()
{
    return us_ticker_read();
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    core_util_critical_section_enter();

    uint64_t _timestamp = (uint64_t)timestamp;

    if (timestamp < time_us_32()) {
        //32 bit timestamp has been wrapped
        //We need to provide a 64 bit timestamp able to fire the irq for this round
        _timestamp = (((time_us_64() >> 32) + 1) << 32) + timestamp;
    } else {
        //Then, at the next round, wrap the 64 bit timer to follow the 32 bit one
        if ((time_us_64() >> 32) > 0) {
            uint64_t current_time = time_us_64();
            uint64_t wrapped_time = current_time - 0xFFFFFFFF;
            timer_hw->timelw = (uint32_t)wrapped_time;
            timer_hw->timehw = 0;
        }
    }
    absolute_time_t target = { _timestamp };
    hardware_alarm_set_target(alarm_num, target);

    core_util_critical_section_exit();
}

void us_ticker_fire_interrupt(void)
{
    irq_set_pending(irq_num);
}

void us_ticker_disable_interrupt(void)
{
    hardware_alarm_cancel(alarm_num);
}

void us_ticker_clear_interrupt(void)
{
    hardware_alarm_cancel(alarm_num);
}

void us_ticker_free(void)
{
    hardware_alarm_unclaim(alarm_num);
}
