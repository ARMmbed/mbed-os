/** \addtogroup rtos */
/** @{*/
/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
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
#include "cmsis_compiler.h"
#include "cmsis.h"

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


#define XPSR_INITIAL_VALUE      0x01000000U

#if    (__DOMAIN_NS == 1U)
#define STACK_FRAME_INIT        0xBCU
#else
#define STACK_FRAME_INIT        0xFDU
#endif

#define IS_EXTENDED_STACK_FRAME(n) (((n) & 0x10U) == 0U)


//  ==== Service Calls definitions ====

#define SVC_ArgN(n) \
  register uint32_t __r##n __asm("r"#n)

#define SVC_ArgR(n,a) \
  register uint32_t __r##n __asm("r"#n) = (uint32_t)a

#define SVC_Arg0()                                                             \
  SVC_ArgN(0);                                                                 \
  SVC_ArgN(1);                                                                 \
  SVC_ArgN(2);                                                                 \
  SVC_ArgN(3)

#define SVC_Arg1(a1)                                                           \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgN(1);                                                                 \
  SVC_ArgN(2);                                                                 \
  SVC_ArgN(3)

#define SVC_Arg2(a1,a2)                                                        \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgN(2);                                                                 \
  SVC_ArgN(3)

#define SVC_Arg3(a1,a2,a3)                                                     \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgR(2,a3);                                                              \
  SVC_ArgN(3)

#define SVC_Arg4(a1,a2,a3,a4)                                                  \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgR(2,a3);                                                              \
  SVC_ArgR(3,a4)

#if   ((__ARM_ARCH_7M__      == 1U) || \
       (__ARM_ARCH_7EM__     == 1U) || \
       (__ARM_ARCH_8M_MAIN__ == 1U))
#define SVC_Call0(f)                                                           \
  __ASM volatile                                                               \
  (                                                                            \
    "ldr r12,="#f"\n\t"                                                        \
    "svc 0"                                                                    \
    : "=r" (__r0), "=r" (__r1), "=r" (__r2), "=r" (__r3)                       \
    :  "r" (__r0),  "r" (__r1),  "r" (__r2),  "r" (__r3)                       \
    : "r12", "lr", "cc"                                                        \
  )
#elif ((__ARM_ARCH_6M__      == 1U) || \
       (__ARM_ARCH_8M_BASE__ == 1U))
#define SVC_Call0(f)                                                           \
  __ASM volatile                                                               \
  (                                                                            \
    "ldr r7,="#f"\n\t"                                                         \
    "svc 0"                                                                    \
    : "=r" (__r0), "=r" (__r1), "=r" (__r2), "=r" (__r3)                       \
    :  "r" (__r0),  "r" (__r1),  "r" (__r2),  "r" (__r3)                       \
    : "r7", "lr", "cc"                                                         \
  )
#endif

#define SVC0_0N(f,t)                                                           \
__attribute__((always_inline))                                                 \
static inline t __svc##f (void) {                                              \
  SVC_Arg0();                                                                  \
  SVC_Call0(os_svc##f);                                                        \
}

#define SVC0_0(f,t)                                                            \
__attribute__((always_inline))                                                 \
static inline t __svc##f (void) {                                              \
  SVC_Arg0();                                                                  \
  SVC_Call0(os_svc##f);                                                        \
  return (t) __r0;                                                             \
}

#define SVC0_1N(f,t,t1)                                                        \
__attribute__((always_inline))                                                 \
static inline t __svc##f (t1 a1) {                                             \
  SVC_Arg1(a1);                                                                \
  SVC_Call0(os_svc##f);                                                        \
}

#define SVC0_1(f,t,t1)                                                         \
__attribute__((always_inline))                                                 \
static inline t __svc##f (t1 a1) {                                             \
  SVC_Arg1(a1);                                                                \
  SVC_Call0(os_svc##f);                                                        \
  return (t) __r0;                                                             \
}

#define SVC0_2(f,t,t1,t2)                                                      \
__attribute__((always_inline))                                                 \
static inline t __svc##f (t1 a1, t2 a2) {                                      \
  SVC_Arg2(a1,a2);                                                             \
  SVC_Call0(os_svc##f);                                                        \
  return (t) __r0;                                                             \
}

#define SVC0_3(f,t,t1,t2,t3)                                                   \
__attribute__((always_inline))                                                 \
static inline t __svc##f (t1 a1, t2 a2, t3 a3) {                               \
  SVC_Arg3(a1,a2,a3);                                                          \
  SVC_Call0(os_svc##f);                                                        \
  return (t) __r0;                                                             \
}

#define SVC0_4(f,t,t1,t2,t3,t4)                                                \
__attribute__((always_inline))                                                 \
static inline t __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                        \
  SVC_Arg4(a1,a2,a3,a4);                                                       \
  SVC_Call0(os_svc##f);                                                        \
  return (t) __r0;                                                             \
}


//  ==== Core Peripherals functions ====

extern uint32_t SystemCoreClock;        // System Clock Frequency (Core Clock)


/// Initialize SVC and PendSV System Service Calls
__STATIC_INLINE void os_SVC_Initialize (void) {
#if   (__ARM_ARCH_8M_MAIN__ == 1U)
  uint32_t p, n;

  SCB->SHPR[10] = 0xFFU;
  n = 32U - (uint32_t)__CLZ(~(SCB->SHPR[10] | 0xFFFFFF00U));
  p = NVIC_GetPriorityGrouping();
  if (p >= n) {
    n = p + 1U;
  }
  SCB->SHPR[7] = (uint8_t)(0xFEU << n);
#elif (__ARM_ARCH_8M_BASE__ == 1U)
  SCB->SHPR[1] |= 0x00FF0000U;
  SCB->SHPR[0] |= (SCB->SHPR[1] << (8+1)) & 0xFC000000U;
#elif ((__ARM_ARCH_7M__ == 1U) || (__ARM_ARCH_7EM__ == 1U))
  uint32_t p, n;

  SCB->SHP[10] = 0xFFU;
  n = 32U - (uint32_t)__CLZ(~(SCB->SHP[10] | 0xFFFFFF00U));
  p = NVIC_GetPriorityGrouping();
  if (p >= n) {
    n = p + 1U;
  }
  SCB->SHP[7] = (uint8_t)(0xFEU << n);
#elif (__ARM_ARCH_6M__ == 1U)
  SCB->SHP[1] |= 0x00FF0000U;
  SCB->SHP[0] |= (SCB->SHP[1] << (8+1)) & 0xFC000000U;
#endif
}

/// Setup SysTick Timer
/// \param[in] period  Timer Load value
__STATIC_INLINE void os_SysTick_Setup (uint32_t period) {
  SysTick->LOAD = period - 1U;
  SysTick->VAL  = 0U;
#if   (__ARM_ARCH_8M_MAIN__ == 1U)
  SCB->SHPR[11] = 0xFFU;
#elif (__ARM_ARCH_8M_BASE__ == 1U)
  SCB->SHPR[1] |= 0xFF000000U;
#elif ((__ARM_ARCH_7M__ == 1U) || (__ARM_ARCH_7EM__ == 1U))
  SCB->SHP[11]  = 0xFFU;
#elif (__ARM_ARCH_6M__ == 1U)
  SCB->SHP[1]  |= 0xFF000000U;
#endif
}

/// Get SysTick Period
/// \return    SysTick Period
__STATIC_INLINE uint32_t os_SysTick_GetPeriod (void) {
  return (SysTick->LOAD + 1U);
}

/// Get SysTick Value
/// \return    SysTick Value
__STATIC_INLINE uint32_t os_SysTick_GetVal (void) {
  return (SysTick->LOAD - SysTick->VAL);
}

/// Get SysTick Overflow (Auto Clear)
/// \return    SysTick Overflow flag
__STATIC_INLINE uint32_t os_SysTick_GetOvf (void) {
  return ((SysTick->CTRL >> 16) & 1U);
}

/// Enable SysTick Timer
__STATIC_INLINE void os_SysTick_Enable (void) {
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk     |
                  SysTick_CTRL_TICKINT_Msk    |
                  SysTick_CTRL_CLKSOURCE_Msk;
}

/// Disable SysTick Timer
__STATIC_INLINE void os_SysTick_Disable (void) {
  SysTick->CTRL = 0U;
}

/// Setup External Tick Timer Interrupt
/// \param[in] irqn  Interrupt number
__STATIC_INLINE void os_ExtTick_SetupIRQ (int32_t irqn) {
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
__STATIC_INLINE void os_ExtTick_EnableIRQ (int32_t irqn) {
  NVIC->ISER[irqn >> 5] = 1U << (irqn & 0x1F);
}

/// Disable External Tick Timer Interrupt
/// \param[in] irqn  Interrupt number
__STATIC_INLINE void os_ExtTick_DisableIRQ (int32_t irqn) {
  NVIC->ICER[irqn >> 5] = 1U << (irqn & 0x1F);
}

/// Get Pending SV (Service Call) and ST (SysTick) Flags
/// \return    Pending SV&ST Flags
__STATIC_INLINE uint8_t os_GetPendSV_ST (void) {
  return ((uint8_t)((SCB->ICSR & (SCB_ICSR_PENDSVSET_Msk | SCB_ICSR_PENDSTSET_Msk)) >> 24));
}

/// Get Pending SV (Service Call) Flag
/// \return    Pending SV Flag
__STATIC_INLINE uint8_t os_GetPendSV (void) {
  return ((uint8_t)((SCB->ICSR & (SCB_ICSR_PENDSVSET_Msk)) >> 24));
}

/// Clear Pending SV (Service Call) and ST (SysTick) Flags
__STATIC_INLINE void os_ClrPendSV_ST (void) {
  SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk | SCB_ICSR_PENDSTCLR_Msk;
}

/// Clear Pending SV (Service Call) Flag
__STATIC_INLINE void os_ClrPendSV (void) {
  SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;
}

/// Set Pending SV (Service Call) Flag
__STATIC_INLINE void os_SetPendSV (void) {
  SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/// Set Pending Flags
/// \param[in] flags  Flags to set
__STATIC_INLINE void os_SetPendFlags (uint8_t flags) {
  SCB->ICSR = ((uint32_t)flags << 24);
}


//  ==== Exclusive Access Operation ====

#if (__EXCLUSIVE_ACCESS == 1U)

/// Exclusive Access Operation: Write (8-bit)
/// \param[in]  mem             Memory address
/// \param[in]  val             Value to write
/// \return                     Previous value
__STATIC_INLINE uint8_t os_exc_wr8 (uint8_t *mem, uint8_t val) {
  register uint32_t res;
  register uint8_t  ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrexb %[ret],[%[mem]]\n\t"
    "strexb %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],exit%=\n\t"
    "b      loop%=\n\t"
  "exit%=:"
  : [ret] "=&l" (ret),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [val] "l"   (val)
  : "memory"
  );

  return ret;
}

/// Exclusive Access Operation: Set bits (32-bit)
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     New value
__STATIC_INLINE uint32_t os_exc_set32 (uint32_t *mem, uint32_t bits) {
  register uint32_t val, res;
  register uint32_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrex %[val],[%[mem]]\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[ret],%[val]\n\t"
    "orrs  %[ret],%[bits]\n\t"
#else
    "orr   %[ret],%[val],%[bits]\n\t"
#endif
    "strex %[res],%[ret],[%[mem]]\n\t"
    "cbz   %[res],exit%=\n\t"
    "b     loop%=\n\t"
  "exit%=:"
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

/// Exclusive Access Operation: Clear bits (32-bit)
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Previous value
__STATIC_INLINE uint32_t os_exc_clr32 (uint32_t *mem, uint32_t bits) {
  register uint32_t val, res;
  register uint32_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],exit%=\n\t"
    "b     loop%=\n\t"
  "exit%=:"
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

/// Exclusive Access Operation: Check if all specified bits (32-bit) are active and clear them
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Active bits before clearing or 0 if not active
__STATIC_INLINE uint32_t os_exc_chk32_all (uint32_t *mem, uint32_t bits) {
  register uint32_t val, res;
  register uint32_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[val],%[ret]\n\t"
    "ands  %[val],%[bits]\n\t"
#else
    "and   %[val],%[ret],%[bits]\n\t"
#endif
    "cmp   %[val],%[bits]\n\t"
    "beq   update%=\n\t"
    "clrex\n\t"
    "movs  %[ret],#0\n\t"
    "b     exit%=\n\t"
  "update%=:\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],exit%=\n\t"
    "b     loop%=\n\t"
  "exit%=:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
  : "cc", "memory"
  );

  return ret;
}

/// Exclusive Access Operation: Check if any specified bits (32-bit) are active and clear them
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Active bits before clearing or 0 if not active
__STATIC_INLINE uint32_t os_exc_chk32_any (uint32_t *mem, uint32_t bits) {
  register uint32_t val, res;
  register uint32_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "tst   %[ret],%[bits]\n\t"
    "bne   update%=\n\t"
    "clrex\n\t"
    "movs  %[ret],#0\n\t"
    "b     exit%=\n\t"
  "update%=:\n\t"
#if (__ARM_ARCH_8M_BASE__ == 1U)
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],exit%=\n\t"
    "b     loop%=\n\t"
  "exit%=:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
  : "cc", "memory"
  );

  return ret;
}

/// Exclusive Access Operation: Increment (32-bit)
/// \param[in]  mem             Memory address
/// \return                     Previous value
__STATIC_INLINE uint32_t os_exc_inc32 (uint32_t *mem) {
  register uint32_t val, res;
  register uint32_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "adds  %[val],%[ret],#1\n\t"
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],exit%=\n\t"
    "b     loop%=\n\t"
  "exit%=:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}

/// Exclusive Access Operation: Increment (16-bit) if Less Than
/// \param[in]  mem             Memory address
/// \param[in]  max             Maximum value
/// \return                     Previous value
__STATIC_INLINE uint16_t os_exc_inc16_lt (uint16_t *mem, uint16_t max) {
  register uint32_t val, res;
  register uint16_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "cmp    %[max],%[ret]\n\t"
    "bhi    update%=\n\t"
    "clrex\n\t"
    "b      exit%=\n\t"
  "update%=:\n\t"
    "adds   %[val],%[ret],#1\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],exit%=\n\t"
    "b      loop%=\n\t"
  "exit%=:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [max] "l"   (max)
  : "cc", "memory"
  );

  return ret;
}

/// Exclusive Access Operation: Increment (16-bit) and clear on Limit
/// \param[in]  mem             Memory address
/// \param[in]  max             Maximum value
/// \return                     Previous value
__STATIC_INLINE uint16_t os_exc_inc16_lim (uint16_t *mem, uint16_t lim) {
  register uint32_t val, res;
  register uint16_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "adds   %[val],%[ret],#1\n\t"
    "cmp    %[lim],%[val]\n\t"
    "bhi    update%=\n\t"
    "movs   %[val],#0\n\t"
  "update%=:\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],exit%=\n\t"
    "b      loop%=\n\t"
  "exit%=:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [lim] "l"   (lim)
  : "cc", "memory"
  );

  return ret;
}

/// Exclusive Access Operation: Decrement (32-bit) if Not Zero
/// \param[in]  mem             Memory address
/// \return                     Previous value
__STATIC_INLINE uint32_t os_exc_dec32_nz (uint32_t *mem) {
  register uint32_t val, res;
  register uint32_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "cbnz  %[ret],update%=\n\t"
    "clrex\n\t"
    "b     exit%=\n\t"
  "update%=:\n\t"
    "subs  %[val],%[ret],#1\n\t"
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],exit%=\n\t"
    "b     loop%=\n\t"
  "exit%=:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}

/// Exclusive Access Operation: Decrement (16-bit) if Not Zero
/// \param[in]  mem             Memory address
/// \return                     Previous value
__STATIC_INLINE uint16_t os_exc_dec16_nz (uint16_t *mem) {
  register uint32_t val, res;
  register uint16_t ret;

  __ASM volatile (
  ".syntax unified\n\t"
  "loop%=:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "cbnz   %[ret],update%=\n\t"
    "clrex\n\t"
    "b      exit%=\n\t"
  "update%=:\n\t"
    "subs   %[val],%[ret],#1\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],exit%=\n\t"
    "b      loop%=\n\t"
  "exit%=:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}

#endif  // (__EXCLUSIVE_ACCESS == 1U)


#endif  // CORE_CM_H_

/** @}*/
