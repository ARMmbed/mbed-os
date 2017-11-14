
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_toolchain Toolchain functions
 * @{
 */

/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "mbed_preprocessor.h"


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
#if defined(__ICCARM__)
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
#else
#define MBED_WEAK __attribute__((weak))
#endif
#endif

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
#define MBED_FORCEINLINE static inline __attribute__((always_inline))
#elif defined(__ICCARM__)
#define MBED_FORCEINLINE _Pragma("inline=forced") static
#else
#define MBED_FORCEINLINE static inline
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
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define MBED_NORETURN __attribute__((noreturn))
#elif defined(__ICCARM__)
#define MBED_NORETURN __noreturn
#else
#define MBED_NORETURN
#endif
#endif

/** MBED_UNREACHABLE
 *  An unreachable statement. If the statement is reached,
 *  behaviour is undefined. Useful in situations where the compiler
 *  cannot deduce the unreachability of code.
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
#define MBED_PRINTF_METHOD(format_idx, first_param_idx) __attribute__ ((__format__(__printf__, format_idx+1, first_param_idx+1)))
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
#define MBED_SCANF_METHOD(format_idx, first_param_idx) __attribute__ ((__format__(__scanf__, format_idx+1, first_param_idx+1)))
#else
#define MBED_SCANF_METHOD(format_idx, first_param_idx)
#endif
#endif

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

#endif

/** @}*/
/** @}*/
