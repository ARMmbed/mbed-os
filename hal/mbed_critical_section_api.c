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
#include "cmsis.h"
#include "hal/critical_section_api.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_toolchain.h"

#include <stdbool.h>

static bool critical_interrupts_enabled = false;
static bool state_saved = false;

static bool are_interrupts_enabled(void)
{
#if defined(__CORTEX_A9)
    return ((__get_CPSR() & 0x80) == 0);
#else
    return ((__get_PRIMASK() & 0x1) == 0);
#endif
}


MBED_WEAK void hal_critical_section_enter(void)
{
    const bool interrupt_state = are_interrupts_enabled();

    __disable_irq();

    if (state_saved == true) {
        return;
    }

    critical_interrupts_enabled = interrupt_state;
    state_saved = true;
}

MBED_WEAK void hal_critical_section_exit(void)
{
    // Interrupts must be disabled on invoking an exit from a critical section
    MBED_ASSERT(!are_interrupts_enabled());
    state_saved = false;

    // Restore the IRQs to their state prior to entering the critical section
    if (critical_interrupts_enabled == true) {
        __enable_irq();
    }
}

MBED_WEAK bool hal_in_critical_section(void)
{
    return (state_saved == true);
}
