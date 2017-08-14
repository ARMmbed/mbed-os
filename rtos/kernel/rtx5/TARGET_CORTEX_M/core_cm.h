/** \addtogroup rtos */
/** @{*/
/*
 * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       Cortex-M Core definitions
 *
 * -----------------------------------------------------------------------------
 */

#ifndef CORE_CM_H_
#define CORE_CM_H_

#include <stdint.h>
#include "cmsis.h"
#include "cmsis_compiler.h"
#include "arm_math.h"

#ifndef __ARM_ARCH_6M__
#define __ARM_ARCH_6M__         0U
#endif
#ifndef __ARM_ARCH_7M__
#define __ARM_ARCH_7M__         0U
#endif
#ifndef __ARM_ARCH_7EM__
#define __ARM_ARCH_7EM__        0U
#endif
#ifndef __ARM_ARCH_8M_BASE__
#define __ARM_ARCH_8M_BASE__    0U
#endif
#ifndef __ARM_ARCH_8M_MAIN__
#define __ARM_ARCH_8M_MAIN__    0U
#endif

#if   ((__ARM_ARCH_6M__      + \
        __ARM_ARCH_7M__      + \
        __ARM_ARCH_7EM__     + \
        __ARM_ARCH_8M_BASE__ + \
        __ARM_ARCH_8M_MAIN__) != 1U)
#error "Unknown ARM Architecture!"
#endif

#ifdef RTE_CMSIS_RTOS2_RTX5_ARMV8M_NS
#define __DOMAIN_NS             1U
#endif

#ifndef __DOMAIN_NS
#define __DOMAIN_NS             0U
#elif ((__DOMAIN_NS          == 1U) && \
      ((__ARM_ARCH_6M__      == 1U) || \
       (__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U)))
#error "Non-secure domain requires ARMv8-M Architecture!"
#endif

#ifndef __EXCLUSIVE_ACCESS
#if   ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U) || \
       (__ARM_ARCH_8M_BASE__ == 1U) || \
       (__ARM_ARCH_8M_MAIN__ == 1U))
#define __EXCLUSIVE_ACCESS      1U
#else
#define __EXCLUSIVE_ACCESS      0U
#endif
#endif


#define IS_PRIVILEGED()         ((__get_CONTROL() & 1U) == 0U)

#define IS_IRQ_MODE()            (__get_IPSR() != 0U)

#if   ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U) || \
       (__ARM_ARCH_8M_MAIN__ == 1U))
#define IS_IRQ_MASKED()         ((__get_PRIMASK() != 0U) || (__get_BASEPRI() != 0U))
#else
#define IS_IRQ_MASKED()          (__get_PRIMASK() != 0U) 
#endif

#define XPSR_INITIAL_VALUE      0x01000000U

#if    (__DOMAIN_NS == 1U)
#define STACK_FRAME_INIT        0xBCU
#else
#define STACK_FRAME_INIT        0xFDU
#endif

#define IS_EXTENDED_STACK_FRAME(n) (((n) & 0x10U) == 0U)


//  ==== Service Calls definitions ====

#if defined(__CC_ARM)

#if   ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U) || \
       (__ARM_ARCH_8M_MAIN__ == 1U))
#define __SVC_INDIRECT(n) __svc_indirect(n)
#elif ((__ARM_ARCH_6M__      == 1U) || \
       (__ARM_ARCH_8M_BASE__ == 1U))
#define __SVC_INDIRECT(n) __svc_indirect_r7(n)
#endif

#if    (__FPU_USED == 1U)
#define SVC_SETUP_PSP                                                          \
  uint32_t control = __get_CONTROL();                                          \
  if ((control & 2U) == 0U) {                                                  \
    __set_PSP((__get_MSP() - ((control & 4U) ? 104U : 32U)) & ~7U);            \
  }
#else
#define SVC_SETUP_PSP                                                          \
  uint32_t control = __get_CONTROL();                                          \
  if ((control & 2U) == 0U) {                                                  \
    __set_PSP((__get_MSP() -                          32U)  & ~7U);            \
  }
#endif

#define SVC0_0N(f,t)                                                           \
__SVC_INDIRECT(0) t    svc##f (t(*)());                                        \
                  t svcRtx##f (void);                                          \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (void) {                                         \
  svc##f(svcRtx##f);                                                           \
}

#define SVC0_0(f,t)                                                            \
__SVC_INDIRECT(0) t    svc##f (t(*)());                                        \
                  t svcRtx##f (void);                                          \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (void) {                                         \
  return svc##f(svcRtx##f);                                                    \
}

#define SVC0_0M(f,t)                                                           \
__SVC_INDIRECT(0) t    svc##f (t(*)());                                        \
                  t svcRtx##f (void);                                          \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (void) {                                         \
  SVC_SETUP_PSP                                                                \
  return svc##f(svcRtx##f);                                                    \
}

#define SVC0_0D SVC0_0

#define SVC0_1N(f,t,t1)                                                        \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1),t1);                                   \
                  t svcRtx##f (t1 a1);                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1) {                                        \
  svc##f(svcRtx##f,a1);                                                        \
}

#define SVC0_1(f,t,t1)                                                         \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1),t1);                                   \
                  t svcRtx##f (t1 a1);                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1) {                                        \
  return svc##f(svcRtx##f,a1);                                                 \
}

#define SVC0_1M(f,t,t1)                                                        \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1),t1);                                   \
                  t svcRtx##f (t1 a1);                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1) {                                        \
  SVC_SETUP_PSP                                                                \
  return svc##f(svcRtx##f,a1);                                                 \
}

#define SVC0_2(f,t,t1,t2)                                                      \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1,t2),t1,t2);                             \
                  t svcRtx##f (t1 a1, t2 a2);                                  \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2) {                                 \
  return svc##f(svcRtx##f,a1,a2);                                              \
}

#define SVC0_2M(f,t,t1,t2)                                                     \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1,t2),t1,t2);                             \
                  t svcRtx##f (t1 a1, t2 a2);                                  \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2) {                                 \
  SVC_SETUP_PSP                                                                \
  return svc##f(svcRtx##f,a1,a2);                                              \
}

#define SVC0_3(f,t,t1,t2,t3)                                                   \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1,t2,t3),t1,t2,t3);                       \
                  t svcRtx##f (t1 a1, t2 a2, t3 a3);                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2, t3 a3) {                          \
  return svc##f(svcRtx##f,a1,a2,a3);                                           \
}

#define SVC0_3M(f,t,t1,t2,t3)                                                  \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1,t2,t3),t1,t2,t3);                       \
                  t svcRtx##f (t1 a1, t2 a2, t3 a3);                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2, t3 a3) {                          \
  SVC_SETUP_PSP                                                                \
  return svc##f(svcRtx##f,a1,a2,a3);                                           \
}

#define SVC0_4(f,t,t1,t2,t3,t4)                                                \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1,t2,t3,t4),t1,t2,t3,t4);                 \
                  t svcRtx##f (t1 a1, t2 a2, t3 a3, t4 a4);                    \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                   \
  return svc##f(svcRtx##f,a1,a2,a3,a4);                                        \
}

#define SVC0_4M(f,t,t1,t2,t3,t4)                                               \
__SVC_INDIRECT(0) t    svc##f (t(*)(t1,t2,t3,t4),t1,t2,t3,t4);                 \
                  t svcRtx##f (t1 a1, t2 a2, t3 a3, t4 a4);                    \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                   \
  SVC_SETUP_PSP                                                                \
  return svc##f(svcRtx##f,a1,a2,a3,a4);                                        \
}

#elif defined(__ICCARM__)

#if   ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U) || \
       (__ARM_ARCH_8M_MAIN__ == 1U))
#define SVC_Setup(f)                                                           \
  __asm(                                                                       \
    "mov r12,%0\n"                                                             \
    :: "r"(&f): "r12"                                                          \
  );
#elif ((__ARM_ARCH_6M__      == 1U) || \
       (__ARM_ARCH_8M_BASE__ == 1U))
#define SVC_Setup(f)                                                           \
  __asm(                                                                       \
    "mov r7,%0\n"                                                              \
    :: "r"(&f): "r7"                                                           \
  );
#endif

#define STRINGIFY(a) #a
#define __SVC_INDIRECT(n) _Pragma(STRINGIFY(swi_number = n)) __swi

#if    (__FPU_USED == 1U)
#define SVC_SETUP_PSP                                                          \
  uint32_t control = __get_CONTROL();                                          \
  if ((control & 2U) == 0U) {                                                  \
    __set_PSP((__get_MSP() - ((control & 4U) ? 104U : 32U)) & ~7U);            \
  }
#else
#define SVC_SETUP_PSP                                                          \
  uint32_t control = __get_CONTROL();                                          \
  if ((control & 2U) == 0U) {                                                  \
    __set_PSP((__get_MSP() -                          32U)  & ~7U);            \
  }
#endif

#define SVC0_0N(f,t)                                                           \
__SVC_INDIRECT(0) t    svc##f ();                                              \
                  t svcRtx##f (void);                                          \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (void) {                                         \
  SVC_Setup(svcRtx##f);                                                        \
  svc##f();                                                                    \
}

#define SVC0_0(f,t)                                                            \
__SVC_INDIRECT(0) t    svc##f ();                                              \
                  t svcRtx##f (void);                                          \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (void) {                                         \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f();                                                             \
}

#define SVC0_0M(f,t)                                                           \
__SVC_INDIRECT(0) t    svc##f ();                                              \
                  t svcRtx##f (void);                                          \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (void) {                                         \
  SVC_SETUP_PSP                                                                \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f();                                                             \
}

#define SVC0_0D SVC0_0

#define SVC0_1N(f,t,t1)                                                        \
__SVC_INDIRECT(0) t    svc##f (t1 a1);                                         \
                  t svcRtx##f (t1 a1);                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1) {                                        \
  SVC_Setup(svcRtx##f);                                                        \
  svc##f(a1);                                                                  \
}

#define SVC0_1(f,t,t1)                                                         \
__SVC_INDIRECT(0) t    svc##f (t1 a1);                                         \
                  t svcRtx##f (t1 a1);                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1) {                                        \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f(a1);                                                           \
}

#define SVC0_1M(f,t,t1)                                                        \
__SVC_INDIRECT(0) t    svc##f (t1 a1);                                         \
                  t svcRtx##f (t1 a1);                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1) {                                        \
  SVC_SETUP_PSP                                                                \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f(a1);                                                           \
}

#define SVC0_2(f,t,t1,t2)                                                      \
__SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2);                                  \
                  t svcRtx##f (t1 a1, t2 a2);                                  \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2) {                                 \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f(a1,a2);                                                        \
}

#define SVC0_2M(f,t,t1,t2)                                                     \
__SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2);                                  \
                  t svcRtx##f (t1 a1, t2 a2);                                  \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2) {                                 \
  SVC_SETUP_PSP                                                                \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f(a1,a2);                                                        \
}

#define SVC0_3(f,t,t1,t2,t3)                                                   \
__SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2, t3 a3);                           \
                  t svcRtx##f (t1 a1, t2 a2, t3 a3);                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2, t3 a3) {                          \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f(a1,a2,a3);                                                     \
}

#define SVC0_3M(f,t,t1,t2,t3)                                                  \
__SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2, t3 a3);                           \
                  t svcRtx##f (t1 a1, t2 a2, t3 a3);                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2, t3 a3) {                          \
  SVC_SETUP_PSP                                                                \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f(a1,a2,a3);                                                     \
}

#define SVC0_4(f,t,t1,t2,t3,t4)                                                \
__SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2, t3 a3, t4 a4);                    \
                  t svcRtx##f (t1 a1, t2 a2, t3 a3, t4 a4);                    \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                   \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f(a1,a2,a3,a4);                                                  \
}

#define SVC0_4M(f,t,t1,t2,t3,t4)                                               \
__SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2, t3 a3, t4 a4);                    \
                  t svcRtx##f (t1 a1, t2 a2, t3 a3, t4 a4);                    \
__attribute__((always_inline))                                                 \
__STATIC_INLINE   t  __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                   \
  SVC_SETUP_PSP                                                                \
  SVC_Setup(svcRtx##f);                                                        \
  return svc##f(a1,a2,a3,a4);                                                  \
}

#else   // !(defined(__CC_ARM) || defined(__ICCARM__))

#if   ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U) || \
       (__ARM_ARCH_8M_MAIN__ == 1U))
#define SVC_RegF "r12"
#elif ((__ARM_ARCH_6M__      == 1U) || \
       (__ARM_ARCH_8M_BASE__ == 1U))
#define SVC_RegF "r7"
#endif

#define SVC_ArgN(n) \
register uint32_t __r##n __ASM("r"#n)

#define SVC_ArgR(n,a) \
register uint32_t __r##n __ASM("r"#n) = (uint32_t)a

#define SVC_ArgF(f) \
register uint32_t __rf   __ASM(SVC_RegF) = (uint32_t)f

#define SVC_In0 "r"(__rf)
#define SVC_In1 "r"(__rf),"r"(__r0)
#define SVC_In2 "r"(__rf),"r"(__r0),"r"(__r1)
#define SVC_In3 "r"(__rf),"r"(__r0),"r"(__r1),"r"(__r2)
#define SVC_In4 "r"(__rf),"r"(__r0),"r"(__r1),"r"(__r2),"r"(__r3)

#define SVC_Out0
#define SVC_Out1 "=r"(__r0)
#define SVC_Out2 "=r"(__r0),"=r"(__r1)

#define SVC_CL0
#define SVC_CL1 "r1"
#define SVC_CL2 "r0","r1"

#define SVC_Call0(in, out, cl)                                                 \
  __ASM volatile ("svc 0" : out : in : cl)

#if   ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U) || \
       (__ARM_ARCH_8M_MAIN__ == 1U))
#if    (__FPU_USED == 1U)
#define SVC_Call0M(in, out, cl)                                                \
  register uint32_t val;                                                       \
  __ASM volatile (                                                             \
  ".syntax unified\n\t"                                                        \
    "mrs   %[val],control\n\t"                                                 \
    "tst   %[val],#2\n\t"                                                      \
    "bne   0f\n\t"                                                             \
    "tst   %[val],#4\n\t"                                                      \
    "mrs   %[val],msp\n\t"                                                     \
    "ite   eq\n\t"                                                             \
    "subeq %[val],#32\n\t"                                                     \
    "subne %[val],#104\n\t"                                                    \
    "bic   %[val],#7\n\t"                                                      \
    "msr   psp,%[val]\n\t"                                                     \
  "0:\n\t"                                                                     \
    "svc 0"                                                                    \
  : out, [val] "=&l" (val) : in : cl)
#else
#define SVC_Call0M(in, out, cl)                                                \
  register uint32_t val;                                                       \
  __ASM volatile (                                                             \
  ".syntax unified\n\t"                                                        \
    "mrs   %[val],control\n\t"                                                 \
    "tst   %[val],#2\n\t"                                                      \
    "bne   0f\n\t"                                                             \
    "mrs   %[val],msp\n\t"                                                     \
    "subs  %[val],#32\n\t"                                                     \
    "bic   %[val],#7\n\t"                                                      \
    "msr   psp,%[val]\n\t"                                                     \
  "0:\n\t"                                                                     \
    "svc 0"                                                                    \
  : out, [val] "=&l" (val) : in : cl)
#endif
#elif ((__ARM_ARCH_6M__      == 1U) || \
       (__ARM_ARCH_8M_BASE__ == 1U))
#define SVC_Call0M(in, out, cl)                                                \
  register uint32_t val;                                                       \
  __ASM volatile (                                                             \
  ".syntax unified\n\t"                                                        \
    "mrs   %[val],control\n\t"                                                 \
    "lsls  %[val],#30\n\t"                                                     \
    "bmi   0f\n\t"                                                             \
    "mrs   %[val],msp\n\t"                                                     \
    "subs  %[val],#32\n\t"                                                     \
    "lsrs  %[val],#3\n\t"                                                      \
    "lsls  %[val],#3\n\t"                                                      \
    "msr   psp,%[val]\n\t"                                                     \
  "0:\n\t"                                                                     \
    "svc 0"                                                                    \
  : out, [val] "=&l" (val) : in : cl)
#endif

#define SVC0_0N(f,t)                                                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (void) {                                            \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0(SVC_In0, SVC_Out0, SVC_CL2);                                       \
}

#define SVC0_0(f,t)                                                            \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (void) {                                            \
  SVC_ArgN(0);                                                                 \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0(SVC_In0, SVC_Out1, SVC_CL1);                                       \
  return (t) __r0;                                                             \
}

#define SVC0_0M(f,t)                                                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (void) {                                            \
  SVC_ArgN(0);                                                                 \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0M(SVC_In0, SVC_Out1, SVC_CL1);                                      \
  return (t) __r0;                                                             \
}

#define SVC0_0D(f,t)                                                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (void) {                                            \
  SVC_ArgN(0);                                                                 \
  SVC_ArgN(1);                                                                 \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0(SVC_In0, SVC_Out2, SVC_CL0);                                       \
  return (((t) __r0) | (((t) __r1) << 32));                                    \
}

#define SVC0_1N(f,t,t1)                                                        \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1) {                                           \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0(SVC_In1, SVC_Out0, SVC_CL1);                                       \
}

#define SVC0_1(f,t,t1)                                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1) {                                           \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0(SVC_In1, SVC_Out1, SVC_CL1);                                       \
  return (t) __r0;                                                             \
}

#define SVC0_1M(f,t,t1)                                                        \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1) {                                           \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0M(SVC_In1, SVC_Out1, SVC_CL1);                                      \
  return (t) __r0;                                                             \
}

#define SVC0_2(f,t,t1,t2)                                                      \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2) {                                    \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0(SVC_In2, SVC_Out1, SVC_CL0);                                       \
  return (t) __r0;                                                             \
}

#define SVC0_2M(f,t,t1,t2)                                                     \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2) {                                    \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0M(SVC_In2, SVC_Out1, SVC_CL0);                                      \
  return (t) __r0;                                                             \
}

#define SVC0_3(f,t,t1,t2,t3)                                                   \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2, t3 a3) {                             \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgR(2,a3);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0(SVC_In3, SVC_Out1, SVC_CL0);                                       \
  return (t) __r0;                                                             \
}

#define SVC0_3M(f,t,t1,t2,t3)                                                  \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2, t3 a3) {                             \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgR(2,a3);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0M(SVC_In3, SVC_Out1, SVC_CL0);                                      \
  return (t) __r0;                                                             \
}

#define SVC0_4(f,t,t1,t2,t3,t4)                                                \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                      \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgR(2,a3);                                                              \
  SVC_ArgR(3,a4);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0(SVC_In4, SVC_Out1, SVC_CL0);                                       \
  return (t) __r0;                                                             \
}

#define SVC0_4M(f,t,t1,t2,t3,t4)                                               \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                      \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgR(2,a3);                                                              \
  SVC_ArgR(3,a4);                                                              \
  SVC_ArgF(svcRtx##f);                                                         \
  SVC_Call0M(SVC_In4, SVC_Out1, SVC_CL0);                                      \
  return (t) __r0;                                                             \
}

#endif


//  ==== Core Peripherals functions ====

extern uint32_t SystemCoreClock;        // System Clock Frequency (Core Clock)


/// Initialize SVC and PendSV System Service Calls
__STATIC_INLINE void SVC_Initialize (void) {
#if   ((__ARM_ARCH_8M_MAIN__ == 1U) || (defined(__CORTEX_M) && (__CORTEX_M == 7U)))
  uint32_t p, n;

  SCB->SHPR[10] = 0xFFU;
  n = 32U - (uint32_t)__CLZ(~(SCB->SHPR[10] | 0xFFFFFF00U));
  p = NVIC_GetPriorityGrouping();
  if (p >= n) {
    n = p + 1U;
  }
  SCB->SHPR[7] = (uint8_t)(0xFEU << n);
#elif  (__ARM_ARCH_8M_BASE__ == 1U)
  SCB->SHPR[1] |= 0x00FF0000U;
  SCB->SHPR[0] |= (SCB->SHPR[1] << (8+1)) & 0xFC000000U;
#elif ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U))
  uint32_t p, n;

  SCB->SHP[10] = 0xFFU;
  n = 32U - (uint32_t)__CLZ(~(SCB->SHP[10] | 0xFFFFFF00U));
  p = NVIC_GetPriorityGrouping();
  if (p >= n) {
    n = p + 1U;
  }

/* Only change the SVCall priority if uVisor is not present. */
#if !(defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED))
  SCB->SHP[7] = (uint8_t)(0xFEU << n);
#endif
#elif  (__ARM_ARCH_6M__      == 1U)
  SCB->SHP[1] |= 0x00FF0000U;
  SCB->SHP[0] |= (SCB->SHP[1] << (8+1)) & 0xFC000000U;
#endif
}

/// Setup SysTick Timer
/// \param[in] period  Timer Load value
__STATIC_INLINE void SysTick_Setup (uint32_t period) {
  SysTick->LOAD = period - 1U;
  SysTick->VAL  = 0U;
#if   ((__ARM_ARCH_8M_MAIN__ == 1U) || (defined(__CORTEX_M) && (__CORTEX_M == 7U)))
  SCB->SHPR[11] = 0xFFU;
#elif  (__ARM_ARCH_8M_BASE__ == 1U)
  SCB->SHPR[1] |= 0xFF000000U;
#elif ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U))
  SCB->SHP[11]  = 0xFFU;
#elif  (__ARM_ARCH_6M__      == 1U)
  SCB->SHP[1]  |= 0xFF000000U;
#endif
}

/// Get SysTick Period
/// \return    SysTick Period
__STATIC_INLINE uint32_t SysTick_GetPeriod (void) {
  return (SysTick->LOAD + 1U);
}

/// Get SysTick Value
/// \return    SysTick Value
__STATIC_INLINE uint32_t SysTick_GetVal (void) {
  uint32_t load = SysTick->LOAD;
  return  (load - SysTick->VAL);
}

/// Get SysTick Overflow (Auto Clear)
/// \return    SysTick Overflow flag
__STATIC_INLINE uint32_t SysTick_GetOvf (void) {
  return ((SysTick->CTRL >> 16) & 1U);
}

/// Enable SysTick Timer
__STATIC_INLINE void SysTick_Enable (void) {
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk     |
                  SysTick_CTRL_TICKINT_Msk    |
                  SysTick_CTRL_CLKSOURCE_Msk;
}

/// Disable SysTick Timer
__STATIC_INLINE void SysTick_Disable (void) {
  SysTick->CTRL = 0U;
}

/// Setup External Tick Timer Interrupt
/// \param[in] irqn  Interrupt number
__STATIC_INLINE void ExtTick_SetupIRQ (int32_t irqn) {
#if    (__ARM_ARCH_8M_MAIN__ == 1U)
  NVIC->IPR[irqn] = 0xFFU;
#elif  (__ARM_ARCH_8M_BASE__ == 1U)
  NVIC->IPR[irqn >> 2] = (NVIC->IPR[irqn >> 2]  & ~(0xFFU << ((irqn & 3) << 3))) |
                                                   (0xFFU << ((irqn & 3) << 3));
#elif ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U))
  NVIC->IP[irqn] = 0xFFU;
#elif  (__ARM_ARCH_6M__      == 1U)
  NVIC->IP[irqn >> 2] = (NVIC->IP[irqn >> 2]  & ~(0xFFU << ((irqn & 3) << 3))) |
                                                 (0xFFU << ((irqn & 3) << 3));
#endif
}

/// Enable External Tick Timer Interrupt
/// \param[in] irqn  Interrupt number
__STATIC_INLINE void ExtTick_EnableIRQ (int32_t irqn) {
  NVIC->ISER[irqn >> 5] = 1U << (irqn & 0x1F);
}

/// Disable External Tick Timer Interrupt
/// \param[in] irqn  Interrupt number
__STATIC_INLINE void ExtTick_DisableIRQ (int32_t irqn) {
  NVIC->ICER[irqn >> 5] = 1U << (irqn & 0x1F);
}

/// Get Pending SV (Service Call) and ST (SysTick) Flags
/// \return    Pending SV&ST Flags
__STATIC_INLINE uint8_t GetPendSV_ST (void) {
  return ((uint8_t)((SCB->ICSR & (SCB_ICSR_PENDSVSET_Msk | SCB_ICSR_PENDSTSET_Msk)) >> 24));
}

/// Get Pending SV (Service Call) Flag
/// \return    Pending SV Flag
__STATIC_INLINE uint8_t GetPendSV (void) {
  return ((uint8_t)((SCB->ICSR & (SCB_ICSR_PENDSVSET_Msk)) >> 24));
}

/// Clear Pending SV (Service Call) and ST (SysTick) Flags
__STATIC_INLINE void ClrPendSV_ST (void) {
  SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk | SCB_ICSR_PENDSTCLR_Msk;
}

/// Clear Pending SV (Service Call) Flag
__STATIC_INLINE void ClrPendSV (void) {
  SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;
}

/// Set Pending SV (Service Call) Flag
__STATIC_INLINE void SetPendSV (void) {
  SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/// Set Pending Flags
/// \param[in] flags  Flags to set
__STATIC_INLINE void SetPendFlags (uint8_t flags) {
  SCB->ICSR = ((uint32_t)flags << 24);
}


//  ==== Exclusive Access Operation ====

#if (__EXCLUSIVE_ACCESS == 1U)

/// Atomic Access Operation: Write (8-bit)
/// \param[in]  mem             Memory address
/// \param[in]  val             Value to write
/// \return                     Previous value
#if defined(__CC_ARM)
static __asm    uint8_t atomic_wr8 (uint8_t *mem, uint8_t val) {
  mov    r2,r0
1
  ldrexb r0,[r2]
  strexb r3,r1,[r2]
  cbz    r3,%F2
  b      %B1
2
  bx     lr
}
#else
__STATIC_INLINE uint8_t atomic_wr8 (uint8_t *mem, uint8_t val) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint8_t  ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrexb %[ret],[%[mem]]\n\t"
    "strexb %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],2f\n\t"
    "b       1b\n"
  "2:"
  : [ret] "=&l" (ret),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [val] "l"   (val)
  : "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Set bits (32-bit)
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     New value
#if defined(__CC_ARM)
static __asm    uint32_t atomic_set32 (uint32_t *mem, uint32_t bits) {
  mov   r2,r0
1
  ldrex r0,[r2]
  orr   r0,r0,r1
  strex r3,r0,[r2]
  cbz   r3,%F2
  b     %B1
2
  bx     lr
}
#else
__STATIC_INLINE uint32_t atomic_set32 (uint32_t *mem, uint32_t bits) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[val],[%[mem]]\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[ret],%[val]\n\t"
    "orrs  %[ret],%[bits]\n\t"
#else
    "orr   %[ret],%[val],%[bits]\n\t"
#endif
    "strex %[res],%[ret],[%[mem]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
#if (__ARM_ARCH_8M_BASE__ == 1U)
  : "memory", "cc"
#else
  : "memory"
#endif
  );

  return ret;
}
#endif

/// Atomic Access Operation: Clear bits (32-bit)
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Previous value
#if defined(__CC_ARM)
static __asm    uint32_t atomic_clr32 (uint32_t *mem, uint32_t bits) {
  push  {r4,lr}
  mov   r2,r0
1
  ldrex r0,[r2]
  bic   r4,r0,r1
  strex r3,r4,[r2]
  cbz   r3,%F2
  b     %B1
2
  pop   {r4,pc}
}
#else
__STATIC_INLINE uint32_t atomic_clr32 (uint32_t *mem, uint32_t bits) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
#if (__ARM_ARCH_8M_BASE__ == 1U)
  : "memory", "cc"
#else
  : "memory"
#endif
  );

  return ret;
}
#endif

/// Atomic Access Operation: Check if all specified bits (32-bit) are active and clear them
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Active bits before clearing or 0 if not active
#if defined(__CC_ARM)
static __asm    uint32_t atomic_chk32_all (uint32_t *mem, uint32_t bits) {
  push  {r4,lr}
  mov   r2,r0
1
  ldrex r0,[r2]
  and   r4,r0,r1
  cmp   r4,r1
  beq   %F2
  clrex
  movs  r0,#0
  pop   {r4,pc}
2
  bic   r4,r0,r1
  strex r3,r4,[r2]
  cbz   r3,%F3
  b     %B1
3
  pop   {r4,pc}
}
#else
__STATIC_INLINE uint32_t atomic_chk32_all (uint32_t *mem, uint32_t bits) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[val],%[ret]\n\t"
    "ands  %[val],%[bits]\n\t"
#else
    "and   %[val],%[ret],%[bits]\n\t"
#endif
    "cmp   %[val],%[bits]\n\t"
    "beq   2f\n\t"
    "clrex\n\t"
    "movs  %[ret],#0\n\t"
    "b     3f\n"
  "2:\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Check if any specified bits (32-bit) are active and clear them
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Active bits before clearing or 0 if not active
#if defined(__CC_ARM)
static __asm    uint32_t atomic_chk32_any (uint32_t *mem, uint32_t bits) {
  push  {r4,lr}
  mov   r2,r0
1
  ldrex r0,[r2]
  tst   r0,r1
  bne   %F2
  clrex
  movs  r0,#0
  pop   {r4,pc}
2
  bic   r4,r0,r1
  strex r3,r4,[r2]
  cbz   r3,%F3
  b     %B1
3
  pop   {r4,pc}
}
#else
__STATIC_INLINE uint32_t atomic_chk32_any (uint32_t *mem, uint32_t bits) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "tst   %[ret],%[bits]\n\t"
    "bne   2f\n\t"
    "clrex\n\t"
    "movs  %[ret],#0\n\t"
    "b     3f\n"
  "2:\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Increment (32-bit)
/// \param[in]  mem             Memory address
/// \return                     Previous value
#if defined(__CC_ARM)
static __asm    uint32_t atomic_inc32 (uint32_t *mem) {
  mov   r2,r0
1
  ldrex r0,[r2]
  adds  r1,r0,#1
  strex r3,r1,[r2]
  cbz   r3,%F2
  b     %B1
2
  bx     lr
}
#else
__STATIC_INLINE uint32_t atomic_inc32 (uint32_t *mem) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "adds  %[val],%[ret],#1\n\t"
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// atomic Access Operation: Increment (32-bit) if Less Than
/// \param[in]  mem             Memory address
/// \param[in]  max             Maximum value
/// \return                     Previous value
#if defined(__CC_ARM)
static __asm    uint32_t atomic_inc32_lt (uint32_t *mem, uint32_t max) {
  push  {r4,lr}
  mov   r2,r0
1
  ldrex r0,[r2]
  cmp   r1,r0
  bhi   %F2
  clrex
  pop   {r4,pc}
2
  adds  r4,r0,#1
  strex r3,r4,[r2]
  cbz   r3,%F3
  b     %B1
3
  pop   {r4,pc}
}
#else
__STATIC_INLINE uint32_t atomic_inc32_lt (uint32_t *mem, uint32_t max) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "cmp   %[max],%[ret]\n\t"
    "bhi    2f\n\t"
    "clrex\n\t"
    "b      3f\n"
  "2:\n\t"
    "adds  %[val],%[ret],#1\n\t"
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [max] "l"   (max)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Increment (16-bit) if Less Than
/// \param[in]  mem             Memory address
/// \param[in]  max             Maximum value
/// \return                     Previous value
#if defined(__CC_ARM)
static __asm    uint16_t atomic_inc16_lt (uint16_t *mem, uint16_t max) {
  push   {r4,lr}
  mov    r2,r0
1
  ldrexh r0,[r2]
  cmp    r1,r0
  bhi    %F2
  clrex
  pop    {r4,pc}
2
  adds   r4,r0,#1
  strexh r3,r4,[r2]
  cbz    r3,%F3
  b      %B1
3
  pop    {r4,pc}
}
#else
__STATIC_INLINE uint16_t atomic_inc16_lt (uint16_t *mem, uint16_t max) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint16_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "cmp    %[max],%[ret]\n\t"
    "bhi    2f\n\t"
    "clrex\n\t"
    "b      3f\n"
  "2:\n\t"
    "adds   %[val],%[ret],#1\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],3f\n\t"
    "b      1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [max] "l"   (max)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Increment (16-bit) and clear on Limit
/// \param[in]  mem             Memory address
/// \param[in]  max             Maximum value
/// \return                     Previous value
#if defined(__CC_ARM)
static __asm    uint16_t atomic_inc16_lim (uint16_t *mem, uint16_t lim) {
  push   {r4,lr}
  mov    r2,r0
1
  ldrexh r0,[r2]
  adds   r4,r0,#1
  cmp    r1,r4
  bhi    %F2
  movs   r4,#0
2
  strexh r3,r4,[r2]
  cbz    r3,%F3
  b      %B1
3
  pop    {r4,pc}
}
#else
__STATIC_INLINE uint16_t atomic_inc16_lim (uint16_t *mem, uint16_t lim) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint16_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "adds   %[val],%[ret],#1\n\t"
    "cmp    %[lim],%[val]\n\t"
    "bhi    2f\n\t"
    "movs   %[val],#0\n"
  "2:\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],3f\n\t"
    "b      1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [lim] "l"   (lim)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Decrement (32-bit) if Not Zero
/// \param[in]  mem             Memory address
/// \return                     Previous value
#if defined(__CC_ARM)
static __asm    uint32_t atomic_dec32_nz (uint32_t *mem) {
  mov   r2,r0
1
  ldrex r0,[r2]
  cbnz  r0,%F2
  clrex
  bx    lr
2
  subs  r1,r0,#1
  strex r3,r1,[r2]
  cbz   r3,%F3
  b     %B1
3
  bx     lr
}
#else
__STATIC_INLINE uint32_t atomic_dec32_nz (uint32_t *mem) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "cbnz  %[ret],2f\n\t"
    "clrex\n\t"
    "b     3f\n"
  "2:\n\t"
    "subs  %[val],%[ret],#1\n\t"
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Decrement (16-bit) if Not Zero
/// \param[in]  mem             Memory address
/// \return                     Previous value
#if defined(__CC_ARM)
static __asm    uint16_t atomic_dec16_nz (uint16_t *mem) {
  mov    r2,r0
1
  ldrexh r0,[r2]
  cbnz   r0,%F2
  clrex
  bx     lr
2
  subs   r1,r0,#1
  strexh r3,r1,[r2]
  cbz    r3,%F3
  b      %B1
3
  bx      lr
}
#else
__STATIC_INLINE uint16_t atomic_dec16_nz (uint16_t *mem) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint16_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "cbnz   %[ret],2f\n\t"
    "clrex\n\t"
    "b      3f\n"
  "2:\n\t"
    "subs   %[val],%[ret],#1\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],3f\n\t"
    "b      1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Link Get
/// \param[in]  root            Root address
/// \return                     Link
#if defined(__CC_ARM)
static __asm    void *atomic_link_get (void **root) {
  mov   r2,r0
1
  ldrex r0,[r2]
  cbnz  r0,%F2
  clrex
  bx    lr
2
  ldr   r1,[r0]
  strex r3,r1,[r2]
  cbz   r3,%F3
  b     %B1
3
  bx     lr
}
#else
__STATIC_INLINE void *atomic_link_get (void **root) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register void    *ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[root]]\n\t"
    "cbnz  %[ret],2f\n\t"
    "clrex\n\t"
    "b     3f\n"
  "2:\n\t"
    "ldr   %[val],[%[ret]]\n\t"
    "strex %[res],%[val],[%[root]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [root] "l"   (root)
  : "cc", "memory"
  );

  return ret;
}
#endif

/// Atomic Access Operation: Link Put
/// \param[in]  root            Root address
/// \param[in]  lnk             Link
#if defined(__CC_ARM)
static __asm    void atomic_link_put (void **root, void *link) {
1
  ldr   r2,[r0]
  str   r2,[r1]
  dmb
  ldrex r2,[r0]
  ldr   r3,[r1]
  cmp   r3,r2
  bne   %B1
  strex r3,r1,[r0]
  cbz   r3,%F2
  b     %B1
2
  bx    lr
}
#else
__STATIC_INLINE void atomic_link_put (void **root, void *link) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val1, val2, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldr   %[val1],[%[root]]\n\t"
    "str   %[val1],[%[link]]\n\t"
    "dmb\n\t"
    "ldrex %[val1],[%[root]]\n\t"
    "ldr   %[val2],[%[link]]\n\t"
    "cmp   %[val2],%[val1]\n\t"
    "bne   1b\n\t"
    "strex %[res],%[link],[%[root]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [val1] "=&l" (val1),
    [val2] "=&l" (val2),
    [res]  "=&l" (res)
  : [root] "l"   (root),
    [link] "l"   (link)
  : "cc", "memory"
  );
}
#endif

#endif  // (__EXCLUSIVE_ACCESS == 1U)


#endif  // CORE_CM_H_

/** @}*/
