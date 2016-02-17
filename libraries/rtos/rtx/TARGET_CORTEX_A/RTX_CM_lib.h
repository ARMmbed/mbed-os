/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    RTX_CM_LIB.H
 *      Purpose: RTX Kernel System Configuration
 *      Rev.:    V4.73
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2013 ARM Germany GmbH
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

#if   defined (__CC_ARM)
#pragma O3
#define __USED __attribute__((used))
#elif defined (__GNUC__)
#pragma GCC optimize ("O3")
#define __USED __attribute__((used))
#elif defined (__ICCARM__)
#define __USED __root
#endif


/*----------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/

#define _declare_box(pool,size,cnt)  uint32_t pool[(((size)+3)/4)*(cnt) + 3]
#define _declare_box8(pool,size,cnt) uint64_t pool[(((size)+7)/8)*(cnt) + 2]

#define OS_TCB_SIZE     52
#define OS_TMR_SIZE     8

#if defined (__CC_ARM) && !defined (__MICROLIB)

typedef void    *OS_ID;
typedef uint32_t OS_TID;
typedef uint32_t OS_MUT[4];
typedef uint32_t OS_RESULT;

#define runtask_id()    rt_tsk_self()
#define mutex_init(m)   rt_mut_init(m)
#define mutex_wait(m)   os_mut_wait(m,0xFFFF)
#define mutex_rel(m)    os_mut_release(m)

extern OS_TID    rt_tsk_self    (void);
extern void      rt_mut_init    (OS_ID mutex);
extern OS_RESULT rt_mut_release (OS_ID mutex);
extern OS_RESULT rt_mut_wait    (OS_ID mutex, uint16_t timeout);

#define os_mut_wait(mutex,timeout) _os_mut_wait((uint32_t)rt_mut_wait,mutex,timeout)
#define os_mut_release(mutex)      _os_mut_release((uint32_t)rt_mut_release,mutex)

OS_RESULT _os_mut_release (uint32_t p, OS_ID mutex)                   __svc_indirect(0);
OS_RESULT _os_mut_wait    (uint32_t p, OS_ID mutex, uint16_t timeout) __svc_indirect(0);

#elif defined (__ICCARM__)

typedef void    *OS_ID;
typedef uint32_t OS_TID;
typedef uint32_t OS_MUT[4];
typedef uint32_t OS_RESULT;

#define runtask_id()    rt_tsk_self()
#define mutex_init(m)   rt_mut_init(m)
#define mutex_del(m)    os_mut_delete(m)
#define mutex_wait(m)   os_mut_wait(m,0xFFFF)
#define mutex_rel(m)    os_mut_release(m)

extern OS_TID    rt_tsk_self    (void);
extern void      rt_mut_init    (OS_ID mutex);
extern OS_RESULT rt_mut_delete  (OS_ID mutex);
extern OS_RESULT rt_mut_release (OS_ID mutex);
extern OS_RESULT rt_mut_wait    (OS_ID mutex, uint16_t timeout);

#pragma swi_number=0
__swi OS_RESULT _os_mut_delete  (OS_ID mutex);

static inline OS_RESULT os_mut_delete(OS_ID mutex)
{
    __asm("mov r12,%0\n" :: "r"(&rt_mut_delete) : "r12" );
    return _os_mut_delete(mutex);
}

#pragma swi_number=0
__swi OS_RESULT _os_mut_release (OS_ID mutex);

static inline OS_RESULT os_mut_release(OS_ID mutex)
{
    __asm("mov r12,%0\n" :: "r"(&rt_mut_release) : "r12" );
    return _os_mut_release(mutex);
}

#pragma swi_number=0
__swi OS_RESULT _os_mut_wait    (OS_ID mutex, uint16_t timeout);

static inline OS_RESULT os_mut_wait(OS_ID mutex, uint16_t timeout)
{
    __asm("mov r12,%0\n" :: "r"(&rt_mut_wait) : "r12" );
    return _os_mut_wait(mutex, timeout);
}

#include <yvals.h> /* for include DLib_Thread.h */

void __iar_system_Mtxinit(__iar_Rmtx *);
void __iar_system_Mtxdst(__iar_Rmtx *);
void __iar_system_Mtxlock(__iar_Rmtx *);
void __iar_system_Mtxunlock(__iar_Rmtx *);




#endif


/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

#if (OS_TIMERS != 0)
#define OS_TASK_CNT (OS_TASKCNT + 1)
#ifndef __MBED_CMSIS_RTOS_CA9
#define OS_PRIV_CNT (OS_PRIVCNT + 2)
#define OS_STACK_SZ (4*(OS_PRIVSTKSIZE+OS_MAINSTKSIZE+OS_TIMERSTKSZ))
#endif
#else
#define OS_TASK_CNT  OS_TASKCNT
#ifndef __MBED_CMSIS_RTOS_CA9
#define OS_PRIV_CNT (OS_PRIVCNT + 1)
#define OS_STACK_SZ (4*(OS_PRIVSTKSIZE+OS_MAINSTKSIZE))
#endif
#endif

uint16_t const os_maxtaskrun = OS_TASK_CNT;
#ifdef __MBED_CMSIS_RTOS_CA9
uint32_t const os_stackinfo  = (OS_STKCHECK<<24)| (OS_IDLESTKSIZE*4);
#else
uint32_t const os_stackinfo  = (OS_STKCHECK<<24)| (OS_PRIV_CNT<<16) | (OS_STKSIZE*4);
#endif
uint32_t const os_rrobin     = (OS_ROBIN << 16) | OS_ROBINTOUT;
uint32_t const os_tickfreq   = OS_CLOCK;
uint16_t const os_tickus_i   = OS_CLOCK/1000000;
uint16_t const os_tickus_f   = (((uint64_t)(OS_CLOCK-1000000*(OS_CLOCK/1000000)))<<16)/1000000;
uint32_t const os_trv        = OS_TRV;
uint8_t  const os_flags      = OS_RUNPRIV;

/* Export following defines to uVision debugger. */
__USED uint32_t const CMSIS_RTOS_API_Version = osCMSIS;
__USED uint32_t const CMSIS_RTOS_RTX_Version = osCMSIS_RTX;
__USED uint32_t const os_clockrate = OS_TICK;
__USED uint32_t const os_timernum  = 0;

/* Memory pool for TCB allocation    */
_declare_box  (mp_tcb, OS_TCB_SIZE, OS_TASK_CNT);
uint16_t const mp_tcb_size = sizeof(mp_tcb);

#ifdef __MBED_CMSIS_RTOS_CA9
/* Memory pool for os_idle_demon stack allocation. */
_declare_box8 (mp_stk, OS_IDLESTKSIZE*4, 1);
uint32_t const mp_stk_size = sizeof(mp_stk);
#else
/* Memory pool for System stack allocation (+os_idle_demon). */
_declare_box8 (mp_stk, OS_STKSIZE*4, OS_TASK_CNT-OS_PRIV_CNT+1);
uint32_t const mp_stk_size = sizeof(mp_stk);

/* Memory pool for user specified stack allocation (+main, +timer) */
uint64_t       os_stack_mem[2+OS_PRIV_CNT+(OS_STACK_SZ/8)];
uint32_t const os_stack_sz = sizeof(os_stack_mem);
#endif

#ifndef OS_FIFOSZ
 #define OS_FIFOSZ      16
#endif

/* Fifo Queue buffer for ISR requests.*/
uint32_t       os_fifo[OS_FIFOSZ*2+1];
uint8_t  const os_fifo_size = OS_FIFOSZ;

/* An array of Active task pointers. */
void *os_active_TCB[OS_TASK_CNT];

/* User Timers Resources */
#if (OS_TIMERS != 0)
extern void osTimerThread (void const *argument);
#ifdef __MBED_CMSIS_RTOS_CA9
osThreadDef(osTimerThread, (osPriority)(OS_TIMERPRIO-3), 4*OS_TIMERSTKSZ);
#else
osThreadDef(osTimerThread, (osPriority)(OS_TIMERPRIO-3), 1, 4*OS_TIMERSTKSZ);
#endif
osThreadId osThreadId_osTimerThread;
osMessageQDef(osTimerMessageQ, OS_TIMERCBQS, void *);
osMessageQId osMessageQId_osTimerMessageQ;
#else
osThreadDef_t os_thread_def_osTimerThread = { NULL };
osThreadId osThreadId_osTimerThread;
osMessageQDef(osTimerMessageQ, 0, void *);
osMessageQId osMessageQId_osTimerMessageQ;
#endif

/* Legacy RTX User Timers not used */
uint32_t       os_tmr = 0;
uint32_t const *m_tmr = NULL;
uint16_t const mp_tmr_size = 0;

#if defined (__CC_ARM) && !defined (__MICROLIB)
 /* A memory space for arm standard library. */
 static uint32_t std_libspace[OS_TASK_CNT][96/4];
 static OS_MUT   std_libmutex[OS_MUTEXCNT];
 static uint32_t nr_mutex;
 extern void  *__libspace_start;
#elif defined (__ICCARM__)
typedef struct os_mut_array {
    OS_MUT   mutex;
    uint32_t used;
} os_mut_array_t;

static os_mut_array_t std_libmutex[OS_MUTEXCNT];/* must be Zero clear */
static uint32_t nr_mutex = 0;
#endif


/*----------------------------------------------------------------------------
 *      RTX Optimizations (empty functions)
 *---------------------------------------------------------------------------*/

#if OS_ROBIN == 0
 void rt_init_robin (void) {;}
 void rt_chk_robin  (void) {;}
#endif

#if OS_STKCHECK == 0
 void rt_stk_check  (void) {;}
#endif


/*----------------------------------------------------------------------------
 *      Standard Library multithreading interface
 *---------------------------------------------------------------------------*/

#if defined (__CC_ARM) && !defined (__MICROLIB)

/*--------------------------- __user_perthread_libspace ---------------------*/

void *__user_perthread_libspace (void) {
  /* Provide a separate libspace for each task. */
  uint32_t idx;

  idx = runtask_id ();
  if (idx == 0) {
    /* RTX not running yet. */
    return (&__libspace_start);
  }
  return ((void *)&std_libspace[idx-1]);
}

/*--------------------------- _mutex_initialize -----------------------------*/

int _mutex_initialize (OS_ID *mutex) {
  /* Allocate and initialize a system mutex. */

  if (nr_mutex >= OS_MUTEXCNT) {
    /* If you are here, you need to increase the number OS_MUTEXCNT. */
    for (;;);
  }
  *mutex = &std_libmutex[nr_mutex++];
  mutex_init (*mutex);
  return (1);
}


/*--------------------------- _mutex_acquire --------------------------------*/

__attribute__((used)) void _mutex_acquire (OS_ID *mutex) {
  /* Acquire a system mutex, lock stdlib resources. */
  if (runtask_id ()) {
    /* RTX running, acquire a mutex. */
    mutex_wait (*mutex);
  }
}


/*--------------------------- _mutex_release --------------------------------*/

__attribute__((used)) void _mutex_release (OS_ID *mutex) {
  /* Release a system mutex, unlock stdlib resources. */
  if (runtask_id ()) {
    /* RTX running, release a mutex. */
    mutex_rel (*mutex);
  }
}

#elif defined (__ICCARM__)

/*--------------------------- __iar_system_Mtxinit --------------------------*/

void __iar_system_Mtxinit(__iar_Rmtx *mutex)
{
    /* Allocate and initialize a system mutex. */
    int32_t idx;

    for (idx = 0; idx < OS_MUTEXCNT; idx++)
    {
        if (std_libmutex[idx].used == 0)
        {
            std_libmutex[idx].used = 1;
            *mutex = &std_libmutex[idx].mutex;
            nr_mutex++;
            break;
        }
    }
    if (nr_mutex >= OS_MUTEXCNT)
    {
        /* If you are here, you need to increase the number OS_MUTEXCNT. */
        for (;;);
    }
  
    mutex_init (*mutex);
}

/*--------------------------- __iar_system_Mtxdst ---------------------------*/

void __iar_system_Mtxdst(__iar_Rmtx *mutex)
{
    /* Free a system mutex. */
    int32_t idx;

    if (nr_mutex == 0)
    {
        for (;;);
    }

    idx = ((((uint32_t)mutex) - ((uint32_t)&std_libmutex[0].mutex))
           / sizeof(os_mut_array_t));

    if (idx >= OS_MUTEXCNT)
    {
        for (;;);
    }

    mutex_del (*mutex);
    std_libmutex[idx].used = 0;
}

/*--------------------------- __iar_system_Mtxlock --------------------------*/

void __iar_system_Mtxlock(__iar_Rmtx *mutex)
{
    /* Acquire a system mutex, lock stdlib resources. */
    if (runtask_id ())
    {
        /* RTX running, acquire a mutex. */
        mutex_wait (*mutex);
    }
}

/*--------------------------- __iar_system_Mtxunlock ------------------------*/

void __iar_system_Mtxunlock(__iar_Rmtx *mutex)
{
    /* Release a system mutex, unlock stdlib resources. */
    if (runtask_id ())
    {
        /* RTX running, release a mutex. */
        mutex_rel (*mutex);
    }
}

#endif


/*----------------------------------------------------------------------------
 *      RTX Startup
 *---------------------------------------------------------------------------*/

/* Main Thread definition */
extern int main (void);
#ifdef __MBED_CMSIS_RTOS_CA9
uint32_t os_thread_def_stack_main [(4 * OS_MAINSTKSIZE) / sizeof(uint32_t)];
osThreadDef_t os_thread_def_main = {(os_pthread)main, osPriorityNormal, 1, 4*OS_MAINSTKSIZE, os_thread_def_stack_main };
#else
osThreadDef_t os_thread_def_main = {(os_pthread)main, osPriorityNormal, 1, 4*OS_MAINSTKSIZE };
#endif

#if defined (__CC_ARM)

#ifdef __MICROLIB
void _main_init (void) __attribute__((section(".ARM.Collect$$$$000000FF")));
#if __TARGET_ARCH_ARM
#pragma push
#pragma arm
#endif
void _main_init (void) {
  osKernelInitialize();
  osThreadCreate(&os_thread_def_main, NULL);
  osKernelStart();
  for (;;);
}
#if __TARGET_ARCH_ARM
#pragma pop
#endif
#else
__asm void __rt_entry (void) {

  IMPORT  __user_setup_stackheap
  IMPORT  __rt_lib_init
  IMPORT  os_thread_def_main
  IMPORT  osKernelInitialize
  IMPORT  osKernelStart
  IMPORT  osThreadCreate
  IMPORT  exit

  BL      __user_setup_stackheap
  MOV     R1,R2
  BL      __rt_lib_init
  BL      osKernelInitialize
  LDR     R0,=os_thread_def_main
  MOVS    R1,#0
  BL      osThreadCreate
  BL      osKernelStart
  BL      exit

  ALIGN
}
#endif

#elif defined (__GNUC__)

#ifdef __CS3__

/* CS3 start_c routine.
 *
 * Copyright (c) 2006, 2007 CodeSourcery Inc
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

#include "cs3.h"

extern void __libc_init_array (void);

__attribute ((noreturn)) void __cs3_start_c (void){
  unsigned regions = __cs3_region_num;
  const struct __cs3_region *rptr = __cs3_regions;

  /* Initialize memory */
  for (regions = __cs3_region_num, rptr = __cs3_regions; regions--; rptr++) {
    long long *src = (long long *)rptr->init;
    long long *dst = (long long *)rptr->data;
    unsigned limit = rptr->init_size;
    unsigned count;

    if (src != dst)
      for (count = 0; count != limit; count += sizeof (long long))
        *dst++ = *src++;
    else
      dst = (long long *)((char *)dst + limit);
    limit = rptr->zero_size;
    for (count = 0; count != limit; count += sizeof (long long))
      *dst++ = 0;
  }

  /* Run initializers.  */
  __libc_init_array ();

  osKernelInitialize();
  osThreadCreate(&os_thread_def_main, NULL);
  osKernelStart();
  for (;;);
}

#else

__attribute__((naked)) void software_init_hook (void) {
  __asm (
    ".syntax unified\n"
    ".arm\n"
    "movs r0,#0\n"
    "movs r1,#0\n"
    "mov  r4,r0\n"
    "mov  r5,r1\n"
    "ldr  r0,= __libc_fini_array\n"
    "bl   atexit\n"
    "bl   __libc_init_array\n"
    "mov  r0,r4\n"
    "mov  r1,r5\n"
    "bl   osKernelInitialize\n"
    "ldr  r0,=os_thread_def_main\n"
    "movs r1,#0\n"
    "bl   osThreadCreate\n"
    "bl   osKernelStart\n"
    "bl   exit\n"
  );
}

#endif

#elif defined (__ICCARM__)

extern void exit(int arg);

void mbed_main(void) {
  int a;

  osKernelInitialize();
  osThreadCreate(&os_thread_def_main, NULL);
  a = osKernelStart();
  exit(a);
}

#endif


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

