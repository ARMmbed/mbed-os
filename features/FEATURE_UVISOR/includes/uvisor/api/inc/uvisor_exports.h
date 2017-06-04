/*
 * Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_UVISOR_EXPORTS_H__
#define __UVISOR_API_UVISOR_EXPORTS_H__

#include <stdint.h>
#include <stddef.h>

/* maximum number of boxes allowed: 1 is the minimum (unprivileged box) */
#define UVISOR_MAX_BOXES 5U

#define UVISOR_WAIT_FOREVER (0xFFFFFFFFUL)

/* extern keyword */
#ifdef  __cplusplus
#define UVISOR_EXTERN extern "C"
#else
#define UVISOR_EXTERN extern
#endif/*__CPP__*/

/** Extern C block macros
 *
 * Use these macros to disable name mangling in C++. Use these macros instead
 * of UVISOR_EXTERN when you also need to initialize the object. C++ compilers
 * warn when initializing an object declared as `extern`. Use of these macros
 * enables the defining of global non-name-mangled symbols in C++ without
 * affecting C code (which doesn't ever name mangle).  */
#ifdef  __cplusplus
#define UVISOR_EXTERN_C_BEGIN extern "C" {
#define UVISOR_EXTERN_C_END }
#else
#define UVISOR_EXTERN_C_BEGIN
#define UVISOR_EXTERN_C_END
#endif

/* asm keyword */
#ifndef asm
#define asm __asm__
#endif

/* Shared compiler attributes */
#if defined(__ICCARM__)
#define UVISOR_ALIGN(x)    __align(x)
#define UVISOR_FORCEINLINE inline
#define UVISOR_PACKED      __packed
#define UVISOR_WEAK        __weak
#define UVISOR_NORETURN    __noreturn
#define UVISOR_RAMFUNC     __ramfunc
#define UVISOR_DEPRECATED
#else
#define UVISOR_ALIGN(x)    __attribute__((aligned(x)))
#define UVISOR_FORCEINLINE inline __attribute__((always_inline))
#define UVISOR_PACKED      __attribute__((packed))
#define UVISOR_WEAK        __attribute__((weak))
#define UVISOR_NORETURN    __attribute__((noreturn))
#define UVISOR_RAMFUNC     __attribute__ ((section (".ramfunc"), noinline))
#define UVISOR_DEPRECATED  __attribute__((deprecated))
#endif

/* array count macro */
#define UVISOR_ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

/** Static Assertion Macro
 *
 * This macro works from both inside and outside function scope.
 *
 * FIXME This is currently not implemented. This issue is tracked at
 * https://github.com/ARMmbed/uvisor/issues/288
 */
#define UVISOR_STATIC_ASSERT(cond, msg)

/* convert macro argument to string */
/* note: this needs one level of indirection, accomplished with the helper macro
 *       __UVISOR_TO_STRING */
#define __UVISOR_TO_STRING(x) #x
#define UVISOR_TO_STRING(x)   __UVISOR_TO_STRING(x)

/* select an overloaded macro, so that 0 to 4 arguments can be used */
#define __UVISOR_MACRO_SELECT(_0, _1, _2, _3, _4, NAME, ...) NAME

/* count macro arguments */
#define UVISOR_MACRO_NARGS(...) \
     __UVISOR_MACRO_SELECT(_0, ##__VA_ARGS__, 4, 3, 2, 1, 0)

/* declare explicit callee-saved registers to hold input arguments (0 to 4) */
/* note: sizeof(type) must be less than or equal to 4 */
#define UVISOR_MACRO_REGS_ARGS(type, ...) \
     __UVISOR_MACRO_SELECT(_0, ##__VA_ARGS__, __UVISOR_MACRO_REGS_ARGS4, \
                                              __UVISOR_MACRO_REGS_ARGS3, \
                                              __UVISOR_MACRO_REGS_ARGS2, \
                                              __UVISOR_MACRO_REGS_ARGS1, \
                                              __UVISOR_MACRO_REGS_ARGS0)(type, ##__VA_ARGS__)
#define __UVISOR_MACRO_REGS_ARGS0(type)
#define __UVISOR_MACRO_REGS_ARGS1(type, a0) \
        register type r0 asm("r0") = (type) a0;
#define __UVISOR_MACRO_REGS_ARGS2(type, a0, a1) \
        register type r0 asm("r0") = (type) a0; \
        register type r1 asm("r1") = (type) a1;
#define __UVISOR_MACRO_REGS_ARGS3(type, a0, a1, a2) \
        register type r0 asm("r0") = (type) a0; \
        register type r1 asm("r1") = (type) a1; \
        register type r2 asm("r2") = (type) a2;
#define __UVISOR_MACRO_REGS_ARGS4(type, a0, a1, a2, a3) \
        register type r0 asm("r0") = (type) a0; \
        register type r1 asm("r1") = (type) a1; \
        register type r2 asm("r2") = (type) a2; \
        register type r3 asm("r3") = (type) a3;

/* declare explicit callee-saved registers to hold output values */
/* note: currently only one output value is allowed, up to 32bits */
#define UVISOR_MACRO_REGS_RETVAL(type, name) \
    register type name asm("r0");

/* declare callee-saved input/output operands for gcc-style inline asm */
/* note: this macro requires that a C variable having the same name of the
 *       corresponding callee-saved register is declared; these operands follow
 *       the official ABI for ARMv7M (e.g. 2 input arguments of 32bits each max,
 *       imply that registers r0 and r1 are used) */
/* note: gcc only */
/* note: for 0 inputs a dummy immediate is passed to avoid errors on a misplaced
 *       comma in the inline assembly */
#ifdef __GNUC__

#define UVISOR_MACRO_GCC_ASM_INPUT(...) \
     __UVISOR_MACRO_SELECT(_0, ##__VA_ARGS__, __UVISOR_MACRO_GCC_ASM_INPUT4, \
                                              __UVISOR_MACRO_GCC_ASM_INPUT3, \
                                              __UVISOR_MACRO_GCC_ASM_INPUT2, \
                                              __UVISOR_MACRO_GCC_ASM_INPUT1, \
                                              __UVISOR_MACRO_GCC_ASM_INPUT0)(__VA_ARGS__)
#define __UVISOR_MACRO_GCC_ASM_INPUT0()               [__dummy] "I" (0)
#define __UVISOR_MACRO_GCC_ASM_INPUT1(a0)             [r0] "r" (r0)
#define __UVISOR_MACRO_GCC_ASM_INPUT2(a0, a1)         [r0] "r" (r0), [r1] "r" (r1)
#define __UVISOR_MACRO_GCC_ASM_INPUT3(a0, a1, a2)     [r0] "r" (r0), [r1] "r" (r1), [r2] "r" (r2)
#define __UVISOR_MACRO_GCC_ASM_INPUT4(a0, a1, a2, a3) [r0] "r" (r0), [r1] "r" (r1), [r2] "r" (r2), [r3] "r" (r3)

#define UVISOR_MACRO_GCC_ASM_OUTPUT(name) [res] "=r" (name)

#endif /* __GNUC__ */

/* this macro multiplexes read/write opcodes depending on the number of
 * arguments */
#define UVISOR_ASM_MEMORY_ACCESS(opcode, type, ...) \
    __UVISOR_MACRO_SELECT(_0, ##__VA_ARGS__, /* no macro for 4 args */   , \
                                             /* no macro for 3 args */   , \
                                             __UVISOR_ASM_MEMORY_ACCESS_W, \
                                             __UVISOR_ASM_MEMORY_ACCESS_R, \
                                             /* no macro for 0 args */   )(opcode, type, ##__VA_ARGS__)
/* the macros that actually generate the assembly code for the memory access are
 * toolchain-specific */
#if defined(__CC_ARM)

/* TODO/FIXME */

#elif defined(__GNUC__)

#define __UVISOR_ASM_MEMORY_ACCESS_R(opcode, type, ...) \
    ({ \
        UVISOR_MACRO_REGS_ARGS(uint32_t, ##__VA_ARGS__); \
        UVISOR_MACRO_REGS_RETVAL(type, res); \
        asm volatile( \
            UVISOR_TO_STRING(opcode)" %[res], [%[r0]]\n" \
            UVISOR_NOP_GROUP \
            : UVISOR_MACRO_GCC_ASM_OUTPUT(res) \
            : UVISOR_MACRO_GCC_ASM_INPUT(__VA_ARGS__) \
        ); \
        res; \
    })

#define __UVISOR_ASM_MEMORY_ACCESS_W(opcode, type, ...) \
    UVISOR_MACRO_REGS_ARGS(uint32_t, ##__VA_ARGS__); \
    asm volatile( \
        UVISOR_TO_STRING(opcode)" %[r1], [%[r0]]\n" \
        UVISOR_NOP_GROUP \
        : \
        : UVISOR_MACRO_GCC_ASM_INPUT(__VA_ARGS__) \
    );

#endif /* defined(__CC_ARM) || defined(__GNUC__) */

typedef struct {
    void (*function)(const void *);
    size_t priority;
    size_t stack_size;
} uvisor_box_main_t;

#endif /* __UVISOR_API_UVISOR_EXPORTS_H__ */
