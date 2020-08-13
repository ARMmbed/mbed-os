/*
 * Copyright (c) 2014-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file nfc_scheduler.h
 * \copyright Copyright (c) ARM Ltd 2014
 * \author Donatien Garnier
 */
/** \addtogroup Core
 *  @{
 *  \name Scheduler
 *  @{
 */

#ifndef NFC_SCHEDULER_H_
#define NFC_SCHEDULER_H_

#include "stack/nfc_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_NONE         0
#define EVENT_TIMEOUT      1
#define EVENT_ABORTED      2
#define EVENT_HW_INTERRUPT 4

struct __nfc_timer;
typedef struct __nfc_timer nfc_scheduler_timer_t;

struct __nfc_task;
typedef struct __nfc_task nfc_task_t;

typedef struct __scheduler {
    nfc_task_t *pNext;
    nfc_scheduler_timer_t *pTimer;
} nfc_scheduler_t;

typedef void (*nfc_task_fn)(uint32_t events, void *pUserData);

struct __nfc_task {
    uint32_t events;
    int64_t timeout; //millisecs
    int64_t timeoutInitial;

    nfc_task_fn fn;
    void *pUserData;

    nfc_task_t *pNext;
};

void nfc_scheduler_timer_init(nfc_scheduler_timer_t *timer);

void nfc_scheduler_timer_start(nfc_scheduler_timer_t *timer);

uint32_t nfc_scheduler_timer_get(nfc_scheduler_timer_t *timer);

void nfc_scheduler_timer_stop(nfc_scheduler_timer_t *timer);

void nfc_scheduler_timer_reset(nfc_scheduler_timer_t *timer);

/** Init scheduler
 * \param pScheduler scheduler instance to init
 * \param pTimer timer instance
 */
void nfc_scheduler_init(nfc_scheduler_t *pScheduler, nfc_scheduler_timer_t *pTimer);

/** Iterate through all tasks
 * \param pScheduler scheduler instance
 * \param events mask of events (except EVENT_TIMEOUT) that have been raised since this function last returned (0 on first call)
 * \return time after which this function must be called again if no other event arises
 */
uint32_t nfc_scheduler_iteration(nfc_scheduler_t *pScheduler, uint32_t events);

/** Queue a task to execute
 * \param pScheduler scheduler instance
 * \param pTask task to queue
 *
 */
void nfc_scheduler_queue_task(nfc_scheduler_t *pScheduler, nfc_task_t *pTask);

/** Remove a task to execute
 * \param pScheduler scheduler instance
 * \param pTask task to remove
 * \param abort abort task if queued
 */
void nfc_scheduler_dequeue_task(nfc_scheduler_t *pScheduler, bool abort, nfc_task_t *pTask);

/** Initialize task with the following parameters
 * \param pTask task to initialize
 * \param events events on which to call task
 * \param timeout if relevant
 * \param fn function to be called
 * \param pUserData data that will be passed to function
 */
void task_init(nfc_task_t *pTask, uint32_t events, uint32_t timeout, nfc_task_fn fn, void *pUserData);

#ifdef __cplusplus
}
#endif

#endif /* NFC_SCHEDULER_H_ */

/**
 * @}
 * @}
 * */

