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
 * Title:       System functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Helper functions ====

/// Put Object into ISR Queue.
/// \param[in]  object          object.
/// \return 1 - success, 0 - failure.
static uint32_t isr_queue_put (void *object) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#else
  uint32_t n;
#endif
  uint16_t max;
  uint32_t ret;

  max = osRtxInfo.isr_queue.max;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  if (osRtxInfo.isr_queue.cnt < max) {
    osRtxInfo.isr_queue.cnt++;
    osRtxInfo.isr_queue.data[osRtxInfo.isr_queue.in] = object;
    if (++osRtxInfo.isr_queue.in == max) {
      osRtxInfo.isr_queue.in = 0U;
    }
    ret = 1U;
  } else {
    ret = 0U;
  }
  
  if (primask == 0U) {
    __enable_irq();
  }
#else
  if (atomic_inc16_lt(&osRtxInfo.isr_queue.cnt, max) < max) {
    n = atomic_inc16_lim(&osRtxInfo.isr_queue.in, max);
    osRtxInfo.isr_queue.data[n] = object;
    ret = 1U;
  } else {
    ret = 0U;
  }
#endif

  return ret;
}

/// Get Object from ISR Queue.
/// \return object or NULL.
static void *isr_queue_get (void) {
#if (__EXCLUSIVE_ACCESS == 0U)
  uint32_t primask = __get_PRIMASK();
#else
  uint32_t n;
#endif
  uint16_t max;
  void    *ret;

  max = osRtxInfo.isr_queue.max;

#if (__EXCLUSIVE_ACCESS == 0U)
  __disable_irq();

  if (osRtxInfo.isr_queue.cnt != 0U) {
    osRtxInfo.isr_queue.cnt--;
    ret = osRtxInfo.isr_queue.data[osRtxInfo.isr_queue.out];
    if (++osRtxInfo.isr_queue.out == max) {
      osRtxInfo.isr_queue.out = 0U;
    }
  } else {
    ret = NULL;
  }
  
  if (primask == 0U) {
    __enable_irq();
  }
#else
  if (atomic_dec16_nz(&osRtxInfo.isr_queue.cnt) != 0U) {
    n = atomic_inc16_lim(&osRtxInfo.isr_queue.out, max);
    ret = osRtxInfo.isr_queue.data[n];
  } else {
    ret = NULL;
  }
#endif

  return ret;
}


//  ==== Library Functions ====

/// Tick Handler.
void osRtxTick_Handler (void) {
  os_thread_t *thread;

  osRtxSysTimerAckIRQ();
  osRtxInfo.kernel.tick++;

  // Process Timers
  if (osRtxInfo.timer.tick != NULL) {
    osRtxInfo.timer.tick();
  }

  // Process Thread Delays
  osRtxThreadDelayTick();

  osRtxThreadDispatch(NULL);

  // Check Round Robin timeout
  if (osRtxInfo.thread.robin.timeout != 0U) {
    if (osRtxInfo.thread.robin.thread != osRtxInfo.thread.run.next) {
      // Reset Round Robin
      osRtxInfo.thread.robin.thread = osRtxInfo.thread.run.next;
      osRtxInfo.thread.robin.tick   = osRtxInfo.thread.robin.timeout;
    } else {
      if (osRtxInfo.thread.robin.tick != 0U) {
        osRtxInfo.thread.robin.tick--;
      }
      if (osRtxInfo.thread.robin.tick == 0U) {
        // Round Robin Timeout
        if (osRtxKernelGetState() == osRtxKernelRunning) {
          thread = osRtxInfo.thread.ready.thread_list;
          if ((thread != NULL) && (thread->priority == osRtxInfo.thread.robin.thread->priority)) {
            osRtxThreadListRemove(thread);
            osRtxThreadReadyPut(osRtxInfo.thread.robin.thread);
            osRtxThreadSwitch(thread);
            osRtxInfo.thread.robin.thread = thread;
            osRtxInfo.thread.robin.tick   = osRtxInfo.thread.robin.timeout;
          }
        }
      }
    }
  }
}

/// Pending Service Call Handler.
void osRtxPendSV_Handler (void) {
  os_object_t *object;

  for (;;) {
    object = isr_queue_get();
    if (object == NULL) {
      break;
    }
    switch (object->id) {
      case osRtxIdThread:
        osRtxInfo.post_process.thread((os_thread_t *)object);
        break;
      case osRtxIdEventFlags:
        osRtxInfo.post_process.event_flags((os_event_flags_t *)object);
        break;
      case osRtxIdSemaphore:
        osRtxInfo.post_process.semaphore((os_semaphore_t *)object);
        break;
      case osRtxIdMemoryPool:
        osRtxInfo.post_process.memory_pool((os_memory_pool_t *)object);
        break;
      case osRtxIdMessage:
        osRtxInfo.post_process.message_queue((os_message_t *)object);
        break;
      default:
        break;
    }
  }

  osRtxThreadDispatch(NULL);
}

/// Register post ISR processing.
/// \param[in]  object          generic object.
void osRtxPostProcess (os_object_t *object) {

  if (isr_queue_put(object) != 0U) {
    if (osRtxInfo.kernel.blocked == 0U) {
      SetPendSV();
    } else {
      osRtxInfo.kernel.pendSV = 1U;
    }
  } else {
    osRtxErrorNotify(osRtxErrorISRQueueOverflow, object);
  }
}


//  ==== Public API ====

/// Setup System Timer.
__WEAK int32_t osRtxSysTimerSetup (void) {

  // Setup SysTick Timer
  SysTick_Setup(osRtxInfo.kernel.sys_freq / osRtxConfig.tick_freq);

  return SysTick_IRQn;                  // Return IRQ number of SysTick
}

/// Enable System Timer.
__WEAK void osRtxSysTimerEnable (void) {
  SysTick_Enable();
}

/// Disable System Timer.
__WEAK void osRtxSysTimerDisable (void) {
  SysTick_Disable();
}

/// Acknowledge System Timer IRQ.
__WEAK void osRtxSysTimerAckIRQ (void) {
  SysTick_GetOvf();
}

/// Get System Timer count.
__WEAK uint32_t osRtxSysTimerGetCount (void) {
  uint32_t tick;
  uint32_t val;

  tick = (uint32_t)osRtxInfo.kernel.tick;
  val  = SysTick_GetVal();
  if (SysTick_GetOvf()) {
    val = SysTick_GetVal();
    tick++;
  }
  val += tick * SysTick_GetPeriod();

  return val;
}

/// Get System Timer frequency.
__WEAK uint32_t osRtxSysTimerGetFreq (void) {
  return osRtxInfo.kernel.sys_freq;
}
