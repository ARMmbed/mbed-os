
/** \addtogroup rtos */
/** @{*/
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
#include <rt_misc.h>
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

#define OS_TCB_SIZE     64
#define OS_TMR_SIZE     8

typedef void    *OS_ID;
typedef uint32_t OS_TID;
typedef uint32_t OS_MUT[4];
typedef uint32_t OS_RESULT;

#if defined (__CC_ARM) && !defined (__MICROLIB)

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
#if       defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED)
uint8_t  const os_flags      = 0;
#else  /* defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED) */
uint8_t  const os_flags      = OS_RUNPRIV;
#endif /* defined(FEATURE_UVISOR) && defined(TARGET_UVISOR_SUPPORTED) */

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

/* singleton mutex */
osMutexId singleton_mutex_id;
osMutexDef(singleton_mutex);

#if defined (__CC_ARM) && !defined (__MICROLIB)
 /* A memory space for arm standard library. */
 static uint32_t std_libspace[OS_TASK_CNT][96/4];
 static OS_MUT   std_libmutex[OS_MUTEXCNT];
 static uint32_t nr_mutex;
 extern void  *__libspace_start;
#endif

#if defined (__ICCARM__)
static osMutexId  std_mutex_id_sys[_MAX_LOCK] = {0};
static OS_MUT     std_mutex_sys[_MAX_LOCK] = {0};
#define _FOPEN_MAX 10
static osMutexId  std_mutex_id_file[_FOPEN_MAX] = {0};
static OS_MUT     std_mutex_file[_FOPEN_MAX] = {0};
void __iar_system_Mtxinit(__iar_Rmtx *mutex) /* Initialize a system lock */
{
  osMutexDef_t def;
  uint32_t index;
  for (index = 0; index < _MAX_LOCK; index++) {
    if (0 == std_mutex_id_sys[index]) {
      def.mutex = &std_mutex_sys[index];
      std_mutex_id_sys[index] = osMutexCreate(&def);
      *mutex = (__iar_Rmtx*)&std_mutex_id_sys[index];
      return;
    }
  }
  // This should never happen
  error("Not enough mutexes\n");
}

void __iar_system_Mtxdst(__iar_Rmtx *mutex)/*Destroy a system lock */
{
  osMutexDelete(*(osMutexId*)*mutex);
  *mutex = 0;
}

void __iar_system_Mtxlock(__iar_Rmtx *mutex) /* Lock a system lock */
{
  osMutexWait(*(osMutexId*)*mutex, osWaitForever);
}

void __iar_system_Mtxunlock(__iar_Rmtx *mutex) /* Unlock a system lock */
{
  osMutexRelease(*(osMutexId*)*mutex);
}

void __iar_file_Mtxinit(__iar_Rmtx *mutex)/*Initialize a file lock */
{
    osMutexDef_t def;
    uint32_t index;
    for (index = 0; index < _FOPEN_MAX; index++) {
      if (0 == std_mutex_id_file[index]) {
        def.mutex = &std_mutex_file[index];
        std_mutex_id_file[index] = osMutexCreate(&def);
        *mutex = (__iar_Rmtx*)&std_mutex_id_file[index];
        return;
      }
    }
    // The variable _FOPEN_MAX needs to be increased
    error("Not enough mutexes\n");
}

void __iar_file_Mtxdst(__iar_Rmtx *mutex) /* Destroy a file lock */
{
  osMutexDelete(*(osMutexId*)*mutex);
  *mutex = 0;
}

void __iar_file_Mtxlock(__iar_Rmtx *mutex) /* Lock a file lock */
{
  osMutexWait(*(osMutexId*)*mutex, osWaitForever);
}

void __iar_file_Mtxunlock(__iar_Rmtx *mutex) /* Unlock a file lock */
{
  osMutexRelease(*(osMutexId*)*mutex);
}

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

#if defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832) || defined (TARGET_STM32F334R8) ||\
    defined(TARGET_STM32F070RB) || defined(TARGET_STM32F072RB) || \
    defined(TARGET_STM32F302R8) || defined(TARGET_STM32F303K8) || defined (TARGET_STM32F334C8) ||\
    defined(TARGET_STM32F103RB)
static uint32_t thread_stack_main[DEFAULT_STACK_SIZE / sizeof(uint32_t)];
#elif defined(TARGET_XDOT_L151CC)
static uint32_t thread_stack_main[DEFAULT_STACK_SIZE * 6 / sizeof(uint32_t)];
#else
static uint32_t thread_stack_main[DEFAULT_STACK_SIZE * 2 / sizeof(uint32_t)];
#endif
osThreadDef_t os_thread_def_main = {(os_pthread)pre_main, osPriorityNormal, 1U, sizeof(thread_stack_main), thread_stack_main};

/*
 * IAR Default Memory layout notes:
 * -Heap defined by "HEAP" region in .icf file
 * -Interrupt stack defined by "CSTACK" region in .icf file
 * -Value INITIAL_SP is ignored
 *
 * IAR Custom Memory layout notes:
 * -There is no custom layout available for IAR - everything must be defined in
 *      the .icf file and use the default layout
 *
 *
 * GCC Default Memory layout notes:
 * -Block of memory from symbol __end__ to define INITIAL_SP used to setup interrupt
 *      stack and heap in the function set_stack_heap()
 * -ISR_STACK_SIZE can be overridden to be larger or smaller
 *
 * GCC Custom Memory layout notes:
 * -Heap can be explicitly placed by defining both HEAP_START and HEAP_SIZE
 * -Interrupt stack can be explicitly placed by defining both ISR_STACK_START and ISR_STACK_SIZE
 *
 *
 * ARM Memory layout
 * -Block of memory from end of region "RW_IRAM1" to define INITIAL_SP used to setup interrupt
 *      stack and heap in the function set_stack_heap()
 * -ISR_STACK_SIZE can be overridden to be larger or smaller
 *
 * ARM Custom Memory layout notes:
 * -Heap can be explicitly placed by defining both HEAP_START and HEAP_SIZE
 * -Interrupt stack can be explicitly placed by defining both ISR_STACK_START and ISR_STACK_SIZE
 *
 */

extern unsigned char *mbed_heap_start;
extern uint32_t mbed_heap_size;

unsigned char *mbed_stack_isr_start = 0;
uint32_t mbed_stack_isr_size = 0;

/*
 * Sanity check values
 */
#if defined(__ICCARM__) &&                                  \
    (defined(HEAP_START) || defined(HEAP_SIZE) ||           \
     defined(ISR_STACK_START) && defined(ISR_STACK_SIZE))
    #error "No custom layout allowed for IAR. Use .icf file instead"
#endif
#if defined(HEAP_START) && !defined(HEAP_SIZE)
    #error "HEAP_SIZE must be defined if HEAP_START is defined"
#endif
#if defined(ISR_STACK_START) && !defined(ISR_STACK_SIZE)
    #error "ISR_STACK_SIZE must be defined if ISR_STACK_START is defined"
#endif
#if defined(HEAP_SIZE) && !defined(HEAP_START)
    #error "HEAP_START must be defined if HEAP_SIZE is defined"
#endif

/* Interrupt stack and heap always defined for IAR
 * Main thread defined here
 */
#if defined(__ICCARM__)
    #pragma section="CSTACK"
    #pragma section="HEAP"
    #define HEAP_START          ((unsigned char*)__section_begin("HEAP"))
    #define HEAP_SIZE           ((uint32_t)__section_size("HEAP"))
    #define ISR_STACK_START     ((unsigned char*)__section_begin("CSTACK"))
    #define ISR_STACK_SIZE      ((uint32_t)__section_size("CSTACK"))
#endif

#if !defined(INITIAL_SP) && !defined(HEAP_START)
    #error "no target defined"
#endif

/* Define heap region if it has not been defined already */
#if !defined(HEAP_START)
    #if defined(__ICCARM__)
        #error "Heap should already be defined for IAR"
    #elif defined(__CC_ARM)
        extern uint32_t          Image$$RW_IRAM1$$ZI$$Limit[];
        #define HEAP_START      ((unsigned char*)Image$$RW_IRAM1$$ZI$$Limit)
        #define HEAP_SIZE       ((uint32_t)((uint32_t)INITIAL_SP - (uint32_t)HEAP_START))
    #elif defined(__GNUC__)
        extern uint32_t         __end__[];
        #define HEAP_START      ((unsigned char*)__end__)
        #define HEAP_SIZE       ((uint32_t)((uint32_t)INITIAL_SP - (uint32_t)HEAP_START))
    #endif
#endif

/* Define stack sizes if they haven't been set already */
#if !defined(ISR_STACK_SIZE)
    #define ISR_STACK_SIZE ((uint32_t)OS_MAINSTKSIZE * 4)
#endif

/*
 * set_stack_heap purpose is to set the following variables:
 * -mbed_heap_start
 * -mbed_heap_size
 * -mbed_stack_isr_start
 * -mbed_stack_isr_size
 *
 * Along with setting up os_thread_def_main
 */
void set_stack_heap(void) {

    unsigned char *free_start = HEAP_START;
    uint32_t free_size = HEAP_SIZE;

#ifdef ISR_STACK_START
    /* Interrupt stack explicitly specified */
    mbed_stack_isr_size = ISR_STACK_SIZE;
    mbed_stack_isr_start = ISR_STACK_START;
#else
    /* Interrupt stack -  reserve space at the end of the free block */
    mbed_stack_isr_size = ISR_STACK_SIZE < free_size ? ISR_STACK_SIZE : free_size;
    mbed_stack_isr_start = free_start + free_size - mbed_stack_isr_size;
    free_size -= mbed_stack_isr_size;
#endif

    /* Heap - everything else */
    mbed_heap_size = free_size;
    mbed_heap_start = free_start;
}

#if defined (__CC_ARM)

#ifdef __MICROLIB

int main(void);
void _main_init (void) __attribute__((section(".ARM.Collect$$$$000000FF")));
void $Super$$__cpp_initialize__aeabi_(void);

void _main_init (void) {
  osKernelInitialize();
#ifdef __MBED_CMSIS_RTOS_CM
  set_stack_heap();
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
  singleton_mutex_id = osMutexCreate(osMutex(singleton_mutex));
  $Super$$__cpp_initialize__aeabi_();
  main();
}

#else

int main(void);

void pre_main (void)
{
    singleton_mutex_id = osMutexCreate(osMutex(singleton_mutex));
    __rt_lib_init((unsigned)mbed_heap_start, (unsigned)(mbed_heap_start + mbed_heap_size));
    main();
}

/* The single memory model is checking for stack collision at run time, verifing
   that the heap pointer is underneath the stack pointer.

   With the RTOS there is not only one stack above the heap, there are multiple
   stacks and some of them are underneath the heap pointer.
*/
#pragma import(__use_two_region_memory)

__asm void __rt_entry (void) {

  IMPORT  __user_setup_stackheap
  IMPORT  _platform_post_stackheap_init
  IMPORT  os_thread_def_main
  IMPORT  osKernelInitialize
#ifdef __MBED_CMSIS_RTOS_CM
  IMPORT  set_stack_heap
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
  /* Ignore return value of __user_setup_stackheap since
   * this will be setup by set_stack_heap
   */
  BL      _platform_post_stackheap_init
  BL      osKernelInitialize
#ifdef __MBED_CMSIS_RTOS_CM
  BL      set_stack_heap
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

osMutexDef(malloc_mutex);
static osMutexId malloc_mutex_id;
osMutexDef(env_mutex);
static osMutexId env_mutex_id;

extern int atexit(void (*func)(void));
extern void __libc_fini_array(void);
extern void __libc_init_array (void);
extern int main(int argc, char **argv);

void pre_main(void) {
    singleton_mutex_id = osMutexCreate(osMutex(singleton_mutex));
    malloc_mutex_id = osMutexCreate(osMutex(malloc_mutex));
    env_mutex_id = osMutexCreate(osMutex(env_mutex));
    __libc_init_array();
    main(0, NULL);
}

__attribute__((naked)) void software_init_hook_rtos (void) {
  __asm (
    "bl   osKernelInitialize\n"
#ifdef __MBED_CMSIS_RTOS_CM
    "bl   set_stack_heap\n"
#endif
    "ldr  r0,=os_thread_def_main\n"
    "movs r1,#0\n"
    "bl   osThreadCreate\n"
    "bl   osKernelStart\n"
    /* osKernelStart should not return */
    "B       .\n"
  );
}

// Opaque declaration of _reent structure
struct _reent;

void __rtos_malloc_lock( struct _reent *_r )
{
    osMutexWait(malloc_mutex_id, osWaitForever);
}

void __rtos_malloc_unlock( struct _reent *_r )
{
    osMutexRelease(malloc_mutex_id);
}

void __rtos_env_lock( struct _reent *_r )
{
    osMutexWait(env_mutex_id, osWaitForever);
}

void __rtos_env_unlock( struct _reent *_r )
{
    osMutexRelease(env_mutex_id);
}

#elif defined (__ICCARM__)

extern void* __vector_table;
extern int  __low_level_init(void);
extern void __iar_data_init3(void);
extern __weak void __iar_init_core( void );
extern __weak void __iar_init_vfp( void );
extern void __iar_dynamic_initialization(void);
extern void mbed_sdk_init(void);
extern void mbed_main(void);
extern int main(void);
extern void exit(int arg);

static uint8_t low_level_init_needed;

void pre_main(void) {
    singleton_mutex_id = osMutexCreate(osMutex(singleton_mutex));
    if (low_level_init_needed) {
        __iar_dynamic_initialization();
    }
    mbed_main();
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
  set_stack_heap();
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

/** @}*/
