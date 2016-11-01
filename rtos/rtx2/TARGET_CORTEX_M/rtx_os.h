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
 * Title:       RTX OS definitions
 *
 * -----------------------------------------------------------------------------
 */

#ifndef RTX_OS_H_
#define RTX_OS_H_

#include <stdint.h>
#include <stddef.h>
#include "cmsis_os2.h"

#ifdef  __cplusplus
extern "C"
{
#endif


/// Kernel Information
#define os_CMSIS_API         20000000   ///< API version (2.0.0)
#define os_CMSIS_RTX         50000000   ///< Kernel version (5.0.0)
#define os_KernelId       "RTX V5.0.0"  ///< Kernel identification string


//  ==== Common definitions ====

/// Object Identifier definitions
#define os_IdInvalid            0x00U
#define os_IdThread             0x01U
#define os_IdTimer              0x02U
#define os_IdEventFlags         0x03U
#define os_IdMutex              0x04U
#define os_IdSemaphore          0x05U
#define os_IdMemoryPool         0x06U
#define os_IdMessage            0x07U
#define os_IdMessageQueue       0x08U

/// Object State definitions (except for Threads and Timers)
#define os_ObjectInactive       0x00U
#define os_ObjectActive         0x01U

/// Object Flags definitions
#define os_FlagSystemObject     0x01U
#define os_FlagSystemMemory     0x02U


//  ==== Kernel definitions ====

/// Kernel State definitions
#define os_KernelInactive           ((uint8_t)osKernelInactive)
#define os_KernelReady              ((uint8_t)osKernelReady)
#define os_KernelRunning            ((uint8_t)osKernelRunning)
#define os_KernelLocked             ((uint8_t)osKernelLocked)
#define os_KernelSuspended          ((uint8_t)osKernelSuspended)


//  ==== Thread definitions ====

/// Thread State definitions (extending osThreadState)
#define os_ThreadStateMask          0x0FU

#define os_ThreadInactive           ((uint8_t)osThreadInactive)
#define os_ThreadReady              ((uint8_t)osThreadReady)
#define os_ThreadRunning            ((uint8_t)osThreadRunning)
#define os_ThreadBlocked            ((uint8_t)osThreadBlocked)
#define os_ThreadTerminated         ((uint8_t)osThreadTerminated)

#define os_ThreadWaitingDelay       (os_ThreadBlocked | 0x10U)
#define os_ThreadWaitingJoin        (os_ThreadBlocked | 0x20U)
#define os_ThreadWaitingThreadFlags (os_ThreadBlocked | 0x30U) 
#define os_ThreadWaitingEventFlags  (os_ThreadBlocked | 0x40U) 
#define os_ThreadWaitingMutex       (os_ThreadBlocked | 0x50U)
#define os_ThreadWaitingSemaphore   (os_ThreadBlocked | 0x60U)
#define os_ThreadWaitingMemoryPool  (os_ThreadBlocked | 0x70U)
#define os_ThreadWaitingMessageGet  (os_ThreadBlocked | 0x80U)
#define os_ThreadWaitingMessagePut  (os_ThreadBlocked | 0x90U)

/// Thread Flags definitions
#define os_ThreadFlagDefStack   0x10U   ///< Default Stack flag

/// Stack Marker definitions
#define os_StackMagicWord       0xE25A2EA5U ///< Stack Magic Word (Stack Base)
#define os_StackFillPattern     0xCCCCCCCCU ///< Stack Fill Pattern 

/// Thread Control Block
typedef struct os_thread_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                        attr;  ///< Object Attributes
  const char                    *name;  ///< Object Name
  struct os_thread_s     *thread_next;  ///< Link pointer to next Thread in Object list
  struct os_thread_s     *thread_prev;  ///< Link pointer to previous Thread in Object list
  struct os_thread_s      *delay_next;  ///< Link pointer to next Thread in Delay list
  struct os_thread_s      *delay_prev;  ///< Link pointer to previous Thread in Delay list
  struct os_thread_s     *thread_join;  ///< Thread waiting to Join
  uint32_t                      delay;  ///< Delay Time
  int8_t                     priority;  ///< Thread Priority
  int8_t                priority_base;  ///< Base Priority
  uint8_t                 stack_frame;  ///< Stack Frame (EXC_RETURN[7..0])
  uint8_t               flags_options;  ///< Thread/Event Flags Options
  int32_t                  wait_flags;  ///< Waiting Thread/Event Flags
  int32_t                thread_flags;  ///< Thread Flags
  struct os_mutex_s       *mutex_list;  ///< Link pointer to list of owned Mutexes
  void                     *stack_mem;  ///< Stack Memory
  uint32_t                 stack_size;  ///< Stack Size
  uint32_t                         sp;  ///< Current Stack Pointer
  uint32_t                  tz_memory;  ///< TrustZone Memory Identifier
} os_thread_t;


//  ==== Timer definitions ====

/// Timer State definitions
#define os_TimerInactive        0x00U   ///< Timer Inactive
#define os_TimerStopped         0x01U   ///< Timer Stopped
#define os_TimerRunning         0x02U   ///< Timer Running

/// Timer Type definitions
#define os_TimerPeriodic        ((uint8_t)osTimerPeriodic)

/// Timer Function Information
typedef struct {
  void                            *fp;  ///< Function Pointer
  void                           *arg;  ///< Function Argument
} os_timer_finfo_t;

/// Timer Control Block
typedef struct os_timer_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                        type;  ///< Timer Type (Periodic/One-shot)
  const char                    *name;  ///< Object Name
  struct os_timer_s             *prev;  ///< Pointer to previous active Timer
  struct os_timer_s             *next;  ///< Pointer to next active Timer
  uint32_t                       tick;  ///< Timer current Tick
  uint32_t                       load;  ///< Timer Load value
  os_timer_finfo_t              finfo;  ///< Timer Function Info
} os_timer_t;


//  ==== Event Flags definitions ====

/// Event Flags Control Block
typedef struct os_event_flags_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                    reserved;
  const char                    *name;  ///< Object Name
  os_thread_t            *thread_list;  ///< Waiting Threads List
  int32_t                 event_flags;  ///< Event Flags
} os_event_flags_t;


//  ==== Mutex definitions ====

/// Mutex Control Block
typedef struct os_mutex_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                        attr;  ///< Object Attributes
  const char                    *name;  ///< Object Name
  os_thread_t            *thread_list;  ///< Waiting Threads List
  os_thread_t           *owner_thread;  ///< Owner Thread
  struct os_mutex_s       *owner_prev;  ///< Pointer to previous owned Mutex
  struct os_mutex_s       *owner_next;  ///< Pointer to next owned Mutex
  uint8_t                        lock;  ///< Lock counter
  uint8_t                  padding[3];
} os_mutex_t;


//  ==== Semaphore definitions ====

/// Semaphore Control Block
typedef struct os_semaphore_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                    reserved;
  const char                    *name;  ///< Object Name
  os_thread_t            *thread_list;  ///< Waiting Threads List
  uint16_t                     tokens;  ///< Current number of tokens
  uint16_t                 max_tokens;  ///< Maximum number of tokens
} os_semaphore_t;


//  ==== Memory Pool definitions ====

/// Memory Pool Information
typedef struct os_mp_info_s {
  uint32_t                 max_blocks;  ///< Maximum number of Blocks
  uint32_t                used_blocks;  ///< Number of used Blocks
  uint32_t                 block_size;  ///< Block Size
  void                    *block_base;  ///< Block Memory Base Address
  void                     *block_lim;  ///< Block Memory Limit Address
  void                    *block_free;  ///< First free Block Address
} os_mp_info_t;

/// Memory Pool Control Block
typedef struct os_memory_pool_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                    reserved;
  const char                    *name;  ///< Object Name
  os_thread_t            *thread_list;  ///< Waiting Threads List
  os_mp_info_t                mp_info;  ///< Memory Pool Info
} os_memory_pool_t;


//  ==== Message Queue definitions ====

/// Message Control Block
typedef struct os_message_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                    priority;  ///< Message Priority
  struct os_message_s           *prev;  ///< Pointer to previous Message
  struct os_message_s           *next;  ///< Pointer to next Message
} os_message_t;

/// Message Queue Control Block
typedef struct os_message_queue_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                    reserved;
  const char                    *name;  ///< Object Name
  os_thread_t            *thread_list;  ///< Waiting Threads List
  os_mp_info_t                mp_info;  ///< Memory Pool Info
  uint32_t                   msg_size;  ///< Message Size
  uint32_t                  msg_count;  ///< Number of queued Messages
  os_message_t             *msg_first;  ///< Pointer to first Message
  os_message_t              *msg_last;  ///< Pointer to last Message
} os_message_queue_t;


//  ==== Generic Object definitions ====

/// Generic Object Control Block
typedef struct os_object_s {
  uint8_t                          id;  ///< Object Identifier
  uint8_t                       state;  ///< Object State
  uint8_t                       flags;  ///< Object Flags
  uint8_t                    reserved;
  const char                    *name;  ///< Object Name
  os_thread_t            *thread_list;  ///< Threads List
} os_object_t;


//  ==== OS Runtime Information definitions ====

/// OS Runtime Information structure
typedef struct {
  const char                   *os_id;  ///< OS Identification
  uint32_t                    version;  ///< OS Version
  struct {                              ///< Kernel Info
    uint8_t                     state;  ///< State
    volatile uint8_t          blocked;  ///< Blocked
    uint8_t                   pendISR;  ///< Pending ISR (SV and SysTick)
    uint8_t                    pendSV;  ///< Pending SV
    uint32_t                 sys_freq;  ///< System Frequency
    uint64_t                     tick;  ///< Tick counter
  } kernel;
  int32_t                   tick_irqn;  ///< Tick Timer IRQ Number
  struct {                              ///< Thread Info
    struct {                            ///< Thread Run Info
      os_thread_t               *curr;  ///< Current running Thread
      os_thread_t               *next;  ///< Next Thread to Run
    } run;
    volatile os_object_t        ready;  ///< Ready List Object
    os_thread_t                 *idle;  ///< Idle Thread
    os_thread_t           *delay_list;  ///< Delay List
    os_thread_t            *wait_list;  ///< Wait List (no Timeout)
    os_thread_t       *terminate_list;  ///< Terminate Thread List
    struct {                            ///< Thread Round Robin Info
      os_thread_t             *thread;  ///< Round Robin Thread
      uint32_t                   tick;  ///< Round Robin Time Tick
      uint32_t                timeout;  ///< Round Robin Timeout
    } robin;
  } thread;
  struct {                              ///< Timer Info
    os_timer_t                  *list;  ///< Active Timer List
    os_thread_t               *thread;  ///< Timer Thread
    os_message_queue_t            *mq;  ///< Timer Message Queue
  } timer;
  struct {                              ///< ISR Post Processing Queue
    uint16_t                      max;  ///< Maximum Items
    uint16_t                      cnt;  ///< Item Count
    uint16_t                       in;  ///< Incoming Item Index
    uint16_t                      out;  ///< Outgoing Item Index
    void                       **data;  ///< Queue Data
  } isr_queue;
  struct {                                  ///< ISR Post Processing functions
    void           (*thread)(os_thread_t*); ///< Thread Post Processing function
    void (*event_flags)(os_event_flags_t*); ///< Event Flags Post Processing function
    void     (*semaphore)(os_semaphore_t*); ///< Semaphore Post Processing function
    void (*memory_pool)(os_memory_pool_t*); ///< Memory Pool Post Processing function
    void   (*message_queue)(os_message_t*); ///< Message Queue Post Processing function
  } post_process;
  struct {                              ///< Memory Pools (Variable Block Size)
    void                       *stack;  ///< Stack Memory
    void                     *mp_data;  ///< Memory Pool Data Memory
    void                     *mq_data;  ///< Message Queue Data Memory
    void                      *common;  ///< Common Memory
  } mem;
  struct {                              ///< Memory Pools (Fixed Block Size)
    os_mp_info_t               *stack;  ///< Stack for Threads
    os_mp_info_t              *thread;  ///< Thread Control Blocks
    os_mp_info_t               *timer;  ///< Timer Control Blocks
    os_mp_info_t         *event_flags;  ///< Event Flags Control Blocks
    os_mp_info_t               *mutex;  ///< Mutex Control Blocks
    os_mp_info_t           *semaphore;  ///< Semaphore Control Blocks
    os_mp_info_t         *memory_pool;  ///< Memory Pool Control Blocks
    os_mp_info_t       *message_queue;  ///< Message Queue Control Blocks
  } mpi;
} os_info_t;

extern os_info_t os_Info;               ///< OS Runtime Information


//  ==== OS API definitions ====

/// Object Limits definitions
#define os_ThreadFlagsLimit     31U     ///< number of Thread Flags available per thread
#define os_EventFlagsLimit      31U     ///< number of Event Flags available per object
#define os_MutexLockLimit       255U    ///< maximum number of recursive mutex locks
#define os_SemaphoreTokenLimit  65535U  ///< maximum number of tokens per semaphore

/// Control Block sizes
#define os_ThreadCbSize         sizeof(os_thread_t)
#define os_TimerCbSize          sizeof(os_timer_t)
#define os_EventFlagsCbSize     sizeof(os_event_flags_t)
#define os_MutexCbSize          sizeof(os_mutex_t)
#define os_SemaphoreCbSize      sizeof(os_semaphore_t)
#define os_MemoryPoolCbSize     sizeof(os_memory_pool_t)
#define os_MessageQueueCbSize   sizeof(os_message_queue_t)

/// Memory size in bytes for Memory Pool storage.
/// \param         block_count   maximum number of memory blocks in memory pool.
/// \param         block_size    memory block size in bytes.
#define os_MemoryPoolMemSize(block_count, block_size) \
  (4*(block_count)*(((block_size)+3)/4))
 
/// Memory size in bytes for Message Queue storage.
/// \param         msg_count     maximum number of messages in queue.
/// \param         msg_size      maximum message size in bytes.
#define os_MessageQueueMemSize(msg_count, msg_size) \
  (4*(msg_count)*(3+(((msg_size)+3)/4)))


//  ==== OS External Functions ====

/// OS Error Codes
#define os_ErrorStackUnderflow      1U
#define os_ErrorISRQueueOverflow    2U
#define os_ErrorTimerQueueOverflow  3U
#define os_ErrorClibSpace           4U
#define os_ErrorClibMutex           5U

/// OS Error Callback function
extern uint32_t os_Error (uint32_t code, void *object_id);

/// OS Idle Thread
extern void os_IdleThread (void *argument);

/// OS Exception handlers
extern void SVC_Handler     (void);
extern void PendSV_Handler  (void);
extern void SysTick_Handler (void);


/// OS System Timer functions (default implementation uses SysTick)

/// Setup System Timer.
/// \return system timer IRQ number.
extern int32_t os_SysTimerSetup (void);

/// Enable System Timer.
extern void os_SysTimerEnable (void);

/// Disable System Timer.
extern void os_SysTimerDisable (void);

/// Acknowledge System Timer IRQ.
extern void os_SysTimerAckIRQ (void);

/// Get System Timer count.
/// \return system timer count.
extern uint32_t os_SysTimerGetCount (void);

/// Get System Timer frequency.
/// \return system timer frequency.
extern uint32_t os_SysTimerGetFreq (void);


//  ==== OS External Configuration ====

/// OS Configuration flags
#define os_ConfigPrivilegedMode     (1UL<<0)    ///< Threads in Privileged mode
#define os_ConfigStackCheck         (1UL<<1)    ///< Stack overrun checking
#define os_ConfigStackWatermark     (1UL<<2)    ///< Stack usage Watermark

/// OS Configuration structure
typedef struct {
  uint32_t                             flags;   ///< OS Configuration Flags
  uint32_t                         tick_freq;   ///< Kernel Tick Frequency
  uint32_t                     robin_timeout;   ///< Round Robin Timeout Tick
  struct {                                      ///< ISR Post Processing Queue
    void                              **data;   ///< Queue Data
    uint16_t                             max;   ///< Maximum Items
    uint16_t                         padding;
  } isr_queue;
  struct {                                      ///< Memory Pools (Variable Block Size)
    void                         *stack_addr;   ///< Stack Memory Address
    uint32_t                      stack_size;   ///< Stack Memory Size
    void                       *mp_data_addr;   ///< Memory Pool Memory Address
    uint32_t                    mp_data_size;   ///< Memory Pool Memory Size
    void                       *mq_data_addr;   ///< Message Queue Data Memory Address
    uint32_t                    mq_data_size;   ///< Message Queue Data Memory Size
    void                        *common_addr;   ///< Common Memory Address
    uint32_t                     common_size;   ///< Common Memory Size
  } mem;
  struct {                                      ///< Memory Pools (Fixed Block Size)
    os_mp_info_t                      *stack;   ///< Stack for Threads
    os_mp_info_t                     *thread;   ///< Thread Control Blocks
    os_mp_info_t                      *timer;   ///< Timer Control Blocks
    os_mp_info_t                *event_flags;   ///< Event Flags Control Blocks
    os_mp_info_t                      *mutex;   ///< Mutex Control Blocks
    os_mp_info_t                  *semaphore;   ///< Semaphore Control Blocks
    os_mp_info_t                *memory_pool;   ///< Memory Pool Control Blocks
    os_mp_info_t              *message_queue;   ///< Message Queue Control Blocks
  } mpi;
  uint32_t                 thread_stack_size;   ///< Default Thread Stack Size
  const 
  osThreadAttr_t           *idle_thread_attr;   ///< Idle Thread Attributes
  const
  osThreadAttr_t          *timer_thread_attr;   ///< Timer Thread Attributes
  const
  osMessageQueueAttr_t        *timer_mq_attr;   ///< Timer Message Queue Attributes
  uint32_t                     timer_mq_mcnt;   ///< Timer Message Queue maximum Messages
} os_config_t;

extern const os_config_t os_Config;             ///< OS Configuration


#ifdef  __cplusplus
}
#endif

#endif  // RTX_OS_H_

/** @}*/
