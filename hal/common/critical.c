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

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <stddef.h>
#include "cmsis.h"
#include "mbed_assert.h"

// Module include
#include "critical.h"

#define EXCLUSIVE_ACCESS (!defined (__CORTEX_M0) && !defined (__CORTEX_M0PLUS))

static volatile uint32_t interrupt_enable_counter = 0;
static volatile bool critical_interrupts_disabled = false;

bool core_util_are_interrupts_enabled(void)
{
#if defined(__CORTEX_A9)
    return ((__get_CPSR() & 0x80) == 0);
#else
    return ((__get_PRIMASK() & 0x1) == 0);
#endif
}

void core_util_critical_section_enter()
{
    bool interrupts_disabled = !core_util_are_interrupts_enabled();
    __disable_irq();

    /* Save the interrupt disabled state as it was prior to any nested critical section lock use */
    if (!interrupt_enable_counter) {
        critical_interrupts_disabled = interrupts_disabled;
    }

    /* If the interrupt_enable_counter overflows or we are in a nested critical section and interrupts
       are enabled, then something has gone badly wrong thus assert an error.
    */
    MBED_ASSERT(interrupt_enable_counter < UINT32_MAX); 
// FIXME
#ifndef   FEATURE_UVISOR
    if (interrupt_enable_counter > 0) {
        MBED_ASSERT(interrupts_disabled);
    }
#else
#warning "core_util_critical_section_enter needs fixing to work from unprivileged code"
#endif /* FEATURE_UVISOR */
    interrupt_enable_counter++;
}

void core_util_critical_section_exit()
{
    /* If critical_section_enter has not previously been called, do nothing */
    if (interrupt_enable_counter) {

// FIXME
#ifndef   FEATURE_UVISOR
        bool interrupts_disabled = !core_util_are_interrupts_enabled(); /* get the current interrupt disabled state */

        MBED_ASSERT(interrupts_disabled); /* Interrupts must be disabled on invoking an exit from a critical section */
#else
#warning "core_util_critical_section_exit needs fixing to work from unprivileged code"
#endif /* FEATURE_UVISOR */

        interrupt_enable_counter--;

        /* Only re-enable interrupts if we are exiting the last of the nested critical sections and
           interrupts were enabled on entry to the first critical section.
        */
        if (!interrupt_enable_counter && !critical_interrupts_disabled) {
            __enable_irq();
        }
    }
}

#if EXCLUSIVE_ACCESS

bool core_util_atomic_cas_u8(uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue)
{
    uint8_t currentValue = __LDREXB((volatile uint8_t*)ptr);
    if (currentValue != *expectedCurrentValue) {
        *expectedCurrentValue = currentValue;
        __CLREX();
        return false;
    }

    return !__STREXB(desiredValue, (volatile uint8_t*)ptr);
}

bool core_util_atomic_cas_u16(uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue)
{
    uint16_t currentValue = __LDREXH((volatile uint16_t*)ptr);
    if (currentValue != *expectedCurrentValue) {
        *expectedCurrentValue = currentValue;
        __CLREX();
        return false;
    }

    return !__STREXH(desiredValue, (volatile uint16_t*)ptr);
}


bool core_util_atomic_cas_u32(uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue)
{
    uint32_t currentValue = __LDREXW((volatile uint32_t*)ptr);
    if (currentValue != *expectedCurrentValue) {
        *expectedCurrentValue = currentValue;
        __CLREX();
        return false;
    }

    return !__STREXW(desiredValue, (volatile uint32_t*)ptr);
}

uint8_t core_util_atomic_incr_u8(uint8_t * valuePtr, uint8_t delta)
{
    uint8_t newValue;
    do {
        newValue = __LDREXB((volatile uint8_t*)valuePtr) + delta;
    } while (__STREXB(newValue, (volatile uint8_t*)valuePtr));
    return newValue;
}

uint16_t core_util_atomic_incr_u16(uint16_t * valuePtr, uint16_t delta)
{
    uint16_t newValue;
    do {
        newValue = __LDREXH((volatile uint16_t*)valuePtr) + delta;
    } while (__STREXH(newValue, (volatile uint16_t*)valuePtr));
    return newValue;
}

uint32_t core_util_atomic_incr_u32(uint32_t * valuePtr, uint32_t delta)
{
    uint32_t newValue;
    do {
        newValue = __LDREXW((volatile uint32_t*)valuePtr) + delta;
    } while (__STREXW(newValue, (volatile uint32_t*)valuePtr));
    return newValue;
}


uint8_t core_util_atomic_decr_u8(uint8_t * valuePtr, uint8_t delta)
{
    uint8_t newValue;
    do {
        newValue = __LDREXB((volatile uint8_t*)valuePtr) - delta;
    } while (__STREXB(newValue, (volatile uint8_t*)valuePtr));
    return newValue;
}

uint16_t core_util_atomic_decr_u16(uint16_t * valuePtr, uint16_t delta)
{
    uint16_t newValue;
    do {
        newValue = __LDREXH((volatile uint16_t*)valuePtr) - delta;
    } while (__STREXH(newValue, (volatile uint16_t*)valuePtr));
    return newValue;
}

uint32_t core_util_atomic_decr_u32(uint32_t * valuePtr, uint32_t delta)
{
    uint32_t newValue;
    do {
        newValue = __LDREXW((volatile uint32_t*)valuePtr) - delta;
    } while (__STREXW(newValue, (volatile uint32_t*)valuePtr));
    return newValue;
}

#else

bool core_util_atomic_cas_u8(uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue)
{
    bool success;
    uint8_t currentValue;
    core_util_critical_section_enter();
    currentValue = *ptr;
    if (currentValue == *expectedCurrentValue) {
        *ptr = desiredValue;
        success = true;
    } else {
        *expectedCurrentValue = currentValue;
        success = false;
    }
    core_util_critical_section_exit();
    return success;
}

bool core_util_atomic_cas_u16(uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue)
{
    bool success;
    uint16_t currentValue;
    core_util_critical_section_enter();
    currentValue = *ptr;
    if (currentValue == *expectedCurrentValue) {
        *ptr = desiredValue;
        success = true;
    } else {
        *expectedCurrentValue = currentValue;
        success = false;
    }
    core_util_critical_section_exit();
    return success;
}


bool core_util_atomic_cas_u32(uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue)
{
    bool success;
    uint32_t currentValue;
    core_util_critical_section_enter();
    currentValue = *ptr;
    if (currentValue == *expectedCurrentValue) {
        *ptr = desiredValue;
        success = true;
    } else {
        *expectedCurrentValue = currentValue;
        success = false;
    }
    core_util_critical_section_exit();
    return success;
}

uint8_t core_util_atomic_incr_u8(uint8_t * valuePtr, uint8_t delta)
{
    uint8_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr + delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint16_t core_util_atomic_incr_u16(uint16_t * valuePtr, uint16_t delta)
{
    uint16_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr + delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint32_t core_util_atomic_incr_u32(uint32_t * valuePtr, uint32_t delta)
{
    uint32_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr + delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}


uint8_t core_util_atomic_decr_u8(uint8_t * valuePtr, uint8_t delta)
{
    uint8_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr - delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint16_t core_util_atomic_decr_u16(uint16_t * valuePtr, uint16_t delta)
{
    uint16_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr - delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint32_t core_util_atomic_decr_u32(uint32_t * valuePtr, uint32_t delta)
{
    uint32_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr - delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

#endif

