
/** \addtogroup rtos */
/** @{*/
/* ----------------------------------------------------------------------
 * $Date:        5. February 2013
 * $Revision:    V1.02
 *
 * Project:      CMSIS-RTOS API
 * Title:        cmsis_os.h RTX header file
 *
 * Version 0.02
 *    Initial Proposal Phase
 * Version 0.03
 *    osKernelStart added, optional feature: main started as thread
 *    osSemaphores have standard behavior
 *    osTimerCreate does not start the timer, added osTimerStart
 *    osThreadPass is renamed to osThreadYield
 * Version 1.01
 *    Support for C++ interface
 *     - const attribute removed from the osXxxxDef_t typedef's
 *     - const attribute added to the osXxxxDef macros
 *    Added: osTimerDelete, osMutexDelete, osSemaphoreDelete
 *    Added: osKernelInitialize
 * Version 1.02
 *    Control functions for short timeouts in microsecond resolution:
 *    Added: osKernelSysTick, osKernelSysTickFrequency, osKernelSysTickMicroSec
 *    Removed: osSignalGet 
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 2013 ARM LIMITED
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


#ifndef _CMSIS_OS_H
#define _CMSIS_OS_H

#define CMSIS_OS_RTX

// __MBED_CMSIS_RTOS_CM captures our changes to the RTX kernel
#ifndef __MBED_CMSIS_RTOS_CM
#define __MBED_CMSIS_RTOS_CM
#endif
// we use __CMSIS_RTOS version, which changes some API in the kernel
#ifndef __CMSIS_RTOS
#define __CMSIS_RTOS
#endif

// The stack space occupied is mainly dependent on the underling C standard library
#if defined(TOOLCHAIN_GCC) || defined(TOOLCHAIN_ARM_STD) || defined(TOOLCHAIN_IAR)
#    define WORDS_STACK_SIZE   512
#elif defined(TOOLCHAIN_ARM_MICRO)
#    define WORDS_STACK_SIZE   128
#endif

#ifdef __MBED_CMSIS_RTOS_CM

/* If os timers macro is set to 0, there's no timer thread created, therefore
 * main thread has tid 0x01  
 */
#if defined(OS_TIMERS) && (OS_TIMERS == 0)
#define MAIN_THREAD_ID 0x01
#else
#define MAIN_THREAD_ID 0x02
#endif
#endif

#if defined(TARGET_XDOT_L151CC)
#define DEFAULT_STACK_SIZE         (WORDS_STACK_SIZE/2)
#else
#define DEFAULT_STACK_SIZE         (WORDS_STACK_SIZE*4)
#endif

#define osCMSIS           0x10002U     ///< CMSIS-RTOS API version (main [31:16] .sub [15:0])

#define osCMSIS_RTX     ((4<<16)|80)   ///< RTOS identification and version (main [31:16] .sub [15:0])

#define osKernelSystemId "RTX V4.80"   ///< RTOS identification string


#define osFeature_MainThread   1       ///< main can be thread
#define osFeature_Pool         1       ///< Memory Pools available
#define osFeature_MailQ        1       ///< Mail Queues available
#define osFeature_MessageQ     1       ///< Message Queues available
#define osFeature_Signals      16      ///< 16 Signal Flags available per thread
#define osFeature_Semaphore    65535   ///< Maximum count for \ref osSemaphoreCreate function
#define osFeature_Wait         0       ///< osWait not available
#define osFeature_SysTick      1       ///< osKernelSysTick functions available
#define osFeature_ThreadEnum   1       ///< Thread enumeration available

#if defined (__CC_ARM)
#define os_InRegs __value_in_regs      // Compiler specific: force struct in registers
#elif defined (__ICCARM__)
#define os_InRegs __value_in_regs      // Compiler specific: force struct in registers
#else
#define os_InRegs
#endif

#include <stdint.h>
#include <stddef.h>

#ifdef  __cplusplus
extern "C"
{
#endif

// ==== Enumeration, structures, defines ====

/// Priority used for thread control.
typedef enum  {
  osPriorityIdle          = -3,          ///< priority: idle (lowest)
  osPriorityLow           = -2,          ///< priority: low
  osPriorityBelowNormal   = -1,          ///< priority: below normal
  osPriorityNormal        =  0,          ///< priority: normal (default)
  osPriorityAboveNormal   = +1,          ///< priority: above normal
  osPriorityHigh          = +2,          ///< priority: high
  osPriorityRealtime      = +3,          ///< priority: realtime (highest)
  osPriorityError         =  0x84        ///< system cannot determine priority or thread has illegal priority
} osPriority;

/// Timeout value.
#define osWaitForever     0xFFFFFFFFU    ///< wait forever timeout value

/// Status code values returned by CMSIS-RTOS functions.
typedef enum  {
  osOK                    =     0,       ///< function completed; no error or event occurred.
  osEventSignal           =  0x08,       ///< function completed; signal event occurred.
  osEventMessage          =  0x10,       ///< function completed; message event occurred.
  osEventMail             =  0x20,       ///< function completed; mail event occurred.
  osEventTimeout          =  0x40,       ///< function completed; timeout occurred.
  osErrorParameter        =  0x80,       ///< parameter error: a mandatory parameter was missing or specified an incorrect object.
  osErrorResource         =  0x81,       ///< resource not available: a specified resource was not available.
  osErrorTimeoutResource  =  0xC1,       ///< resource not available within given time: a specified resource was not available within the timeout period.
  osErrorISR              =  0x82,       ///< not allowed in ISR context: the function cannot be called from interrupt service routines.
  osErrorISRRecursive     =  0x83,       ///< function called multiple times from ISR with same object.
  osErrorPriority         =  0x84,       ///< system cannot determine priority or thread has illegal priority.
  osErrorNoMemory         =  0x85,       ///< system is out of memory: it was impossible to allocate or reserve memory for the operation.
  osErrorValue            =  0x86,       ///< value of a parameter is out of range.
  osErrorOS               =  0xFF,       ///< unspecified RTOS error: run-time error but no other error message fits.
  os_status_reserved      =  0x7FFFFFFF  ///< prevent from enum down-size compiler optimization.
} osStatus;


/// Timer type value for the timer definition.
typedef enum  {
  osTimerOnce             =     0,       ///< one-shot timer
  osTimerPeriodic         =     1        ///< repeating timer
} os_timer_type;

typedef enum {
  osThreadInfoState,
  osThreadInfoStackSize,
  osThreadInfoStackMax,
  osThreadInfoEntry,
  osThreadInfoArg,

  osThreadInfo_reserved   =  0x7FFFFFFF  ///< prevent from enum down-size compiler optimization.
} osThreadInfo;

/// Entry point of a thread.
typedef void (*os_pthread) (void const *argument);

/// Entry point of a timer call back function.
typedef void (*os_ptimer) (void const *argument);

// >>> the following data type definitions may shall adapted towards a specific RTOS

/// Thread ID identifies the thread (pointer to a thread control block).
typedef struct os_thread_cb *osThreadId;

/// Timer ID identifies the timer (pointer to a timer control block).
typedef struct os_timer_cb *osTimerId;

/// Mutex ID identifies the mutex (pointer to a mutex control block).
typedef struct os_mutex_cb *osMutexId;

/// Semaphore ID identifies the semaphore (pointer to a semaphore control block).
typedef struct os_semaphore_cb *osSemaphoreId;

/// Pool ID identifies the memory pool (pointer to a memory pool control block).
typedef struct os_pool_cb *osPoolId;

/// Message ID identifies the message queue (pointer to a message queue control block).
typedef struct os_messageQ_cb *osMessageQId;

/// Mail ID identifies the mail queue (pointer to a mail queue control block).
typedef struct os_mailQ_cb *osMailQId;

/// Thread enumeration ID identifies the enumeration (pointer to a thread enumeration control block).
typedef uint32_t *osThreadEnumId;

/// Thread Definition structure contains startup information of a thread.
typedef struct os_thread_def  {
  os_pthread               pthread;    ///< start address of thread function
  osPriority             tpriority;    ///< initial thread priority
  uint32_t               instances;    ///< maximum number of instances of that thread function
  uint32_t               stacksize;    ///< stack size requirements in bytes; 0 is default stack size
#ifdef __MBED_CMSIS_RTOS_CM
  uint32_t               *stack_pointer;  ///< pointer to the stack memory block
#endif
} osThreadDef_t;

/// Timer Definition structure contains timer parameters.
typedef struct os_timer_def  {
  os_ptimer                 ptimer;    ///< start address of a timer function
  void                      *timer;    ///< pointer to internal data
} osTimerDef_t;

/// Mutex Definition structure contains setup information for a mutex.
typedef struct os_mutex_def  {
  void                      *mutex;    ///< pointer to internal data
} osMutexDef_t;

/// Semaphore Definition structure contains setup information for a semaphore.
typedef struct os_semaphore_def  {
  void                  *semaphore;    ///< pointer to internal data
} osSemaphoreDef_t;

/// Definition structure for memory block allocation.
typedef struct os_pool_def  {
  uint32_t                 pool_sz;    ///< number of items (elements) in the pool
  uint32_t                 item_sz;    ///< size of an item
  void                       *pool;    ///< pointer to memory for pool
} osPoolDef_t;

/// Definition structure for message queue.
typedef struct os_messageQ_def  {
  uint32_t                queue_sz;    ///< number of elements in the queue
  void                       *pool;    ///< memory array for messages
} osMessageQDef_t;

/// Definition structure for mail queue.
typedef struct os_mailQ_def  {
  uint32_t                queue_sz;    ///< number of elements in the queue
  uint32_t                 item_sz;    ///< size of an item
  void                       *pool;    ///< memory array for mail
} osMailQDef_t;

/// Event structure contains detailed information about an event.
typedef struct  {
  osStatus                 status;     ///< status code: event or error information
  union  {
    uint32_t                    v;     ///< message as 32-bit value
    void                       *p;     ///< message or mail as void pointer
    int32_t               signals;     ///< signal flags
  } value;                             ///< event value
  union  {
    osMailQId             mail_id;     ///< mail id obtained by \ref osMailCreate
    osMessageQId       message_id;     ///< message id obtained by \ref osMessageCreate
  } def;                               ///< event definition
} osEvent;


//  ==== Kernel Control Functions ====

/// Initialize the RTOS Kernel for creating objects.
/// \return status code that indicates the execution status of the function.
osStatus osKernelInitialize (void);

/// Start the RTOS Kernel.
/// \return status code that indicates the execution status of the function.
osStatus osKernelStart (void);

/// Check if the RTOS kernel is already started.
/// \return 0 RTOS is not started, 1 RTOS is started.
int32_t osKernelRunning(void);

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))     // System Timer available

/// \cond INTERNAL_VARIABLES
extern uint32_t const os_tickfreq;
extern uint16_t const os_tickus_i;
extern uint16_t const os_tickus_f;
/// \endcond

/// Get the RTOS kernel system timer counter.
/// \return RTOS kernel system timer as 32-bit value 
uint32_t osKernelSysTick (void);

/// The RTOS kernel system timer frequency in Hz.
/// \note Reflects the system timer setting and is typically defined in a configuration file.
#define osKernelSysTickFrequency os_tickfreq

/// Convert a microseconds value to a RTOS kernel system timer value.
/// \param         microsec     time value in microseconds.
/// \return time value normalized to the \ref osKernelSysTickFrequency
/*
#define osKernelSysTickMicroSec(microsec) (((uint64_t)microsec * (osKernelSysTickFrequency)) / 1000000)
*/
#define osKernelSysTickMicroSec(microsec) ((microsec * os_tickus_i) + ((microsec * os_tickus_f) >> 16))

#endif    // System Timer available

//  ==== Thread Management ====

/// Create a Thread Definition with function, priority, and stack requirements.
/// \param         name         name of the thread function.
/// \param         priority     initial priority of the thread function.
/// \param         instances    number of possible thread instances.
/// \param         stacksz      stack size (in bytes) requirements for the thread function.
///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osThreadDef(name, priority, instances, stacksz)  \
extern const osThreadDef_t os_thread_def_##name
#else                            // define the object
#ifdef __MBED_CMSIS_RTOS_CM
#define osThreadDef(name, priority, stacksz)  \
uint32_t os_thread_def_stack_##name [stacksz / sizeof(uint32_t)]; \
const osThreadDef_t os_thread_def_##name = \
{ (name), (priority), 1, (stacksz), (os_thread_def_stack_##name) }
#else
#define osThreadDef(name, priority, instances, stacksz)  \
const osThreadDef_t os_thread_def_##name = \
{ (name), (priority), (instances), (stacksz)  }
#endif
#endif

/// Access a Thread definition.
/// \param         name          name of the thread definition object.
///       macro body is implementation specific in every CMSIS-RTOS.
#define osThread(name)  \
&os_thread_def_##name

/// Create a thread and add it to Active Threads and set it to state READY.
/// \param[in]     thread_def    thread definition referenced with \ref osThread.
/// \param[in]     argument      pointer that is passed to the thread function as start argument.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId osThreadCreate (const osThreadDef_t *thread_def, void *argument);

osThreadId osThreadContextCreate (const osThreadDef_t *thread_def, void *argument, void *context);

/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId osThreadGetId (void);

/// Terminate execution of a thread and remove it from Active Threads.
/// \param[in]     thread_id   thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus osThreadTerminate (osThreadId thread_id);

/// Pass control to next thread that is in state \b READY.
/// \return status code that indicates the execution status of the function.
osStatus osThreadYield (void);

/// Change priority of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     priority      new priority value for the thread function.
/// \return status code that indicates the execution status of the function.
osStatus osThreadSetPriority (osThreadId thread_id, osPriority priority);

/// Get current priority of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \return current priority value of the thread function.
osPriority osThreadGetPriority (osThreadId thread_id);

#ifdef __MBED_CMSIS_RTOS_CM
/// Get current thread state.
uint8_t osThreadGetState (osThreadId thread_id);
#endif

/// Get into from an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     info          information to read.
/// \return current state of the thread function.
/// \return requested info that includes the status code.
os_InRegs osEvent _osThreadGetInfo(osThreadId thread_id, osThreadInfo info);

//  ==== Generic Wait Functions ====

/// Wait for Timeout (Time Delay).
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue "Time delay" value
/// \return status code that indicates the execution status of the function.
osStatus osDelay (uint32_t millisec);

#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))     // Generic Wait available

/// Wait for Signal, Message, Mail, or Timeout.
/// \param[in] millisec          \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
/// \return event that contains signal, message, or mail information or error code.
os_InRegs osEvent osWait (uint32_t millisec);

#endif  // Generic Wait available


//  ==== Timer Management Functions ====
/// Define a Timer object.
/// \param         name          name of the timer object.
/// \param         function      name of the timer call back function.
#if defined (osObjectsExternal)  // object is external
#define osTimerDef(name, function)  \
extern const osTimerDef_t os_timer_def_##name
#else                            // define the object
#define osTimerDef(name, function)  \
uint32_t os_timer_cb_##name[6]; \
const osTimerDef_t os_timer_def_##name = \
{ (function), (os_timer_cb_##name) }
#endif

/// Access a Timer definition.
/// \param         name          name of the timer object.
#define osTimer(name) \
&os_timer_def_##name

/// Create a timer.
/// \param[in]     timer_def     timer object referenced with \ref osTimer.
/// \param[in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
/// \param[in]     argument      argument to the timer call back function.
/// \return timer ID for reference by other functions or NULL in case of error.
osTimerId osTimerCreate (const osTimerDef_t *timer_def, os_timer_type type, void *argument);

/// Start or restart a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue "Time delay" value of the timer.
/// \return status code that indicates the execution status of the function.
osStatus osTimerStart (osTimerId timer_id, uint32_t millisec);

/// Stop the timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \return status code that indicates the execution status of the function.
osStatus osTimerStop (osTimerId timer_id);

/// Delete a timer that was created by \ref osTimerCreate.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
/// \return status code that indicates the execution status of the function.
osStatus osTimerDelete (osTimerId timer_id);


//  ==== Signal Management ====

/// Set the specified Signal Flags of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     signals       specifies the signal flags of the thread that should be set.
/// \return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
int32_t osSignalSet (osThreadId thread_id, int32_t signals);

/// Clear the specified Signal Flags of an active thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
/// \param[in]     signals       specifies the signal flags of the thread that shall be cleared.
/// \return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters or call from ISR.
int32_t osSignalClear (osThreadId thread_id, int32_t signals);

/// Wait for one or more Signal Flags to become signaled for the current \b RUNNING thread.
/// \param[in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return event flag information or error code.
os_InRegs osEvent osSignalWait (int32_t signals, uint32_t millisec);


//  ==== Mutex Management ====

/// Define a Mutex.
/// \param         name          name of the mutex object.
#if defined (osObjectsExternal)  // object is external
#define osMutexDef(name)  \
extern const osMutexDef_t os_mutex_def_##name
#else                            // define the object
#define osMutexDef(name)  \
uint32_t os_mutex_cb_##name[4] = { 0 }; \
const osMutexDef_t os_mutex_def_##name = { (os_mutex_cb_##name) }
#endif

/// Access a Mutex definition.
/// \param         name          name of the mutex object.
#define osMutex(name)  \
&os_mutex_def_##name

/// Create and Initialize a Mutex object.
/// \param[in]     mutex_def     mutex definition referenced with \ref osMutex.
/// \return mutex ID for reference by other functions or NULL in case of error.
osMutexId osMutexCreate (const osMutexDef_t *mutex_def);

/// Wait until a Mutex becomes available.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus osMutexWait (osMutexId mutex_id, uint32_t millisec);

/// Release a Mutex that was obtained by \ref osMutexWait.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \return status code that indicates the execution status of the function.
osStatus osMutexRelease (osMutexId mutex_id);

/// Delete a Mutex that was created by \ref osMutexCreate.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
/// \return status code that indicates the execution status of the function.
osStatus osMutexDelete (osMutexId mutex_id);


//  ==== Semaphore Management Functions ====

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))     // Semaphore available

/// Define a Semaphore object.
/// \param         name          name of the semaphore object.
#if defined (osObjectsExternal)  // object is external
#define osSemaphoreDef(name)  \
extern const osSemaphoreDef_t os_semaphore_def_##name
#else                            // define the object
#define osSemaphoreDef(name)  \
uint32_t os_semaphore_cb_##name[2] = { 0 }; \
const osSemaphoreDef_t os_semaphore_def_##name = { (os_semaphore_cb_##name) }
#endif

/// Access a Semaphore definition.
/// \param         name          name of the semaphore object.
#define osSemaphore(name)  \
&os_semaphore_def_##name

/// Create and Initialize a Semaphore object used for managing resources.
/// \param[in]     semaphore_def semaphore definition referenced with \ref osSemaphore.
/// \param[in]     count         number of available resources.
/// \return semaphore ID for reference by other functions or NULL in case of error.
osSemaphoreId osSemaphoreCreate (const osSemaphoreDef_t *semaphore_def, int32_t count);

/// Wait until a Semaphore token becomes available.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return number of available tokens, or -1 in case of incorrect parameters.
int32_t osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec);

/// Release a Semaphore token.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \return status code that indicates the execution status of the function.
osStatus osSemaphoreRelease (osSemaphoreId semaphore_id);

/// Delete a Semaphore that was created by \ref osSemaphoreCreate.
/// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
/// \return status code that indicates the execution status of the function.
osStatus osSemaphoreDelete (osSemaphoreId semaphore_id);

#endif     // Semaphore available


//  ==== Memory Pool Management Functions ====

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))  // Memory Pool Management available

/// \brief Define a Memory Pool.
/// \param         name          name of the memory pool.
/// \param         no            maximum number of blocks (objects) in the memory pool.
/// \param         type          data type of a single block (object).
#if defined (osObjectsExternal)  // object is external
#define osPoolDef(name, no, type)   \
extern const osPoolDef_t os_pool_def_##name
#else                            // define the object
#define osPoolDef(name, no, type)   \
uint32_t os_pool_m_##name[3+((sizeof(type)+3)/4)*(no)]; \
const osPoolDef_t os_pool_def_##name = \
{ (no), sizeof(type), (os_pool_m_##name) }
#endif

/// \brief Access a Memory Pool definition.
/// \param         name          name of the memory pool
#define osPool(name) \
&os_pool_def_##name

/// Create and Initialize a memory pool.
/// \param[in]     pool_def      memory pool definition referenced with \ref osPool.
/// \return memory pool ID for reference by other functions or NULL in case of error.
osPoolId osPoolCreate (const osPoolDef_t *pool_def);

/// Allocate a memory block from a memory pool.
/// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
/// \return address of the allocated memory block or NULL in case of no memory available.
void *osPoolAlloc (osPoolId pool_id);

/// Allocate a memory block from a memory pool and set memory block to zero.
/// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
/// \return address of the allocated memory block or NULL in case of no memory available.
void *osPoolCAlloc (osPoolId pool_id);

/// Return an allocated memory block back to a specific memory pool.
/// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
/// \param[in]     block         address of the allocated memory block that is returned to the memory pool.
/// \return status code that indicates the execution status of the function.
osStatus osPoolFree (osPoolId pool_id, void *block);

#endif   // Memory Pool Management available


//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))     // Message Queues available

/// \brief Create a Message Queue Definition.
/// \param         name          name of the queue.
/// \param         queue_sz      maximum number of messages in the queue.
/// \param         type          data type of a single message element (for debugger).
#if defined (osObjectsExternal)  // object is external
#define osMessageQDef(name, queue_sz, type)   \
extern const osMessageQDef_t os_messageQ_def_##name
#else                            // define the object
#define osMessageQDef(name, queue_sz, type)   \
uint32_t os_messageQ_q_##name[4+(queue_sz)] = { 0 }; \
const osMessageQDef_t os_messageQ_def_##name = \
{ (queue_sz), (os_messageQ_q_##name) }
#endif

/// \brief Access a Message Queue Definition.
/// \param         name          name of the queue
#define osMessageQ(name) \
&os_messageQ_def_##name

/// Create and Initialize a Message Queue.
/// \param[in]     queue_def     queue definition referenced with \ref osMessageQ.
/// \param[in]     thread_id     thread ID (obtained by \ref osThreadCreate or \ref osThreadGetId) or NULL.
/// \return message queue ID for reference by other functions or NULL in case of error.
osMessageQId osMessageCreate (const osMessageQDef_t *queue_def, osThreadId thread_id);

/// Put a Message to a Queue.
/// \param[in]     queue_id      message queue ID obtained with \ref osMessageCreate.
/// \param[in]     info          message information.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec);

/// Get a Message or Wait for a Message from a Queue.
/// \param[in]     queue_id      message queue ID obtained with \ref osMessageCreate.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return event information that includes status code.
os_InRegs osEvent osMessageGet (osMessageQId queue_id, uint32_t millisec);

#endif     // Message Queues available


//  ==== Mail Queue Management Functions ====

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))     // Mail Queues available

/// \brief Create a Mail Queue Definition.
/// \param         name          name of the queue
/// \param         queue_sz      maximum number of messages in queue
/// \param         type          data type of a single message element
#if defined (osObjectsExternal)  // object is external
#define osMailQDef(name, queue_sz, type) \
extern const osMailQDef_t os_mailQ_def_##name
#else                            // define the object
#define osMailQDef(name, queue_sz, type) \
uint32_t os_mailQ_q_##name[4+(queue_sz)] = { 0 }; \
uint32_t os_mailQ_m_##name[3+((sizeof(type)+3)/4)*(queue_sz)]; \
void *   os_mailQ_p_##name[2] = { (os_mailQ_q_##name), os_mailQ_m_##name }; \
const osMailQDef_t os_mailQ_def_##name =  \
{ (queue_sz), sizeof(type), (os_mailQ_p_##name) }
#endif

/// \brief Access a Mail Queue Definition.
/// \param         name          name of the queue
#define osMailQ(name)  \
&os_mailQ_def_##name

/// Create and Initialize mail queue.
/// \param[in]     queue_def     reference to the mail queue definition obtain with \ref osMailQ
/// \param[in]     thread_id     thread ID (obtained by \ref osThreadCreate or \ref osThreadGetId) or NULL.
/// \return mail queue ID for reference by other functions or NULL in case of error.
osMailQId osMailCreate (const osMailQDef_t *queue_def, osThreadId thread_id);

/// Allocate a memory block from a mail.
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
/// \return pointer to memory block that can be filled with mail or NULL in case of error.
void *osMailAlloc (osMailQId queue_id, uint32_t millisec);

/// Allocate a memory block from a mail and set memory block to zero.
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
/// \return pointer to memory block that can be filled with mail or NULL in case of error.
void *osMailCAlloc (osMailQId queue_id, uint32_t millisec);

/// Put a mail to a queue.
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     mail          memory block previously allocated with \ref osMailAlloc or \ref osMailCAlloc.
/// \return status code that indicates the execution status of the function.
osStatus osMailPut (osMailQId queue_id, void *mail);

/// Get a mail from a queue.
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
/// \return event that contains mail information or error code.
os_InRegs osEvent osMailGet (osMailQId queue_id, uint32_t millisec);

/// Free a memory block from a mail.
/// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
/// \param[in]     mail          pointer to the memory block that was obtained with \ref osMailGet.
/// \return status code that indicates the execution status of the function.
osStatus osMailFree (osMailQId queue_id, void *mail);

#endif  // Mail Queues available


//  ==== Thread Enumeration Functions ====

#if (defined (osFeature_ThreadEnum)  &&  (osFeature_ThreadEnum != 0))     // Thread enumeration available

/// Start a thread enumeration.
/// \return an enumeration ID or NULL on error.
osThreadEnumId _osThreadsEnumStart(void);

/// Get the next task ID in the enumeration.
/// \return a thread ID or NULL on if the end of the enumeration has been reached.
osThreadId _osThreadEnumNext(osThreadEnumId enum_id);

/// Free the enumeration structure.
/// \param[in]     enum_id       pointer to the enumeration ID that was obtained with \ref _osThreadsEnumStart.
/// \return status code that indicates the execution status of the function.
osStatus _osThreadEnumFree(osThreadEnumId enum_id);

#endif  // Thread Enumeration available


//  ==== RTX Extensions ====

/// Suspend the RTX task scheduler.
/// \return number of ticks, for how long the system can sleep or power-down.
uint32_t os_suspend (void);

/// Resume the RTX task scheduler
/// \param[in]     sleep_time    specifies how long the system was in sleep or power-down mode.
void os_resume (uint32_t sleep_time);


#ifdef  __cplusplus
}
#endif

#endif  // _CMSIS_OS_H

/** @}*/
