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

#ifndef __MBED_ATOMIC_IMPL_H__
#define __MBED_ATOMIC_IMPL_H__

#ifndef __MBED_UTIL_ATOMIC_H__
#error "mbed_atomic_impl.h is designed to be included only by mbed_atomic.h"
#endif

#include <stdint.h>
#include "cmsis.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MBED_DEBUG
/* Plain loads must not have "release" or "acquire+release" order */
#define MBED_CHECK_LOAD_ORDER(order) MBED_ASSERT((order) != mbed_memory_order_release && (order) != mbed_memory_order_acq_rel)

/* Plain stores must not have "consume", "acquire" or "acquire+release" order */
#define MBED_CHECK_STORE_ORDER(order) MBED_ASSERT((order) != mbed_memory_order_consume && (order) != mbed_memory_order_acquire && (order) != mbed_memory_order_acq_rel)

/* Compare exchange needs failure order no stronger than success, and failure can't be "release" or "acquire+release" */
#define MBED_CHECK_CAS_ORDER(success, failure) \
    MBED_ASSERT((failure) <= (success) && (failure) != mbed_memory_order_release && (failure) != mbed_memory_order_acq_rel)
#else
#define MBED_CHECK_LOAD_ORDER(order) (void)0
#define MBED_CHECK_STORE_ORDER(order) (void)0
#define MBED_CHECK_CAS_ORDER(success, failure) (void)0
#endif

/* This is currently just to silence unit tests, so no better test required */
#ifdef __MBED__
#define MBED_ATOMIC_PTR_SIZE 32
#else
#define MBED_ATOMIC_PTR_SIZE 64
#endif

/* Place barrier after a load or read-modify-write if a consume or acquire operation */
#define MBED_ACQUIRE_BARRIER(order) do { \
    if ((order) & (mbed_memory_order_consume|mbed_memory_order_acquire)) { \
        MBED_BARRIER(); \
    } } while (0)

/* Place barrier before a store or read-modify-write if a release operation */
#define MBED_RELEASE_BARRIER(order) do { \
    if ((order) & mbed_memory_order_release) { \
        MBED_BARRIER(); \
    } } while (0)

/* Place barrier after a plain store if a sequentially consistent */
#define MBED_SEQ_CST_BARRIER(order) do { \
    if ((order) == mbed_memory_order_seq_cst) { \
        MBED_BARRIER(); \
    } } while (0)



#if MBED_EXCLUSIVE_ACCESS

/* This header file provides C inline definitions for atomic functions. */
/* For C99 inline semantic compatibility, mbed_atomic_impl.c has out-of-line definitions. */

/****************************** ASSEMBLER **********************************/

// Fiddle about with constraints. These work for GCC and clang, but
// IAR appears to be restricted to having only a single constraint,
// so we can't do immediates.
#if MBED_EXCLUSIVE_ACCESS_THUMB1
#define MBED_DOP_REG "l" // Need low register to get 16-bit 3-op ADD/SUB
#define MBED_CMP_IMM "I" // CMP 8-bit immediate
#define MBED_SUB3_IMM "L" // -7 to +7
#else
#define MBED_DOP_REG "r" // Can use 32-bit 3-op ADD/SUB, so any registers
#define MBED_CMP_IMM "IL" // CMP or CMN, 12-bit immediate
#define MBED_SUB3_IMM "IL" // SUB or ADD, 12-bit immediate
#endif

#if defined __clang__ || defined __GNUC__
#define DO_MBED_LOCKFREE_EXCHG_ASM(M)                           \
    __asm volatile (                                            \
        ".syntax unified\n\t"                                   \
        "LDREX"#M "\t%[oldValue], %[value]\n\t"                 \
        "STREX"#M "\t%[fail], %[newValue], %[value]\n\t"        \
      : [oldValue] "=&r" (oldValue),                            \
        [fail] "=&r" (fail),                                    \
        [value] "+Q" (*valuePtr)                                \
      : [newValue] "r" (newValue)                               \
      :                                                         \
    )
#elif defined __ICCARM__
/* In IAR "r" means low register if Thumbv1 (there's no way to specify any register...) */
#define DO_MBED_LOCKFREE_EXCHG_ASM(M)                           \
    asm volatile (                                              \
        "LDREX"#M "\t%[oldValue], [%[valuePtr]]\n"              \
        "STREX"#M "\t%[fail], %[newValue], [%[valuePtr]]\n"     \
      : [oldValue] "=&r" (oldValue),                            \
        [fail] "=&r" (fail)                                     \
      : [valuePtr] "r" (valuePtr),                              \
        [newValue] "r" (newValue)                               \
      : "memory"                                                \
    )
#endif

#if defined __clang__ || defined __GNUC__
#define DO_MBED_LOCKFREE_NEWVAL_2OP_ASM(OP, Constants, M)       \
    __asm volatile (                                            \
        ".syntax unified\n\t"                                   \
        "LDREX"#M "\t%[newValue], %[value]\n\t"                 \
        #OP       "\t%[newValue], %[arg]\n\t"                   \
        "STREX"#M "\t%[fail], %[newValue], %[value]\n\t"        \
      : [newValue] "=&" MBED_DOP_REG (newValue),                \
        [fail] "=&r" (fail),                                    \
        [value] "+Q" (*valuePtr)                                \
      : [arg] Constants MBED_DOP_REG (arg)                      \
      : "cc"                                                    \
    )
#elif defined __ICCARM__
/* In IAR "r" means low register if Thumbv1 (there's no way to specify any register...) */
/* IAR does not support "ADDS reg, reg", so write as 3-operand */
#define DO_MBED_LOCKFREE_NEWVAL_2OP_ASM(OP, Constants, M)       \
    asm volatile (                                              \
        "LDREX"#M "\t%[newValue], [%[valuePtr]]\n"              \
        #OP       "\t%[newValue], %[newValue], %[arg]\n"        \
        "STREX"#M "\t%[fail], %[newValue], [%[valuePtr]]\n"     \
      : [newValue] "=&r" (newValue),                            \
        [fail] "=&r" (fail)                                     \
      : [valuePtr] "r" (valuePtr),                              \
        [arg] "r" (arg)                                         \
      : "memory", "cc"                                          \
    )
#endif

#if defined __clang__ || defined __GNUC__
#define DO_MBED_LOCKFREE_OLDVAL_3OP_ASM(OP, Constants, M)       \
    __asm volatile (                                            \
        ".syntax unified\n\t"                                   \
        "LDREX"#M "\t%[oldValue], %[value]\n\t"                 \
        #OP       "\t%[newValue], %[oldValue], %[arg]\n\t"      \
        "STREX"#M "\t%[fail], %[newValue], %[value]\n\t"        \
      : [oldValue] "=&" MBED_DOP_REG (oldValue),                \
        [newValue] "=&" MBED_DOP_REG (newValue),                \
        [fail] "=&r" (fail),                                    \
        [value] "+Q" (*valuePtr)                                \
      : [arg] Constants MBED_DOP_REG (arg)                      \
      : "cc"                                                    \
    )
#elif defined __ICCARM__
/* In IAR "r" means low register if Thumbv1 (there's no way to specify any register...) */
#define DO_MBED_LOCKFREE_OLDVAL_3OP_ASM(OP, Constants, M)       \
    asm volatile (                                              \
        "LDREX"#M "\t%[oldValue], [%[valuePtr]]\n"              \
        #OP       "\t%[newValue], %[oldValue], %[arg]\n"        \
        "STREX"#M "\t%[fail], %[newValue], [%[valuePtr]]\n"     \
      : [oldValue] "=&r" (oldValue),                            \
        [newValue] "=&r" (newValue),                            \
        [fail] "=&r" (fail)                                     \
      : [valuePtr] "r" (valuePtr),                              \
        [arg] "r" (arg)                                         \
      : "memory", "cc"                                          \
    )
#endif

/* Bitwise operations are harder to do in ARMv8-M baseline - there
 * are only 2-operand versions of the instructions.
 */
#if defined __clang__ || defined __GNUC__
#define DO_MBED_LOCKFREE_OLDVAL_2OP_ASM(OP, Constants, M)       \
    __asm volatile (                                            \
        ".syntax unified\n\t"                                   \
        "LDREX"#M "\t%[oldValue], %[value]\n\t"                 \
        "MOV"     "\t%[newValue], %[oldValue]\n\t"              \
        #OP       "\t%[newValue], %[arg]\n\t"                   \
        "STREX"#M "\t%[fail], %[newValue], %[value]\n\t"        \
      : [oldValue] "=&r" (oldValue),                            \
        [newValue] "=&l" (newValue),                            \
        [fail] "=&r" (fail),                                    \
        [value] "+Q" (*valuePtr)                                \
      : [arg] Constants "l" (arg)                               \
      : "cc"                                                    \
    )
#elif defined __ICCARM__
#define DO_MBED_LOCKFREE_OLDVAL_2OP_ASM(OP, Constants, M)       \
    asm volatile (                                              \
        "LDREX"#M "\t%[oldValue], [%[valuePtr]]\n"              \
        "MOV"     "\t%[newValue], %[oldValue]\n"                \
        #OP       "\t%[newValue], %[arg]\n"                     \
        "STREX"#M "\t%[fail], %[newValue], [%[valuePtr]]\n"     \
      : [oldValue] "=&r" (oldValue),                            \
        [newValue] "=&r" (newValue),                            \
        [fail] "=&r" (fail)                                     \
      : [valuePtr] "r" (valuePtr),                              \
        [arg] "r" (arg)                                         \
      : "memory", "cc"                                          \
    )
#endif

/* Note that we split ARM and Thumb implementations for CAS, as
 * the key distinction is the handling of conditions. Thumb-2 IT is
 * partially deprecated, so avoid it, making Thumb-1 and Thumb-2
 * implementations the same.
 */
#if MBED_EXCLUSIVE_ACCESS_ARM
#if defined __clang__ || defined __GNUC__
#define DO_MBED_LOCKFREE_CAS_WEAK_ASM(M)                        \
    __asm volatile (                                            \
        ".syntax unified\n\t"                                   \
        "LDREX"#M  "\t%[oldValue], %[value]\n\t"                \
        "SUBS"     "\t%[fail], %[oldValue], %[expectedValue]\n\t"\
        "STREX"#M"EQ\t%[fail], %[desiredValue], %[value]\n\t"   \
      : [oldValue] "=&r" (oldValue),                            \
        [fail] "=&r" (fail),                                    \
        [value] "+Q" (*ptr)                                     \
      : [desiredValue] "r" (desiredValue),                      \
        [expectedValue] "ILr" (expectedValue)                   \
      : "cc"                                                    \
    )
#elif defined __ICCARM__
#define DO_MBED_LOCKFREE_CAS_WEAK_ASM(M)                        \
    asm volatile (                                              \
        "LDREX"#M  "\t%[oldValue], [%[valuePtr]]\n"             \
        "SUBS"     "\t%[fail], %[oldValue], %[expectedValue]\n" \
        "STREX"#M"EQ\t%[fail], %[desiredValue], [%[valuePtr]]\n"\
      : [oldValue] "=&r" (oldValue),                            \
        [fail] "=&r" (fail)                                     \
      : [desiredValue] "r" (desiredValue),                      \
        [expectedValue] "r" (expectedValue),                    \
        [valuePtr] "r" (ptr),                                   \
      : "memory", "cc"                                          \
    )
#endif
#else // MBED_EXCLUSIVE_ACCESS_ARM
#if defined __clang__ || defined __GNUC__
#define DO_MBED_LOCKFREE_CAS_WEAK_ASM(M)                        \
    __asm volatile (                                            \
        ".syntax unified\n\t"                                   \
        "LDREX"#M  "\t%[oldValue], %[value]\n\t"                \
        "SUBS"     "\t%[fail], %[oldValue], %[expectedValue]\n\t"\
        "BNE"      "\t%=f\n\t"                                  \
        "STREX"#M  "\t%[fail], %[desiredValue], %[value]\n"     \
        "%=:"                                                   \
      : [oldValue] "=&" MBED_DOP_REG (oldValue),                \
        [fail] "=&" MBED_DOP_REG (fail),                        \
        [value] "+Q" (*ptr)                                     \
      : [desiredValue] "r" (desiredValue),                      \
        [expectedValue] MBED_SUB3_IMM MBED_DOP_REG (expectedValue) \
      : "cc"                                                    \
    )
#elif defined __ICCARM__
#define DO_MBED_LOCKFREE_CAS_WEAK_ASM(M)                        \
    asm volatile (                                              \
        "LDREX"#M  "\t%[oldValue], [%[valuePtr]]\n"             \
        "SUBS"     "\t%[fail], %[oldValue], %[expectedValue]\n" \
        "BNE"      "\tdone\n\t"                                 \
        "STREX"#M  "\t%[fail], %[desiredValue], [%[valuePtr]]\n"\
        "done:"                                                 \
      : [oldValue] "=&r" (oldValue),                            \
        [fail] "=&r" (fail)                                     \
      : [desiredValue] "r" (desiredValue),                      \
        [expectedValue] "r" (expectedValue),                    \
        [valuePtr] "r" (ptr)                                    \
      : "memory", "cc"                                          \
    )
#endif
#endif // MBED_EXCLUSIVE_ACCESS_ARM

/* For strong CAS, conditional execution is complex enough to
 * not be worthwhile, so all implementations look like Thumb-1.
 * (This is the operation for which STREX returning 0 for success
 * is beneficial.)
 */
#if defined __clang__ || defined __GNUC__
#define DO_MBED_LOCKFREE_CAS_STRONG_ASM(M)                      \
    __asm volatile (                                            \
        ".syntax unified\n\t"                                   \
        "\n%=:\n\t"                                             \
        "LDREX"#M  "\t%[oldValue], %[value]\n\t"                \
        "SUBS"     "\t%[fail], %[oldValue], %[expectedValue]\n\t"\
        "BNE"      "\t%=f\n"                                    \
        "STREX"#M  "\t%[fail], %[desiredValue], %[value]\n\t"   \
        "CMP"      "\t%[fail], #0\n\t"                          \
        "BNE"      "\t%=b\n"                                    \
        "%=:"                                                   \
      : [oldValue] "=&" MBED_DOP_REG (oldValue),                \
        [fail] "=&" MBED_DOP_REG (fail),                        \
        [value] "+Q" (*ptr)                                     \
      : [desiredValue] "r" (desiredValue),                      \
        [expectedValue] MBED_SUB3_IMM MBED_DOP_REG (expectedValue) \
      : "cc"                                                    \
    )
#elif defined __ICCARM__
#define DO_MBED_LOCKFREE_CAS_STRONG_ASM(M)                      \
    asm volatile (                                              \
        "retry:\n"                                              \
        "LDREX"#M  "\t%[oldValue], [%[valuePtr]]\n"             \
        "SUBS"     "\t%[fail], %[oldValue], %[expectedValue]\n" \
        "BNE"      "\tdone\n"                                   \
        "STREX"#M  "\t%[fail], %[desiredValue], [%[valuePtr]]\n"\
        "CMP"      "\t%[fail], #0\n"                            \
        "BNE"      "\tretry\n"                                  \
        "done:"                                                 \
      : [oldValue] "=&r" (oldValue),                            \
        [fail] "=&r" (fail)                                     \
      : [desiredValue] "r" (desiredValue),                      \
        [expectedValue] "r" (expectedValue),                    \
        [valuePtr] "r" (ptr)                                    \
      : "memory", "cc"                                          \
    )
#endif

/********************* LOCK-FREE IMPLEMENTATION MACROS ****************/

/* Note care taken with types here. Values which the assembler outputs correctly
 * narrowed, or inputs without caring about width, are marked as type T. Other
 * values are uint32_t. It's not clear from documentation whether assembler
 * assumes anything about widths,  but try to signal correctly to get necessary
 * narrowing, and avoid unnecessary.
 * Tests show that GCC in particular will pass in unnarrowed values - eg passing
 * "uint8_t arg = -1" to the assembler as 0xFFFFFFFF. This is fine for, eg, add_u8,
 * but wouldn't be for compare_and_exchange_u8.
 * On the other hand, it seems to be impossible to stop GCC inserting narrowing
 * instructions for the output - it will always put in UXTB for the oldValue of
 * an operation.
 */
#define DO_MBED_LOCKFREE_EXCHG_OP(T, fn_suffix, M)                              \
inline T core_util_atomic_exchange_##fn_suffix(volatile T *valuePtr, T newValue) \
{                                                                               \
    T oldValue;                                                                 \
    uint32_t fail;                                                              \
    MBED_BARRIER();                                                             \
    DO_MBED_LOCKFREE_EXCHG_ASM(M);                                              \
    MBED_BARRIER();                                                             \
    return oldValue;                                                            \
}                                                                               \
                                                                                \
MBED_FORCEINLINE T core_util_atomic_exchange_explicit_##fn_suffix(              \
        volatile T *valuePtr, T newValue, mbed_memory_order order)              \
{                                                                               \
    T oldValue;                                                                 \
    uint32_t fail;                                                              \
    MBED_RELEASE_BARRIER(order);                                                \
    DO_MBED_LOCKFREE_EXCHG_ASM(M);                                              \
    MBED_ACQUIRE_BARRIER(order);                                                \
    return oldValue;                                                            \
}

#define DO_MBED_LOCKFREE_CAS_WEAK_OP(T, fn_suffix, M)                           \
inline bool core_util_atomic_compare_exchange_weak_##fn_suffix(volatile T *ptr, T *expectedCurrentValue, T desiredValue) \
{                                                                               \
    MBED_BARRIER();                                                             \
    T oldValue;                                                                 \
    uint32_t fail, expectedValue = *expectedCurrentValue;                       \
    DO_MBED_LOCKFREE_CAS_WEAK_ASM(M);                                           \
    if (fail) {                                                                 \
        *expectedCurrentValue = oldValue;                                       \
    }                                                                           \
    MBED_BARRIER();                                                             \
    return !fail;                                                               \
}                                                                               \
                                                                                \
MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_##fn_suffix(volatile T *ptr, T *expectedCurrentValue, T desiredValue, mbed_memory_order success, mbed_memory_order failure) \
{                                                                               \
    MBED_CHECK_CAS_ORDER(success, failure);                                     \
    MBED_RELEASE_BARRIER(success);                                              \
    T oldValue;                                                                 \
    uint32_t fail, expectedValue = *expectedCurrentValue;                       \
    DO_MBED_LOCKFREE_CAS_WEAK_ASM(M);                                           \
    if (fail) {                                                                 \
        *expectedCurrentValue = oldValue;                                       \
    }                                                                           \
    MBED_ACQUIRE_BARRIER(fail ? failure : success);                             \
    return !fail;                                                               \
}

#define DO_MBED_LOCKFREE_CAS_STRONG_OP(T, fn_suffix, M)                         \
inline bool core_util_atomic_cas_##fn_suffix(volatile T *ptr, T *expectedCurrentValue, T desiredValue) \
{                                                                               \
    MBED_BARRIER();                                                             \
    T oldValue;                                                                 \
    uint32_t fail, expectedValue = *expectedCurrentValue;                       \
    DO_MBED_LOCKFREE_CAS_STRONG_ASM(M);                                         \
    if (fail) {                                                                 \
        *expectedCurrentValue = oldValue;                                       \
    }                                                                           \
    MBED_BARRIER();                                                             \
    return !fail;                                                               \
}                                                                               \
                                                                                \
MBED_FORCEINLINE bool core_util_atomic_cas_explicit_##fn_suffix(volatile T *ptr, T *expectedCurrentValue, T desiredValue, mbed_memory_order success, mbed_memory_order failure) \
{                                                                               \
    MBED_CHECK_CAS_ORDER(success, failure);                                     \
    MBED_RELEASE_BARRIER(success);                                              \
    T oldValue;                                                                 \
    uint32_t fail, expectedValue = *expectedCurrentValue;                       \
    DO_MBED_LOCKFREE_CAS_STRONG_ASM(M);                                         \
    if (fail) {                                                                 \
        *expectedCurrentValue = oldValue;                                       \
    }                                                                           \
    MBED_ACQUIRE_BARRIER(fail ? failure : success);                             \
    return !fail;                                                               \
}


#define DO_MBED_LOCKFREE_NEWVAL_2OP(name, OP, Constants, T, fn_suffix, M)       \
inline T core_util_atomic_##name##_##fn_suffix(volatile T *valuePtr, T arg)     \
{                                                                               \
    uint32_t fail, newValue;                                                    \
    MBED_BARRIER();                                                             \
    do {                                                                        \
        DO_MBED_LOCKFREE_NEWVAL_2OP_ASM(OP, Constants, M);                      \
    } while (fail);                                                             \
    MBED_BARRIER();                                                             \
    return (T) newValue;                                                        \
}                                                                               \
                                                                                \
MBED_FORCEINLINE T core_util_atomic_##name##_explicit_##fn_suffix(              \
        volatile T *valuePtr, T arg, mbed_memory_order order)                   \
{                                                                               \
    uint32_t fail, newValue;                                                    \
    MBED_RELEASE_BARRIER(order);                                                \
    do {                                                                        \
        DO_MBED_LOCKFREE_NEWVAL_2OP_ASM(OP, Constants, M);                      \
    } while (fail);                                                             \
    MBED_ACQUIRE_BARRIER(order);                                                \
    return (T) newValue;                                                        \
}                                                                               \

#define DO_MBED_LOCKFREE_OLDVAL_2OP(name, OP, Constants, T, fn_suffix, M)       \
inline T core_util_atomic_##name##_##fn_suffix(volatile T *valuePtr, T arg)     \
{                                                                               \
    T oldValue;                                                                 \
    uint32_t fail, newValue;                                                    \
    MBED_BARRIER();                                                             \
    do {                                                                        \
        DO_MBED_LOCKFREE_OLDVAL_2OP_ASM(OP, Constants, M);                      \
    } while (fail);                                                             \
    MBED_BARRIER();                                                             \
    return oldValue;                                                            \
}                                                                               \
                                                                                \
MBED_FORCEINLINE T core_util_atomic_##name##_explicit_##fn_suffix(              \
        volatile T *valuePtr, T arg, mbed_memory_order order)                   \
{                                                                               \
    T oldValue;                                                                 \
    uint32_t fail, newValue;                                                    \
    MBED_RELEASE_BARRIER(order);                                                \
    do {                                                                        \
        DO_MBED_LOCKFREE_OLDVAL_2OP_ASM(OP, Constants, M);                      \
    } while (fail);                                                             \
    MBED_ACQUIRE_BARRIER(order);                                                \
    return oldValue;                                                            \
}                                                                               \

#define DO_MBED_LOCKFREE_OLDVAL_3OP(name, OP, Constants, T, fn_suffix, M)       \
inline T core_util_atomic_##name##_##fn_suffix(volatile T *valuePtr, T arg) {   \
    T oldValue;                                                                 \
    uint32_t fail, newValue;                                                    \
    MBED_BARRIER();                                                             \
    do {                                                                        \
        DO_MBED_LOCKFREE_OLDVAL_3OP_ASM(OP, Constants, M);                      \
    } while (fail);                                                             \
    MBED_BARRIER();                                                             \
    return oldValue;                                                            \
}                                                                               \
                                                                                \
MBED_FORCEINLINE T core_util_atomic_##name##_explicit_##fn_suffix(              \
        volatile T *valuePtr, T arg, mbed_memory_order order)                   \
{                                                                               \
    T oldValue;                                                                 \
    uint32_t fail, newValue;                                                    \
    MBED_RELEASE_BARRIER(order);                                                \
    do {                                                                        \
        DO_MBED_LOCKFREE_OLDVAL_3OP_ASM(OP, Constants, M);                      \
    } while (fail);                                                             \
    MBED_ACQUIRE_BARRIER(order);                                                \
    return oldValue;                                                            \
}                                                                               \

inline bool core_util_atomic_flag_test_and_set(volatile core_util_atomic_flag *valuePtr)
{
    MBED_BARRIER();
    bool oldValue, newValue = true;
    uint32_t fail;
    do {
        DO_MBED_LOCKFREE_EXCHG_ASM(B);
    } while (fail);
    MBED_BARRIER();
    return oldValue;
}

MBED_FORCEINLINE bool core_util_atomic_flag_test_and_set_explicit(volatile core_util_atomic_flag *valuePtr, mbed_memory_order order)
{
    MBED_RELEASE_BARRIER(order);
    bool oldValue, newValue = true;
    uint32_t fail;
    do {
        DO_MBED_LOCKFREE_EXCHG_ASM(B);
    } while (fail);
    MBED_ACQUIRE_BARRIER(order);
    return oldValue;
}

/********************* LOCK-FREE IMPLEMENTATION DEFINITIONS ****************/

#define DO_MBED_LOCKFREE_EXCHG_OPS() \
    DO_MBED_LOCKFREE_EXCHG_OP(uint8_t,  u8,  B) \
    DO_MBED_LOCKFREE_EXCHG_OP(uint16_t, u16, H) \
    DO_MBED_LOCKFREE_EXCHG_OP(uint32_t, u32,  )

#define DO_MBED_LOCKFREE_NEWVAL_2OPS(name, OP, Constants) \
    DO_MBED_LOCKFREE_NEWVAL_2OP(name, OP, Constants, uint8_t,  u8,  B) \
    DO_MBED_LOCKFREE_NEWVAL_2OP(name, OP, Constants, uint16_t, u16, H) \
    DO_MBED_LOCKFREE_NEWVAL_2OP(name, OP, Constants, uint32_t, u32,  )

#define DO_MBED_LOCKFREE_OLDVAL_3OPS(name, OP, Constants) \
    DO_MBED_LOCKFREE_OLDVAL_3OP(name, OP, Constants, uint8_t,  u8,  B) \
    DO_MBED_LOCKFREE_OLDVAL_3OP(name, OP, Constants, uint16_t, u16, H) \
    DO_MBED_LOCKFREE_OLDVAL_3OP(name, OP, Constants, uint32_t, u32,  )

#define DO_MBED_LOCKFREE_OLDVAL_2OPS(name, OP, Constants) \
    DO_MBED_LOCKFREE_OLDVAL_2OP(name, OP, Constants, uint8_t,  u8,  B) \
    DO_MBED_LOCKFREE_OLDVAL_2OP(name, OP, Constants, uint16_t, u16, H) \
    DO_MBED_LOCKFREE_OLDVAL_2OP(name, OP, Constants, uint32_t, u32,  )

#define DO_MBED_LOCKFREE_CAS_STRONG_OPS() \
    DO_MBED_LOCKFREE_CAS_STRONG_OP(uint8_t,  u8,  B) \
    DO_MBED_LOCKFREE_CAS_STRONG_OP(uint16_t, u16, H) \
    DO_MBED_LOCKFREE_CAS_STRONG_OP(uint32_t, u32,  )

#define DO_MBED_LOCKFREE_CAS_WEAK_OPS() \
    DO_MBED_LOCKFREE_CAS_WEAK_OP(uint8_t,  u8,  B) \
    DO_MBED_LOCKFREE_CAS_WEAK_OP(uint16_t, u16, H) \
    DO_MBED_LOCKFREE_CAS_WEAK_OP(uint32_t, u32,  )

// Note that these macros define a number of functions that are
// not in mbed_atomic.h, like core_util_atomic_and_fetch_u16.
// These are not documented via the doxygen in mbed_atomic.h, so
// for now should be regarded as internal only. They are used by the
// Atomic<T> template as an optimisation though.

// We always use the "S" form of operations - avoids yet another
// possible unneeded distinction between Thumbv1 and Thumbv2, and
// may reduce code size by allowing 16-bit instructions.
#if !MBED_EXCLUSIVE_ACCESS_THUMB1
// I constraint is 12-bit modified immediate constant
// L constraint is negated 12-bit modified immediate constant
// (relying on assembler to swap ADD/SUB)
// We could permit J (-4095 to +4095) if we used ADD/SUB
// instead of ADDS/SUBS, but then that would block generation
// of the 16-bit forms. Shame we can't specify "don't care"
// for the "S", or get the GNU multi-alternative to
// choose ADDS/ADD appropriately.

DO_MBED_LOCKFREE_OLDVAL_3OPS(fetch_add, ADDS, "IL")
DO_MBED_LOCKFREE_NEWVAL_2OPS(incr,      ADDS, "IL")
DO_MBED_LOCKFREE_OLDVAL_3OPS(fetch_sub, SUBS, "IL")
DO_MBED_LOCKFREE_NEWVAL_2OPS(decr,      SUBS, "IL")
// K constraint is inverted 12-bit modified immediate constant
// (relying on assembler substituting BIC for AND)
DO_MBED_LOCKFREE_OLDVAL_3OPS(fetch_and, ANDS, "IK")
DO_MBED_LOCKFREE_NEWVAL_2OPS(and_fetch, ANDS, "IK")
#if MBED_EXCLUSIVE_ACCESS_ARM
// ARM does not have ORN instruction, so take plain immediates.
DO_MBED_LOCKFREE_OLDVAL_3OPS(fetch_or,  ORRS, "I")
DO_MBED_LOCKFREE_NEWVAL_2OPS(or_fetch,  ORRS, "I")
#else
// Thumb-2 has ORN instruction, and assembler substitutes ORN for ORR.
DO_MBED_LOCKFREE_OLDVAL_3OPS(fetch_or,  ORRS, "IK")
DO_MBED_LOCKFREE_NEWVAL_2OPS(or_fetch,  ORRS, "IK")
#endif
// I constraint is 12-bit modified immediate operand
DO_MBED_LOCKFREE_OLDVAL_3OPS(fetch_xor, EORS, "I")
DO_MBED_LOCKFREE_NEWVAL_2OPS(xor_fetch, EORS, "I")
#else // MBED_EXCLUSIVE_ACCESS_THUMB1
// I constraint is 0-255; J is -255 to -1, suitable for
// 2-op ADD/SUB (relying on assembler to swap ADD/SUB)
// L constraint is -7 to +7, suitable for 3-op ADD/SUB
// (relying on assembler to swap ADD/SUB)
DO_MBED_LOCKFREE_OLDVAL_3OPS(fetch_add, ADDS, "L")
DO_MBED_LOCKFREE_NEWVAL_2OPS(incr,      ADDS, "IJ")
DO_MBED_LOCKFREE_OLDVAL_3OPS(fetch_sub, SUBS, "L")
DO_MBED_LOCKFREE_NEWVAL_2OPS(decr,      SUBS, "IJ")
DO_MBED_LOCKFREE_OLDVAL_2OPS(fetch_and, ANDS, "")
DO_MBED_LOCKFREE_NEWVAL_2OPS(and_fetch, ANDS, "")
DO_MBED_LOCKFREE_OLDVAL_2OPS(fetch_or,  ORRS, "")
DO_MBED_LOCKFREE_NEWVAL_2OPS(or_fetch,  ORRS, "")
DO_MBED_LOCKFREE_OLDVAL_2OPS(fetch_xor, EORS, "")
DO_MBED_LOCKFREE_NEWVAL_2OPS(xor_fetch, EORS, "")
#endif

DO_MBED_LOCKFREE_EXCHG_OPS()
DO_MBED_LOCKFREE_CAS_STRONG_OPS()
DO_MBED_LOCKFREE_CAS_WEAK_OPS()

#define DO_MBED_LOCKED_FETCH_OP_ORDERINGS(name) \
    DO_MBED_LOCKED_FETCH_OP_ORDERING(name, uint64_t, u64)
#define DO_MBED_LOCKED_CAS_ORDERINGS(name) \
    DO_MBED_LOCKED_CAS_ORDERING(name, uint64_t, u64)
#else // MBED_EXCLUSIVE_ACCESS
/* All the operations are locked, so need no ordering parameters */
#define DO_MBED_LOCKED_FETCH_OP_ORDERINGS(name) \
    DO_MBED_LOCKED_FETCH_OP_ORDERING(name, uint8_t,  u8) \
    DO_MBED_LOCKED_FETCH_OP_ORDERING(name, uint16_t, u16) \
    DO_MBED_LOCKED_FETCH_OP_ORDERING(name, uint32_t, u32) \
    DO_MBED_LOCKED_FETCH_OP_ORDERING(name, uint64_t, u64)
#define DO_MBED_LOCKED_CAS_ORDERINGS(name) \
    DO_MBED_LOCKED_CAS_ORDERING(name, uint8_t,  u8) \
    DO_MBED_LOCKED_CAS_ORDERING(name, uint16_t, u16) \
    DO_MBED_LOCKED_CAS_ORDERING(name, uint32_t, u32) \
    DO_MBED_LOCKED_CAS_ORDERING(name, uint64_t, u64)

MBED_FORCEINLINE bool core_util_atomic_flag_test_and_set_explicit(volatile core_util_atomic_flag *valuePtr, mbed_memory_order order)
{
    return core_util_atomic_flag_test_and_set(valuePtr);
}
#endif // MBED_EXCLUSIVE_ACCESS

/********************* OPERATIONS THAT ARE ALWAYS LOCK-FREE  ****************/

/* Lock-free loads and stores don't need assembler - just aligned accesses */
/* Silly ordering of `T volatile` is because T can be `void *` */
#define DO_MBED_LOCKFREE_LOADSTORE(T, V, fn_suffix)                             \
MBED_FORCEINLINE T core_util_atomic_load_##fn_suffix(T const V *valuePtr)       \
{                                                                               \
    T value = *valuePtr;                                                        \
    MBED_BARRIER();                                                             \
    return value;                                                               \
}                                                                               \
                                                                                \
MBED_FORCEINLINE T core_util_atomic_load_explicit_##fn_suffix(T const V *valuePtr, mbed_memory_order order) \
{                                                                               \
    MBED_CHECK_LOAD_ORDER(order);                                               \
    T value = *valuePtr;                                                        \
    MBED_ACQUIRE_BARRIER(order);                                                \
    return value;                                                               \
}                                                                               \
                                                                                \
MBED_FORCEINLINE void core_util_atomic_store_##fn_suffix(T V *valuePtr, T value) \
{                                                                               \
    MBED_BARRIER();                                                             \
    *valuePtr = value;                                                          \
    MBED_BARRIER();                                                             \
}                                                                               \
                                                                                \
MBED_FORCEINLINE void core_util_atomic_store_explicit_##fn_suffix(T V *valuePtr, T value, mbed_memory_order order) \
{                                                                               \
    MBED_CHECK_STORE_ORDER(order);                                              \
    MBED_RELEASE_BARRIER(order);                                                \
    *valuePtr = value;                                                          \
    MBED_SEQ_CST_BARRIER(order);                                                \
}

MBED_FORCEINLINE void core_util_atomic_flag_clear(volatile core_util_atomic_flag *flagPtr)
{
    MBED_BARRIER();
    flagPtr->_flag = false;
    MBED_BARRIER();
}

MBED_FORCEINLINE void core_util_atomic_flag_clear_explicit(volatile core_util_atomic_flag *flagPtr, mbed_memory_order order)
{
    MBED_CHECK_STORE_ORDER(order);
    MBED_RELEASE_BARRIER(order);
    flagPtr->_flag = false;
    MBED_SEQ_CST_BARRIER(order);
}

#ifdef __cplusplus
// Temporarily turn off extern "C", so we can provide non-volatile load/store
// overloads for efficiency. All these functions are static inline, so this has
// no linkage effect exactly, it just permits the overloads.
} // extern "C"

// For efficiency it's worth having non-volatile overloads
MBED_FORCEINLINE void core_util_atomic_flag_clear(core_util_atomic_flag *flagPtr)
{
    MBED_BARRIER();
    flagPtr->_flag = false;
    MBED_BARRIER();
}

MBED_FORCEINLINE void core_util_atomic_flag_clear_explicit(core_util_atomic_flag *flagPtr, mbed_memory_order order)
{
    MBED_RELEASE_BARRIER(order);
    flagPtr->_flag = false;
    MBED_SEQ_CST_BARRIER(order);
}

DO_MBED_LOCKFREE_LOADSTORE(uint8_t,, u8)
DO_MBED_LOCKFREE_LOADSTORE(uint16_t,, u16)
DO_MBED_LOCKFREE_LOADSTORE(uint32_t,, u32)
DO_MBED_LOCKFREE_LOADSTORE(int8_t,, s8)
DO_MBED_LOCKFREE_LOADSTORE(int16_t,, s16)
DO_MBED_LOCKFREE_LOADSTORE(int32_t,, s32)
DO_MBED_LOCKFREE_LOADSTORE(bool,, bool)
DO_MBED_LOCKFREE_LOADSTORE(void *,, ptr)

#endif

DO_MBED_LOCKFREE_LOADSTORE(uint8_t, volatile, u8)
DO_MBED_LOCKFREE_LOADSTORE(uint16_t, volatile, u16)
DO_MBED_LOCKFREE_LOADSTORE(uint32_t, volatile, u32)
DO_MBED_LOCKFREE_LOADSTORE(int8_t, volatile, s8)
DO_MBED_LOCKFREE_LOADSTORE(int16_t, volatile, s16)
DO_MBED_LOCKFREE_LOADSTORE(int32_t, volatile, s32)
DO_MBED_LOCKFREE_LOADSTORE(bool, volatile, bool)
DO_MBED_LOCKFREE_LOADSTORE(void *, volatile, ptr)

#ifdef __cplusplus
extern "C" {
#endif

/********************* GENERIC VARIANTS - SIGNED, BOOL, POINTERS  ****************/

MBED_FORCEINLINE int64_t core_util_atomic_load_s64(const volatile int64_t *valuePtr)
{
    return (int64_t)core_util_atomic_load_u64((const volatile uint64_t *)valuePtr);
}

MBED_FORCEINLINE void core_util_atomic_store_s64(volatile int64_t *valuePtr, int64_t desiredValue)
{
    core_util_atomic_store_u64((volatile uint64_t *)valuePtr, (uint64_t)desiredValue);
}

#define DO_MBED_SIGNED_CAS_OP(name, T, fn_suffix)                                           \
MBED_FORCEINLINE bool core_util_atomic_##name##_s##fn_suffix(volatile T *ptr,               \
        T *expectedCurrentValue, T desiredValue)                                            \
{                                                                                           \
    return core_util_atomic_##name##_u##fn_suffix((volatile u##T *)ptr,                     \
                   (u##T *)expectedCurrentValue, (u##T)desiredValue);                       \
}                                                                                           \
                                                                                            \
MBED_FORCEINLINE bool core_util_atomic_##name##_explicit_s##fn_suffix(volatile T *ptr,      \
        T *expectedCurrentValue, T desiredValue,                                            \
        mbed_memory_order success, mbed_memory_order failure)                               \
{                                                                                           \
    return core_util_atomic_##name##_explicit_u##fn_suffix((volatile u##T *)ptr,            \
                   (u##T *)expectedCurrentValue, (u##T)desiredValue, success, failure);     \
}

#define DO_MBED_SIGNED_CAS_OPS(name) \
        DO_MBED_SIGNED_CAS_OP(name, int8_t, 8) \
        DO_MBED_SIGNED_CAS_OP(name, int16_t, 16) \
        DO_MBED_SIGNED_CAS_OP(name, int32_t, 32) \
        DO_MBED_SIGNED_CAS_OP(name, int64_t, 64)

DO_MBED_SIGNED_CAS_OPS(cas)
DO_MBED_SIGNED_CAS_OPS(compare_exchange_weak)

MBED_FORCEINLINE bool core_util_atomic_cas_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue)
{
    return core_util_atomic_cas_u8((volatile uint8_t *)ptr, (uint8_t *)expectedCurrentValue, desiredValue);
}

MBED_FORCEINLINE bool core_util_atomic_cas_explicit_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue, mbed_memory_order success, mbed_memory_order failure)
{
    return core_util_atomic_cas_explicit_u8((volatile uint8_t *)ptr, (uint8_t *)expectedCurrentValue, desiredValue, success, failure);
}

inline bool core_util_atomic_cas_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return core_util_atomic_cas_u32(
               (volatile uint32_t *)ptr,
               (uint32_t *)expectedCurrentValue,
               (uint32_t)desiredValue);
#else
    return core_util_atomic_cas_u64(
               (volatile uint64_t *)ptr,
               (uint64_t *)expectedCurrentValue,
               (uint64_t)desiredValue);
#endif
}

MBED_FORCEINLINE bool core_util_atomic_cas_explicit_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue, mbed_memory_order success, mbed_memory_order failure)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return core_util_atomic_cas_explicit_u32(
               (volatile uint32_t *)ptr,
               (uint32_t *)expectedCurrentValue,
               (uint32_t)desiredValue,
               success, failure);
#else
    return core_util_atomic_cas_explicit_u64(
               (volatile uint64_t *)ptr,
               (uint64_t *)expectedCurrentValue,
               (uint64_t)desiredValue,
               success, failure);
#endif
}

MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue)
{
    return core_util_atomic_compare_exchange_weak_u8((volatile uint8_t *)ptr, (uint8_t *)expectedCurrentValue, desiredValue);
}

MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_bool(volatile bool *ptr, bool *expectedCurrentValue, bool desiredValue, mbed_memory_order success, mbed_memory_order failure)
{
    return core_util_atomic_compare_exchange_weak_explicit_u8((volatile uint8_t *)ptr, (uint8_t *)expectedCurrentValue, desiredValue, success, failure);
}

MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return core_util_atomic_compare_exchange_weak_u32(
               (volatile uint32_t *)ptr,
               (uint32_t *)expectedCurrentValue,
               (uint32_t)desiredValue);
#else
    return core_util_atomic_compare_exchange_weak_u64(
               (volatile uint64_t *)ptr,
               (uint64_t *)expectedCurrentValue,
               (uint64_t)desiredValue);
#endif
}

MBED_FORCEINLINE bool core_util_atomic_compare_exchange_weak_explicit_ptr(void *volatile *ptr, void **expectedCurrentValue, void *desiredValue, mbed_memory_order success, mbed_memory_order failure)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return core_util_atomic_compare_exchange_weak_explicit_u32(
               (volatile uint32_t *)ptr,
               (uint32_t *)expectedCurrentValue,
               (uint32_t)desiredValue,
               success, failure);
#else
    return core_util_atomic_compare_exchange_weak_explicit_u64(
               (volatile uint64_t *)ptr,
               (uint64_t *)expectedCurrentValue,
               (uint64_t)desiredValue,
               success, failure);
#endif
}

#define DO_MBED_SIGNED_FETCH_OP(name, T, fn_suffix)                                         \
MBED_FORCEINLINE T core_util_atomic_##name##_s##fn_suffix(volatile T *valuePtr, T arg)      \
{                                                                                           \
    return (T)core_util_atomic_##name##_u##fn_suffix((volatile u##T *)valuePtr, (u##T)arg); \
}

#define DO_MBED_SIGNED_EXPLICIT_FETCH_OP(name, T, fn_suffix)                                \
MBED_FORCEINLINE T core_util_atomic_##name##_explicit_s##fn_suffix(volatile T *valuePtr, T arg, mbed_memory_order order) \
{                                                                                           \
    return (T)core_util_atomic_##name##_explicit_u##fn_suffix((volatile u##T *)valuePtr, (u##T)arg, order); \
}

#define DO_MBED_SIGNED_FETCH_OPS(name) \
    DO_MBED_SIGNED_FETCH_OP(name, int8_t, 8) \
    DO_MBED_SIGNED_FETCH_OP(name, int16_t, 16) \
    DO_MBED_SIGNED_FETCH_OP(name, int32_t, 32) \
    DO_MBED_SIGNED_FETCH_OP(name, int64_t, 64)

#define DO_MBED_SIGNED_EXPLICIT_FETCH_OPS(name) \
        DO_MBED_SIGNED_EXPLICIT_FETCH_OP(name, int8_t, 8) \
        DO_MBED_SIGNED_EXPLICIT_FETCH_OP(name, int16_t, 16) \
        DO_MBED_SIGNED_EXPLICIT_FETCH_OP(name, int32_t, 32) \
        DO_MBED_SIGNED_EXPLICIT_FETCH_OP(name, int64_t, 64)

DO_MBED_SIGNED_FETCH_OPS(exchange)
DO_MBED_SIGNED_FETCH_OPS(incr)
DO_MBED_SIGNED_FETCH_OPS(decr)
DO_MBED_SIGNED_FETCH_OPS(fetch_add)
DO_MBED_SIGNED_FETCH_OPS(fetch_sub)

DO_MBED_SIGNED_EXPLICIT_FETCH_OPS(exchange)
DO_MBED_SIGNED_EXPLICIT_FETCH_OPS(fetch_add)
DO_MBED_SIGNED_EXPLICIT_FETCH_OPS(fetch_sub)

MBED_FORCEINLINE bool core_util_atomic_exchange_bool(volatile bool *valuePtr, bool desiredValue)
{
    return (bool)core_util_atomic_exchange_u8((volatile uint8_t *)valuePtr, desiredValue);
}

MBED_FORCEINLINE bool core_util_atomic_exchange_explicit_bool(volatile bool *valuePtr, bool desiredValue, mbed_memory_order order)
{
    return (bool)core_util_atomic_exchange_explicit_u8((volatile uint8_t *)valuePtr, desiredValue, order);
}

inline void *core_util_atomic_exchange_ptr(void *volatile *valuePtr, void *desiredValue)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return (void *)core_util_atomic_exchange_u32((volatile uint32_t *)valuePtr, (uint32_t)desiredValue);
#else
    return (void *)core_util_atomic_exchange_u64((volatile uint64_t *)valuePtr, (uint64_t)desiredValue);
#endif
}

MBED_FORCEINLINE void *core_util_atomic_exchange_explicit_ptr(void *volatile *valuePtr, void *desiredValue, mbed_memory_order order)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return (void *)core_util_atomic_exchange_explicit_u32((volatile uint32_t *)valuePtr, (uint32_t)desiredValue, order);
#else
    return (void *)core_util_atomic_exchange_explicit_u64((volatile uint64_t *)valuePtr, (uint64_t)desiredValue, order);
#endif
}

inline void *core_util_atomic_incr_ptr(void *volatile *valuePtr, ptrdiff_t delta)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return (void *)core_util_atomic_incr_u32((volatile uint32_t *)valuePtr, (uint32_t)delta);
#else
    return (void *)core_util_atomic_incr_u64((volatile uint64_t *)valuePtr, (uint64_t)delta);
#endif
}

inline void *core_util_atomic_decr_ptr(void *volatile *valuePtr, ptrdiff_t delta)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return (void *)core_util_atomic_decr_u32((volatile uint32_t *)valuePtr, (uint32_t)delta);
#else
    return (void *)core_util_atomic_decr_u64((volatile uint64_t *)valuePtr, (uint64_t)delta);
#endif
}

MBED_FORCEINLINE void *core_util_atomic_fetch_add_ptr(void *volatile *valuePtr, ptrdiff_t arg)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return (void *)core_util_atomic_fetch_add_u32((volatile uint32_t *)valuePtr, (uint32_t)arg);
#else
    return (void *)core_util_atomic_fetch_add_u64((volatile uint64_t *)valuePtr, (uint64_t)arg);
#endif
}

MBED_FORCEINLINE void *core_util_atomic_fetch_add_explicit_ptr(void *volatile *valuePtr, ptrdiff_t arg, mbed_memory_order order)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return (void *)core_util_atomic_fetch_add_explicit_u32((volatile uint32_t *)valuePtr, (uint32_t)arg, order);
#else
    return (void *)core_util_atomic_fetch_add_explicit_u64((volatile uint64_t *)valuePtr, (uint64_t)arg, order);
#endif
}

MBED_FORCEINLINE void *core_util_atomic_fetch_sub_ptr(void *volatile *valuePtr, ptrdiff_t arg)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return (void *)core_util_atomic_fetch_sub_u32((volatile uint32_t *)valuePtr, (uint32_t)arg);
#else
    return (void *)core_util_atomic_fetch_sub_u64((volatile uint64_t *)valuePtr, (uint64_t)arg);
#endif
}

MBED_FORCEINLINE void *core_util_atomic_fetch_sub_explicit_ptr(void *volatile *valuePtr, ptrdiff_t arg, mbed_memory_order order)
{
#if MBED_ATOMIC_PTR_SIZE == 32
    return (void *)core_util_atomic_fetch_sub_explicit_u32((volatile uint32_t *)valuePtr, (uint32_t)arg, order);
#else
    return (void *)core_util_atomic_fetch_sub_explicit_u64((volatile uint64_t *)valuePtr, (uint64_t)arg, order);
#endif
}

/***************** DUMMY EXPLICIT ORDERING FOR LOCKED OPS  *****************/

/* Need to throw away the ordering information for all locked operations */
MBED_FORCEINLINE uint64_t core_util_atomic_load_explicit_u64(const volatile uint64_t *valuePtr, MBED_UNUSED mbed_memory_order order)
{
    MBED_CHECK_LOAD_ORDER(order);
    return core_util_atomic_load_u64(valuePtr);
}

MBED_FORCEINLINE int64_t core_util_atomic_load_explicit_s64(const volatile int64_t *valuePtr, MBED_UNUSED mbed_memory_order order)
{
    MBED_CHECK_LOAD_ORDER(order);
    return core_util_atomic_load_s64(valuePtr);
}

MBED_FORCEINLINE void core_util_atomic_store_explicit_u64(volatile uint64_t *valuePtr, uint64_t desiredValue, MBED_UNUSED mbed_memory_order order)
{
    MBED_CHECK_STORE_ORDER(order);
    core_util_atomic_store_u64(valuePtr, desiredValue);
}

MBED_FORCEINLINE void core_util_atomic_store_explicit_s64(volatile int64_t *valuePtr, int64_t desiredValue, MBED_UNUSED mbed_memory_order order)
{
    MBED_CHECK_STORE_ORDER(order);
    core_util_atomic_store_s64(valuePtr, desiredValue);
}

#define DO_MBED_LOCKED_FETCH_OP_ORDERING(name, T, fn_suffix)                    \
MBED_FORCEINLINE T core_util_atomic_##name##_explicit_##fn_suffix(              \
        volatile T *valuePtr, T arg, MBED_UNUSED mbed_memory_order order)       \
{                                                                               \
    return core_util_atomic_##name##_##fn_suffix(valuePtr, arg);                \
}

#define DO_MBED_LOCKED_CAS_ORDERING(name, T, fn_suffix)                         \
MBED_FORCEINLINE bool core_util_atomic_##name##_explicit_##fn_suffix(           \
        volatile T *ptr, T *expectedCurrentValue, T desiredValue,               \
        MBED_UNUSED mbed_memory_order success,                                  \
        MBED_UNUSED mbed_memory_order failure)                                  \
{                                                                               \
    MBED_CHECK_CAS_ORDER(success, failure);                                     \
    return core_util_atomic_##name##_##fn_suffix(ptr, expectedCurrentValue, desiredValue); \
}

DO_MBED_LOCKED_FETCH_OP_ORDERINGS(exchange)
DO_MBED_LOCKED_FETCH_OP_ORDERINGS(fetch_add)
DO_MBED_LOCKED_FETCH_OP_ORDERINGS(fetch_sub)
DO_MBED_LOCKED_FETCH_OP_ORDERINGS(fetch_and)
DO_MBED_LOCKED_FETCH_OP_ORDERINGS(fetch_or)
DO_MBED_LOCKED_FETCH_OP_ORDERINGS(fetch_xor)
DO_MBED_LOCKED_CAS_ORDERINGS(cas)
DO_MBED_LOCKED_CAS_ORDERINGS(compare_exchange_weak)

#ifdef __cplusplus
} // extern "C"

/***************** TEMPLATE IMPLEMENTATIONS  *****************/

/* Each of these groups provides specialisations for the T template for each of
 * the small types (there is no base implementation), and the base implementation
 * of the T * template.
 */
#define DO_MBED_ATOMIC_LOAD_TEMPLATE(T, fn_suffix)                              \
template<>                                                                      \
inline T core_util_atomic_load(const volatile T *valuePtr) noexcept             \
{                                                                               \
    return core_util_atomic_load_##fn_suffix(valuePtr);                         \
}                                                                               \
                                                                                \
template<>                                                                      \
inline T core_util_atomic_load(const T *valuePtr) noexcept                      \
{                                                                               \
    return core_util_atomic_load_##fn_suffix(valuePtr);                         \
}                                                                               \
                                                                                \
template<>                                                                      \
inline T core_util_atomic_load_explicit(const volatile T *valuePtr, mbed_memory_order order)  noexcept \
{                                                                               \
    return core_util_atomic_load_explicit_##fn_suffix(valuePtr, order);         \
}                                                                               \
                                                                                \
template<>                                                                      \
inline T core_util_atomic_load_explicit(const T *valuePtr, mbed_memory_order order) noexcept \
{                                                                               \
    return core_util_atomic_load_explicit_##fn_suffix(valuePtr, order);         \
}

template<typename T>
inline T *core_util_atomic_load(T *const volatile *valuePtr) noexcept
{
    return (T *) core_util_atomic_load_ptr((void *const volatile *) valuePtr);
}

template<typename T>
inline T *core_util_atomic_load(T *const *valuePtr) noexcept
{
    return (T *) core_util_atomic_load_ptr((void *const *) valuePtr);
}

template<typename T>
inline T *core_util_atomic_load_explicit(T *const volatile *valuePtr, mbed_memory_order order) noexcept
{
    return (T *) core_util_atomic_load_explicit_ptr((void *const volatile *) valuePtr, order);
}

template<typename T>
inline T *core_util_atomic_load_explicit(T *const *valuePtr, mbed_memory_order order) noexcept
{
    return (T *) core_util_atomic_load_explicit_ptr((void *const *) valuePtr, order);
}

DO_MBED_ATOMIC_LOAD_TEMPLATE(uint8_t,  u8)
DO_MBED_ATOMIC_LOAD_TEMPLATE(uint16_t, u16)
DO_MBED_ATOMIC_LOAD_TEMPLATE(uint32_t, u32)
DO_MBED_ATOMIC_LOAD_TEMPLATE(uint64_t, u64)
DO_MBED_ATOMIC_LOAD_TEMPLATE(int8_t,  s8)
DO_MBED_ATOMIC_LOAD_TEMPLATE(int16_t, s16)
DO_MBED_ATOMIC_LOAD_TEMPLATE(int32_t, s32)
DO_MBED_ATOMIC_LOAD_TEMPLATE(int64_t, s64)
DO_MBED_ATOMIC_LOAD_TEMPLATE(bool, bool)

#define DO_MBED_ATOMIC_STORE_TEMPLATE(T, fn_suffix)                             \
template<>                                                                      \
inline void core_util_atomic_store(volatile T *valuePtr, T val) noexcept        \
{                                                                               \
    core_util_atomic_store_##fn_suffix(valuePtr, val);                          \
}                                                                               \
                                                                                \
template<>                                                                      \
inline void core_util_atomic_store(T *valuePtr, T val) noexcept                 \
{                                                                               \
    core_util_atomic_store_##fn_suffix(valuePtr, val);                          \
}                                                                               \
                                                                                \
template<>                                                                      \
inline void core_util_atomic_store_explicit(volatile T *valuePtr, T val, mbed_memory_order order) noexcept \
{                                                                               \
    core_util_atomic_store_explicit_##fn_suffix(valuePtr, val, order);          \
}                                                                               \
                                                                                \
template<>                                                                      \
inline void core_util_atomic_store_explicit(T *valuePtr, T val, mbed_memory_order order) noexcept \
{                                                                               \
    core_util_atomic_store_explicit_##fn_suffix(valuePtr, val, order);          \
}

template<typename T>
inline void core_util_atomic_store(T *volatile *valuePtr, T *val) noexcept
{
    core_util_atomic_store_ptr((void *volatile *) valuePtr, val);
}

template<typename T>
inline void core_util_atomic_store(T **valuePtr, T *val) noexcept
{
    core_util_atomic_store_ptr((void **) valuePtr, val);
}

template<typename T>
inline void core_util_atomic_store_explicit(T *volatile *valuePtr, T *val, mbed_memory_order order) noexcept
{
    core_util_atomic_store_ptr((void *volatile *) valuePtr, val, order);
}

template<typename T>
inline void core_util_atomic_store_explicit(T **valuePtr, T *val, mbed_memory_order order) noexcept
{
    core_util_atomic_store_ptr((void **) valuePtr, val, order);
}

DO_MBED_ATOMIC_STORE_TEMPLATE(uint8_t,  u8)
DO_MBED_ATOMIC_STORE_TEMPLATE(uint16_t, u16)
DO_MBED_ATOMIC_STORE_TEMPLATE(uint32_t, u32)
DO_MBED_ATOMIC_STORE_TEMPLATE(uint64_t, u64)
DO_MBED_ATOMIC_STORE_TEMPLATE(int8_t,  s8)
DO_MBED_ATOMIC_STORE_TEMPLATE(int16_t, s16)
DO_MBED_ATOMIC_STORE_TEMPLATE(int32_t, s32)
DO_MBED_ATOMIC_STORE_TEMPLATE(int64_t, s64)
DO_MBED_ATOMIC_STORE_TEMPLATE(bool, bool)

#define DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, T, fn_suffix)                             \
template<> inline                                                                           \
bool core_util_atomic_##tname(volatile T *ptr, T *expectedCurrentValue, T desiredValue) noexcept \
{                                                                                           \
    return core_util_atomic_##fname##_##fn_suffix(ptr, expectedCurrentValue, desiredValue); \
}

template<typename T>
inline bool core_util_atomic_compare_exchange_strong(T *volatile *ptr, T **expectedCurrentValue, T *desiredValue) noexcept
{
    return core_util_atomic_cas_ptr((void *volatile *) ptr, (void **) expectedCurrentValue, desiredValue);
}

template<typename T>
inline bool core_util_atomic_compare_exchange_weak(T *volatile *ptr, T **expectedCurrentValue, T *desiredValue) noexcept
{
    return core_util_atomic_compare_exchange_weak_ptr((void *volatile *) ptr, (void **) expectedCurrentValue, desiredValue);
}

#define DO_MBED_ATOMIC_CAS_TEMPLATES(tname, fname) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, uint8_t,  u8) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, uint16_t, u16) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, uint32_t, u32) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, uint64_t, u64) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, int8_t,   s8) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, int16_t,  s16) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, int32_t,  s32) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, int64_t,  s64) \
        DO_MBED_ATOMIC_CAS_TEMPLATE(tname, fname, bool, bool)

DO_MBED_ATOMIC_CAS_TEMPLATES(compare_exchange_strong, cas)
DO_MBED_ATOMIC_CAS_TEMPLATES(compare_exchange_weak, compare_exchange_weak)

#define DO_MBED_ATOMIC_OP_TEMPLATE(name, T, fn_suffix)                          \
template<>                                                                      \
inline T core_util_atomic_##name(volatile T *valuePtr, T arg) noexcept          \
{                                                                               \
    return core_util_atomic_##name##_##fn_suffix(valuePtr, arg);                \
}                                                                               \
                                                                                \
template<>                                                                      \
inline T core_util_atomic_##name##_explicit(volatile T *valuePtr, T arg,        \
        mbed_memory_order order) noexcept                                       \
{                                                                               \
    return core_util_atomic_##name##_explicit_##fn_suffix(valuePtr, arg, order); \
}


template<>
inline bool core_util_atomic_exchange(volatile bool *valuePtr, bool arg) noexcept
{
    return core_util_atomic_exchange_bool(valuePtr, arg);
}

template<>
inline bool core_util_atomic_exchange_explicit(volatile bool *valuePtr, bool arg, mbed_memory_order order) noexcept
{
    return core_util_atomic_exchange_explicit_bool(valuePtr, arg, order);
}

template<typename T>
inline T *core_util_atomic_exchange(T *volatile *valuePtr, T *arg) noexcept
{
    return (T *) core_util_atomic_exchange_ptr((void *volatile *) valuePtr, arg);
}

template<typename T>
inline T *core_util_atomic_exchange_explicit(T *volatile *valuePtr, T *arg, mbed_memory_order order) noexcept
{
    return (T *) core_util_atomic_fetch_add_explicit_ptr((void *volatile *) valuePtr, arg, order);
}

template<typename T>
inline T *core_util_atomic_fetch_add(T *volatile *valuePtr, ptrdiff_t arg) noexcept
{
    return (T *) core_util_atomic_fetch_add_ptr((void *volatile *) valuePtr, arg * sizeof(T));
}

template<typename T>
inline T *core_util_atomic_fetch_add_explicit(T *volatile *valuePtr, ptrdiff_t arg, mbed_memory_order order) noexcept
{
    return (T *) core_util_atomic_fetch_add_explicit_ptr((void *volatile *) valuePtr, arg * sizeof(T), order);
}

template<typename T>
inline T *core_util_atomic_fetch_sub(T *volatile *valuePtr, ptrdiff_t arg) noexcept
{
    return (T *) core_util_atomic_fetch_sub_ptr((void *volatile *) valuePtr, arg * sizeof(T));
}

template<typename T>
inline T *core_util_atomic_fetch_sub_explicit(T *volatile *valuePtr, ptrdiff_t arg, mbed_memory_order order) noexcept
{
    return (T *) core_util_atomic_fetch_sub_explicit_ptr((void *volatile *) valuePtr, arg * sizeof(T), order);
}


#define DO_MBED_ATOMIC_OP_U_TEMPLATES(name) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, uint8_t,  u8) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, uint16_t, u16) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, uint32_t, u32) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, uint64_t, u64)

#define DO_MBED_ATOMIC_OP_S_TEMPLATES(name) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, int8_t,   s8) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, int16_t,  s16) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, int32_t,  s32) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, int64_t,  s64)

#define DO_MBED_ATOMIC_MANUAL_PRE_OP_TEMPLATE(name, T, fn_suffix, postname, OP) \
template<>                                                                      \
inline T core_util_atomic_##name(volatile T *valuePtr, T arg) noexcept          \
{                                                                               \
    return core_util_atomic_##postname##_##fn_suffix(valuePtr, arg) OP;         \
}                                                                               \
                                                                                \
template<>                                                                      \
inline T core_util_atomic_##name##_explicit(volatile T *valuePtr, T arg,        \
        mbed_memory_order order) noexcept                                       \
{                                                                               \
    return core_util_atomic_##postname##_explicit_##fn_suffix(valuePtr, arg, order) OP; \
}

DO_MBED_ATOMIC_OP_U_TEMPLATES(exchange)
DO_MBED_ATOMIC_OP_S_TEMPLATES(exchange)
DO_MBED_ATOMIC_OP_U_TEMPLATES(fetch_add)
DO_MBED_ATOMIC_OP_S_TEMPLATES(fetch_add)
DO_MBED_ATOMIC_OP_U_TEMPLATES(fetch_sub)
DO_MBED_ATOMIC_OP_S_TEMPLATES(fetch_sub)
DO_MBED_ATOMIC_OP_U_TEMPLATES(fetch_and)
DO_MBED_ATOMIC_OP_U_TEMPLATES(fetch_or)
DO_MBED_ATOMIC_OP_U_TEMPLATES(fetch_xor)

namespace mstd {
namespace impl {

// Use custom assembler forms for pre-ops where available, else construct from post-ops
#if MBED_EXCLUSIVE_ACCESS
#define DO_MBED_ATOMIC_PRE_OP_TEMPLATES(name, postname, OP) \
        template<typename T> T core_util_atomic_##name(volatile T *valuePtr, T arg) noexcept; \
        template<typename T> T core_util_atomic_##name##_explicit(volatile T *valuePtr, T arg, mbed_memory_order order) noexcept; \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, uint8_t, u8) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, uint16_t, u16) \
        DO_MBED_ATOMIC_OP_TEMPLATE(name, uint32_t, u32) \
        DO_MBED_ATOMIC_MANUAL_PRE_OP_TEMPLATE(name, uint64_t, u64, postname, OP)
#else
#define DO_MBED_ATOMIC_PRE_OP_TEMPLATES(name, postname, OP) \
        template<typename T> T core_util_atomic_##name(volatile T *valuePtr, T arg) noexcept; \
        template<typename T> T core_util_atomic_##name##_explicit(volatile T *valuePtr, T arg, mbed_memory_order order) noexcept; \
        DO_MBED_ATOMIC_MANUAL_PRE_OP_TEMPLATE(name, uint8_t, u8, postname, OP) \
        DO_MBED_ATOMIC_MANUAL_PRE_OP_TEMPLATE(name, uint16_t, u16, postname, OP) \
        DO_MBED_ATOMIC_MANUAL_PRE_OP_TEMPLATE(name, uint32_t, u32, postname, OP) \
        DO_MBED_ATOMIC_MANUAL_PRE_OP_TEMPLATE(name, uint64_t, u64, postname, OP)
#endif

// *INDENT-OFF*
DO_MBED_ATOMIC_PRE_OP_TEMPLATES(incr,      fetch_add, + arg)
DO_MBED_ATOMIC_PRE_OP_TEMPLATES(decr,      fetch_sub, - arg)
DO_MBED_ATOMIC_PRE_OP_TEMPLATES(and_fetch, fetch_and, & arg)
DO_MBED_ATOMIC_PRE_OP_TEMPLATES(or_fetch,  fetch_or,  | arg)
DO_MBED_ATOMIC_PRE_OP_TEMPLATES(xor_fetch, fetch_xor, ^ arg)
// *INDENT-ON*

}
}

#endif // __cplusplus

#undef MBED_DOP_REG
#undef MBED_CMP_IMM
#undef MBED_SUB3_IMM
#undef DO_MBED_LOCKFREE_EXCHG_ASM
#undef DO_MBED_LOCKFREE_NEWVAL_2OP_ASM
#undef DO_MBED_LOCKFREE_OLDVAL_3OP_ASM
#undef DO_MBED_LOCKFREE_OLDVAL_2OP_ASM
#undef DO_MBED_LOCKFREE_CAS_WEAK_ASM
#undef DO_MBED_LOCKFREE_CAS_STRONG_ASM
#undef DO_MBED_LOCKFREE_LOADSTORE
#undef DO_MBED_LOCKFREE_EXCHG_OP
#undef DO_MBED_LOCKFREE_CAS_WEAK_OP
#undef DO_MBED_LOCKFREE_CAS_STRONG_OP
#undef DO_MBED_LOCKFREE_NEWVAL_2OP
#undef DO_MBED_LOCKFREE_OLDVAL_2OP
#undef DO_MBED_LOCKFREE_OLDVAL_3OP
#undef DO_MBED_LOCKFREE_EXCHG_OPS
#undef DO_MBED_LOCKFREE_NEWVAL_2OPS
#undef DO_MBED_LOCKFREE_OLDVAL_2OPS
#undef DO_MBED_LOCKFREE_OLDVAL_3OPS
#undef DO_MBED_LOCKFREE_CAS_WEAK_OPS
#undef DO_MBED_LOCKFREE_CAS_STRONG_OPS
#undef DO_MBED_SIGNED_CAS_OP
#undef DO_MBED_SIGNED_CAS_OPS
#undef DO_MBED_SIGNED_FETCH_OP
#undef DO_MBED_SIGNED_EXPLICIT_FETCH_OP
#undef DO_MBED_SIGNED_FETCH_OPS
#undef DO_MBED_SIGNED_EXPLICIT_FETCH_OPS
#undef DO_MBED_LOCKED_FETCH_OP_ORDERINGS
#undef DO_MBED_LOCKED_CAS_ORDERINGS
#undef MBED_ACQUIRE_BARRIER
#undef MBED_RELEASE_BARRIER
#undef MBED_SEQ_CST_BARRIER
#undef DO_MBED_ATOMIC_LOAD_TEMPLATE
#undef DO_MBED_ATOMIC_STORE_TEMPLATE
#undef DO_MBED_ATOMIC_EXCHANGE_TEMPLATE
#undef DO_MBED_ATOMIC_CAS_TEMPLATE
#undef DO_MBED_ATOMIC_CAS_TEMPLATES
#undef DO_MBED_ATOMIC_FETCH_TEMPLATE
#undef DO_MBED_ATOMIC_FETCH_U_TEMPLATES
#undef DO_MBED_ATOMIC_FETCH_S_TEMPLATES

#endif
