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


// Warning for unsupported compilers
#if !defined(__GNUC__)   /* GCC        */ \
 && !defined(__CC_ARM)   /* ARMCC      */ \
 && !defined(__clang__)  /* LLVM/Clang */ \
 && !defined(__ICCARM__) /* IAR        */
#warning "This compiler is not yet supported."
#endif


// Attributes

/** PACKED
 *  Pack a structure, preventing any padding from being added between fields.
 *
 *  @code
 *  #include "toolchain.h"
 *
 *  typedef struct {
 *      char x;
 *      int y;
 *  } PACKED foo;
 *  @endcode
 */
#ifndef PACKED
#if defined(__ICCARM__)
#define PACKED __packed
#else
#define PACKED __attribute__((packed))
#endif
#endif

/** ALIGN(N)
 *  Declare a variable to be aligned on an N-byte boundary.
 *  
 *  @code
 *  #include "toolchain.h"
 *
 *  ALIGN(16) char a;
 *  @endcode
 */
#ifndef ALIGN
#if defined(__ICCARM__)
#define _ALIGN(N) _Pragma(#N)
#define ALIGN(N) _ALIGN(data_alignment=N)
#else
#define ALIGN(N) __attribute__((aligned(N)))
#endif
#endif

/** UNUSED
 *  Declare a function argument to be unused, suppressing compiler warnings
 *
 *  @code
 *  #include "toolchain.h"
 *
 *  void foo(UNUSED int arg) {
 *
 *  }
 *  @endcode
 */
#ifndef UNUSED
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define UNUSED __attribute__((__unused__))
#else
#define UNUSED
#endif
#endif

/** WEAK
 *  Mark a function as being weak.
 *  
 *  @note
 *  weak functions are not friendly to making code re-usable, as they can only
 *  be overridden once (and if they are multiply overridden the linker will emit
 *  no warning). You should not normally use weak symbols as part of the API to
 *  re-usable modules.
 *  
 *  @code
 *  #include "toolchain.h"
 *  
 *  WEAK void foo() {
 *      // a weak implementation of foo that can be overriden by a definition
 *      // without  __weak
 *  }
 *  @endcode
 */
#ifndef WEAK
#if defined(__ICCARM__)
#define WEAK __weak
#else
#define WEAK __attribute__((weak))
#endif
#endif

/** PURE
 *  Hint to the compiler that a function depends only on parameters
 *
 *  @code
 *  #include "toolchain.h"
 *
 *  PURE int foo(int arg){
 *      // no access to global variables
 *  }
 *  @endcode
 */
#ifndef PURE
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define PURE __attribute__((const))
#else
#define PURE
#endif
#endif

/** FORCEINLINE
 *  Declare a function that must always be inlined. Failure to inline
 *  such a function will result in an error.
 *
 *  @code
 *  #include "toolchain.h"
 *  
 *  FORCEINLINE void foo() {
 *  
 *  }
 *  @endcode
 */
#ifndef FORCEINLINE
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define FORCEINLINE static inline __attribute__((always_inline))
#elif defined(__ICCARM__)
#define FORCEINLINE _Pragma("inline=force") static
#else
#define FORCEINLINE static inline
#endif
#endif

/** NORETURN
 *  Declare a function that will never return.
 *
 *  @code
 *  #include "toolchain.h"
 *  
 *  NORETURN void foo() {
 *      // must never return
 *      while (1) {}
 *  }
 *  @endcode
 */
#ifndef NORETURN
#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM)
#define NORETURN __attribute__((noreturn))
#elif defined(__ICCARM__)
#define NORETURN __noreturn
#else
#define NORETURN
#endif
#endif

/** UNREACHABLE
 *  An unreachable statement. If the statement is reached,
 *  behaviour is undefined. Useful in situations where the compiler
 *  cannot deduce the unreachability of code.
 *
 *  @code
 *  #include "toolchain.h"
 *
 *  void foo(int arg) {
 *      switch (arg) {
 *          case 1: return 1;
 *          case 2: return 2;
 *          ...
 *      }
 *      UNREACHABLE;
 *  }
 *  @endcode
 */
#ifndef UNREACHABLE
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__CC_ARM)
#define UNREACHABLE __builtin_unreachable()
#else
#define UNREACHABLE while (1)
#endif
#endif

/** DEPRECATED("message string")
 *  Mark a function declaration as deprecated, if it used then a warning will be
 *  issued by the compiler possibly including the provided message. Note that not
 *  all compilers are able to display the message.
 *  
 *  @code
 *  #include "toolchain.h"
 *  
 *  DEPRECATED("don't foo any more, bar instead")
 *  void foo(int arg);
 *  @endcode
 */
#ifndef DEPRECATED
#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED(M) __attribute__((deprecated(M)))
#elif defined(__CC_ARM)
#define DEPRECATED(M) __attribute__((deprecated))
#else
#define DEPRECATED(M)
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
#ifndef EXTERN
#define EXTERN extern
#endif

// Backwards compatibility
#ifndef EXTERN
#define EXTERN extern
#endif

#endif
