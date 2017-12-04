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
 * Title:       Timer functions
 *
 * -----------------------------------------------------------------------------
 */

#include "rtx_lib.h"


//  ==== Helper functions ====

/// Insert Timer into the Timer List sorted by Time.
/// \param[in]  timer           timer object.
/// \param[in]  tick            timer tick.
static void TimerInsert (os_timer_t *timer, uint32_t tick) {
  os_timer_t *prev, *next;

  prev = NULL;
  next = osRtxInfo.timer.list;
  while ((next != NULL) && (next->tick <= tick)) {
    tick -= next->tick;
    prev  = next;
    next  = next->next;
  }
  timer->tick = tick;
  timer->prev = prev;
  timer->next = next;
  if (next != NULL) {
    next->tick -= timer->tick;
    next->prev  = timer;
  }
  if (prev != NULL) {
    prev->next = timer;
  } else {
    osRtxInfo.timer.list = timer;
  }
}

/// Remove Timer from the Timer List.
/// \param[in]  timer           timer object.
static void TimerRemove (os_timer_t *timer) {

  if (timer->next != NULL) {
    timer->next->tick += timer->tick;
    timer->next->prev  = timer->prev;
  }
  if (timer->prev != NULL) {
    timer->prev->next  = timer->next;
  } else {
    osRtxInfo.timer.list = timer->next;
  }
}

/// Unlink Timer from the Timer List Head.
/// \param[in]  timer           timer object.
static void TimerUnlink (os_timer_t *timer) {

  if (timer->next != NULL) {
    timer->next->prev = timer->prev;
  }
  osRtxInfo.timer.list = timer->next;
}


//  ==== Library functions ====

/// Timer Tick (called each SysTick).
void osRtxTimerTick (void) {
  os_timer_t *timer;
  osStatus_t  status;

  timer = osRtxInfo.timer.list;
  if (timer == NULL) {
    return;
  }

  timer->tick--;
  while ((timer != NULL) && (timer->tick == 0U)) {
    TimerUnlink(timer);
    status = osMessageQueuePut(osRtxInfo.timer.mq, &timer->finfo, 0U, 0U);
    if (status != osOK) {
      osRtxErrorNotify(osRtxErrorTimerQueueOverflow, timer);
    }
    if (timer->type == osRtxTimerPeriodic) {
      TimerInsert(timer, timer->load);
    } else {
      timer->state = osRtxTimerStopped;
    }
    timer = osRtxInfo.timer.list;
  }
}

/// Timer Thread
__WEAK void osRtxTimerThread (void *argument) {
  os_timer_finfo_t finfo;
  osStatus_t       status;
  (void)           argument;

  osRtxInfo.timer.mq = osMessageQueueNew(osRtxConfig.timer_mq_mcnt, sizeof(os_timer_finfo_t), osRtxConfig.timer_mq_attr);
  if (osRtxInfo.timer.mq == NULL) {
    return;
  }
  osRtxInfo.timer.tick = osRtxTimerTick;
  for (;;) {
    status = osMessageQueueGet(osRtxInfo.timer.mq, &finfo, NULL, osWaitForever);
    if (status == osOK) {
      EvrRtxTimerCallback(*(osTimerFunc_t)finfo.fp, finfo.arg);
      (*(osTimerFunc_t)finfo.fp)(finfo.arg);
    }
  }
}

//  ==== Service Calls ====

//  Service Calls definitions
SVC0_4M(TimerNew,       osTimerId_t,  osTimerFunc_t, osTimerType_t, void *, const osTimerAttr_t *)
SVC0_1 (TimerGetName,   const char *, osTimerId_t)
SVC0_2 (TimerStart,     osStatus_t,   osTimerId_t, uint32_t)
SVC0_1 (TimerStop,      osStatus_t,   osTimerId_t)
SVC0_1 (TimerIsRunning, uint32_t,     osTimerId_t)
SVC0_1 (TimerDelete,    osStatus_t,   osTimerId_t)

/// Create and Initialize a timer.
/// \note API identical to osTimerNew
osTimerId_t svcRtxTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr) {
  os_timer_t *timer;
  uint8_t     flags;
  const char *name;

  // Check parameters
  if ((func == NULL) || ((type != osTimerOnce) && (type != osTimerPeriodic))) {
    EvrRtxTimerError(NULL, osErrorParameter);
    return NULL;
  }

  // Process attributes
  if (attr != NULL) {
    name  = attr->name;
    timer = attr->cb_mem;
    if (timer != NULL) {
      if (((uint32_t)timer & 3U) || (attr->cb_size < sizeof(os_timer_t))) {
        EvrRtxTimerError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    } else {
      if (attr->cb_size != 0U) {
        EvrRtxTimerError(NULL, osRtxErrorInvalidControlBlock);
        return NULL;
      }
    }
  } else {
    name  = NULL;
    timer = NULL;
  }

  // Allocate object memory if not provided
  if (timer == NULL) {
    if (osRtxInfo.mpi.timer != NULL) {
      timer = osRtxMemoryPoolAlloc(osRtxInfo.mpi.timer);
    } else {
      timer = osRtxMemoryAlloc(osRtxInfo.mem.common, sizeof(os_timer_t), 1U);
    }
    if (timer == NULL) {
      EvrRtxTimerError(NULL, osErrorNoMemory);
      return NULL;
    }
    flags = osRtxFlagSystemObject;
  } else {
    flags = 0U;
  }

  // Initialize control block
  timer->id        = osRtxIdTimer;
  timer->state     = osRtxTimerStopped;
  timer->flags     = flags;
  timer->type      = (uint8_t)type;
  timer->name      = name;
  timer->prev      = NULL;
  timer->next      = NULL;
  timer->tick      = 0U;
  timer->load      = 0U;
  timer->finfo.fp  = (void *)func;
  timer->finfo.arg = argument;

  EvrRtxTimerCreated(timer);

  return timer;
}

/// Get name of a timer.
/// \note API identical to osTimerGetName
const char *svcRtxTimerGetName (osTimerId_t timer_id) {
  os_timer_t *timer = (os_timer_t *)timer_id;

  // Check parameters
  if ((timer == NULL) || (timer->id != osRtxIdTimer)) {
    EvrRtxTimerGetName(timer, NULL);
    return NULL;
  }

  // Check object state
  if (timer->state == osRtxObjectInactive) {
    EvrRtxTimerGetName(timer, NULL);
    return NULL;
  }

  EvrRtxTimerGetName(timer, timer->name);

  return timer->name;
}

/// Start or restart a timer.
/// \note API identical to osTimerStart
osStatus_t svcRtxTimerStart (osTimerId_t timer_id, uint32_t ticks) {
  os_timer_t *timer = (os_timer_t *)timer_id;

  // Check parameters
  if ((timer == NULL) || (timer->id != osRtxIdTimer) || (ticks == 0U)) {
    EvrRtxTimerError(timer, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  switch (timer->state) {
    case osRtxTimerStopped:
      if (osRtxInfo.timer.tick == NULL) {
        EvrRtxTimerError(timer, osErrorResource);
        return osErrorResource;
      }
      timer->state = osRtxTimerRunning;
      timer->load  = ticks;
      break;
    case osRtxTimerRunning:
      TimerRemove(timer);
      break;
    case osRtxTimerInactive:
    default:
      EvrRtxTimerError(timer, osErrorResource);
      return osErrorResource;
  }

  TimerInsert(timer, ticks);

  EvrRtxTimerStarted(timer);

  return osOK;
}

/// Stop a timer.
/// \note API identical to osTimerStop
osStatus_t svcRtxTimerStop (osTimerId_t timer_id) {
  os_timer_t *timer = (os_timer_t *)timer_id;

  // Check parameters
  if ((timer == NULL) || (timer->id != osRtxIdTimer)) {
    EvrRtxTimerError(timer, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  if (timer->state != osRtxTimerRunning) {
    EvrRtxTimerError(timer, osErrorResource);
    return osErrorResource;
  }

  timer->state = osRtxTimerStopped;

  TimerRemove(timer);

  EvrRtxTimerStopped(timer);

  return osOK;
}

/// Check if a timer is running.
/// \note API identical to osTimerIsRunning
uint32_t svcRtxTimerIsRunning (osTimerId_t timer_id) {
  os_timer_t *timer = (os_timer_t *)timer_id;

  // Check parameters
  if ((timer == NULL) || (timer->id != osRtxIdTimer)) {
    EvrRtxTimerIsRunning(timer, 0U);
    return 0U;
  }

  // Check object state
  if (timer->state == osRtxTimerRunning) {
    EvrRtxTimerIsRunning(timer, 1U);
    return 1U;
  }

  EvrRtxTimerIsRunning(timer, 0U);
  return 0U;
}

/// Delete a timer.
/// \note API identical to osTimerDelete
osStatus_t svcRtxTimerDelete (osTimerId_t timer_id) {
  os_timer_t *timer = (os_timer_t *)timer_id;

  // Check parameters
  if ((timer == NULL) || (timer->id != osRtxIdTimer)) {
    EvrRtxTimerError(timer, osErrorParameter);
    return osErrorParameter;
  }

  // Check object state
  switch (timer->state) {
    case osRtxTimerStopped:
      break;
    case osRtxTimerRunning:
      TimerRemove(timer);
      break;
    case osRtxTimerInactive:
    default:
      EvrRtxTimerError(timer, osErrorResource);
      return osErrorResource;
  }

  // Mark object as inactive
  timer->state = osRtxTimerInactive;

  // Free object memory
  if (timer->flags & osRtxFlagSystemObject) {
    if (osRtxInfo.mpi.timer != NULL) {
      osRtxMemoryPoolFree(osRtxInfo.mpi.timer, timer);
    } else {
      osRtxMemoryFree(osRtxInfo.mem.common, timer);
    }
  }

  EvrRtxTimerDestroyed(timer);

  return osOK;
}


//  ==== Public API ====

/// Create and Initialize a timer.
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr) {
  EvrRtxTimerNew(func, type, argument, attr);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxTimerError(NULL, osErrorISR);
    return NULL;
  }
  return __svcTimerNew(func, type, argument, attr);
}

/// Get name of a timer.
const char *osTimerGetName (osTimerId_t timer_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxTimerGetName(timer_id, NULL);
    return NULL;
  }
  return __svcTimerGetName(timer_id);
}

/// Start or restart a timer.
osStatus_t osTimerStart (osTimerId_t timer_id, uint32_t ticks) {
  EvrRtxTimerStart(timer_id, ticks);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxTimerError(timer_id, osErrorISR);
    return osErrorISR;
  }
  return __svcTimerStart(timer_id, ticks);
}

/// Stop a timer.
osStatus_t osTimerStop (osTimerId_t timer_id) {
  EvrRtxTimerStop(timer_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxTimerError(timer_id, osErrorISR);
    return osErrorISR;
  }
  return __svcTimerStop(timer_id);
}

/// Check if a timer is running.
uint32_t osTimerIsRunning (osTimerId_t timer_id) {
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxTimerIsRunning(timer_id, 0U);
    return 0U;
  }
  return __svcTimerIsRunning(timer_id);
}

/// Delete a timer.
osStatus_t osTimerDelete (osTimerId_t timer_id) {
  EvrRtxTimerDelete(timer_id);
  if (IS_IRQ_MODE() || IS_IRQ_MASKED()) {
    EvrRtxTimerError(timer_id, osErrorISR);
    return osErrorISR;
  }
  return __svcTimerDelete(timer_id);
}
