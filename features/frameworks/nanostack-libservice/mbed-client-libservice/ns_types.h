/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * ns_types.h - Basic compiler and type setup for Nanostack libraries.
 */
#ifndef NS_TYPES_H_
#define NS_TYPES_H_

/** \file
 * \brief Basic compiler and type setup
 *
 * We currently assume C99 or later.
 *
 * C99 features being relied on:
 *
 *   - <inttypes.h> and <stdbool.h>
 *   - inline (with C99 semantics, not C++ as per default GCC);
 *   - designated initialisers;
 *   - compound literals;
 *   - restrict;
 *   - [static N] in array parameters;
 *   - declarations in for statements;
 *   - mixing declarations and statements
 *
 * Compilers should be set to C99 or later mode when building Nanomesh source.
 * For GCC this means "-std=gnu99" (C99 with usual GNU extensions).
 *
 * Also, a little extra care is required for public header files that could be
 * included from C++, especially as C++ lacks some C99 features.
 *
 * (TODO: as this is exposed to API users, do we need a predefine to distinguish
 * internal and external use, for finer control? Not yet, but maybe...)
 */

/* Make sure <stdint.h> defines its macros if C++ */
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#include <stddef.h>
#include <inttypes.h> // includes <stdint.h>; debugf() users need PRIu32 etc
#include <stdbool.h>

/*
 * Create the optional <stdint.h> 24-bit types if they don't exist (worth trying
 * to use them, as they could exist and be more efficient than 32-bit on 8-bit
 * systems...)
 */
#ifndef UINT_LEAST24_MAX
typedef uint_least32_t uint_least24_t;
#define UINT_LEAST24_MAX UINT_LEAST32_MAX
#define UINT24_C(x) UINT32_C(x)
#define PRIoLEAST24 PRIoLEAST32
#define PRIuLEAST24 PRIuLEAST32
#define PRIxLEAST24 PRIxLEAST32
#define PRIXLEAST24 PRIXLEAST32
#endif

#ifndef INT_LEAST24_MAX
typedef int_least32_t int_least24_t;
#define INT_LEAST24_MIN INT_LEAST32_MIN
#define INT_LEAST24_MAX INT_LEAST32_MAX
#define INT24_C(x) INT32_C(x)
#define PRIdLEAST24 PRIdLEAST32
#define PRIiLEAST24 PRIiLEAST32
#endif

#ifndef UINT_FAST24_MAX
typedef uint_fast32_t uint_fast24_t;
#define UINT_FAST24_MAX UINT_FAST32_MAX
#define PRIoFAST24 PRIoFAST32
#define PRIuFAST24 PRIuFAST32
#define PRIxFAST24 PRIxFAST32
#define PRIXFAST24 PRIXFAST32
#endif

#ifndef INT_FAST24_MAX
typedef int_fast32_t int_fast24_t;
#define INT_FAST24_MIN INT_FAST32_MIN
#define INT_FAST24_MAX INT_FAST32_MAX
#define PRIdFAST24 PRIdFAST32
#define PRIiFAST24 PRIiFAST32
#endif

/* Function attribute - C11 "noreturn" or C++11 "[[noreturn]]" */
#ifndef NS_NORETURN
#if defined  __cplusplus && __cplusplus >= 201103L
#define NS_NORETURN [[noreturn]]
#elif !defined  __cplusplus && __STDC_VERSION__ >= 201112L
#define NS_NORETURN _Noreturn
#elif defined __GNUC__
#define NS_NORETURN __attribute__((__noreturn__))
#elif defined __IAR_SYSTEMS_ICC__
#define NS_NORETURN __noreturn
#else
#define NS_NORETURN
#endif
#endif

/* C11's "alignas" macro, emulated for integer expressions if necessary */
#ifndef __alignas_is_defined
#if defined __TASKING__
#define alignas(n) __align(n)
#define __alignas_is_defined 1
#elif (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L) || (defined __cplusplus && __cplusplus >= 201103L)
# if defined __ARMCC_VERSION && __ARMCC_VERSION < 6120000
/* Workaround for Arm Compiler versions prior to 6.12 */
#   if !defined __cplusplus
#     define alignas _Alignas
#   endif
#   define __alignas_is_defined 1
# else
#   include <stdalign.h>
# endif
#elif defined __GNUC__
#define alignas(n) __attribute__((__aligned__(n)))
#define __alignas_is_defined 1
#elif defined __IAR_SYSTEMS_ICC__
/* Does this really just apply to the next variable? */
#define alignas(n) __Alignas(data_alignment=n)
#define __Alignas(x) _Pragma(#x)
#define __alignas_is_defined 1
#endif
#endif

/**
 * Marker for functions or objects that may be unused, suppressing warnings.
 * Place after the identifier:
 * ~~~
 *    static int X MAYBE_UNUSED = 3;
 *    static int foo(void) MAYBE_UNUSED;
 * ~~~
 */
#if defined __GNUC__
#define MAYBE_UNUSED __attribute__((unused))
#else
#define MAYBE_UNUSED
#endif

/*
 * C++ (even C++11) doesn't provide restrict: define away or provide
 * alternative.
 */
#ifdef __cplusplus
#ifdef __GNUC__
#define restrict __restrict
#else
#define restrict
#endif
#endif /* __cplusplus */


/**
 * C++ doesn't allow "static" in function parameter types: ie
 * ~~~
 *    entry_t *find_entry(const uint8_t address[static 16])
 * ~~~
 * If a header file may be included from C++, use this __static define instead.
 *
 * (Syntax introduced in C99 - `uint8_t address[16]` in a prototype was always
 * equivalent to `uint8_t *address`, but the C99 addition of static tells the
 * compiler that address is never NULL, and always points to at least 16
 * elements. This adds no new type-checking, but the information could aid
 * compiler optimisation, and it can serve as documentation).
 */
#ifdef __cplusplus
#define __static
#else
#define __static static
#endif

#ifdef __GNUC__
#define NS_GCC_VERSION (__GNUC__ * 10000 \
                   + __GNUC_MINOR__ * 100 \
                   + __GNUC_PATCHLEVEL__)
#endif

/** \brief Compile-time assertion
 *
 * C11 provides _Static_assert, as does GCC even in C99 mode (and
 * as a freestanding implementation, we can't rely on <assert.h> to get
 * the static_assert macro).
 * C++11 provides static_assert as a keyword, as does G++ in C++0x mode.
 *
 * The assertion acts as a declaration that can be placed at file scope, in a
 * code block (except after a label), or as a member of a struct/union. It
 * produces a compiler error if "test" evaluates to 0.
 *
 * Note that this *includes* the required semicolon when defined, else it
 * is totally empty, permitting use in structs. (If the user provided the `;`,
 * it would leave an illegal stray `;` if unavailable).
 */
#ifdef __cplusplus
# if __cplusplus >= 201103L || __cpp_static_assert >= 200410
# define NS_STATIC_ASSERT(test, str) static_assert(test, str);
# elif defined __GXX_EXPERIMENTAL_CXX0X__  && NS_GCC_VERSION >= 40300
# define NS_STATIC_ASSERT(test, str) __extension__ static_assert(test, str);
# else
# define NS_STATIC_ASSERT(test, str)
# endif
#else /* C */
# if __STDC_VERSION__ >= 201112L
# define NS_STATIC_ASSERT(test, str) _Static_assert(test, str);
# elif defined __GNUC__ && NS_GCC_VERSION >= 40600
# ifdef _Static_assert
/*
 * Some versions of glibc cdefs.h (which comes in via <stdint.h> above)
 * attempt to define their own _Static_assert (if GCC < 4.6 or
 * __STRICT_ANSI__) using an extern declaration, which doesn't work in a
 * struct/union.
 *
 * For GCC >= 4.6 and __STRICT_ANSI__, we can do better - just use
 * the built-in _Static_assert with __extension__. We have to do this, as
 * ns_list.h needs to use it in a union. No way to get at it though, without
 * overriding their define.
 */
#   undef _Static_assert
#   define _Static_assert(x, y) __extension__ _Static_assert(x, y)
# endif
# define NS_STATIC_ASSERT(test, str) __extension__ _Static_assert(test, str);
# else
# define NS_STATIC_ASSERT(test, str)
#endif
#endif

/** \brief Pragma to suppress warnings about unusual pointer values.
 *
 * Useful if using "poison" values.
 */
#ifdef __IAR_SYSTEMS_ICC__
#define NS_FUNNY_INTPTR_OK      _Pragma("diag_suppress=Pe1053")
#define NS_FUNNY_INTPTR_RESTORE _Pragma("diag_default=Pe1053")
#else
#define NS_FUNNY_INTPTR_OK
#define NS_FUNNY_INTPTR_RESTORE
#endif

/** \brief Pragma to suppress warnings about always true/false comparisons
 */
#if defined __GNUC__ && NS_GCC_VERSION >= 40600
#define NS_FUNNY_COMPARE_OK         _Pragma("GCC diagnostic push") \
                                    _Pragma("GCC diagnostic ignored \"-Wtype-limits\"")
#define NS_FUNNY_COMPARE_RESTORE    _Pragma("GCC diagnostic pop")
#else
#define NS_FUNNY_COMPARE_OK
#define NS_FUNNY_COMPARE_RESTORE
#endif

/** \brief Pragma to suppress warnings arising from dummy definitions.
 *
 * Useful when you have function-like macros that returning constants
 * in cut-down builds. Can be fairly cavalier about disabling as we
 * do not expect every build to use this macro. Generic builds of
 * components should ensure this is not included by only using it in
 * a ifdef blocks providing dummy definitions.
 */
#if defined __IAR_SYSTEMS_ICC__
// controlling expression is constant
#define NS_DUMMY_DEFINITIONS_OK _Pragma("diag_suppress=Pe236")
#else
#define NS_DUMMY_DEFINITIONS_OK
#endif

/** \brief Convert pointer to member to pointer to containing structure */
#define NS_CONTAINER_OF(ptr, type, member) \
    ((type *) ((char *) (ptr) - offsetof(type, member)))

/*
 * Inlining could cause problems when mixing with C++; provide a mechanism to
 * disable it. This could also be turned off for other reasons (although
 * this can usually be done through a compiler flag, eg -O0 on gcc).
 */
#ifndef __cplusplus
#define NS_ALLOW_INLINING
#endif

/* There is inlining problem in GCC version 4.1.x and we know it works in 4.6.3 */
#if defined __GNUC__ && NS_GCC_VERSION < 40600
#undef NS_ALLOW_INLINING
#endif

/** \brief Mark a potentially-inlineable function.
 *
 * We follow C99 semantics, which requires precisely one external definition.
 * To also allow inlining to be totally bypassed under control of
 * NS_ALLOW_INLINING, code can be structured as per the example of ns_list:
 *
 * foo.h
 * -----
 * ~~~
 *    NS_INLINE int my_func(int);
 *
 *    #if defined NS_ALLOW_INLINING || defined FOO_FN
 *    #ifndef FOO_FN
 *    #define FOO_FN NS_INLINE
 *    #endif
 *    FOO_FN int my_func(int a)
 *    {
 *        definition;
 *    }
 *    #endif
 * ~~~
 * foo.c
 * -----
 * ~~~
 *    #define FOO_FN extern
 *    #include "foo.h"
 * ~~~
 * Which generates:
 * ~~~
 *                 NS_ALLOW_INLINING set          NS_ALLOW_INLINING unset
 *                 =====================          =======================
 *                 Include foo.h                  Include foo.h
 *                 -------------                  -------------
 *                 inline int my_func(int);       int my_func(int);
 *
 *                 // inline definition
 *                 inline int my_func(int a)
 *                 {
 *                     definition;
 *                 }
 *
 *                 Compile foo.c                  Compile foo.c
 *                 -------------                  -------------
 *    (from .h)    inline int my_func(int);       int my_func(int);
 *
 *                 // external definition
 *                 // because of no "inline"      // normal external definition
 *                 extern int my_func(int a)      extern int my_func(int a)
 *                 {                              {
 *                     definition;                    definition;
 *                 }                              }
 * ~~~
 *
 * Note that even with inline keywords, whether the compiler inlines or not is
 * up to it. For example, gcc at "-O0" will not inline at all, and will always
 * call the real functions in foo.o, just as if NS_ALLOW_INLINING was unset.
 * At "-O2", gcc could potentially inline everything, meaning that foo.o is not
 * referenced at all.
 *
 * Alternatively, you could use "static inline", which gives every caller its
 * own internal definition. This is compatible with C++ inlining (which expects
 * the linker to eliminate duplicates), but in C it's less efficient if the code
 * ends up non-inlined, and it's harder to breakpoint. I don't recommend it
 * except for the most trivial functions (which could then probably be macros).
 */
#ifdef NS_ALLOW_INLINING
#define NS_INLINE inline
#else
#define NS_INLINE
#endif

#if defined __SDCC_mcs51 || defined __ICC8051__ || defined __C51__

/* The 8051 environments: SDCC (historic), IAR (current), Keil (future?) */

#define NS_LARGE            __xdata
#define NS_LARGE_PTR        __xdata
#ifdef __ICC8051__
#define NS_REENTRANT
#define NS_REENTRANT_PREFIX __idata_reentrant
#else
#define NS_REENTRANT        __reentrant
#define NS_REENTRANT_PREFIX
#endif
#define NS_NEAR_FUNC        __near_func

#else

/* "Normal" systems. Define it all away. */
#define NS_LARGE
#define NS_LARGE_PTR
#define NS_REENTRANT
#define NS_REENTRANT_PREFIX
#define NS_NEAR_FUNC

#endif

/** \brief Scatter-gather descriptor
 *
 * Slightly optimised for small platforms - we assume we won't need any
 * element bigger than 64K.
 */
typedef struct ns_iovec {
    void *iov_base;
    uint_fast16_t iov_len;
} ns_iovec_t;

#endif /* NS_TYPES_H */
