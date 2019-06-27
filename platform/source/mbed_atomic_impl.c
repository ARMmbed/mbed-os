/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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

#include "platform/mbed_assert.h"
#include "platform/mbed_atomic.h"
#include "platform/mbed_critical.h"

/* Inline bool implementations in the header use uint8_t versions to manipulate the bool */
MBED_STATIC_ASSERT(sizeof(bool) == sizeof(uint8_t), "Surely bool is a byte");

/* Inline implementations in the header use uint32_t versions to manipulate pointers */
MBED_STATIC_ASSERT(sizeof(void *) == sizeof(uint32_t), "Alas, pointers must be 32-bit");


#define DO_MBED_LOCKED_OP(name, OP, retValue, T, fn_suffix)             \
T core_util_atomic_##name##_##fn_suffix(volatile T *valuePtr, T arg)    \
{                                                                       \
    T oldValue, newValue;                                               \
    core_util_critical_section_enter();                                 \
    oldValue = *valuePtr;                                               \
    newValue = OP;                                                      \
    *valuePtr = newValue;                                               \
    core_util_critical_section_exit();                                  \
    return retValue;                                                    \
}

#define DO_MBED_LOCKED_CAS_OP(T, fn_suffix)                                                     \
bool core_util_atomic_cas_##fn_suffix(volatile T *ptr, T *expectedCurrentValue, T desiredValue) \
{                                                                                               \
    bool success;                                                                               \
    T currentValue;                                                                             \
    core_util_critical_section_enter();                                                         \
    currentValue = *ptr;                                                                        \
    if (currentValue == *expectedCurrentValue) {                                                \
        *ptr = desiredValue;                                                                    \
        success = true;                                                                         \
    } else {                                                                                    \
        *expectedCurrentValue = currentValue;                                                   \
        success = false;                                                                        \
    }                                                                                           \
    core_util_critical_section_exit();                                                          \
    return success;                                                                             \
}                                                                                               \
                                                                                                \
bool core_util_atomic_compare_exchange_weak_##fn_suffix(volatile T *ptr,                        \
        T *expectedCurrentValue, T desiredValue)                                                \
{                                                                                               \
    return core_util_atomic_cas_##fn_suffix(ptr, expectedCurrentValue, desiredValue);           \
}

#if MBED_EXCLUSIVE_ACCESS
/* These are the C99 external definitions for the inline functions */
/* We maintain external definitions rather than using "static inline" for backwards binary compatibility
 * and to give the compiler plenty of leeway to choose to not inline in both C and C++ modes
 */

extern inline bool core_util_atomic_flag_test_and_set(volatile core_util_atomic_flag *flagPtr);

extern inline uint8_t core_util_atomic_exchange_u8(volatile uint8_t *valuePtr, uint8_t newValue);
extern inline uint16_t core_util_atomic_exchange_u16(volatile uint16_t *valuePtr, uint16_t newValue);
extern inline uint32_t core_util_atomic_exchange_u32(volatile uint32_t *valuePtr, uint32_t newValue);
extern inline uint8_t core_util_atomic_incr_u8(volatile uint8_t *valuePtr, uint8_t arg);
extern inline uint16_t core_util_atomic_incr_u16(volatile uint16_t *valuePtr, uint16_t arg);
extern inline uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t arg);
extern inline uint8_t core_util_atomic_decr_u8(volatile uint8_t *valuePtr, uint8_t arg);
extern inline uint16_t core_util_atomic_decr_u16(volatile uint16_t *valuePtr, uint16_t arg);
extern inline uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t arg);
extern inline uint8_t core_util_atomic_fetch_add_u8(volatile uint8_t *valuePtr, uint8_t arg);
extern inline uint16_t core_util_atomic_fetch_add_u16(volatile uint16_t *valuePtr, uint16_t arg);
extern inline uint32_t core_util_atomic_fetch_add_u32(volatile uint32_t *valuePtr, uint32_t arg);
extern inline uint8_t core_util_atomic_fetch_sub_u8(volatile uint8_t *valuePtr, uint8_t arg);
extern inline uint16_t core_util_atomic_fetch_sub_u16(volatile uint16_t *valuePtr, uint16_t arg);
extern inline uint32_t core_util_atomic_fetch_sub_u32(volatile uint32_t *valuePtr, uint32_t arg);
extern inline uint8_t core_util_atomic_fetch_and_u8(volatile uint8_t *valuePtr, uint8_t arg);
extern inline uint16_t core_util_atomic_fetch_and_u16(volatile uint16_t *valuePtr, uint16_t arg);
extern inline uint32_t core_util_atomic_fetch_and_u32(volatile uint32_t *valuePtr, uint32_t arg);
extern inline uint8_t core_util_atomic_fetch_or_u8(volatile uint8_t *valuePtr, uint8_t arg);
extern inline uint16_t core_util_atomic_fetch_or_u16(volatile uint16_t *valuePtr, uint16_t arg);
extern inline uint32_t core_util_atomic_fetch_or_u32(volatile uint32_t *valuePtr, uint32_t arg);
extern inline uint8_t core_util_atomic_fetch_xor_u8(volatile uint8_t *valuePtr, uint8_t arg);
extern inline uint16_t core_util_atomic_fetch_xor_u16(volatile uint16_t *valuePtr, uint16_t arg);
extern inline uint32_t core_util_atomic_fetch_xor_u32(volatile uint32_t *valuePtr, uint32_t arg);
extern inline bool core_util_atomic_cas_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue);
extern inline bool core_util_atomic_cas_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue);
extern inline bool core_util_atomic_cas_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue);
extern inline bool core_util_atomic_compare_exchange_weak_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue);
extern inline bool core_util_atomic_compare_exchange_weak_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue);
extern inline bool core_util_atomic_compare_exchange_weak_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue);

#else

bool core_util_atomic_flag_test_and_set(volatile core_util_atomic_flag *flagPtr)
{
    core_util_critical_section_enter();
    uint8_t currentValue = flagPtr->_flag;
    flagPtr->_flag = true;
    core_util_critical_section_exit();
    return currentValue;
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

/* Now locked operations for whichever we don't have lock-free ones for */
#if MBED_EXCLUSIVE_ACCESS
/* Just need 64-bit locked operations */
#define DO_MBED_LOCKED_OPS(name, OP, retValue) \
    DO_MBED_LOCKED_OP(name, OP, retValue, uint64_t, u64)
#define DO_MBED_LOCKED_CAS_OPS() \
    DO_MBED_LOCKED_CAS_OP(uint64_t, u64)
#else
/* All the operations are locked */
#define DO_MBED_LOCKED_OPS(name, OP, retValue) \
    DO_MBED_LOCKED_OP(name, OP, retValue, uint8_t,  u8) \
    DO_MBED_LOCKED_OP(name, OP, retValue, uint16_t, u16) \
    DO_MBED_LOCKED_OP(name, OP, retValue, uint32_t, u32) \
    DO_MBED_LOCKED_OP(name, OP, retValue, uint64_t, u64)
#define DO_MBED_LOCKED_CAS_OPS() \
    DO_MBED_LOCKED_CAS_OP(uint8_t,  u8) \
    DO_MBED_LOCKED_CAS_OP(uint16_t, u16) \
    DO_MBED_LOCKED_CAS_OP(uint32_t, u32) \
    DO_MBED_LOCKED_CAS_OP(uint64_t, u64)
#endif

// *INDENT-OFF*
DO_MBED_LOCKED_OPS(exchange,  arg,            oldValue)
DO_MBED_LOCKED_OPS(incr,      oldValue + arg, newValue)
DO_MBED_LOCKED_OPS(decr,      oldValue - arg, newValue)
DO_MBED_LOCKED_OPS(fetch_add, oldValue + arg, oldValue)
DO_MBED_LOCKED_OPS(fetch_sub, oldValue - arg, oldValue)
DO_MBED_LOCKED_OPS(fetch_and, oldValue & arg, oldValue)
DO_MBED_LOCKED_OPS(fetch_or,  oldValue | arg, oldValue)
DO_MBED_LOCKED_OPS(fetch_xor, oldValue ^ arg, oldValue)
DO_MBED_LOCKED_CAS_OPS()
// *INDENT-ON*

/* Similar functions for s32 etc are static inline, but these are extern inline for legacy binary compatibility */
extern inline void *core_util_atomic_exchange_ptr(void *volatile *valuePtr, void *desiredValue);
extern inline void *core_util_atomic_incr_ptr(void *volatile *valuePtr, ptrdiff_t delta);
extern inline void *core_util_atomic_decr_ptr(void *volatile *valuePtr, ptrdiff_t delta);
extern inline bool core_util_atomic_cas_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue);
