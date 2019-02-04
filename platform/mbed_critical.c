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

static uint32_t critical_section_reentrancy_counter = 0;

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
    switch (__get_CPSR() & 0x1FU) {
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
    hal_critical_section_enter();

    // If the reentrancy counter overflows something has gone badly wrong.
    MBED_ASSERT(critical_section_reentrancy_counter < UINT32_MAX);

    ++critical_section_reentrancy_counter;
}

void core_util_critical_section_exit(void)
{

    // If critical_section_enter has not previously been called, do nothing
    if (critical_section_reentrancy_counter == 0) {
        return;
    }

    --critical_section_reentrancy_counter;

    if (critical_section_reentrancy_counter == 0) {
        hal_critical_section_exit();
    }
}

/* Inline bool implementations in the header use uint8_t versions to manipulate the bool */
MBED_STATIC_ASSERT(sizeof(bool) == sizeof(uint8_t), "Surely bool is a byte");

#if MBED_EXCLUSIVE_ACCESS

/* Supress __ldrex and __strex deprecated warnings - "#3731-D: intrinsic is deprecated" */
#if defined (__CC_ARM)
#pragma diag_suppress 3731
#endif

bool core_util_atomic_flag_test_and_set(volatile core_util_atomic_flag *flagPtr)
{
    MBED_BARRIER();
    uint8_t currentValue;
    do {
        currentValue = __LDREXB(&flagPtr->_flag);
    } while (__STREXB(true, &flagPtr->_flag));
    MBED_BARRIER();
    return currentValue;
}

bool core_util_atomic_cas_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue)
{
    MBED_BARRIER();
    do {
        uint8_t currentValue = __LDREXB(ptr);
        if (currentValue != *expectedCurrentValue) {
            *expectedCurrentValue = currentValue;
            __CLREX();
            return false;
        }
    } while (__STREXB(desiredValue, ptr));
    MBED_BARRIER();
    return true;
}

bool core_util_atomic_cas_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue)
{
    MBED_BARRIER();
    do {
        uint16_t currentValue = __LDREXH(ptr);
        if (currentValue != *expectedCurrentValue) {
            *expectedCurrentValue = currentValue;
            __CLREX();
            return false;
        }
    } while (__STREXH(desiredValue, ptr));
    MBED_BARRIER();
    return true;
}


bool core_util_atomic_cas_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue)
{
    MBED_BARRIER();
    do {
        uint32_t currentValue = __LDREXW(ptr);
        if (currentValue != *expectedCurrentValue) {
            *expectedCurrentValue = currentValue;
            __CLREX();
            return false;
        }
    } while (__STREXW(desiredValue, ptr));
    MBED_BARRIER();
    return true;
}

uint8_t core_util_atomic_exchange_u8(volatile uint8_t *valuePtr, uint8_t desiredValue)
{
    MBED_BARRIER();
    uint8_t currentValue;
    do {
        currentValue = __LDREXB(valuePtr);
    } while (__STREXB(desiredValue, valuePtr));
    MBED_BARRIER();
    return currentValue;
}

uint16_t core_util_atomic_exchange_u16(volatile uint16_t *valuePtr, uint16_t desiredValue)
{
    MBED_BARRIER();
    uint16_t currentValue;
    do {
        currentValue = __LDREXH(valuePtr);
    } while (__STREXH(desiredValue, valuePtr));
    MBED_BARRIER();
    return currentValue;
}

uint32_t core_util_atomic_exchange_u32(volatile uint32_t *valuePtr, uint32_t desiredValue)
{
    MBED_BARRIER();
    uint32_t currentValue;
    do {
        currentValue = __LDREXW(valuePtr);
    } while (__STREXW(desiredValue, valuePtr));
    MBED_BARRIER();
    return currentValue;
}

uint8_t core_util_atomic_incr_u8(volatile uint8_t *valuePtr, uint8_t delta)
{
    MBED_BARRIER();
    uint8_t newValue;
    do {
        newValue = __LDREXB(valuePtr) + delta;
    } while (__STREXB(newValue, valuePtr));
    MBED_BARRIER();
    return newValue;
}

uint16_t core_util_atomic_incr_u16(volatile uint16_t *valuePtr, uint16_t delta)
{
    MBED_BARRIER();
    uint16_t newValue;
    do {
        newValue = __LDREXH(valuePtr) + delta;
    } while (__STREXH(newValue, valuePtr));
    MBED_BARRIER();
    return newValue;
}

uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t delta)
{
    MBED_BARRIER();
    uint32_t newValue;
    do {
        newValue = __LDREXW(valuePtr) + delta;
    } while (__STREXW(newValue, valuePtr));
    MBED_BARRIER();
    return newValue;
}


uint8_t core_util_atomic_decr_u8(volatile uint8_t *valuePtr, uint8_t delta)
{
    MBED_BARRIER();
    uint8_t newValue;
    do {
        newValue = __LDREXB(valuePtr) - delta;
    } while (__STREXB(newValue, valuePtr));
    MBED_BARRIER();
    return newValue;
}

uint16_t core_util_atomic_decr_u16(volatile uint16_t *valuePtr, uint16_t delta)
{
    MBED_BARRIER();
    uint16_t newValue;
    do {
        newValue = __LDREXH(valuePtr) - delta;
    } while (__STREXH(newValue, valuePtr));
    MBED_BARRIER();
    return newValue;
}

uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t delta)
{
    MBED_BARRIER();
    uint32_t newValue;
    do {
        newValue = __LDREXW(valuePtr) - delta;
    } while (__STREXW(newValue, valuePtr));
    MBED_BARRIER();
    return newValue;
}

#else

bool core_util_atomic_flag_test_and_set(volatile core_util_atomic_flag *flagPtr)
{
    core_util_critical_section_enter();
    uint8_t currentValue = flagPtr->_flag;
    flagPtr->_flag = true;
    core_util_critical_section_exit();
    return currentValue;
}

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


uint8_t core_util_atomic_exchange_u8(volatile uint8_t *ptr, uint8_t desiredValue)
{
    core_util_critical_section_enter();
    uint8_t currentValue = *ptr;
    *ptr = desiredValue;
    core_util_critical_section_exit();
    return currentValue;
}

uint16_t core_util_atomic_exchange_u16(volatile uint16_t *ptr, uint16_t desiredValue)
{
    core_util_critical_section_enter();
    uint16_t currentValue = *ptr;
    *ptr = desiredValue;
    core_util_critical_section_exit();
    return currentValue;
}

uint32_t core_util_atomic_exchange_u32(volatile uint32_t *ptr, uint32_t desiredValue)
{
    core_util_critical_section_enter();
    uint32_t currentValue = *ptr;
    *ptr = desiredValue;
    core_util_critical_section_exit();
    return currentValue;
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

/* No architecture we support has LDREXD/STREXD, so must always disable IRQs for 64-bit operations */
uint64_t core_util_atomic_load_u64(const volatile uint64_t *valuePtr)
{
    core_util_critical_section_enter();
    uint64_t currentValue = *valuePtr;
    core_util_critical_section_exit();
    return currentValue;
}

void core_util_atomic_store_u64(volatile uint64_t *valuePtr, uint64_t desiredValue)
{
    core_util_critical_section_enter();
    *valuePtr = desiredValue;
    core_util_critical_section_exit();
}

uint64_t core_util_atomic_exchange_u64(volatile uint64_t *valuePtr, uint64_t desiredValue)
{
    core_util_critical_section_enter();
    uint64_t currentValue = *valuePtr;
    *valuePtr = desiredValue;
    core_util_critical_section_exit();
    return currentValue;
}

bool core_util_atomic_cas_u64(volatile uint64_t *ptr, uint64_t *expectedCurrentValue, uint64_t desiredValue)
{
    bool success;
    uint64_t currentValue;
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

uint64_t core_util_atomic_incr_u64(volatile uint64_t *valuePtr, uint64_t delta)
{
    uint64_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr + delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

uint64_t core_util_atomic_decr_u64(volatile uint64_t *valuePtr, uint64_t delta)
{
    uint64_t newValue;
    core_util_critical_section_enter();
    newValue = *valuePtr - delta;
    *valuePtr = newValue;
    core_util_critical_section_exit();
    return newValue;
}

MBED_STATIC_ASSERT(sizeof(void *) == sizeof(uint32_t), "Alas, pointers must be 32-bit");

bool core_util_atomic_cas_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue)
{
    return core_util_atomic_cas_u32(
               (volatile uint32_t *)ptr,
               (uint32_t *)expectedCurrentValue,
               (uint32_t)desiredValue);
}

void *core_util_atomic_exchange_ptr(void *volatile *valuePtr, void *desiredValue)
{
    return (void *)core_util_atomic_exchange_u32((volatile uint32_t *)valuePtr, (uint32_t)desiredValue);
}

void *core_util_atomic_incr_ptr(void *volatile *valuePtr, ptrdiff_t delta)
{
    return (void *)core_util_atomic_incr_u32((volatile uint32_t *)valuePtr, (uint32_t)delta);
}

void *core_util_atomic_decr_ptr(void *volatile *valuePtr, ptrdiff_t delta)
{
    return (void *)core_util_atomic_decr_u32((volatile uint32_t *)valuePtr, (uint32_t)delta);
}

