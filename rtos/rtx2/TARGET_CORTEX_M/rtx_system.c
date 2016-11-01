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
 * Title:       System functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Helper functions ====

/// Put Object into ISR Queue.
/// \param[in]  object          object.
/// \return 1 - success, 0 - failure.
static uint32_t os_isr_queue_put (void *object) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#else
  uint32_t n;
#endif
  uint16_t max;
  uint32_t ret;

  max = os_Info.isr_queue.max;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  if (os_Info.isr_queue.cnt < max) {
    os_Info.isr_queue.cnt++;
    os_Info.isr_queue.data[os_Info.isr_queue.in] = object;
    if (++os_Info.isr_queue.in == max) {
      os_Info.isr_queue.in = 0U;
    }
    ret = 1U;
  } else {
    ret = 0U;
  }
  
  if (primask == 0U) {
    __enable_irq();
  }
#else
  if (os_exc_inc16_lt(&os_Info.isr_queue.cnt, max) < max) {
    n = os_exc_inc16_lim(&os_Info.isr_queue.in, max);
    os_Info.isr_queue.data[n] = object;
    ret = 1U;
  } else {
    ret = 0U;
  }
#endif

  return ret;
}

/// Get Object from ISR Queue.
/// \return object or NULL.
static void *os_isr_queue_get (void) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#else
  uint32_t n;
#endif
  uint16_t max;
  void    *ret;

  max = os_Info.isr_queue.max;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  if (os_Info.isr_queue.cnt != 0U) {
    os_Info.isr_queue.cnt--;
    ret = os_Info.isr_queue.data[os_Info.isr_queue.out];
    if (++os_Info.isr_queue.out == max) {
      os_Info.isr_queue.out = 0U;
    }
  } else {
    ret = NULL;
  }
  
  if (primask == 0U) {
    __enable_irq();
  }
#else
  if (os_exc_dec16_nz(&os_Info.isr_queue.cnt) != 0U) {
    n = os_exc_inc16_lim(&os_Info.isr_queue.out, max);
    ret = os_Info.isr_queue.data[n];
  } else {
    ret = NULL;
  }
#endif

  return ret;
}


//  ==== Library Functions ====

/// Tick Handler.
void os_Tick_Handler (void) {
  os_thread_t *thread;

  os_SysTimerAckIRQ();
  os_Info.kernel.tick++;

  // Process Thread Delays
  os_ThreadDelayTick();

  // Process Timers
  os_TimerTick();

  os_ThreadDispatch(NULL);

  // Check Round Robin timeout
  if (os_Info.thread.robin.timeout != 0U) {
    if (os_Info.thread.robin.thread != os_Info.thread.run.next) {
      // Reset Round Robin
      os_Info.thread.robin.thread = os_Info.thread.run.next;
      os_Info.thread.robin.tick   = os_Info.thread.robin.timeout;
    } else {
      if (os_Info.thread.robin.tick != 0U) {
        os_Info.thread.robin.tick--;
      }
      if (os_Info.thread.robin.tick == 0U) {
        // Round Robin Timeout
        if (os_KernelGetState() == os_KernelRunning) {
          thread = os_Info.thread.ready.thread_list;
          if ((thread != NULL) && (thread->priority == os_Info.thread.robin.thread->priority)) {
            os_ThreadListRemove(thread);
            os_ThreadReadyPut(os_Info.thread.robin.thread);
            os_ThreadSwitch(thread);
            os_Info.thread.robin.thread = thread;
            os_Info.thread.robin.tick   = os_Info.thread.robin.timeout;
          }
        }
      }
    }
  }
}

/// Pending Service Call Handler.
void os_PendSV_Handler (void) {
  os_object_t *object;

  for (;;) {
    object = os_isr_queue_get();
    if (object == NULL) {
      break;
    }
    switch (object->id) {
      case os_IdThread:
        os_Info.post_process.thread((os_thread_t *)object);
        break;
      case os_IdEventFlags:
        os_Info.post_process.event_flags((os_event_flags_t *)object);
        break;
      case os_IdSemaphore:
        os_Info.post_process.semaphore((os_semaphore_t *)object);
        break;
      case os_IdMemoryPool:
        os_Info.post_process.memory_pool((os_memory_pool_t *)object);
        break;
      case os_IdMessage:
        os_Info.post_process.message_queue((os_message_t *)object);
        break;
      default:
        break;
    }
  }

  os_ThreadDispatch(NULL);
}

/// Register post ISR processing.
/// \param[in]  object          generic object.
void os_PostProcess (os_object_t *object) {

  if (os_isr_queue_put(object) != 0U) {
    if (os_Info.kernel.blocked == 0U) {
      os_SetPendSV();
    } else {
      os_Info.kernel.pendSV = 1U;
    }
  } else {
    os_Error(os_ErrorISRQueueOverflow, object);
  }
}


//  ==== Public API ====

/// Setup System Timer.
__WEAK int32_t os_SysTimerSetup (void) {

  // Setup SysTick Timer
  os_SysTick_Setup(os_Info.kernel.sys_freq / os_Config.tick_freq);

  return SysTick_IRQn;                  // Return IRQ number of SysTick
}

/// Enable System Timer.
__WEAK void os_SysTimerEnable (void) {
  os_SysTick_Enable();
}

/// Disable System Timer.
__WEAK void os_SysTimerDisable (void) {
  os_SysTick_Disable();
}

/// Acknowledge System Timer IRQ.
__WEAK void os_SysTimerAckIRQ (void) {
  os_SysTick_GetOvf();
}

/// Get System Timer count.
__WEAK uint32_t os_SysTimerGetCount (void) {
  uint32_t tick;
  uint32_t val;

  tick = (uint32_t)os_Info.kernel.tick;
  val  = os_SysTick_GetVal();
  if (os_SysTick_GetOvf()) {
    val = os_SysTick_GetVal();
    tick++;
  }
  val += tick * os_SysTick_GetPeriod();

  return val;
}

/// Get System Timer frequency.
__WEAK uint32_t os_SysTimerGetFreq (void) {
  return os_Info.kernel.sys_freq;
}
