/*
 * Copyright (c) 2015-2018, ARM Limited, All Rights Reserved
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
 * \file nfc_scheduler.c
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define __DEBUG__ 0
#ifndef __MODULE__
#define __MODULE__ "nfc_scheduler.c"
#endif

#include "platform/nfc_scheduler.h"

void nfc_scheduler_init(nfc_scheduler_t *pScheduler, nfc_scheduler_timer_t *pTimer)
{
    pScheduler->pNext = NULL;
    pScheduler->pTimer = pTimer;

    //Start timer
    nfc_scheduler_timer_start(pTimer);
}

#define MAX_TIMEOUT UINT32_MAX

uint32_t nfc_scheduler_iteration(nfc_scheduler_t *pScheduler, uint32_t events)
{
    while (true) {
        nfc_task_t *pPrioTask = NULL;
        nfc_task_t *pPrioTaskPrevious = NULL;
        uint32_t prioTaskEvent = 0;
        int64_t timeout;
        nfc_task_t *pPreviousTask = NULL;
        nfc_task_t *pTask = pScheduler->pNext;

        if (pTask == NULL) {
            NFC_DBG("Empty queue, %lu ms elapsed", nfc_scheduler_timer_get(pScheduler->pTimer));
            //Empty queue, return
            return MAX_TIMEOUT;
        }

        //Get timer value
        uint32_t timeElapsed = nfc_scheduler_timer_get(pScheduler->pTimer);
        NFC_DBG("%lu ms elapsed", timeElapsed);
        nfc_scheduler_timer_reset(pScheduler->pTimer);

        do {
            //Apply timeouts
            if (pTask->events & EVENT_TIMEOUT) {
                pTask->timeout -= timeElapsed;
            }
            pPreviousTask = pTask;
            pTask = pTask->pNext;
        } while (pTask != NULL);

        pTask = pScheduler->pNext;
        pPreviousTask = NULL;
        timeout = MAX_TIMEOUT;
        do {
            //Check which task should be woken up first
            if ((events & EVENT_HW_INTERRUPT) && (pTask->events & EVENT_HW_INTERRUPT)) {
                //Hardware interrupts have prio
                pPrioTask = pTask;
                pPrioTaskPrevious = pPreviousTask;
                timeout = 0;
                events &= ~EVENT_HW_INTERRUPT; //Only one task gets triggered per event
                prioTaskEvent = EVENT_HW_INTERRUPT;
                break;
            } else if ((pTask->events & EVENT_TIMEOUT) && (pTask->timeout < timeout)) {
                pPrioTask = pTask;
                pPrioTaskPrevious = pPreviousTask;
                timeout = pTask->timeout;
                prioTaskEvent = EVENT_TIMEOUT;
            }
            pPreviousTask = pTask;
            pTask = pTask->pNext;
        } while (pTask != NULL);

        if (pPrioTask == NULL) {
            //No task to wake up, exit
            NFC_DBG("No task to wake up");
            return MAX_TIMEOUT;
        }

        if (timeout >  0) {
            //No task to wake up yet
            if (timeout > MAX_TIMEOUT) {
                NFC_DBG("No task to wake up");
                return MAX_TIMEOUT;
            } else {
                NFC_DBG("No task to wake up, wait %lu ms", timeout);
                return timeout;
            }
        }

        //Dequeue task
        if (pPrioTaskPrevious == NULL) {
            pScheduler->pNext = pPrioTask->pNext;
        } else {
            pPrioTaskPrevious->pNext = pPrioTask->pNext;
        }
        pPrioTask->pNext = NULL;

        //Execute task
        NFC_DBG("Calling task %p - events %02X", pPrioTask, prioTaskEvent);
        pPrioTask->fn(prioTaskEvent, pPrioTask->pUserData);
        events &= ~EVENT_HW_INTERRUPT; //Only one task gets triggered per event
    }
    return MAX_TIMEOUT;
}

void nfc_scheduler_queue_task(nfc_scheduler_t *pScheduler, nfc_task_t *pTask)
{
    pTask->timeout = pTask->timeoutInitial + nfc_scheduler_timer_get(pScheduler->pTimer);
    NFC_DBG("Queuing task %p: events %1X, timeout %lu ms", pTask, pTask->events, pTask->timeout);
    //Find last task
    nfc_task_t *pPrevTask = pScheduler->pNext;
    pTask->pNext = NULL;
    if (pPrevTask == NULL) {
        pScheduler->pNext = pTask;
        return;
    }
    while (pPrevTask->pNext != NULL) {
        pPrevTask = pPrevTask->pNext;
    }
    pPrevTask->pNext = pTask;
}

void nfc_scheduler_dequeue_task(nfc_scheduler_t *pScheduler, bool abort, nfc_task_t *pTask)
{
    NFC_DBG("Dequeuing task %p", pTask);
    //Find task
    nfc_task_t *pPrevTask = pScheduler->pNext;
    if (pPrevTask == NULL) {
        pTask->pNext = NULL;
        return;
    }
    if (pPrevTask == pTask) {
        if (abort) {
            pTask->fn(EVENT_ABORTED, pTask->pUserData);
        }
        pScheduler->pNext = pTask->pNext;
        pTask->pNext = NULL;
        return;
    }
    while (pPrevTask->pNext != NULL) {
        if (pPrevTask->pNext == pTask) {
            if (abort) {
                pTask->fn(EVENT_ABORTED, pTask->pUserData);
            }
            pPrevTask->pNext = pTask->pNext;
            pTask->pNext = NULL;
            return;
        }
        pPrevTask = pPrevTask->pNext;
    }
    pTask->pNext = NULL;
}

void task_init(nfc_task_t *pTask, uint32_t events, uint32_t timeout, nfc_task_fn fn, void *pUserData)
{
    pTask->events = events;
    pTask->timeoutInitial = timeout;
    pTask->fn = fn;
    pTask->pUserData = pUserData;
    pTask->pNext = NULL;
}
