/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    RT_HAL_CM.H
 *      Purpose: Hardware Abstraction Layer for Cortex-M definitions
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2015 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#include "cmsis.h"
/* Definitions */
#define INITIAL_xPSR    0x10000000
#define DEMCR_TRCENA    0x01000000
#define ITM_ITMENA      0x00000001
#define MAGIC_WORD      0xE25A2EA5

#define SYS_TICK_IRQn TIMER0_IRQn

extern void rt_set_PSP (U32 stack);
extern U32  rt_get_PSP (void);
extern void os_set_env (void);
extern void SysTick_Handler (void);
extern void *_alloc_box (void *box_mem);
extern int  _free_box (void *box_mem, void *box);

extern void rt_init_stack (P_TCB p_TCB, FUNCP task_body);
extern void rt_ret_val  (P_TCB p_TCB, U32 v0);
extern void rt_ret_val2 (P_TCB p_TCB, U32 v0, U32 v1);

extern void dbg_init (void);
extern void dbg_task_notify (P_TCB p_tcb, BOOL create);
extern void dbg_task_switch (U32 task_id);


#if defined (__CC_ARM)          /* ARM Compiler */

#if ((__TARGET_ARCH_7_M || __TARGET_ARCH_7E_M) && !NO_EXCLUSIVE_ACCESS)
 #define __USE_EXCLUSIVE_ACCESS
#else
 #undef  __USE_EXCLUSIVE_ACCESS
#endif

#elif defined (__GNUC__)        /* GNU Compiler */

#undef  __USE_EXCLUSIVE_ACCESS

#if defined (__CORTEX_M0) || defined (__CORTEX_M0PLUS)
#define __TARGET_ARCH_6S_M 1
#else
#define __TARGET_ARCH_6S_M 0
#endif

#if defined (__VFP_FP__) && !defined(__SOFTFP__)
#define __TARGET_FPU_VFP 1
#else
#define __TARGET_FPU_VFP 0
#endif

#define __inline inline
#define __weak   __attribute__((weak))


#elif defined (__ICCARM__)      /* IAR Compiler */

#undef  __USE_EXCLUSIVE_ACCESS

#if (__CORE__ == __ARM6M__)
#define __TARGET_ARCH_6S_M 1
#else
#define __TARGET_ARCH_6S_M 0
#endif

#if defined __ARMVFP__
#define __TARGET_FPU_VFP 1
#else
#define __TARGET_FPU_VFP 0
#endif

#define __inline inline

#endif


/* NVIC registers */

#define OS_PEND_IRQ()   NVIC_PendIRQ(SYS_TICK_IRQn)
#define OS_PENDING      NVIC_PendingIRQ(SYS_TICK_IRQn)
#define OS_UNPEND(fl)   NVIC_UnpendIRQ(SYS_TICK_IRQn)
#define OS_PEND(fl,p)   NVIC_PendIRQ(SYS_TICK_IRQn)
#define OS_LOCK()       NVIC_DisableIRQ(SYS_TICK_IRQn)
#define OS_UNLOCK()     NVIC_EnableIRQ(SYS_TICK_IRQn)

#define OS_X_PENDING    NVIC_PendingIRQ(SYS_TICK_IRQn)
#define OS_X_UNPEND(fl) NVIC_UnpendIRQ(SYS_TICK_IRQn)
#define OS_X_PEND(fl,p) NVIC_PendIRQ(SYS_TICK_IRQn)

#define OS_X_INIT(n)    NVIC_EnableIRQ(n)
#define OS_X_LOCK(n)    NVIC_DisableIRQ(n)
#define OS_X_UNLOCK(n)  NVIC_EnableIRQ(n)

/* Variables */
extern BIT dbg_msg;

/* Functions */
#ifdef __USE_EXCLUSIVE_ACCESS
 #define rt_inc(p)     while(__strex((__ldrex(p)+1),p))
 #define rt_dec(p)     while(__strex((__ldrex(p)-1),p))
#else
 #define rt_inc(p)     __disable_irq();(*p)++;__enable_irq();
 #define rt_dec(p)     __disable_irq();(*p)--;__enable_irq();
#endif

__inline static U32 rt_inc_qi (U32 size, U8 *count, U8 *first) {
  U32 cnt,c2;
#ifdef __USE_EXCLUSIVE_ACCESS
  do {
    if ((cnt = __ldrex(count)) == size) {
      __clrex();
      return (cnt); }
  } while (__strex(cnt+1, count));
  do {
    c2 = (cnt = __ldrex(first)) + 1;
    if (c2 == size) c2 = 0;
  } while (__strex(c2, first));
#else
  __disable_irq();
  if ((cnt = *count) < size) {
    *count = cnt+1;
    c2 = (cnt = *first) + 1;
    if (c2 == size) c2 = 0;
    *first = c2;
  }
  __enable_irq ();
#endif
  return (cnt);
}

__inline static void rt_systick_init (void) {
#if SYS_TICK_IRQn == TIMER0_IRQn
 #define SYS_TICK_TIMER  LPC_TIM0
 LPC_SC->PCONP |= (1 << PCTIM0);
 LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(1<<3))) | (1<<2); //PCLK == CPUCLK
#elif SYS_TICK_IRQn == TIMER1_IRQn
 #define SYS_TICK_TIMER LPC_TIM1
 LPC_SC->PCONP |= (1 << PCTIM1);
 LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(1<<5))) | (1<<4); //PCLK == CPUCLK
#elif SYS_TICK_IRQn == TIMER2_IRQn
 #define SYS_TICK_TIMER LPC_TIM2
 LPC_SC->PCONP |= (1 << PCTIM2);
 LPC_SC->PCLKSEL1 = (LPC_SC->PCLKSEL1 & (~(1<<13))) | (1<<12); //PCLK == CPUCLK
#else
 #define SYS_TICK_TIMER LPC_TIM3
 LPC_SC->PCONP |= (1 << PCTIM3);
 LPC_SC->PCLKSEL1 = (LPC_SC->PCLKSEL1 & (~(1<<15))) | (1<<14); //PCLK == CPUCLK
#endif

  // setup Timer to count forever
  //interrupt_reg
  SYS_TICK_TIMER->TCR = 2; // reset & disable timer 0
  SYS_TICK_TIMER->TC = os_trv;
  SYS_TICK_TIMER->PR = 0; // set the prescale divider
  //Reset of TC and Interrupt when MR3  MR2 matches TC
  SYS_TICK_TIMER->MCR = (1 << 9) |(1 << 10); //TMCR_MR3_R_Msk | TMCR_MR3_I_Msk
  SYS_TICK_TIMER->MR3 = os_trv; // match registers
  SYS_TICK_TIMER->CCR = 0; // disable compare registers
  SYS_TICK_TIMER->EMR = 0; // disable external match register
  // initialize the interrupt vector
  NVIC_SetVector(SYS_TICK_IRQn, (uint32_t)&SysTick_Handler);
  SYS_TICK_TIMER->TCR = 1; // enable timer 0
}

__inline static void rt_svc_init (void) {
// TODO: add svcInit

}

#ifdef DBG_MSG
#define DBG_INIT() dbg_init()
#define DBG_TASK_NOTIFY(p_tcb,create) if (dbg_msg) dbg_task_notify(p_tcb,create)
#define DBG_TASK_SWITCH(task_id)      if (dbg_msg && (os_tsk.new_tsk != os_tsk.run)) \
                                                   dbg_task_switch(task_id)
#else
#define DBG_INIT()
#define DBG_TASK_NOTIFY(p_tcb,create)
#define DBG_TASK_SWITCH(task_id)
#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

