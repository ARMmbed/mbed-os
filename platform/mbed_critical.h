
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

#ifndef __MBED_UTIL_CRITICAL_H__
#define __MBED_UTIL_CRITICAL_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "mbed_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_critical critical section function
 * @{
 */

/** Determine the current interrupts enabled state
  *
  * This function can be called to determine whether or not interrupts are currently enabled.
  * @note
  * NOTE:
  * This function works for both cortex-A and cortex-M, although the underlying implementation
  * differs.
  * @return true if interrupts are enabled, false otherwise
  */
bool core_util_are_interrupts_enabled(void);

/** Determine if this code is executing from an interrupt
  *
  * This function can be called to determine if the code is running on interrupt context.
  * @note
  * NOTE:
  * This function works for both cortex-A and cortex-M, although the underlying implementation
  * differs.
  * @return true if in an isr, false otherwise
  */
bool core_util_is_isr_active(void);

/** Mark the start of a critical section
  *
  * This function should be called to mark the start of a critical section of code.
  * @note
  * NOTES:
  * 1) The use of this style of critical section is targetted at C based implementations.
  * 2) These critical sections can be nested.
  * 3) The interrupt enable state on entry to the first critical section (of a nested set, or single
  *    section) will be preserved on exit from the section.
  * 4) This implementation will currently only work on code running in privileged mode.
  */
void core_util_critical_section_enter(void);

/** Mark the end of a critical section
  *
  * This function should be called to mark the end of a critical section of code.
  * @note
  * NOTES:
  * 1) The use of this style of critical section is targetted at C based implementations.
  * 2) These critical sections can be nested.
  * 3) The interrupt enable state on entry to the first critical section (of a nested set, or single
  *    section) will be preserved on exit from the section.
  * 4) This implementation will currently only work on code running in privileged mode.
  */
void core_util_critical_section_exit(void);

/**
 * Determine if we are currently in a critical section
 *
 * @return true if in a critical section, false otherwise.
 */
bool core_util_in_critical_section(void);

/**@}*/

/**
 * \defgroup platform_atomic atomic functions
 *
 * Atomic functions function analogously to C11 and C++11 - loads have
 * acquire semantics, stores have release semantics, and atomic operations
 * are sequentially consistent. Atomicity is enforced both between threads and
 * interrupt handlers.
 *
 * @{
 */

/**
 * A lock-free, primitive atomic flag.
 *
 * Emulate C11's atomic_flag. The flag is initially in an indeterminate state
 * unless explicitly initialized with CORE_UTIL_ATOMIC_FLAG_INIT.
 */
typedef struct core_util_atomic_flag {
    uint8_t _flag;
} core_util_atomic_flag;

/**
 * Initializer for a core_util_atomic_flag.
 *
 * Example:
 * ~~~
 *     core_util_atomic_flag in_progress = CORE_UTIL_ATOMIC_FLAG_INIT;
 * ~~~
 */
#define CORE_UTIL_ATOMIC_FLAG_INIT { 0 }

/**
 * Atomic test and set.
 *
 * Atomically tests then sets the flag to true, returning the previous value.
 *
 * @param  flagPtr Target flag being tested and set.
 * @return         The previous value.
 */
bool core_util_atomic_flag_test_and_set(volatile core_util_atomic_flag *flagPtr);

/**
 * Atomic clear.
 *
 * @param  flagPtr Target flag being cleared.
 */
MBED_FORCEINLINE void core_util_atomic_flag_clear(volatile core_util_atomic_flag *flagPtr)
{
    MBED_BARRIER();
    flagPtr->_flag = false;
    MBED_BARRIER();
}

/**
 * Atomic compare and set. It compares the contents of a memory location to a
 * given value and, only if they are the same, modifies the contents of that
 * memory location to a given new value. This is done as a single atomic
 * operation. The atomicity guarantees that the new value is calculated based on
 * up-to-date information; if the value had been updated by another thread in
 * the meantime, the write would fail due to a mismatched expectedCurrentValue.
 *
 * Refer to https://en.wikipedia.org/wiki/Compare-and-set [which may redirect
 * you to the article on compare-and swap].
 *
 * @param  ptr                  The target memory location.
 * @param[in,out] expectedCurrentValue A pointer to some location holding the
 *                              expected current value of the data being set atomically.
 *                              The computed 'desiredValue' should be a function of this current value.
 *                              @note: This is an in-out parameter. In the
 *                              failure case of atomic_cas (where the
 *                              destination isn't set), the pointee of expectedCurrentValue is
 *                              updated with the current value.
 * @param[in] desiredValue      The new value computed based on '*expectedCurrentValue'.
 *
 * @return                      true if the memory location was atomically
 *                              updated with the desired value (after verifying
 *                              that it contained the expectedCurrentValue),
 *                              false otherwise. In the failure case,
 *                              exepctedCurrentValue is updated with the new
 *                              value of the target memory location.
 *
 * pseudocode:
 * function cas(p : pointer to int, old : pointer to int, new : int) returns bool {
 *     if *p != *old {
 *         *old = *p
 *         return false
 *     }
 *     *p = new
 *     return true
 * }
 *
 * @note: In the failure case (where the destination isn't set), the value
 * pointed to by expectedCurrentValue is instead updated with the current value.
 * This property helps writing concise code for the following incr:
 *
 * function incr(p : pointer to int, a : int) returns int {
 *     done = false
 *     value = *p // This fetch operation need not be atomic.
 *     while not done {
 *         done = atomic_cas(p, &value, value + a) // *value gets updated automatically until success
 *     }
 *     return value + a
 * }
 *
 * @note: This corresponds to the C11 "atomic_compare_exchange_strong" - it
 * always succeeds if the current value is expected, as per the pseudocode
 * above; it will not spuriously fail as "atomic_compare_exchange_weak" may.
 */
bool core_util_atomic_cas_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue);

/** \copydoc core_util_atomic_cas_u8 */
bool core_util_atomic_cas_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue);

/** \copydoc core_util_atomic_cas_u8 */
bool core_util_atomic_cas_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue);

/** \copydoc core_util_atomic_cas_u8 */
bool core_util_atomic_cas_u64(volatile uint64_t *ptr, uint64_t *expectedCurrentValue, uint64_t desiredValue);

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_cas_s8(volatile int8_t *ptr, int8_t *expectedCurrentValue, int8_t desiredValue)
{
    return (int8_t)core_util_atomic_cas_u8((volatile uint8_t *)ptr, (uint8_t *)expectedCurrentValue, (uint8_t)desiredValue);
}

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_cas_s16(volatile int16_t *ptr, int16_t *expectedCurrentValue, int16_t desiredValue)
{
    return (int16_t)core_util_atomic_cas_u16((volatile uint16_t *)ptr, (uint16_t *)expectedCurrentValue, (uint16_t)desiredValue);
}
/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_cas_s32(volatile int32_t *ptr, int32_t *expectedCurrentValue, int32_t desiredValue)
{
    return (int32_t)core_util_atomic_cas_u32((volatile uint32_t *)ptr, (uint32_t *)expectedCurrentValue, (uint32_t)desiredValue);
}

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_cas_s64(volatile int64_t *ptr, int64_t *expectedCurrentValue, int64_t desiredValue)
{
    return (int64_t)core_util_atomic_cas_u64((volatile uint64_t *)ptr, (uint64_t *)expectedCurrentValue, (uint64_t)desiredValue);
}

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue)
{
    return (bool)core_util_atomic_cas_u8((volatile uint8_t *)ptr, (uint8_t *)expectedCurrentValue, desiredValue);
}

/** \copydoc core_util_atomic_cas_u8 */
bool core_util_atomic_cas_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue);

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE uint8_t core_util_atomic_load_u8(const volatile uint8_t *valuePtr)
{
    uint8_t value = *valuePtr;
    MBED_BARRIER();
    return value;
}

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE uint16_t core_util_atomic_load_u16(const volatile uint16_t *valuePtr)
{
    uint16_t value = *valuePtr;
    MBED_BARRIER();
    return value;
}

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE uint32_t core_util_atomic_load_u32(const volatile uint32_t *valuePtr)
{
    uint32_t value = *valuePtr;
    MBED_BARRIER();
    return value;
}

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
uint64_t core_util_atomic_load_u64(const volatile uint64_t *valuePtr);

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE int8_t core_util_atomic_load_s8(const volatile int8_t *valuePtr)
{
    int8_t value = *valuePtr;
    MBED_BARRIER();
    return value;
}

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE int16_t core_util_atomic_load_s16(const volatile int16_t *valuePtr)
{
    int16_t value = *valuePtr;
    MBED_BARRIER();
    return value;
}

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE int32_t core_util_atomic_load_s32(const volatile int32_t *valuePtr)
{
    int32_t value = *valuePtr;
    MBED_BARRIER();
    return value;
}

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE int64_t core_util_atomic_load_s64(const volatile int64_t *valuePtr)
{
    return (int64_t)core_util_atomic_load_u64((const volatile uint64_t *)valuePtr);
}

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE bool core_util_atomic_load_bool(const volatile bool *valuePtr)
{
    bool value = *valuePtr;
    MBED_BARRIER();
    return value;
}

/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE void *core_util_atomic_load_ptr(void *const volatile *valuePtr)
{
    void *value = *valuePtr;
    MBED_BARRIER();
    return value;
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_u8(volatile uint8_t *valuePtr, uint8_t desiredValue)
{
    MBED_BARRIER();
    *valuePtr = desiredValue;
    MBED_BARRIER();
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_u16(volatile uint16_t *valuePtr, uint16_t desiredValue)
{
    MBED_BARRIER();
    *valuePtr = desiredValue;
    MBED_BARRIER();
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_u32(volatile uint32_t *valuePtr, uint32_t desiredValue)
{
    MBED_BARRIER();
    *valuePtr = desiredValue;
    MBED_BARRIER();
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
void core_util_atomic_store_u64(volatile uint64_t *valuePtr, uint64_t desiredValue);

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_s8(volatile int8_t *valuePtr, int8_t desiredValue)
{
    MBED_BARRIER();
    *valuePtr = desiredValue;
    MBED_BARRIER();
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_s16(volatile int16_t *valuePtr, int16_t desiredValue)
{
    MBED_BARRIER();
    *valuePtr = desiredValue;
    MBED_BARRIER();
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_s32(volatile int32_t *valuePtr, int32_t desiredValue)
{
    MBED_BARRIER();
    *valuePtr = desiredValue;
    MBED_BARRIER();
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_s64(volatile int64_t *valuePtr, int64_t desiredValue)
{
    core_util_atomic_store_u64((volatile uint64_t *)valuePtr, (uint64_t)desiredValue);
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_bool(volatile bool *valuePtr, bool desiredValue)
{
    MBED_BARRIER();
    *valuePtr = desiredValue;
    MBED_BARRIER();
}

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_ptr(void *volatile *valuePtr, void *desiredValue)
{
    MBED_BARRIER();
    *valuePtr = desiredValue;
    MBED_BARRIER();
}

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
uint8_t core_util_atomic_exchange_u8(volatile uint8_t *valuePtr, uint8_t desiredValue);

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
uint16_t core_util_atomic_exchange_u16(volatile uint16_t *valuePtr, uint16_t desiredValue);

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
uint32_t core_util_atomic_exchange_u32(volatile uint32_t *valuePtr, uint32_t desiredValue);

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
uint64_t core_util_atomic_exchange_u64(volatile uint64_t *valuePtr, uint64_t desiredValue);

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
MBED_FORCEINLINE int8_t core_util_atomic_exchange_s8(volatile int8_t *valuePtr, int8_t desiredValue)
{
    return (int8_t)core_util_atomic_exchange_u8((volatile uint8_t *)valuePtr, (uint8_t)desiredValue);
}

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
MBED_FORCEINLINE int16_t core_util_atomic_exchange_s16(volatile int16_t *valuePtr, int16_t desiredValue)
{
    return (int16_t)core_util_atomic_exchange_u16((volatile uint16_t *)valuePtr, (uint16_t)desiredValue);
}

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
MBED_FORCEINLINE int32_t core_util_atomic_exchange_s32(volatile int32_t *valuePtr, int32_t desiredValue)
{
    return (int32_t)core_util_atomic_exchange_u32((volatile uint32_t *)valuePtr, (uint32_t)desiredValue);
}

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
MBED_FORCEINLINE int64_t core_util_atomic_exchange_s64(volatile int64_t *valuePtr, int64_t desiredValue)
{
    return (int64_t)core_util_atomic_exchange_u64((volatile uint64_t *)valuePtr, (uint64_t)desiredValue);
}

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
MBED_FORCEINLINE bool core_util_atomic_exchange_bool(volatile bool *valuePtr, bool desiredValue)
{
    return (bool)core_util_atomic_exchange_u8((volatile uint8_t *)valuePtr, desiredValue);
}

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
void *core_util_atomic_exchange_ptr(void *volatile *valuePtr, void *desiredValue);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
uint8_t core_util_atomic_incr_u8(volatile uint8_t *valuePtr, uint8_t delta);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
uint16_t core_util_atomic_incr_u16(volatile uint16_t *valuePtr, uint16_t delta);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t delta);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
uint64_t core_util_atomic_incr_u64(volatile uint64_t *valuePtr, uint64_t delta);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
MBED_FORCEINLINE int8_t core_util_atomic_incr_s8(volatile int8_t *valuePtr, int8_t delta)
{
    return (int8_t)core_util_atomic_incr_u8((volatile uint8_t *)valuePtr, (uint8_t)delta);
}

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
MBED_FORCEINLINE int16_t core_util_atomic_incr_s16(volatile int16_t *valuePtr, int16_t delta)
{
    return (int16_t)core_util_atomic_incr_u16((volatile uint16_t *)valuePtr, (uint16_t)delta);
}

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
MBED_FORCEINLINE int32_t core_util_atomic_incr_s32(volatile int32_t *valuePtr, int32_t delta)
{
    return (int32_t)core_util_atomic_incr_u32((volatile uint32_t *)valuePtr, (uint32_t)delta);
}

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
MBED_FORCEINLINE int64_t core_util_atomic_incr_s64(volatile int64_t *valuePtr, int64_t delta)
{
    return (int64_t)core_util_atomic_incr_u64((volatile uint64_t *)valuePtr, (uint64_t)delta);
}

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented in bytes.
 * @return          The new incremented value.
 *
 * @note The type of the pointer argument is not taken into account
 *       and the pointer is incremented by bytes.
 */
void *core_util_atomic_incr_ptr(void *volatile *valuePtr, ptrdiff_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
uint8_t core_util_atomic_decr_u8(volatile uint8_t *valuePtr, uint8_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
uint16_t core_util_atomic_decr_u16(volatile uint16_t *valuePtr, uint16_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
uint64_t core_util_atomic_decr_u64(volatile uint64_t *valuePtr, uint64_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
MBED_FORCEINLINE int8_t core_util_atomic_decr_s8(volatile int8_t *valuePtr, int8_t delta)
{
    return (int8_t)core_util_atomic_decr_u8((volatile uint8_t *)valuePtr, (uint8_t)delta);
}

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
MBED_FORCEINLINE int16_t core_util_atomic_decr_s16(volatile int16_t *valuePtr, int16_t delta)
{
    return (int16_t)core_util_atomic_decr_u16((volatile uint16_t *)valuePtr, (uint16_t)delta);
}

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
MBED_FORCEINLINE int32_t core_util_atomic_decr_s32(volatile int32_t *valuePtr, int32_t delta)
{
    return (int32_t)core_util_atomic_decr_u32((volatile uint32_t *)valuePtr, (uint32_t)delta);
}

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
MBED_FORCEINLINE int64_t core_util_atomic_decr_s64(volatile int64_t *valuePtr, int64_t delta)
{
    return (int64_t)core_util_atomic_decr_u64((volatile uint64_t *)valuePtr, (uint64_t)delta);
}

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented in bytes.
 * @return          The new decremented value.
 *
 * @note The type of the pointer argument is not taken into account
 *       and the pointer is decremented by bytes
 */
void *core_util_atomic_decr_ptr(void *volatile *valuePtr, ptrdiff_t delta);

#ifdef __cplusplus
} // extern "C"
#endif
/**@}*/

/**@}*/

#endif // __MBED_UTIL_CRITICAL_H__



