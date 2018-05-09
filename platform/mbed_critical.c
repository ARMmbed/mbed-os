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

/* Declare __STDC_LIMIT_MACROS so stdint.h defines UINT32_MAX when using C++ */
#define __STDC_LIMIT_MACROS
#include "hal/critical_section_api.h"

#include "cmsis.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_toolchain.h"

// if __EXCLUSIVE_ACCESS rtx macro not defined, we need to get this via own-set architecture macros
#ifndef MBED_EXCLUSIVE_ACCESS
#ifndef __EXCLUSIVE_ACCESS
#if ((__ARM_ARCH_7M__      == 1U) || \
    (__ARM_ARCH_7EM__     == 1U) || \
    (__ARM_ARCH_8M_BASE__ == 1U) || \
    (__ARM_ARCH_8M_MAIN__ == 1U)) || \
    (__ARM_ARCH_7A__ == 1U)
#define MBED_EXCLUSIVE_ACCESS      1U
#elif (__ARM_ARCH_6M__ == 1U)
#define MBED_EXCLUSIVE_ACCESS      0U
#else
#error "Unknown architecture for exclusive access"
#endif
#else 
#define MBED_EXCLUSIVE_ACCESS __EXCLUSIVE_ACCESS
#endif
#endif

static volatile uint32_t critical_section_reentrancy_counter = 0;

bool core_util_are_interrupts_enabled(void)
{
#if defined(__CORTEX_A9)
    return ((__get_CPSR() & 0x80) == 0);
#else
    return ((__get_PRIMASK() & 0x1) == 0);
#endif
}

bool core_util_is_isr_active(void)
{
#if defined(__CORTEX_A9)
    switch(__get_CPSR() & 0x1FU) {
        case CPSR_M_USR:
        case CPSR_M_SYS:
            return false;
        case CPSR_M_SVC:
        default:
            return true;
    }
#else
    return (__get_IPSR() != 0U);
#endif
}

bool core_util_in_critical_section(void)
{
    return hal_in_critical_section();
}

void core_util_critical_section_enter(void)
{
// FIXME
#ifdef FEATURE_UVISOR
    #warning "core_util_critical_section_enter needs fixing to work from unprivileged code"
#else
    // If the reentrancy counter overflows something has gone badly wrong.
    MBED_ASSERT(critical_section_reentrancy_counter < UINT32_MAX);
#endif /* FEATURE_UVISOR */

    hal_critical_section_enter();

    ++critical_section_reentrancy_counter;
}

void core_util_critical_section_exit(void)
{
// FIXME
#ifdef FEATURE_UVISOR
    #warning "core_util_critical_section_exit needs fixing to work from unprivileged code"
#endif /* FEATURE_UVISOR */

    // If critical_section_enter has not previously been called, do nothing
    if (critical_section_reentrancy_counter == 0) {
        return;
    }

    --critical_section_reentrancy_counter;

    if (critical_section_reentrancy_counter == 0) {
        hal_critical_section_exit();
    }
}

#if MBED_EXCLUSIVE_ACCESS

/* Supress __ldrex and __strex deprecated warnings - "#3731-D: intrinsic is deprecated" */
#if defined (__CC_ARM) 
#pragma diag_suppress 3731
#endif

bool core_util_atomic_cas_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue)
{
    do {
        uint8_t currentValue = __LDREXB(ptr);
        if (currentValue != *expectedCurrentValue) {
            *expectedCurrentValue = currentValue;
            __CLREX();
            return false;
        }
    } while (__STREXB(desiredValue, ptr));
    return true;
}

bool core_util_atomic_cas_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue)
{
    do {
        uint16_t currentValue = __LDREXH(ptr);
        if (currentValue != *expectedCurrentValue) {
            *expectedCurrentValue = currentValue;
            __CLREX();
            return false;
        }
    } while (__STREXH(desiredValue, ptr));
    return true;
}


bool core_util_atomic_cas_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue)
{
    do {
        uint32_t currentValue = __LDREXW(ptr);
        if (currentValue != *expectedCurrentValue) {
            *expectedCurrentValue = currentValue;
            __CLREX();
            return false;
        }
    } while (__STREXW(desiredValue, ptr));
    return true;
}

uint8_t core_util_atomic_incr_u8(volatile uint8_t *valuePtr, uint8_t delta)
{
    uint8_t newValue;
    do {
        newValue = __LDREXB(valuePtr) + delta;
    } while (__STREXB(newValue, valuePtr));
    return newValue;
}

uint16_t core_util_atomic_incr_u16(volatile uint16_t *valuePtr, uint16_t delta)
{
    uint16_t newValue;
    do {
        newValue = __LDREXH(valuePtr) + delta;
    } while (__STREXH(newValue, valuePtr));
    return newValue;
}

uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t delta)
{
    uint32_t newValue;
    do {
        newValue = __LDREXW(valuePtr) + delta;
    } while (__STREXW(newValue, valuePtr));
    return newValue;
}


uint8_t core_util_atomic_decr_u8(volatile uint8_t *valuePtr, uint8_t delta)
{
    uint8_t newValue;
    do {
        newValue = __LDREXB(valuePtr) - delta;
    } while (__STREXB(newValue, valuePtr));
    return newValue;
}

uint16_t core_util_atomic_decr_u16(volatile uint16_t *valuePtr, uint16_t delta)
{
    uint16_t newValue;
    do {
        newValue = __LDREXH(valuePtr) - delta;
    } while (__STREXH(newValue, valuePtr));
    return newValue;
}

uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t delta)
{
    uint32_t newValue;
    do {
        newValue = __LDREXW(valuePtr) - delta;
    } while (__STREXW(newValue, valuePtr));
    return newValue;
}

#else

bool core_util_atomic_cas_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue)
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

bool core_util_atomic_cas_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue)
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


bool core_util_atomic_cas_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue)
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


uint8_t core_util_atomic_incr_u8(volatile uint8_t *valuePtr, uint8_t delta)
{
    uint8_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr + delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint16_t core_util_atomic_incr_u16(volatile uint16_t *valuePtr, uint16_t delta)
{
    uint16_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr + delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t delta)
{
    uint32_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr + delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}


uint8_t core_util_atomic_decr_u8(volatile uint8_t *valuePtr, uint8_t delta)
{
    uint8_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr - delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint16_t core_util_atomic_decr_u16(volatile uint16_t *valuePtr, uint16_t delta)
{
    uint16_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr - delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t delta)
{
    uint32_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr - delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

#endif


bool core_util_atomic_cas_ptr(void * volatile *ptr, void **expectedCurrentValue, void *desiredValue) {
    return core_util_atomic_cas_u32(
            (volatile uint32_t *)ptr,
            (uint32_t *)expectedCurrentValue,
            (uint32_t)desiredValue);
}

void *core_util_atomic_incr_ptr(void * volatile *valuePtr, ptrdiff_t delta) {
    return (void *)core_util_atomic_incr_u32((volatile uint32_t *)valuePtr, (uint32_t)delta);
}

void *core_util_atomic_decr_ptr(void * volatile *valuePtr, ptrdiff_t delta) {
    return (void *)core_util_atomic_decr_u32((volatile uint32_t *)valuePtr, (uint32_t)delta);
}

