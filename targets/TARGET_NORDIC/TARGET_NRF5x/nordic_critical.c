/*
 * Copyright (c) 2015-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <stdint.h>
#include "hal/critical_section_api.h"
#include "platform/mbed_assert.h"
#include "mbed_nrf_sdk_adaptation.h"

/* all these variables are only modified from inside the critical section,
 * whether it be  __disable_irq or sd_nvic_critical_region_enter.
 *
 * It is safe to read state_saved as a check of critical section state, as
 * it must always read as false from outside the critical section. The other
 * variables are only valid when state_saved is true (which indicates we're
 * in a critical section).
 *
 * Note that `entered_using_softdevice` is only a slight optimisation, on the
 * assumption that storing this locally is likey faster than calling
 * mbed_softdevice_is_enabled(). We still have to assume that the softdevice
 * can't become enabled or disabled during a critical section, so this isn't
 * to get a different answer from making the call.
 */
static bool state_saved;                    // have entered, and written other state
static bool critical_interrupts_enabled;    // interrupts were enabled when we entered (ie state to restore on exit)
static bool entered_using_softdevice;       // we used softdevice when entering

static bool are_interrupts_enabled(void)
{
    return (__get_PRIMASK() & 0x1) == 0;
}

/* Standard routines are copied from mbed_critical_section_api.c */
static void standard_critical_section_enter(void)
{
    const bool interrupt_state = are_interrupts_enabled();

    __disable_irq();

    if (state_saved) {
        return;
    }

    critical_interrupts_enabled = interrupt_state;
    entered_using_softdevice = false;
    state_saved = true;
}

static void standard_critical_section_exit(void)
{
    // Interrupts must be disabled on invoking an exit from a critical section
    MBED_ASSERT(!are_interrupts_enabled());
    state_saved = false;

    // Restore the IRQs to their state prior to entering the critical section
    if (critical_interrupts_enabled) {
        __enable_irq();
    }
}

/* Behaviour here carefully mirrors standard version - noting and restoring
 * original state. */
static void nordic_nvic_critical_region_enter(void)
{
    uint8_t nested;

#ifdef MBED_DEBUG
    uint32_t sd_result = sd_nvic_critical_region_enter(&nested);
    // Will likely recurse, as assert failure will try to take critical region
    MBED_ASSERT(sd_result == NRF_SUCCESS);
#else
    (void) sd_nvic_critical_region_enter(&nested);
#endif

    if (state_saved) {
        return;
    }

    critical_interrupts_enabled = !nested;
    entered_using_softdevice = true;
    state_saved = true;
}

static void nordic_nvic_critical_region_exit(void)
{
    state_saved = false;

#ifdef MBED_DEBUG
    uint32_t sd_result = sd_nvic_critical_region_exit(!critical_interrupts_enabled);
    MBED_ASSERT(sd_result == NRF_SUCCESS);
#else
    (void) sd_nvic_critical_region_exit(!critical_interrupts_enabled);
#endif
}

void hal_critical_section_enter()
{
    if (mbed_softdevice_is_enabled()) {
        nordic_nvic_critical_region_enter();
    } else {
        standard_critical_section_enter();
    }
}

void hal_critical_section_exit()
{
    MBED_ASSERT(state_saved);
#ifdef MBED_DEBUG
    MBED_ASSERT(entered_using_softdevice == mbed_softdevice_is_enabled());
#endif
    if (entered_using_softdevice) {
        nordic_nvic_critical_region_exit();
    } else {
        standard_critical_section_exit();
    }
}

bool hal_in_critical_section(void)
{
    return state_saved;
}

