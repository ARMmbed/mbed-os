/*----------------------------------------------------------------------------
 *      CMSIS-RTOS  -  RTX
 *----------------------------------------------------------------------------
 *      Name:    RTX_CM_LIB.H
 *      Purpose: RTX Kernel System Configuration
 *      Rev.:    V4.79
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
#include "mbed_error.h"

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
#define mutex_wait(m)   os_mut_wait(m,0xFFFFU)
#define mutex_rel(m)    os_mut_release(m)

extern uint8_t   os_running;
extern OS_TID    rt_tsk_self    (void);
extern void      rt_mut_init    (OS_ID mutex);
extern OS_RESULT rt_mut_release (OS_ID mutex);
extern OS_RESULT rt_mut_wait    (OS_ID mutex, uint16_t timeout);

#define os_mut_wait(mutex,timeout) _os_mut_wait((uint32_t)rt_mut_wait,mutex,timeout)
#define os_mut_release(mutex)      _os_mut_release((uint32_t)rt_mut_release,mutex)

OS_RESULT _os_mut_release (uint32_t p, OS_ID mutex)                   __svc_indirect(0);
OS_RESULT _os_mut_wait    (uint32_t p, OS_ID mutex, uint16_t timeout) __svc_indirect(0);

#endif


/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

#if (OS_TASKCNT == 0)
#error "Invalid number of concurrent running threads!"
#endif

#if (OS_PRIVCNT >= OS_TASKCNT)
#error "Too many threads with user-provided stack size!"
#endif

#if (OS_TIMERS != 0)
#define OS_TASK_CNT (OS_TASKCNT + 1)
#ifndef __MBED_CMSIS_RTOS_CM
#define OS_PRIV_CNT (OS_PRIVCNT + 2)
#define OS_STACK_SZ (4*(OS_PRIVSTKSIZE+OS_MAINSTKSIZE+OS_TIMERSTKSZ))
#endif
#else
#define OS_TASK_CNT  OS_TASKCNT
#ifndef __MBED_CMSIS_RTOS_CM
#define OS_PRIV_CNT (OS_PRIVCNT + 1)
#define OS_STACK_SZ (4*(OS_PRIVSTKSIZE+OS_MAINSTKSIZE))
#endif
#endif

#ifndef OS_STKINIT
#define OS_STKINIT  0
#endif

uint16_t const os_maxtaskrun = OS_TASK_CNT;
#ifdef __MBED_CMSIS_RTOS_CM
uint32_t const os_stackinfo  = (OS_STKINIT<<28) | (OS_STKCHECK<<24) | (OS_IDLESTKSIZE*4);
#else
uint32_t const os_stackinfo  = (OS_STKINIT<<28) | (OS_STKCHECK<<24) | (OS_PRIV_CNT<<16) | (OS_STKSIZE*4);
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
__USED uint32_t const os_timernum  = 0U;

/* Memory pool for TCB allocation    */
_declare_box  (mp_tcb, OS_TCB_SIZE, OS_TASK_CNT);
uint16_t const mp_tcb_size = sizeof(mp_tcb);

#ifdef __MBED_CMSIS_RTOS_CM
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
#ifdef __MBED_CMSIS_RTOS_CM
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
osMessageQDef(osTimerMessageQ, 0U, void *);
osMessageQId osMessageQId_osTimerMessageQ;
#endif

/* Legacy RTX User Timers not used */
uint32_t       os_tmr = 0U; 
uint32_t const *m_tmr = NULL;
uint16_t const mp_tmr_size = 0U;

#if defined (__CC_ARM) && !defined (__MICROLIB)
 /* A memory space for arm standard library. */
 static uint32_t std_libspace[OS_TASK_CNT][96/4];
 static OS_MUT   std_libmutex[OS_MUTEXCNT];
 static uint32_t nr_mutex;
 extern void  *__libspace_start;
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

  idx = (os_running != 0U) ? runtask_id () : 0U;
  if (idx == 0U) {
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
    error("Not enough stdlib mutexes\n");
  }
  *mutex = &std_libmutex[nr_mutex++];
  mutex_init (*mutex);
  return (1);
}


/*--------------------------- _mutex_acquire --------------------------------*/

__attribute__((used)) void _mutex_acquire (OS_ID *mutex) {
  /* Acquire a system mutex, lock stdlib resources. */
  if (os_running) {
    /* RTX running, acquire a mutex. */
    mutex_wait (*mutex);
  }
}


/*--------------------------- _mutex_release --------------------------------*/

__attribute__((used)) void _mutex_release (OS_ID *mutex) {
  /* Release a system mutex, unlock stdlib resources. */
  if (os_running) {
    /* RTX running, release a mutex. */
    mutex_rel (*mutex);
  }
}

#endif


/*----------------------------------------------------------------------------
 *      RTX Startup
 *---------------------------------------------------------------------------*/

/* Main Thread definition */
extern void pre_main (void);
osThreadDef_t os_thread_def_main = {(os_pthread)pre_main, osPriorityNormal, 1U, 0U, NULL};

// This define should be probably moved to the CMSIS layer
#if   defined(TARGET_LPC1768)
#define INITIAL_SP            (0x10008000UL)

#elif defined(TARGET_LPC11U24)
#define INITIAL_SP            (0x10002000UL)

#elif defined(TARGET_LPC11U35_401) || defined(TARGET_LPC11U35_501) || defined(TARGET_LPCCAPPUCCINO)
#define INITIAL_SP            (0x10002000UL)

#elif defined(TARGET_LPC1114)
#define INITIAL_SP            (0x10001000UL)

#elif defined(TARGET_LPC812)
#define INITIAL_SP            (0x10001000UL)

#elif defined(TARGET_LPC824) || defined(TARGET_SSCI824)
#define INITIAL_SP            (0x10002000UL)

#elif defined(TARGET_KL25Z)
#define INITIAL_SP            (0x20003000UL)

#elif defined(TARGET_KL26Z)
#define INITIAL_SP            (0x20003000UL)

#elif defined(TARGET_KL27Z)
#define INITIAL_SP            (0x20003000UL)

#elif defined(TARGET_K64F)
#define INITIAL_SP            (0x20030000UL)

#elif defined(TARGET_K22F)
#define INITIAL_SP            (0x20010000UL)

#elif defined(TARGET_KL46Z)
#define INITIAL_SP            (0x20006000UL)

#elif defined(TARGET_KL43Z)
#define INITIAL_SP            (0x20006000UL)

#elif defined(TARGET_KL05Z)
#define INITIAL_SP            (0x20000C00UL)

#elif defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)
#define INITIAL_SP            (0x10010000UL)

#elif defined(TARGET_LPC4330)
#define INITIAL_SP            (0x10008000UL)

#elif defined(TARGET_LPC4337)
#define INITIAL_SP            (0x10008000UL)

#elif defined(TARGET_LPC1347)
#define INITIAL_SP            (0x10002000UL)

#elif defined(TARGET_STM32F100RB) || defined(TARGET_STM32F051R8)
#define INITIAL_SP            (0x20002000UL)

#elif defined(TARGET_DISCO_F303VC)
#define INITIAL_SP            (0x2000A000UL)

#elif defined(TARGET_STM32F407) || defined(TARGET_F407VG)
#define INITIAL_SP            (0x20020000UL)

#elif defined(TARGET_STM32F401RE)
#define INITIAL_SP            (0x20018000UL)

#elif defined(TARGET_LPC1549)
#define INITIAL_SP            (0x02009000UL)

#elif defined(TARGET_LPC11U68)
#define INITIAL_SP            (0x10008000UL)

#elif defined(TARGET_STM32F411RE)
#define INITIAL_SP            (0x20020000UL)

#elif defined(TARGET_STM32F410RB)
#define INITIAL_SP            (0x20008000UL)

#elif defined(TARGET_STM32F103RB) || defined(TARGET_STM32L073RZ)
#define INITIAL_SP            (0x20005000UL)

#elif defined(TARGET_STM32F302R8)
#define INITIAL_SP            (0x20004000UL)

#elif  defined(TARGET_STM32F334R8)
#define INITIAL_SP            (0x20003000UL)

#elif  defined(TARGET_STM32F334C8)
#define INITIAL_SP            (0x20003000UL)

#elif  defined(TARGET_STM32F405RG)
#define INITIAL_SP            (0x20020000UL)

#elif defined(TARGET_STM32F429ZI)
#define INITIAL_SP            (0x20030000UL)

#elif defined(TARGET_STM32L031K6) || defined(TARGET_STM32L053R8) || defined(TARGET_STM32L053C8)
#define INITIAL_SP            (0x20002000UL)

#elif defined(TARGET_STM32F072RB)
#define INITIAL_SP            (0x20004000UL)

#elif defined(TARGET_STM32F091RC)
#define INITIAL_SP            (0x20008000UL)

#elif defined(TARGET_STM32F401VC)
#define INITIAL_SP            (0x20010000UL)

#elif defined(TARGET_STM32F303RE)
#define INITIAL_SP            (0x20010000UL)

#elif defined(TARGET_STM32F303K8)
#define INITIAL_SP            (0x20003000UL)

#elif (defined(TARGET_STM32F746NG) || defined(TARGET_STM32F746ZG))
#define INITIAL_SP            (0x20050000UL)

#elif defined(TARGET_MAX32610) || defined(TARGET_MAX32600)
#define INITIAL_SP            (0x20008000UL)

#elif defined(TARGET_TEENSY3_1)
#define INITIAL_SP            (0x20008000UL)

#elif defined(TARGET_STM32L152RE)
#define INITIAL_SP            (0x20014000UL)

#elif defined(TARGET_NZ32_SC151)
#define INITIAL_SP            (0x20008000UL)

#elif (defined(TARGET_STM32F446RE) || defined(TARGET_STM32F446VE))
#define INITIAL_SP            (0x20020000UL)

#elif defined(TARGET_STM32F070RB) || defined(TARGET_STM32F030R8)
#define INITIAL_SP            (0x20002000UL)

#elif defined(TARGET_STM32L476VG)
#define INITIAL_SP            (0x20018000UL)

#elif defined(TARGET_STM32L476RG)
#define INITIAL_SP            (0x20018000UL)

#elif defined(TARGET_STM32F469NI)
#define INITIAL_SP            (0x20050000UL)

#elif defined(TARGET_STM32L152RC)
#define INITIAL_SP            (0x20008000UL)


#else
#error "no target defined"

#endif

#ifdef __CC_ARM
extern uint32_t          Image$$RW_IRAM1$$ZI$$Limit[];
#define HEAP_START      (Image$$RW_IRAM1$$ZI$$Limit)
#elif defined(__GNUC__)
extern uint32_t          __end__[];
#define HEAP_START      (__end__)
#elif defined(__ICCARM__)
#pragma section="HEAP"
#define HEAP_START     (void *)__section_begin("HEAP")
#endif

void set_main_stack(void) {
    // That is the bottom of the main stack block: no collision detection
    os_thread_def_main.stack_pointer = HEAP_START;

    // Leave OS_MAINSTKSIZE words for the scheduler and interrupts
    os_thread_def_main.stacksize = (INITIAL_SP - (unsigned int)HEAP_START) - (OS_MAINSTKSIZE * 4);
}

#if defined (__CC_ARM)

#ifdef __MICROLIB

int main(void);
void _main_init (void) __attribute__((section(".ARM.Collect$$$$000000FF")));
void $Super$$__cpp_initialize__aeabi_(void);

void _main_init (void) {
  osKernelInitialize();
#ifdef __MBED_CMSIS_RTOS_CM
  set_main_stack();
#endif
  osThreadCreate(&os_thread_def_main, NULL);
  osKernelStart();
  for (;;);
}

void $Sub$$__cpp_initialize__aeabi_(void)
{
  // this should invoke C++ initializers prior _main_init, we keep this empty and
  // invoke them after _main_init (=starts RTX kernel)
}

void pre_main()
{
  $Super$$__cpp_initialize__aeabi_();
  main();
}

#else

void * armcc_heap_base;
void * armcc_heap_top;

__asm void pre_main (void)
{
  IMPORT  __rt_lib_init
  IMPORT  main
  IMPORT  armcc_heap_base
  IMPORT  armcc_heap_top

  LDR     R0,=armcc_heap_base
  LDR     R1,=armcc_heap_top
  LDR     R0,[R0]
  LDR     R1,[R1]
  /* Save link register (keep 8 byte alignment with dummy R4) */
  PUSH    {R4, LR}
  BL      __rt_lib_init
  BL       main
  /* Return to the thread destroy function.
   */
  POP     {R4, PC}
  ALIGN
}

/* The single memory model is checking for stack collision at run time, verifing
   that the heap pointer is underneath the stack pointer.

   With the RTOS there is not only one stack above the heap, there are multiple
   stacks and some of them are underneath the heap pointer.
*/
#pragma import(__use_two_region_memory)

__asm void __rt_entry (void) {

  IMPORT  __user_setup_stackheap
  IMPORT  armcc_heap_base
  IMPORT  armcc_heap_top
  IMPORT  os_thread_def_main
  IMPORT  osKernelInitialize
#ifdef __MBED_CMSIS_RTOS_CM
  IMPORT  set_main_stack
#endif
  IMPORT  osKernelStart
  IMPORT  osThreadCreate

  /* __user_setup_stackheap returns:
   * - Heap base in r0 (if the program uses the heap).
   * - Stack base in sp.
   * - Heap limit in r2 (if the program uses the heap and uses two-region memory).
   *
   * More info can be found in:
   * ARM Compiler ARM C and C++ Libraries and Floating-Point Support User Guide
   */
  BL      __user_setup_stackheap
  LDR     R3,=armcc_heap_base
  LDR     R4,=armcc_heap_top
  STR     R0,[R3]
  STR     R2,[R4]
  BL      osKernelInitialize
#ifdef __MBED_CMSIS_RTOS_CM
  BL      set_main_stack
#endif
  LDR     R0,=os_thread_def_main
  MOVS    R1,#0
  BL      osThreadCreate
  BL      osKernelStart
  /* osKernelStart should not return */
  B       .

  ALIGN
}

#endif

#elif defined (__GNUC__)

extern void __libc_fini_array(void);
extern void __libc_init_array (void);
extern int main(int argc, char **argv);

void pre_main(void) {
    atexit(__libc_fini_array);
    __libc_init_array();
    main(0, NULL);
}

__attribute__((naked)) void software_init_hook (void) {
  __asm (
    ".syntax unified\n"
    ".thumb\n"
    "bl   osKernelInitialize\n"
#ifdef __MBED_CMSIS_RTOS_CM
    "bl   set_main_stack\n"
#endif
    "ldr  r0,=os_thread_def_main\n"
    "movs r1,#0\n"
    "bl   osThreadCreate\n"
    "bl   osKernelStart\n"
    /* osKernelStart should not return */
    "B       .\n"
  );
}

#elif defined (__ICCARM__)

extern void* __vector_table;
extern int  __low_level_init(void);
extern void __iar_data_init3(void);
extern __weak void __iar_init_core( void );
extern __weak void __iar_init_vfp( void );
extern void __iar_dynamic_initialization(void);
extern void mbed_sdk_init(void);
extern void exit(int arg);

static uint8_t low_level_init_needed;

void pre_main(void) {
    if (low_level_init_needed) {
        __iar_dynamic_initialization();
    }
    main();
}

#pragma required=__vector_table
void __iar_program_start( void )
{
#ifdef __MBED_CMSIS_RTOS_CM
  __iar_init_core();
  __iar_init_vfp();

  uint8_t low_level_init_needed_local;

  low_level_init_needed_local = __low_level_init();
  if (low_level_init_needed_local) {
    __iar_data_init3();
    mbed_sdk_init();
  }
  /* Store in a global variable after RAM has been initialized */
  low_level_init_needed = low_level_init_needed_local;
#endif
  osKernelInitialize();
#ifdef __MBED_CMSIS_RTOS_CM
  set_main_stack();
#endif
  osThreadCreate(&os_thread_def_main, NULL);
  osKernelStart();
  /* osKernelStart should not return */
  while (1);
}

#endif


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
