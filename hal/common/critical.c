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


#include <stdint.h>
#include <stddef.h>
#include "cmsis.h"
#include "mbed_assert.h"

// Module include
#include "critical.h"

static volatile uint32_t interrupt_enable_counter = 0;
static volatile uint32_t critical_primask = 0;

void core_util_critical_section_enter()
{
    uint32_t primask = __get_PRIMASK(); /* get the current interrupt enabled state */
    __disable_irq();

    /* Save the interrupt enabled state as it was prior to any nested critical section lock use */
    if (!interrupt_enable_counter) {
        critical_primask = primask & 0x1;
    }

    /* If the interrupt_enable_counter overflows or we are in a nested critical section and interrupts
       are enabled, then something has gone badly wrong thus assert an error.
    */
    MBED_ASSERT(interrupt_enable_counter < UINT32_MAX); 
    if (interrupt_enable_counter > 0) {
        MBED_ASSERT(primask & 0x1);
    }
    interrupt_enable_counter++;
}

void core_util_critical_section_exit()
{
    /* If critical_section_enter has not previously been called, do nothing */
    if (interrupt_enable_counter) {

        uint32_t primask = __get_PRIMASK(); /* get the current interrupt enabled state */

        MBED_ASSERT(primask & 0x1); /* Interrupts must be disabled on invoking an exit from a critical section */

        interrupt_enable_counter--;

        /* Only re-enable interrupts if we are exiting the last of the nested critical sections and
           interrupts were enabled on entry to the first critical section.
        */
        if (!interrupt_enable_counter && !critical_primask) {
            __enable_irq();
        }
    }
}
