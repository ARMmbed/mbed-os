
/*
 * Copyright (c) 2015-2019, ARM Limited, All Rights Reserved
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

#ifndef __MBED_UTIL_ATOMIC_H__
#define __MBED_UTIL_ATOMIC_H__

#include "cmsis.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "platform/mbed_toolchain.h"

/** \addtogroup platform-public-api */
/** @{*/

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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Memory order constraints for atomic operations. Intended semantics
 * are as per C++11.
 */
typedef enum mbed_memory_order {
    /* Bits 0 = consume
     *      1 = acquire (explicitly requested, or implied by seq.cst)
     *      2 = release (explicitly requested, or implied by seq.cst)
     *      4 = sequentially consistent
     */
    mbed_memory_order_relaxed = 0x00,
    mbed_memory_order_consume = 0x01,
    mbed_memory_order_acquire = 0x02,
    mbed_memory_order_release = 0x04,
    mbed_memory_order_acq_rel = 0x06,
    mbed_memory_order_seq_cst = 0x16
} mbed_memory_order;

// if __EXCLUSIVE_ACCESS rtx macro not defined, we need to get this via own-set architecture macros
#ifndef MBED_EXCLUSIVE_ACCESS
#ifndef __EXCLUSIVE_ACCESS
#if defined __arm__ || defined __ICC_ARM__ || defined __ARM_ARCH
#if ((__ARM_ARCH_7M__      == 1U) || \
    (__ARM_ARCH_7EM__     == 1U) || \
    (__ARM_ARCH_8M_BASE__ == 1U) || \
    (__ARM_ARCH_8M_MAIN__ == 1U) || \
    (__ARM_ARCH_8_1M_MAIN__ == 1U)) || \
    (__ARM_ARCH_7A__ == 1U)
#define MBED_EXCLUSIVE_ACCESS      1U
#define MBED_EXCLUSIVE_ACCESS_THUMB1 (__ARM_ARCH_8M_BASE__ == 1U)
#ifdef __ICCARM__
#if __CPU_MODE__ == 2
#define MBED_EXCLUSIVE_ACCESS_ARM  1U
#else
#define MBED_EXCLUSIVE_ACCESS_ARM  0U
#endif
#else
#if !defined (__thumb__)
#define MBED_EXCLUSIVE_ACCESS_ARM  1U
#else
#define MBED_EXCLUSIVE_ACCESS_ARM  0U
#endif
#endif
#elif (__ARM_ARCH_6M__ == 1U)
#define MBED_EXCLUSIVE_ACCESS      0U
#elif defined __aarch64__ // Apple M1 Mac
#define MBED_EXCLUSIVE_ACCESS      0U
#else
#error "Unknown ARM architecture for exclusive access"
#endif // __ARM_ARCH_xxx
#else // __arm__ || defined __ICC_ARM__ || defined __ARM_ARCH
// Seem to be compiling for non-ARM, so stick with critical section implementations
#define MBED_EXCLUSIVE_ACCESS      0U
#endif
#else
#define MBED_EXCLUSIVE_ACCESS __EXCLUSIVE_ACCESS
#endif
#endif

#if MBED_EXCLUSIVE_ACCESS
#define MBED_INLINE_IF_EX inline
#else
#define MBED_INLINE_IF_EX
#endif

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
MBED_INLINE_IF_EX bool core_util_atomic_flag_test_and_set(volatile core_util_atomic_flag *flagPtr);

/** \ copydoc core_util_atomic_flag_test_and_set
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE bool core_util_atomic_flag_test_and_set_explicit(volatile core_util_atomic_flag *valuePtr, mbed_memory_order order);

/**
 * Atomic clear.
 *
 * @param  flagPtr Target flag being cleared.
 */
MBED_FORCEINLINE void core_util_atomic_flag_clear(volatile core_util_atomic_flag *flagPtr);

/** \ copydoc core_util_atomic_flag_clear
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE void core_util_atomic_flag_clear_explicit(volatile core_util_atomic_flag *flagPtr, mbed_memory_order order);


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
 *     value = atomic_load(p)
 *     while not done {
 *         done = atomic_cas(p, &value, value + a) // *value gets updated automatically until success
 *     }
 *     return value + a
 * }
 *
 * However, if the call is made in a loop like this, the atomic_compare_exchange_weak
 * functions are to be preferred.
 *
 * @note: This corresponds to the C11 "atomic_compare_exchange_strong" - it
 * always succeeds if the current value is expected, as per the pseudocode
 * above; it will not spuriously fail as "atomic_compare_exchange_weak" may.
 * This call would normally be used when a fail return does not retry.
 */
MBED_INLINE_IF_EX bool core_util_atomic_cas_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue);

/** \copydoc core_util_atomic_cas_u8
 * @param success memory ordering constraint for successful exchange
 * @param failure memory ordering constraint for failure
 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
MBED_INLINE_IF_EX bool core_util_atomic_cas_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
MBED_INLINE_IF_EX bool core_util_atomic_cas_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
bool core_util_atomic_cas_u64(volatile uint64_t *ptr, uint64_t *expectedCurrentValue, uint64_t desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_u64(volatile uint64_t *ptr, uint64_t *expectedCurrentValue, uint64_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_s8(volatile int8_t *ptr, int8_t *expectedCurrentValue, int8_t desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_s8(volatile int8_t *ptr, int8_t *expectedCurrentValue, int8_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_s16(volatile int16_t *ptr, int16_t *expectedCurrentValue, int16_t desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_s16(volatile int16_t *ptr, int16_t *expectedCurrentValue, int16_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_s32(volatile int32_t *ptr, int32_t *expectedCurrentValue, int32_t desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_s32(volatile int32_t *ptr, int32_t *expectedCurrentValue, int32_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_s64(volatile int64_t *ptr, int64_t *expectedCurrentValue, int64_t desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_s64(volatile int64_t *ptr, int64_t *expectedCurrentValue, int64_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_cas_u8 */
inline bool core_util_atomic_cas_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue);

/** \copydoc core_util_atomic_cas_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue, mbed_memory_order success, mbed_memory_order failure);



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
 *     if *p != *old or spurious failure {
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
 *         done = atomic_compare_exchange_weak(p, &value, value + a) // *value gets updated automatically until success
 *     }
 *     return value + a
 * }
 *
 * @note: This corresponds to the C11 "atomic_compare_exchange_weak" - it
 * may spuriously fail if the current value is expected, as per the pseudocode
 * above; it will not spuriously fail as "atomic_compare_exchange_weak" may.
 * This call would normally be used when a fail return will cause a retry anyway,
 * saving the need for an extra loop inside the cas operation.
 */
MBED_INLINE_IF_EX bool core_util_atomic_compare_exchange_weak_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_u8
 * @param success memory ordering constraint for successful exchange
 * @param failure memory ordering constraint for failure
 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_u8(volatile uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
MBED_INLINE_IF_EX bool core_util_atomic_compare_exchange_weak_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_u16(volatile uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
MBED_INLINE_IF_EX bool core_util_atomic_compare_exchange_weak_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_u32(volatile uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
bool core_util_atomic_compare_exchange_weak_u64(volatile uint64_t *ptr, uint64_t *expectedCurrentValue, uint64_t desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_u64(volatile uint64_t *ptr, uint64_t *expectedCurrentValue, uint64_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_s8(volatile int8_t *ptr, int8_t *expectedCurrentValue, int8_t desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_s8(volatile int8_t *ptr, int8_t *expectedCurrentValue, int8_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_s16(volatile int16_t *ptr, int16_t *expectedCurrentValue, int16_t desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_s16(volatile int16_t *ptr, int16_t *expectedCurrentValue, int16_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_s32(volatile int32_t *ptr, int32_t *expectedCurrentValue, int32_t desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_s32(volatile int32_t *ptr, int32_t *expectedCurrentValue, int32_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_s64(volatile int64_t *ptr, int64_t *expectedCurrentValue, int64_t desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_s64(volatile int64_t *ptr, int64_t *expectedCurrentValue, int64_t desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue, mbed_memory_order success, mbed_memory_order failure);

/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue);

/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue, mbed_memory_order success, mbed_memory_order failure);


/**
 * Atomic load.
 * @param  valuePtr Target memory location.
 * @return          The loaded value.
 */
MBED_FORCEINLINE uint8_t core_util_atomic_load_u8(const volatile uint8_t *valuePtr);

/**
 * \copydoc core_util_atomic_load_u8
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE uint8_t core_util_atomic_load_explicit_u8(const volatile uint8_t *valuePtr, mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE uint16_t core_util_atomic_load_u16(const volatile uint16_t *valuePtr);

/** \copydoc core_util_atomic_load_explicit_u8 */
MBED_FORCEINLINE uint16_t core_util_atomic_load_explicit_u16(const volatile uint16_t *valuePtr, mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE uint32_t core_util_atomic_load_u32(const volatile uint32_t *valuePtr);

/** \copydoc core_util_atomic_load_explicit_u8 */
MBED_FORCEINLINE uint32_t core_util_atomic_load_explicit_u32(const volatile uint32_t *valuePtr, mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
uint64_t core_util_atomic_load_u64(const volatile uint64_t *valuePtr);

/** \copydoc core_util_atomic_load_explicit_u8 */
MBED_FORCEINLINE uint64_t core_util_atomic_load_explicit_u64(const volatile uint64_t *valuePtr, mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_load_s8(const volatile int8_t *valuePtr);

/** \copydoc core_util_atomic_load_explicit_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_load_explicit_s8(const volatile int8_t *valuePtr, mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_load_s16(const volatile int16_t *valuePtr);

/** \copydoc core_util_atomic_load_explicit_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_load_explicit_s16(const volatile int16_t *valuePtr, mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_load_s32(const volatile int32_t *valuePtr);

/** \copydoc core_util_atomic_load_explicit_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_load_explicit_s32(const volatile int32_t *valuePtr, mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_load_s64(const volatile int64_t *valuePtr);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_load_explicit_s64(const volatile int64_t *valuePtr, MBED_UNUSED mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE bool core_util_atomic_load_bool(const volatile bool *valuePtr);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE bool core_util_atomic_load_explicit_bool(const volatile bool *valuePtr, mbed_memory_order order);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE void *core_util_atomic_load_ptr(void *const volatile *valuePtr);

/** \copydoc core_util_atomic_load_u8 */
MBED_FORCEINLINE void *core_util_atomic_load_explicit_ptr(void *const volatile *valuePtr, mbed_memory_order order);

/**
 * Atomic store.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 */
MBED_FORCEINLINE void core_util_atomic_store_u8(volatile uint8_t *valuePtr, uint8_t desiredValue);

/**
 * \copydoc core_util_atomic_store_u8
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_u8(volatile uint8_t *valuePtr, uint8_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
MBED_FORCEINLINE void core_util_atomic_store_u16(volatile uint16_t *valuePtr, uint16_t desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_u16(volatile uint16_t *valuePtr, uint16_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
MBED_FORCEINLINE void core_util_atomic_store_u32(volatile uint32_t *valuePtr, uint32_t desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_u32(volatile uint32_t *valuePtr, uint32_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
void core_util_atomic_store_u64(volatile uint64_t *valuePtr, uint64_t desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_u64(volatile uint64_t *valuePtr, uint64_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
MBED_FORCEINLINE void core_util_atomic_store_s8(volatile int8_t *valuePtr, int8_t desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_s8(volatile int8_t *valuePtr, int8_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
MBED_FORCEINLINE void core_util_atomic_store_s16(volatile int16_t *valuePtr, int16_t desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_s16(volatile int16_t *valuePtr, int16_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
MBED_FORCEINLINE void core_util_atomic_store_s32(volatile int32_t *valuePtr, int32_t desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_s32(volatile int32_t *valuePtr, int32_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
MBED_FORCEINLINE void core_util_atomic_store_s64(volatile int64_t *valuePtr, int64_t desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_s64(volatile int64_t *valuePtr, int64_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
MBED_FORCEINLINE void core_util_atomic_store_bool(volatile bool *valuePtr, bool desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_bool(volatile bool *valuePtr, bool desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_store_u8 */
MBED_FORCEINLINE void core_util_atomic_store_ptr(void *volatile *valuePtr, void *desiredValue);

/** \copydoc core_util_atomic_store_explicit_u8 */
MBED_FORCEINLINE void core_util_atomic_store_explicit_ptr(void *volatile *valuePtr, void *desiredValue, mbed_memory_order order);

/**
 * Atomic exchange.
 * @param  valuePtr     Target memory location.
 * @param  desiredValue The value to store.
 * @return              The previous value.
 */
MBED_INLINE_IF_EX uint8_t core_util_atomic_exchange_u8(volatile uint8_t *valuePtr, uint8_t desiredValue);

/** \copydoc core_util_atomic_exchange_u8
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE uint8_t core_util_atomic_exchange_explicit_u8(volatile uint8_t *valuePtr, uint8_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
MBED_INLINE_IF_EX uint16_t core_util_atomic_exchange_u16(volatile uint16_t *valuePtr, uint16_t desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE uint16_t core_util_atomic_exchange_explicit_u16(volatile uint16_t *valuePtr, uint16_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
MBED_INLINE_IF_EX uint32_t core_util_atomic_exchange_u32(volatile uint32_t *valuePtr, uint32_t desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE uint32_t core_util_atomic_exchange_explicit_u32(volatile uint32_t *valuePtr, uint32_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
uint64_t core_util_atomic_exchange_u64(volatile uint64_t *valuePtr, uint64_t desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE uint64_t core_util_atomic_exchange_explicit_u64(volatile uint64_t *valuePtr, uint64_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_exchange_s8(volatile int8_t *valuePtr, int8_t desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_exchange_explicit_s8(volatile int8_t *valuePtr, int8_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_exchange_s16(volatile int16_t *valuePtr, int16_t desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_exchange_explicit_s16(volatile int16_t *valuePtr, int16_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_exchange_s32(volatile int32_t *valuePtr, int32_t desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_exchange_explicit_s32(volatile int32_t *valuePtr, int32_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_exchange_s64(volatile int64_t *valuePtr, int64_t desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_exchange_explicit_s64(volatile int64_t *valuePtr, int64_t desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
MBED_FORCEINLINE bool core_util_atomic_exchange_bool(volatile bool *valuePtr, bool desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE bool core_util_atomic_exchange_explicit_bool(volatile bool *valuePtr, bool desiredValue, mbed_memory_order order);

/** \copydoc core_util_atomic_exchange_u8 */
inline void *core_util_atomic_exchange_ptr(void *volatile *valuePtr, void *desiredValue);

/** \copydoc core_util_atomic_exchange_explicit_u8 */
MBED_FORCEINLINE void *core_util_atomic_exchange_explicit_ptr(void *volatile *valuePtr, void *desiredValue, mbed_memory_order order);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
MBED_INLINE_IF_EX uint8_t core_util_atomic_incr_u8(volatile uint8_t *valuePtr, uint8_t delta);

/** \copydoc core_util_atomic_incr_u8 */
MBED_INLINE_IF_EX uint16_t core_util_atomic_incr_u16(volatile uint16_t *valuePtr, uint16_t delta);

/** \copydoc core_util_atomic_incr_u8 */
MBED_INLINE_IF_EX uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t delta);

/** \copydoc core_util_atomic_incr_u8 */
uint64_t core_util_atomic_incr_u64(volatile uint64_t *valuePtr, uint64_t delta);

/** \copydoc core_util_atomic_incr_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_incr_s8(volatile int8_t *valuePtr, int8_t delta);

/** \copydoc core_util_atomic_incr_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_incr_s16(volatile int16_t *valuePtr, int16_t delta);

/** \copydoc core_util_atomic_incr_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_incr_s32(volatile int32_t *valuePtr, int32_t delta);

/** \copydoc core_util_atomic_incr_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_incr_s64(volatile int64_t *valuePtr, int64_t delta);

/** \copydoc core_util_atomic_incr_u8 */
inline void *core_util_atomic_incr_ptr(void *volatile *valuePtr, ptrdiff_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
MBED_INLINE_IF_EX uint8_t core_util_atomic_decr_u8(volatile uint8_t *valuePtr, uint8_t delta);

/** \copydoc core_util_atomic_decr_u8 */
MBED_INLINE_IF_EX uint16_t core_util_atomic_decr_u16(volatile uint16_t *valuePtr, uint16_t delta);

/** \copydoc core_util_atomic_decr_u8 */
MBED_INLINE_IF_EX uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t delta);

/** \copydoc core_util_atomic_decr_u8 */
uint64_t core_util_atomic_decr_u64(volatile uint64_t *valuePtr, uint64_t delta);

/** \copydoc core_util_atomic_decr_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_decr_s8(volatile int8_t *valuePtr, int8_t delta);

/** \copydoc core_util_atomic_decr_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_decr_s16(volatile int16_t *valuePtr, int16_t delta);

/** \copydoc core_util_atomic_decr_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_decr_s32(volatile int32_t *valuePtr, int32_t delta);

/** \copydoc core_util_atomic_decr_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_decr_s64(volatile int64_t *valuePtr, int64_t delta);

/** \copydoc core_util_atomic_decr_u8 */
inline void *core_util_atomic_decr_ptr(void *volatile *valuePtr, ptrdiff_t delta);

/**
 * Atomic add.
 * @param  valuePtr Target memory location being modified.
 * @param  arg      The argument for the addition.
 * @return          The original value.
 */
MBED_INLINE_IF_EX uint8_t core_util_atomic_fetch_add_u8(volatile uint8_t *valuePtr, uint8_t arg);

/** \copydoc core_util_atomic_fetch_add_u8
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE uint8_t core_util_atomic_fetch_add_explicit_u8(volatile uint8_t *valuePtr, uint8_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_add_u8 */
MBED_INLINE_IF_EX uint16_t core_util_atomic_fetch_add_u16(volatile uint16_t *valuePtr, uint16_t arg);

/** \copydoc core_util_atomic_fetch_add_explicit_u8 */
MBED_FORCEINLINE uint16_t core_util_atomic_fetch_add_explicit_u16(volatile uint16_t *valuePtr, uint16_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_add_u8 */
MBED_INLINE_IF_EX uint32_t core_util_atomic_fetch_add_u32(volatile uint32_t *valuePtr, uint32_t arg);

/** \copydoc core_util_atomic_fetch_add_explicit_u8 */
MBED_FORCEINLINE uint32_t core_util_atomic_fetch_add_explicit_u32(volatile uint32_t *valuePtr, uint32_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_add_u8 */
uint64_t core_util_atomic_fetch_add_u64(volatile uint64_t *valuePtr, uint64_t arg);

/** \copydoc core_util_atomic_fetch_add_explicit_u8 */
MBED_FORCEINLINE uint64_t core_util_atomic_fetch_add_explicit_u64(volatile uint64_t *valuePtr, uint64_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_add_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_fetch_add_s8(volatile int8_t *valuePtr, int8_t arg);

/** \copydoc core_util_atomic_fetch_add_explicit_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_fetch_add_explicit_s8(volatile int8_t *valuePtr, int8_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_add_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_fetch_add_s16(volatile int16_t *valuePtr, int16_t arg);

/** \copydoc core_util_atomic_fetch_add_explicit_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_fetch_add_explicit_s16(volatile int16_t *valuePtr, int16_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_add_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_fetch_add_s32(volatile int32_t *valuePtr, int32_t arg);

/** \copydoc core_util_atomic_fetch_add_explicit_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_fetch_add_explicit_s32(volatile int32_t *valuePtr, int32_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_add_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_fetch_add_s64(volatile int64_t *valuePtr, int64_t arg);

/** \copydoc core_util_atomic_fetch_add_explicit_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_fetch_add_explicit_s64(volatile int64_t *valuePtr, int64_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_add_u8 */
MBED_FORCEINLINE void *core_util_atomic_fetch_add_ptr(void *volatile *valuePtr, ptrdiff_t arg);

/** \copydoc core_util_atomic_fetch_add_explicit_u8 */
MBED_FORCEINLINE void *core_util_atomic_fetch_add_explicit_ptr(void *volatile *valuePtr, ptrdiff_t arg, mbed_memory_order order);

/**
 * Atomic subtract.
 * @param  valuePtr Target memory location being modified.
 * @param  arg      The argument for the subtraction.
 * @return          The original value.
 */
MBED_INLINE_IF_EX uint8_t core_util_atomic_fetch_sub_u8(volatile uint8_t *valuePtr, uint8_t arg);

/** \copydoc core_util_atomic_fetch_sub_u8
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE uint8_t core_util_atomic_fetch_sub_explicit_u8(volatile uint8_t *valuePtr, uint8_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_sub_u8 */
MBED_INLINE_IF_EX uint16_t core_util_atomic_fetch_sub_u16(volatile uint16_t *valuePtr, uint16_t arg);

/** \copydoc core_util_atomic_fetch_sub_explicit_u8 */
MBED_FORCEINLINE uint16_t core_util_atomic_fetch_sub_explicit_u16(volatile uint16_t *valuePtr, uint16_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_sub_u8 */
MBED_INLINE_IF_EX uint32_t core_util_atomic_fetch_sub_u32(volatile uint32_t *valuePtr, uint32_t arg);

/** \copydoc core_util_atomic_fetch_sub_explicit_u8 */
MBED_FORCEINLINE uint32_t core_util_atomic_fetch_sub_explicit_u32(volatile uint32_t *valuePtr, uint32_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_sub_u8 */
uint64_t core_util_atomic_fetch_sub_u64(volatile uint64_t *valuePtr, uint64_t arg);

/** \copydoc core_util_atomic_fetch_sub_explicit_u8 */
MBED_FORCEINLINE uint64_t core_util_atomic_fetch_sub_explicit_u64(volatile uint64_t *valuePtr, uint64_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_sub_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_fetch_sub_s8(volatile int8_t *valuePtr, int8_t arg);

/** \copydoc core_util_atomic_fetch_sub_explicit_u8 */
MBED_FORCEINLINE int8_t core_util_atomic_fetch_sub_explicit_s8(volatile int8_t *valuePtr, int8_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_sub_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_fetch_sub_s16(volatile int16_t *valuePtr, int16_t arg);

/** \copydoc core_util_atomic_fetch_sub_explicit_u8 */
MBED_FORCEINLINE int16_t core_util_atomic_fetch_sub_explicit_s16(volatile int16_t *valuePtr, int16_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_sub_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_fetch_sub_s32(volatile int32_t *valuePtr, int32_t arg);

/** \copydoc core_util_atomic_fetch_sub_explicit_u8 */
MBED_FORCEINLINE int32_t core_util_atomic_fetch_sub_explicit_s32(volatile int32_t *valuePtr, int32_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_sub_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_fetch_sub_s64(volatile int64_t *valuePtr, int64_t arg);

/** \copydoc core_util_atomic_fetch_sub_explicit_u8 */
MBED_FORCEINLINE int64_t core_util_atomic_fetch_sub_explicit_s64(volatile int64_t *valuePtr, int64_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_sub_u8 */
MBED_FORCEINLINE void *core_util_atomic_fetch_sub_ptr(void *volatile *valuePtr, ptrdiff_t arg);

/** \copydoc core_util_atomic_fetch_sub_explicit_u8 */
MBED_FORCEINLINE void *core_util_atomic_fetch_sub_explicit_ptr(void *volatile *valuePtr, ptrdiff_t arg, mbed_memory_order order);

/**
 * Atomic bitwise and.
 * @param  valuePtr Target memory location being modified.
 * @param  arg      The argument for the bitwise operation.
 * @return          The original value.
 */
MBED_INLINE_IF_EX uint8_t core_util_atomic_fetch_and_u8(volatile uint8_t *valuePtr, uint8_t arg);

/** \copydoc core_util_atomic_fetch_and_u8
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE uint8_t core_util_atomic_fetch_and_explicit_u8(volatile uint8_t *valuePtr, uint8_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_and_u8 */
MBED_INLINE_IF_EX uint16_t core_util_atomic_fetch_and_u16(volatile uint16_t *valuePtr, uint16_t arg);

/** \copydoc core_util_atomic_fetch_and_explicit_u8 */
MBED_FORCEINLINE uint16_t core_util_atomic_fetch_and_explicit_u16(volatile uint16_t *valuePtr, uint16_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_and_u8 */
MBED_INLINE_IF_EX uint32_t core_util_atomic_fetch_and_u32(volatile uint32_t *valuePtr, uint32_t arg);

/** \copydoc core_util_atomic_fetch_and_explicit_u8 */
MBED_FORCEINLINE uint32_t core_util_atomic_fetch_and_explicit_u32(volatile uint32_t *valuePtr, uint32_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_and_u8 */
uint64_t core_util_atomic_fetch_and_u64(volatile uint64_t *valuePtr, uint64_t arg);

/** \copydoc core_util_atomic_fetch_and_explicit_u8 */
MBED_FORCEINLINE uint64_t core_util_atomic_fetch_and_explicit_u64(volatile uint64_t *valuePtr, uint64_t arg, mbed_memory_order order);

/**
 * Atomic bitwise inclusive or.
 * @param  valuePtr Target memory location being modified.
 * @param  arg      The argument for the bitwise operation.
 * @return          The original value.
 */
MBED_INLINE_IF_EX uint8_t core_util_atomic_fetch_or_u8(volatile uint8_t *valuePtr, uint8_t arg);

/** \copydoc core_util_atomic_fetch_or_u8
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE uint8_t core_util_atomic_fetch_or_explicit_u8(volatile uint8_t *valuePtr, uint8_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_or_u8 */
MBED_INLINE_IF_EX uint16_t core_util_atomic_fetch_or_u16(volatile uint16_t *valuePtr, uint16_t arg);

/** \copydoc core_util_atomic_fetch_or_explicit_u8 */
MBED_FORCEINLINE uint16_t core_util_atomic_fetch_or_explicit_u16(volatile uint16_t *valuePtr, uint16_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_or_u8 */
MBED_INLINE_IF_EX uint32_t core_util_atomic_fetch_or_u32(volatile uint32_t *valuePtr, uint32_t arg);

/** \copydoc core_util_atomic_fetch_or_explicit_u8 */
MBED_FORCEINLINE uint32_t core_util_atomic_fetch_or_explicit_u32(volatile uint32_t *valuePtr, uint32_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_or_u8 */
uint64_t core_util_atomic_fetch_or_u64(volatile uint64_t *valuePtr, uint64_t arg);

/** \copydoc core_util_atomic_fetch_or_explicit_u8 */
MBED_FORCEINLINE uint64_t core_util_atomic_fetch_or_explicit_u64(volatile uint64_t *valuePtr, uint64_t arg, mbed_memory_order order);

/**
 * Atomic bitwise exclusive or.
 * @param  valuePtr Target memory location being modified.
 * @param  arg      The argument for the bitwise operation.
 * @return          The original value.
 */
MBED_INLINE_IF_EX uint8_t core_util_atomic_fetch_xor_u8(volatile uint8_t *valuePtr, uint8_t arg);

/** \copydoc core_util_atomic_fetch_xor_u8
 * @param order memory ordering constraint
 */
MBED_FORCEINLINE uint8_t core_util_atomic_fetch_xor_explicit_u8(volatile uint8_t *valuePtr, uint8_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_xor_u8 */
MBED_INLINE_IF_EX uint16_t core_util_atomic_fetch_xor_u16(volatile uint16_t *valuePtr, uint16_t arg);

/** \copydoc core_util_atomic_fetch_xor_explicit_u8 */
MBED_FORCEINLINE uint16_t core_util_atomic_fetch_xor_explicit_u16(volatile uint16_t *valuePtr, uint16_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_xor_u8 */
MBED_INLINE_IF_EX uint32_t core_util_atomic_fetch_xor_u32(volatile uint32_t *valuePtr, uint32_t arg);

/** \copydoc core_util_atomic_fetch_xor_explicit_u8 */
MBED_FORCEINLINE uint32_t core_util_atomic_fetch_xor_explicit_u32(volatile uint32_t *valuePtr, uint32_t arg, mbed_memory_order order);

/** \copydoc core_util_atomic_fetch_xor_u8 */
uint64_t core_util_atomic_fetch_xor_u64(volatile uint64_t *valuePtr, uint64_t arg);

/** \copydoc core_util_atomic_fetch_xor_explicit_u8 */
MBED_FORCEINLINE uint64_t core_util_atomic_fetch_xor_explicit_u64(volatile uint64_t *valuePtr, uint64_t arg, mbed_memory_order order);

#ifdef __cplusplus
} // extern "C"

#include <mstd_type_traits>

// For each operation, two overloaded templates:
// * one for non-pointer types, which has implementations based on the
//   u8/u16/u32/u64/s8/s16/s32/s64/bool functions above. No base implementation.
// * one for any pointer type, generically implemented based on ptr function above.
//
// Templates use standard C/C++ naming - old incr/decr/cas forms are not provided.
//
// The `type_identity_t<T>` used here means "same type as T", blocking template
// argument deduction. It forces type selection based on the type of the actual pointer
// to the atomic. If just `T` was used, the following would be ambiguous:
// core_util_atomic_store(&my_uint8_t, 1) - it wouldn't be able to select between T
// being uint8_t and int.

/** \copydoc core_util_atomic_load_u8 */
template<typename T> T core_util_atomic_load(const volatile T *valuePtr) noexcept;
/** \copydoc core_util_atomic_load_u8 */
template<typename T> T core_util_atomic_load(const T *valuePtr) noexcept;
/** \copydoc core_util_atomic_store_u8 */
template<typename T> void core_util_atomic_store(volatile T *valuePtr, mstd::type_identity_t<T> desiredValue) noexcept;
/** \copydoc core_util_atomic_store_u8 */
template<typename T> void core_util_atomic_store(T *valuePtr, mstd::type_identity_t<T> desiredValue) noexcept;
/** \copydoc core_util_atomic_exchange_u8 */
template<typename T> T core_util_atomic_exchange(volatile T *ptr, mstd::type_identity_t<T> desiredValue) noexcept;
/** \copydoc core_util_atomic_cas_u8 */
template<typename T> bool core_util_atomic_compare_exchange_strong(volatile T *ptr, mstd::type_identity_t<T> *expectedCurrentValue, mstd::type_identity_t<T> desiredValue) noexcept;
/** \copydoc core_util_atomic_compare_exchange_weak_u8 */
template<typename T> bool core_util_atomic_compare_exchange_weak(volatile T *ptr, mstd::type_identity_t<T> *expectedCurrentValue, mstd::type_identity_t<T> desiredValue) noexcept;
/** \copydoc core_util_fetch_add_u8 */
template<typename T> T core_util_atomic_fetch_add(volatile T *valuePtr, mstd::type_identity_t<T> arg) noexcept;
/** \copydoc core_util_fetch_sub_u8 */
template<typename T> T core_util_atomic_fetch_sub(volatile T *valuePtr, mstd::type_identity_t<T> arg) noexcept;
/** \copydoc core_util_fetch_and_u8 */
template<typename T> T core_util_atomic_fetch_and(volatile T *valuePtr, mstd::type_identity_t<T> arg) noexcept;
/** \copydoc core_util_fetch_or_u8 */
template<typename T> T core_util_atomic_fetch_or(volatile T *valuePtr, mstd::type_identity_t<T> arg) noexcept;
/** \copydoc core_util_fetch_xor_u8 */
template<typename T> T core_util_atomic_fetch_xor(volatile T *valuePtr, mstd::type_identity_t<T> arg) noexcept;

/** \copydoc core_util_atomic_load_explicit_u8 */
template<typename T> T core_util_atomic_load_explicit(const volatile T *valuePtr, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_load_explicit_u8 */
template<typename T> T core_util_atomic_load_explicit(const T *valuePtr, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_store_explicit_u8 */
template<typename T> void core_util_atomic_store_explicit(volatile T *valuePtr, mstd::type_identity_t<T> desiredValue, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_store_explicit_u8 */
template<typename T> void core_util_atomic_store_explicit(T *valuePtr, mstd::type_identity_t<T> desiredValue, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_exchange_explicit_u8 */
template<typename T> T core_util_atomic_exchange_explicit(volatile T *ptr, mstd::type_identity_t<T> desiredValue, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_cas_explicit_u8 */
template<typename T> bool core_util_atomic_compare_exchange_strong_explicit(volatile T *ptr, mstd::type_identity_t<T> *expectedCurrentValue, mstd::type_identity_t<T> desiredValue, mbed_memory_order success, mbed_memory_order failure) noexcept;
/** \copydoc core_util_atomic_compare_exchange_weak_explicit_u8 */
template<typename T> bool core_util_atomic_compare_exchange_weak_explicit(volatile T *ptr, mstd::type_identity_t<T> *expectedCurrentValue, mstd::type_identity_t<T> desiredValue, mbed_memory_order success, mbed_memory_order failure) noexcept;
/** \copydoc core_util_fetch_add_explicit_u8 */
template<typename T> T core_util_atomic_fetch_add_explicit(volatile T *valuePtr, mstd::type_identity_t<T> arg, mbed_memory_order order) noexcept;
/** \copydoc core_util_fetch_sub_explicit_u8 */
template<typename T> T core_util_atomic_fetch_sub_explicit(volatile T *valuePtr, mstd::type_identity_t<T> arg, mbed_memory_order order) noexcept;
/** \copydoc core_util_fetch_and_explicit_u8 */
template<typename T> T core_util_atomic_fetch_and_explicit(volatile T *valuePtr, mstd::type_identity_t<T> arg, mbed_memory_order order) noexcept;
/** \copydoc core_util_fetch_or_explicit_u8 */
template<typename T> T core_util_atomic_fetch_or_explicit(volatile T *valuePtr, mstd::type_identity_t<T> arg, mbed_memory_order order) noexcept;
/** \copydoc core_util_fetch_xor_explicit_u8 */
template<typename T> T core_util_atomic_fetch_xor_explicit(volatile T *valuePtr, mstd::type_identity_t<T> arg, mbed_memory_order order) noexcept;

/** \copydoc core_util_atomic_load_ptr */
template<typename T> inline T *core_util_atomic_load(T *const volatile *valuePtr) noexcept;
/** \copydoc core_util_atomic_load_ptr */
template<typename T> inline T *core_util_atomic_load(T *const *valuePtr) noexcept;
/** \copydoc core_util_atomic_store_ptr */
template<typename T> inline void core_util_atomic_store(T *volatile *valuePtr, mstd::type_identity_t<T> *desiredValue) noexcept;
/** \copydoc core_util_atomic_store_ptr */
template<typename T> inline void core_util_atomic_store(T **valuePtr, mstd::type_identity_t<T> *desiredValue) noexcept;
/** \copydoc core_util_atomic_exchange_ptr */
template<typename T> inline T *core_util_atomic_exchange(T *volatile *valuePtr, mstd::type_identity_t<T> *desiredValue) noexcept;
/** \copydoc core_util_atomic_cas_ptr */
template<typename T> inline bool core_util_atomic_compare_exchange_strong(T *volatile *ptr, mstd::type_identity_t<T> **expectedCurrentValue, mstd::type_identity_t<T> *desiredValue) noexcept;
/** \copydoc core_util_atomic_compare_exchange_weak_ptr */
template<typename T> inline bool core_util_atomic_compare_exchange_weak(T *volatile *ptr, mstd::type_identity_t<T> **expectedCurrentValue, mstd::type_identity_t<T> *desiredValue) noexcept;
/** \copydoc core_util_fetch_add_ptr */
template<typename T> inline T *core_util_atomic_fetch_add(T *volatile *valuePtr, ptrdiff_t arg) noexcept;
/** \copydoc core_util_fetch_sub_ptr */
template<typename T> inline T *core_util_atomic_fetch_sub(T *volatile *valuePtr, ptrdiff_t arg) noexcept;

/** \copydoc core_util_atomic_load_explicit_ptr */
template<typename T> inline T *core_util_atomic_load_explicit(T *const volatile *valuePtr, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_load_explicit_ptr */
template<typename T> inline T *core_util_atomic_load_explicit(T *const *valuePtr, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_store_explicit_ptr */
template<typename T> inline void core_util_atomic_store_explicit(T *volatile *valuePtr, mstd::type_identity_t<T> *desiredValue, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_store_explicit_ptr */
template<typename T> inline void core_util_atomic_store_explicit(T **valuePtr, mstd::type_identity_t<T> *desiredValue, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_exchange_explicit_ptr */
template<typename T> inline T *core_util_atomic_exchange_explicit(T *volatile *valuePtr, mstd::type_identity_t<T> *desiredValue, mbed_memory_order order) noexcept;
/** \copydoc core_util_atomic_cas_explicit_ptr */
template<typename T> inline bool core_util_atomic_compare_exchange_strong_explicit(T *volatile *ptr, mstd::type_identity_t<T> **expectedCurrentValue, mstd::type_identity_t<T> *desiredValue, mbed_memory_order success, mbed_memory_order failure) noexcept;
/** \copydoc core_util_atomic_compare_exchange_weak_explicit_ptr */
template<typename T> inline bool core_util_atomic_compare_exchange_weak_explicit(T *volatile *ptr, mstd::type_identity_t<T> **expectedCurrentValue, mstd::type_identity_t<T> *desiredValue, mbed_memory_order success, mbed_memory_order failure) noexcept;
/** \copydoc core_util_fetch_add_explicit_ptr */
template<typename T> inline T *core_util_atomic_fetch_add_explicit(T *volatile *valuePtr, ptrdiff_t arg, mbed_memory_order order) noexcept;
/** \copydoc core_util_fetch_sub_explicit_ptr */
template<typename T> inline T *core_util_atomic_fetch_sub_explicit(T *volatile *valuePtr, ptrdiff_t arg, mbed_memory_order order) noexcept;

#endif // __cplusplus

/**@}*/

/**@}*/

/* Hide the implementation away */
#include "platform/internal/mbed_atomic_impl.h"

#endif // __MBED_UTIL_ATOMICL_H__



