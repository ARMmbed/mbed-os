
/** \addtogroup platform */
/** @{*/
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

#ifdef __cplusplus
extern "C" {
#endif


/** Determine the current interrupts enabled state
  *
  * This function can be called to determine whether or not interrupts are currently enabled.
  * \note
  * NOTE:
  * This function works for both cortex-A and cortex-M, although the underlyng implementation
  * differs.
  * @return true if interrupts are enabled, false otherwise
  */
bool core_util_are_interrupts_enabled(void);

/** Mark the start of a critical section
  *
  * This function should be called to mark the start of a critical section of code.
  * \note
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
  * \note
  * NOTES:
  * 1) The use of this style of critical section is targetted at C based implementations.
  * 2) These critical sections can be nested.
  * 3) The interrupt enable state on entry to the first critical section (of a nested set, or single
  *    section) will be preserved on exit from the section.
  * 4) This implementation will currently only work on code running in privileged mode.
  */
void core_util_critical_section_exit(void);

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
 *                              @Note: This is an in-out parameter. In the
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
 * @Note: In the failure case (where the destination isn't set), the value
 * pointed to by expectedCurrentValue is still updated with the current value.
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
 */
bool core_util_atomic_cas_u8(uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue);

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
 *                              @Note: This is an in-out parameter. In the
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
 * @Note: In the failure case (where the destination isn't set), the value
 * pointed to by expectedCurrentValue is still updated with the current value.
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
 */
bool core_util_atomic_cas_u16(uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue);

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
 *                              @Note: This is an in-out parameter. In the
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
 * @Note: In the failure case (where the destination isn't set), the value
 * pointed to by expectedCurrentValue is still updated with the current value.
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
 */
bool core_util_atomic_cas_u32(uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue);

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
 *                              @Note: This is an in-out parameter. In the
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
 * @Note: In the failure case (where the destination isn't set), the value
 * pointed to by expectedCurrentValue is still updated with the current value.
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
 */
bool core_util_atomic_cas_ptr(void **ptr, void **expectedCurrentValue, void *desiredValue);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
uint8_t core_util_atomic_incr_u8(uint8_t *valuePtr, uint8_t delta);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
uint16_t core_util_atomic_incr_u16(uint16_t *valuePtr, uint16_t delta);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented.
 * @return          The new incremented value.
 */
uint32_t core_util_atomic_incr_u32(uint32_t *valuePtr, uint32_t delta);

/**
 * Atomic increment.
 * @param  valuePtr Target memory location being incremented.
 * @param  delta    The amount being incremented in bytes.
 * @return          The new incremented value.
 *
 * @note The type of the pointer argument is not taken into account
 *       and the pointer is incremented by bytes.
 */
void *core_util_atomic_incr_ptr(void **valuePtr, ptrdiff_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
uint8_t core_util_atomic_decr_u8(uint8_t *valuePtr, uint8_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
uint16_t core_util_atomic_decr_u16(uint16_t *valuePtr, uint16_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented.
 * @return          The new decremented value.
 */
uint32_t core_util_atomic_decr_u32(uint32_t *valuePtr, uint32_t delta);

/**
 * Atomic decrement.
 * @param  valuePtr Target memory location being decremented.
 * @param  delta    The amount being decremented in bytes.
 * @return          The new decremented value.
 *
 * @note The type of the pointer argument is not taken into account
 *       and the pointer is decremented by bytes
 */
void *core_util_atomic_decr_ptr(void **valuePtr, ptrdiff_t delta);

#ifdef __cplusplus
} // extern "C"
#endif


#endif // __MBED_UTIL_CRITICAL_H__

/** @}*/
