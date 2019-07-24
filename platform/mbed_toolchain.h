
/** \ingroup mbed-os-public */
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_toolchain Toolchain functions
 * @{
 */

/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_TOOLCHAIN_H
#define MBED_TOOLCHAIN_H

#include "platform/mbed_preprocessor.h"


// Warning for unsupported compilers
#if !defined(__GNUC__)   /* GCC        */ \
 && !defined(__CC_ARM)   /* ARMCC      */ \
 && !defined(__clang__)  /* LLVM/Clang */ \
 && !defined(__ICCARM__) /* IAR        */
#warning "This compiler is not yet supported."
#endif


// Attributes

/** MBED_PACKED
 *  Pack a structure, preventing any padding from being added between fields.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_PACKED(struct) foo {
 *      char x;
 *      int y;
 *  };
 *  @endcode
 */
#ifndef MBED_PACKED
#if defined(__ICCARM__)
#define MBED_PACKED(struct) __packed struct
#else
#define MBED_PACKED(struct) struct __attribute__((packed))
#endif
#endif

/** MBED_ALIGN(N)
 *  Declare a variable to be aligned on an N-byte boundary.
 *
 *  @note
 *  IAR does not support alignment greater than word size on the stack
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_ALIGN(16) char a;
 *  @endcode
 */
#ifndef MBED_ALIGN
#if __cplusplus >= 201103 && !defined __CC_ARM
#define MBED_ALIGN(N) alignas(N)
#elif __STDC_VERSION__ >= 201112 && !defined __CC_ARM
#define MBED_ALIGN(N) _Alignas(N)
#elif defined(__ICCARM__)
#define MBED_ALIGN(N) _Pragma(MBED_STRINGIFY(data_alignment=N))
#else
#define MBED_ALIGN(N) __attribute__((aligned(N)))
#endif
#endif

/** MBED_UNUSED
 *  Declare a function argument to be unused, suppressing compiler warnings
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  void foo(MBED_UNUSED int arg) {
 *
 *  }
 *  @endcode
 */
#ifndef MBED_UNUSED
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define MBED_UNUSED __attribute__((__unused__))
#else
#define MBED_UNUSED
#endif
#endif

/** MBED_USED
 *  Inform the compiler that a static variable is to be retained in the object file, even if it is unreferenced.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_USED int foo;
 *
 *  @endcode
 */
#ifndef MBED_USED
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define MBED_USED __attribute__((used))
#elif defined(__ICCARM__)
#define MBED_USED __root
#else
#define MBED_USED
#endif
#endif

/** MBED_WEAK
 *  Mark a function as being weak.
 *
 *  @note
 *  Functions should only be marked as weak in the source file. The header file
 *  should contain a regular function declaration to insure the function is emitted.
 *  A function marked weak will not be emitted if an alternative non-weak
 *  implementation is defined.
 *
 *  @note
 *  Weak functions are not friendly to making code re-usable, as they can only
 *  be overridden once (and if they are multiply overridden the linker will emit
 *  no warning). You should not normally use weak symbols as part of the API to
 *  re-usable modules.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_WEAK void foo() {
 *      // a weak implementation of foo that can be overriden by a definition
 *      // without  __weak
 *  }
 *  @endcode
 */
#ifndef MBED_WEAK
#if defined(__ICCARM__)
#define MBED_WEAK __weak
#elif defined(__MINGW32__)
#define MBED_WEAK
#else
#define MBED_WEAK __attribute__((weak))
#endif
#endif

/** MBED_COMPILER_BARRIER
 * Stop the compiler moving memory accesses.
 *
 * The barrier stops memory accesses from being moved from one side of the
 * barrier to the other for safety against other threads and interrupts.
 *
 * This macro should only be used if we know only one CPU is accessing the data,
 * or we are otherwise synchronising CPUs via acquire/release instructions.
 * Otherwise, use MBED_BARRIER, which will act as a compiler barrier and also
 * a CPU barrier if necessary.
 *
 * @internal
 * This is not for use by normal code - it is a building block for the
 * higher-level functions in mbed_critical.h. Higher-level lock/unlock or
 * acquire/release APIs always provide ordering semantics, using this if
 * necessary.
 *
 * @code
 *  #include "mbed_toolchain.h"
 *
 *  void atomic_flag_clear_armv8(atomic_flag *flagPtr)
 *  {
 *      // ARMv8 LDA and STL instructions provide sequential consistency against
 *      // other CPUs, so no CPU barrier is needed. But we still need compiler
 *      // barriers to give us sequentially-consistent release semantics with
 *      // respect to compiler reordering - __STLB does not currently
 *      // include this.
 *      MBED_COMPILER_BARRIER();
 *      __STLB(&flagPtr->_flag, false);
 *      MBED_COMPILER_BARRIER();
 *  }
 */
#ifdef __CC_ARM
#define MBED_COMPILER_BARRIER() __memory_changed()
#elif defined(__GNUC__) || defined(__clang__) || defined(__ICCARM__)
#define MBED_COMPILER_BARRIER() asm volatile("" : : : "memory")
#else
#error "Missing MBED_COMPILER_BARRIER implementation"
#endif

/** MBED_BARRIER
 * Stop the compiler, and CPU if SMP, from moving memory accesses.
 *
 * The barrier stops memory accesses from being moved from one side of the
 * barrier to the other for safety against other threads and interrupts,
 * potentially on other CPUs.
 *
 * In a single-CPU system, this is just a compiler barrier.
 * If we supported multiple CPUs, this would be a DMB (with implied compiler
 * barrier).
 *
 * @internal
 * This is not for use by normal code - it is a building block for the
 * higher-level functions in mbed_critical.h. Higher-level lock/unlock or
 * acquire/release APIs always provide ordering semantics, using this if
 * necessary.
 * @code
 *  #include "mbed_toolchain.h"
 *
 *  void atomic_flag_clear_armv7(atomic_flag *flagPtr)
 *  {
 *      // ARMv7 LDR and STR instructions do not provide any ordering
 *      // consistency against other CPUs, so explicit barrier DMBs are needed
 *      // for a multi-CPU system, otherwise just compiler barriers for single-CPU.
 *      MBED_BARRIER();
 *      flagPtr->_flag = false;
 *      MBED_BARRIER();
 *  }
 */
#define MBED_BARRIER() MBED_COMPILER_BARRIER()

/** MBED_PURE
 *  Hint to the compiler that a function depends only on parameters
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_PURE int foo(int arg){
 *      // no access to global variables
 *  }
 *  @endcode
 */
#ifndef MBED_PURE
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define MBED_PURE __attribute__((const))
#else
#define MBED_PURE
#endif
#endif

/** MBED_NOINLINE
 *  Declare a function that must not be inlined.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_NOINLINE void foo() {
 *
 *  }
 *  @endcode
 */
#ifndef MBED_NOINLINE
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define MBED_NOINLINE __attribute__((noinline))
#elif defined(__ICCARM__)
#define MBED_NOINLINE _Pragma("inline=never")
#else
#define MBED_NOINLINE
#endif
#endif

/** MBED_FORCEINLINE
 *  Declare a function that must always be inlined. Failure to inline
 *  such a function will result in an error.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_FORCEINLINE void foo() {
 *
 *  }
 *  @endcode
 */
#ifndef MBED_FORCEINLINE
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define MBED_FORCEINLINE inline __attribute__((always_inline))
#elif defined(__ICCARM__)
#define MBED_FORCEINLINE _Pragma("inline=forced")
#else
#define MBED_FORCEINLINE inline
#endif
#endif

/** MBED_NORETURN
 *  Declare a function that will never return.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_NORETURN void foo() {
 *      // must never return
 *      while (1) {}
 *  }
 *  @endcode
 */
#ifndef MBED_NORETURN
#if __cplusplus >= 201103
#define MBED_NORETURN [[noreturn]]
#elif __STDC_VERSION__ >= 201112
#define MBED_NORETURN _Noreturn
#elif defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define MBED_NORETURN __attribute__((noreturn))
#elif defined(__ICCARM__)
#define MBED_NORETURN __noreturn
#else
#define MBED_NORETURN
#endif
#endif

/** MBED_UNREACHABLE
 *  An unreachable statement. If the statement is reached,
 *  behavior is undefined. Useful in situations where the compiler
 *  cannot deduce if the code is unreachable.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  void foo(int arg) {
 *      switch (arg) {
 *          case 1: return 1;
 *          case 2: return 2;
 *          ...
 *      }
 *      MBED_UNREACHABLE;
 *  }
 *  @endcode
 */
#ifndef MBED_UNREACHABLE
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__CC_ARM)
#define MBED_UNREACHABLE __builtin_unreachable()
#else
#define MBED_UNREACHABLE while (1)
#endif
#endif

/** MBED_DEPRECATED("message string")
 *  Mark a function declaration as deprecated, if it used then a warning will be
 *  issued by the compiler possibly including the provided message. Note that not
 *  all compilers are able to display the message.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_DEPRECATED("don't foo any more, bar instead")
 *  void foo(int arg);
 *  @endcode
 */
#ifndef MBED_DEPRECATED
#if defined(__CC_ARM)
#define MBED_DEPRECATED(M) __attribute__((deprecated))
#elif defined(__GNUC__) || defined(__clang__)
#define MBED_DEPRECATED(M) __attribute__((deprecated(M)))
#else
#define MBED_DEPRECATED(M)
#endif
#endif

/** MBED_DEPRECATED_SINCE("version", "message string")
 *  Mark a function declaration as deprecated, noting that the declaration was
 *  deprecated on the specified version. If the function is used then a warning
 *  will be issued by the compiler possibly including the provided message.
 *  Note that not all compilers are able to display this message.
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_DEPRECATED_SINCE("mbed-os-5.1", "don't foo any more, bar instead")
 *  void foo(int arg);
 *  @endcode
 */
#define MBED_DEPRECATED_SINCE(D, M) MBED_DEPRECATED(M " [since " D "]")

/** MBED_CALLER_ADDR()
 * Returns the caller of the current function.
 *
 * @note
 * This macro is only implemented for GCC and ARMCC.
 *
 * @code
 * #include "mbed_toolchain.h"
 *
 * printf("This function was called from %p", MBED_CALLER_ADDR());
 * @endcode
 *
 * @return Address of the calling function
 */
#ifndef MBED_CALLER_ADDR
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__CC_ARM)
#define MBED_CALLER_ADDR() __builtin_extract_return_addr(__builtin_return_address(0))
#elif defined(__CC_ARM)
#define MBED_CALLER_ADDR() __builtin_return_address(0)
#else
#define MBED_CALLER_ADDR() (NULL)
#endif
#endif

#ifndef MBED_SECTION
#if (defined(__GNUC__) || defined(__clang__)) || defined(__CC_ARM)
#define MBED_SECTION(name) __attribute__ ((section (name)))
#elif defined(__ICCARM__)
#define MBED_SECTION(name) _Pragma(MBED_STRINGIFY(location=name))
#else
#error "Missing MBED_SECTION directive"
#endif
#endif

/**
 * Macro expanding to a string literal of the enclosing function name.
 *
 * The string returned takes into account language specificity and yield human
 * readable content.
 *
 * As an example, if the macro is used within a C++ function then the string
 * literal containing the function name will contain the complete signature of
 * the function - including template parameters - and namespace qualifications.
 */
#ifndef MBED_PRETTY_FUNCTION
#define MBED_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#ifndef MBED_PRINTF
#if defined(__GNUC__) || defined(__CC_ARM)
#define MBED_PRINTF(format_idx, first_param_idx) __attribute__ ((__format__(__printf__, format_idx, first_param_idx)))
#else
#define MBED_PRINTF(format_idx, first_param_idx)
#endif
#endif

#ifndef MBED_PRINTF_METHOD
#if defined(__GNUC__) || defined(__CC_ARM)
#define MBED_PRINTF_METHOD(format_idx, first_param_idx) __attribute__ ((__format__(__printf__, format_idx+1, first_param_idx == 0 ? 0 : first_param_idx+1)))
#else
#define MBED_PRINTF_METHOD(format_idx, first_param_idx)
#endif
#endif

#ifndef MBED_SCANF
#if defined(__GNUC__) || defined(__CC_ARM)
#define MBED_SCANF(format_idx, first_param_idx) __attribute__ ((__format__(__scanf__, format_idx, first_param_idx)))
#else
#define MBED_SCANF(format_idx, first_param_idx)
#endif
#endif

#ifndef MBED_SCANF_METHOD
#if defined(__GNUC__) || defined(__CC_ARM)
#define MBED_SCANF_METHOD(format_idx, first_param_idx) __attribute__ ((__format__(__scanf__, format_idx+1, first_param_idx == 0 ? 0 : first_param_idx+1)))
#else
#define MBED_SCANF_METHOD(format_idx, first_param_idx)
#endif
#endif

// Macro containing the filename part of the value of __FILE__. Defined as
// string literal.
#ifndef MBED_FILENAME
#if defined(__CC_ARM)
#define MBED_FILENAME __MODULE__
#elif defined(__GNUC__)
#define MBED_FILENAME (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __builtin_strrchr(__FILE__, '\\') ? __builtin_strrchr(__FILE__, '\\') + 1 : __FILE__)
#elif defined(__ICCARM__)
#define MBED_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define MBED_FILENAME __FILE__
#endif
#endif // #ifndef MBED_FILENAME

// FILEHANDLE declaration
#if defined(TOOLCHAIN_ARM)
#include <rt_sys.h>
#endif

#ifndef FILEHANDLE
typedef int FILEHANDLE;
#endif

// Backwards compatibility
#ifndef WEAK
#define WEAK MBED_WEAK
#endif

#ifndef PACKED
#define PACKED MBED_PACKED()
#endif

#ifndef EXTERN
#define EXTERN extern
#endif

/** MBED_NONSECURE_ENTRY
 *  Declare a function that can be called from non-secure world or secure world
 *
 *  @code
 *  #include "mbed_toolchain.h"
 *
 *  MBED_NONSECURE_ENTRY void foo() {
 *
 *  }
 *  @endcode
 */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3L)
#if defined (__ICCARM__)
#define MBED_NONSECURE_ENTRY       __cmse_nonsecure_entry
#else
#define MBED_NONSECURE_ENTRY       __attribute__((cmse_nonsecure_entry))
#endif
#else
#define MBED_NONSECURE_ENTRY
#endif

#endif

/** @}*/
/** @}*/
