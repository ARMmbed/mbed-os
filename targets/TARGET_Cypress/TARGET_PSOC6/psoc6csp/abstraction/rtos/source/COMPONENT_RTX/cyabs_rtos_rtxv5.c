/***************************************************************************//**
* \file cyabs_rtos.c
*
* \brief
* Implementation for CMSIS RTOS v2 abstraction
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <stdlib.h>
#include "cy_utils.h"
#include "cyabs_rtos.h"

#if defined(__cplusplus)
extern "C" {
#endif


/******************************************************
*                 Error Converter
******************************************************/

/* Last received error status */
static cy_rtos_error_t dbgErr;
cy_rtos_error_t cy_rtos_last_error() { return dbgErr; }

/* Converts internal error type to external error type */
static cy_rslt_t error_converter(cy_rtos_error_t internalError)
{
    cy_rslt_t value;

    switch (internalError)
    {
        case osOK:
            value = CY_RSLT_SUCCESS;
            break;
        case osErrorTimeout:
            value = CY_RTOS_TIMEOUT;
            break;
        case osErrorParameter:
            value = CY_RTOS_BAD_PARAM;
            break;
        case osErrorNoMemory:
            value = CY_RTOS_NO_MEMORY;
            break;
        case osError:
        case osErrorResource:
        case osErrorISR:
        default:
            value = CY_RTOS_GENERAL_ERROR;
            break;
    }

    /* Update the last known error status */
    dbgErr = internalError;
    return value;
}


/******************************************************
*                 Threads
******************************************************/

cy_rslt_t cy_rtos_create_thread(cy_thread_t *thread, cy_thread_entry_fn_t entry_function,
            const char *name, void *stack, uint32_t stack_size, cy_thread_priority_t priority, cy_thread_arg_t arg)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    osThreadAttr_t attr;

    if (thread == NULL || stack_size < CY_RTOS_MIN_STACK_SIZE)
        status = CY_RTOS_BAD_PARAM;
    else if (stack != NULL && (0 != (((uint32_t)stack) & CY_RTOS_ALIGNMENT_MASK)))
        status = CY_RTOS_ALIGNMENT_ERROR;
    else
    {
        attr.name = name;
        attr.attr_bits = osThreadJoinable;
        attr.cb_size = osRtxThreadCbSize;
        attr.stack_size = stack_size;
        attr.priority = (osPriority_t)priority;
        attr.tz_module = 0;
        attr.reserved = 0;

        /* Allocate stack if NULL was passed */
        if ((uint32_t *)stack == NULL)
        {
            /* Note: 1 malloc so that it can be freed with 1 call when terminating */
            uint32_t cb_mem_pad = (~osRtxThreadCbSize + 1) & CY_RTOS_ALIGNMENT_MASK;
            attr.cb_mem = malloc(osRtxThreadCbSize + cb_mem_pad + stack_size);
            if (attr.cb_mem != NULL)
                attr.stack_mem = (uint32_t *)((uint32_t)attr.cb_mem + osRtxThreadCbSize + cb_mem_pad);
        }
        else
        {
            attr.cb_mem = malloc(osRtxThreadCbSize);
            attr.stack_mem = stack;
        }

        if (attr.cb_mem == NULL)
            status = CY_RTOS_NO_MEMORY;
        else
        {
            CY_ASSERT(((uint32_t)attr.cb_mem & CY_RTOS_ALIGNMENT_MASK) == 0UL);
            CY_ASSERT(((uint32_t)attr.stack_mem & CY_RTOS_ALIGNMENT_MASK) == 0UL);
            *thread = osThreadNew((osThreadFunc_t)entry_function, arg, &attr);
            CY_ASSERT((*thread == attr.cb_mem) || (*thread == NULL));
            status = (*thread == NULL) ? CY_RTOS_GENERAL_ERROR : CY_RSLT_SUCCESS;
        }
    }

    return status;
}

cy_rslt_t cy_rtos_exit_thread()
{
    osThreadExit();
}

cy_rslt_t cy_rtos_terminate_thread(cy_thread_t *thread)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (thread == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osThreadTerminate(*thread);
        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_is_thread_running(cy_thread_t *thread, bool *running)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((thread == NULL) || (running == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
    {
        *running = (osThreadGetState(*thread) == osThreadRunning) ? true : false;
    }

    return status;
}

cy_rslt_t cy_rtos_get_thread_state(cy_thread_t *thread, cy_thread_state_t *state)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((thread == NULL) || (state == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
    {
        switch (osThreadGetState(*thread))
        {
            case osThreadInactive:
                *state = CY_THREAD_STATE_INACTIVE;
                break;
            case osThreadReady:
                *state = CY_THREAD_STATE_READY;
                break;
            case osThreadRunning:
                *state = CY_THREAD_STATE_RUNNING;
                break;
            case osThreadBlocked:
                *state = CY_THREAD_STATE_BLOCKED;
                break;
            case osThreadTerminated:
                *state = CY_THREAD_STATE_TERMINATED;
                break;
            case osThreadError:
            case osThreadReserved:
            default:
                *state = CY_THREAD_STATE_UNKNOWN;
                break;
        }
    }

    return status;
}

cy_rslt_t cy_rtos_join_thread(cy_thread_t *thread)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (thread == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osThreadJoin(*thread);
        status = error_converter(statusInternal);

        if (status == CY_RSLT_SUCCESS)
        {
            free(*thread);
            *thread = NULL;
        }
    }

    return status;
}

cy_rslt_t cy_rtos_get_thread_handle(cy_thread_t *thread)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if (thread == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
        *thread = osThreadGetId();

    return status;
}


/******************************************************
*                 Mutexes
******************************************************/

cy_rslt_t cy_rtos_init_mutex(cy_mutex_t *mutex)
{
    cy_rslt_t status;
    osMutexAttr_t attr;

    if (mutex == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        attr.name = NULL;
        attr.attr_bits = osMutexRecursive | osMutexPrioInherit;
        attr.cb_mem = malloc(osRtxMutexCbSize);
        attr.cb_size = osRtxMutexCbSize;

        if (attr.cb_mem == NULL)
            status = CY_RTOS_NO_MEMORY;
        else
        {
            CY_ASSERT(((uint32_t)attr.cb_mem & CY_RTOS_ALIGNMENT_MASK) == 0UL);
            *mutex = osMutexNew(&attr);
            CY_ASSERT((*mutex == attr.cb_mem) || (*mutex == NULL));
            status = (*mutex == NULL) ? CY_RTOS_GENERAL_ERROR : CY_RSLT_SUCCESS;
        }
    }

    return status;
}

cy_rslt_t cy_rtos_get_mutex(cy_mutex_t *mutex, cy_time_t timeout_ms)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (mutex == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osMutexAcquire(*mutex, timeout_ms);
        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_set_mutex(cy_mutex_t *mutex)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (mutex == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osMutexRelease(*mutex);
        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_deinit_mutex(cy_mutex_t *mutex)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (mutex == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osMutexDelete(*mutex);
        status = error_converter(statusInternal);

        if (status == CY_RSLT_SUCCESS)
        {
            free(*mutex);
            *mutex = NULL;
        }
    }

    return status;
}


/******************************************************
*                 Semaphores
******************************************************/

cy_rslt_t cy_rtos_init_semaphore(cy_semaphore_t *semaphore, uint32_t maxcount, uint32_t initcount)
{
    cy_rslt_t status;
    osSemaphoreAttr_t attr;

    if (semaphore == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        attr.name = NULL;
        attr.attr_bits = 0U;
        attr.cb_mem = malloc(osRtxSemaphoreCbSize);
        attr.cb_size = osRtxSemaphoreCbSize;

        if (attr.cb_mem == NULL)
            status = CY_RTOS_NO_MEMORY;
        else
        {
            CY_ASSERT(((uint32_t)attr.cb_mem & CY_RTOS_ALIGNMENT_MASK) == 0UL);
            *semaphore = osSemaphoreNew(maxcount, initcount, &attr);
            CY_ASSERT((*semaphore == attr.cb_mem) || (*semaphore == NULL));
            status = (*semaphore == NULL) ? CY_RTOS_GENERAL_ERROR : CY_RSLT_SUCCESS;
        }
    }

    return status;
}

cy_rslt_t cy_rtos_get_semaphore(cy_semaphore_t *semaphore, cy_time_t timeout_ms, bool in_isr)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    cy_rtos_error_t statusInternal;

    if (semaphore == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        /* Not allowed to be called in ISR if timeout != 0 */
        if ((!in_isr) || (in_isr && (timeout_ms == 0U)))
            statusInternal = osSemaphoreAcquire(*semaphore, timeout_ms);
        else
            statusInternal = osErrorISR;

        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_set_semaphore(cy_semaphore_t *semaphore, bool in_isr)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    cy_rtos_error_t statusInternal;
    (void)in_isr; // Unused parameter in this implementation

    if (semaphore == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osSemaphoreRelease(*semaphore);
        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_deinit_semaphore(cy_semaphore_t *semaphore)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (semaphore == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osSemaphoreDelete(*semaphore);
        status = error_converter(statusInternal);
        if (status == CY_RSLT_SUCCESS)
        {
            free(*semaphore);
            *semaphore = NULL;
        }
    }

    return status;
}


/******************************************************
*                 Events
******************************************************/

#define CY_RTOS_EVENT_ERRORFLAG		0x80000000UL
#define CY_RTOS_EVENT_FLAGS 		0x7FFFFFFFUL

cy_rslt_t cy_rtos_init_event(cy_event_t *event)
{
    cy_rslt_t status;
    osEventFlagsAttr_t attr;

    if (event == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        attr.name = NULL;
        attr.attr_bits = 0U;
        attr.cb_mem = malloc(osRtxEventFlagsCbSize);
        attr.cb_size = osRtxEventFlagsCbSize;

        if (attr.cb_mem == NULL)
            status = CY_RTOS_NO_MEMORY;
        else
        {
            CY_ASSERT(((uint32_t)attr.cb_mem & CY_RTOS_ALIGNMENT_MASK) == 0UL);
            *event = osEventFlagsNew(&attr);
            CY_ASSERT((*event == attr.cb_mem) || (*event == NULL));
            status = (*event == NULL) ? CY_RTOS_GENERAL_ERROR : CY_RSLT_SUCCESS;
        }
    }

    return status;
}

cy_rslt_t cy_rtos_setbits_event(cy_event_t *event, uint32_t bits, bool in_isr)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    cy_rtos_error_t statusInternal;
    (void)in_isr; // Unused parameter in this implementation

    if (event == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = (osStatus_t)osEventFlagsSet(*event, bits);
        if ((statusInternal & CY_RTOS_EVENT_ERRORFLAG) != 0UL)
            status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_clearbits_event(cy_event_t *event, uint32_t bits, bool in_isr)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    cy_rtos_error_t statusInternal;
    (void)in_isr; // Unused parameter in this implementation

    if (event == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = (osStatus_t)osEventFlagsClear(*event, bits);
        if ((statusInternal & CY_RTOS_EVENT_ERRORFLAG) != 0UL)
            status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_getbits_event(cy_event_t *event, uint32_t *bits)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((event == NULL) || (bits == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
        *bits = osEventFlagsGet(*event);

    return status;
}

cy_rslt_t cy_rtos_waitbits_event(cy_event_t *event, uint32_t *bits, bool clear, bool all, cy_time_t timeout)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    cy_rtos_error_t statusInternal;
    uint32_t flagOption;

    if ((event == NULL) || (bits == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
    {
        flagOption = (all) ? osFlagsWaitAll : osFlagsWaitAny;
        if (!clear)
            flagOption |= osFlagsNoClear;

        statusInternal = (osStatus_t)osEventFlagsWait(*event, *bits, flagOption, timeout);
        if ((statusInternal & CY_RTOS_EVENT_ERRORFLAG) == 0UL)
            *bits = statusInternal;
        else
            status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_deinit_event(cy_event_t *event)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (event == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osEventFlagsDelete(*event);
        status = error_converter(statusInternal);
        if (status == CY_RSLT_SUCCESS)
        {
            free(*event);
            *event = NULL;
        }
    }

    return status;
}


/******************************************************
*                 Queues
******************************************************/

cy_rslt_t cy_rtos_init_queue(cy_queue_t *queue, size_t length, size_t itemsize)
{
    cy_rslt_t status;
    osMessageQueueAttr_t attr;

    if (queue == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        attr.name = NULL;
        attr.attr_bits = 0U;
        attr.cb_size = osRtxMessageQueueCbSize;
        uint32_t blockSize = ((itemsize + 3U) & ~3UL) + sizeof(osRtxMessage_t);
        attr.mq_size = blockSize * length;

        /* Note: 1 malloc for both so that they can be freed with 1 call */
        uint32_t cb_mem_pad = (8 - (osRtxMessageQueueCbSize & 0x07)) & 0x07;
        attr.cb_mem = malloc(osRtxMessageQueueCbSize + cb_mem_pad + attr.mq_size);
        if (attr.cb_mem != NULL)
            attr.mq_mem = (uint32_t *)((uint32_t)attr.cb_mem + osRtxMessageQueueCbSize + cb_mem_pad);

        if (attr.cb_mem == NULL)
            status = CY_RTOS_NO_MEMORY;
        else
        {
            CY_ASSERT(((uint32_t)attr.cb_mem & CY_RTOS_ALIGNMENT_MASK) == 0UL);
            CY_ASSERT(((uint32_t)attr.mq_mem & CY_RTOS_ALIGNMENT_MASK) == 0UL);
            *queue = osMessageQueueNew(length, itemsize, &attr);
            CY_ASSERT((*queue == attr.cb_mem) || (*queue == NULL));
            status = (*queue == NULL) ? CY_RTOS_GENERAL_ERROR : CY_RSLT_SUCCESS;
        }
    }

    return status;
}

cy_rslt_t cy_rtos_put_queue(cy_queue_t *queue, const void *item_ptr, cy_time_t timeout_ms, bool in_isr)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if ((queue == NULL) || (item_ptr == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
    {
        /* Not allowed to be called in ISR if timeout != 0 */
        if ((!in_isr) || (in_isr && (timeout_ms == 0U)))
            statusInternal = osMessageQueuePut(*queue, (uint8_t *)item_ptr, 0u, timeout_ms);
        else
            statusInternal = osErrorISR;

        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_get_queue(cy_queue_t *queue, void *item_ptr, cy_time_t timeout_ms, bool in_isr)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if ((queue == NULL) || (item_ptr == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
    {
        /* Not allowed to be called in ISR if timeout != 0 */
        if ((!in_isr) || (in_isr && (timeout_ms == 0U)))
            statusInternal = osMessageQueueGet(*queue, (uint8_t *)item_ptr, 0u, timeout_ms);
        else
            statusInternal = osErrorISR;

        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_count_queue(cy_queue_t *queue, size_t *num_waiting)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((queue == NULL) || (num_waiting == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
        *num_waiting = osMessageQueueGetCount(*queue);

    return status;
}

cy_rslt_t cy_rtos_space_queue(cy_queue_t *queue, size_t *num_spaces)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((queue == NULL) || (num_spaces == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
        *num_spaces = osMessageQueueGetSpace(*queue);

    return status;
}

cy_rslt_t cy_rtos_reset_queue(cy_queue_t *queue)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (queue == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osMessageQueueReset(*queue);
        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_deinit_queue(cy_queue_t *queue)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (queue == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osMessageQueueDelete(*queue);
        status = error_converter(statusInternal);

        if (status == CY_RSLT_SUCCESS)
        {
            free(*queue);
            *queue = NULL;
        }
    }

    return status;
}


/******************************************************
*                 Timers
******************************************************/

cy_rslt_t cy_rtos_init_timer(cy_timer_t *timer, cy_timer_trigger_type_t type,
        cy_timer_callback_t fun, cy_timer_callback_arg_t arg)
{
    cy_rslt_t status;
    osTimerAttr_t attr;

    if (timer == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        attr.name = NULL;
        attr.attr_bits = 0U;
        attr.cb_mem = malloc(osRtxTimerCbSize);
        attr.cb_size = osRtxTimerCbSize;

        if (attr.cb_mem == NULL)
            status = CY_RTOS_NO_MEMORY;
        else
        {
            osTimerType_t osTriggerType = (CY_TIMER_TYPE_PERIODIC == type)
                ? osTimerPeriodic
                : osTimerOnce;

            CY_ASSERT(((uint32_t)attr.cb_mem & CY_RTOS_ALIGNMENT_MASK) == 0UL);
            *timer = osTimerNew( (osTimerFunc_t)fun, osTriggerType, (void *)arg, &attr );
            CY_ASSERT((*timer == attr.cb_mem) || (*timer == NULL));
            status = (*timer == NULL) ? CY_RTOS_GENERAL_ERROR : CY_RSLT_SUCCESS;
        }
    }

    return status;
}

cy_rslt_t cy_rtos_start_timer(cy_timer_t *timer, cy_time_t num_ms)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (timer == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        /* Get Number of ticks per second */
        uint32_t tick_freq = osKernelGetTickFreq();

        /* Convert ticks count to time in milliseconds */
        if (tick_freq != 0)
        {
            uint32_t ticks = ((num_ms * tick_freq) / 1000);
            statusInternal = osTimerStart(*timer, ticks);
            status = error_converter(statusInternal);
        }
        else
            status = CY_RTOS_GENERAL_ERROR;
    }

    return status;
}

cy_rslt_t cy_rtos_stop_timer(cy_timer_t *timer)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (timer == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osTimerStop(*timer);
        status = error_converter(statusInternal);
    }

    return status;
}

cy_rslt_t cy_rtos_is_running_timer(cy_timer_t *timer, bool *state)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;

    if ((timer == NULL) || (state == NULL))
        status = CY_RTOS_BAD_PARAM;
    else
        *state = osTimerIsRunning(*timer);

    return status;
}

cy_rslt_t cy_rtos_deinit_timer(cy_timer_t *timer)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    if (timer == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        statusInternal = osTimerDelete(*timer);
        status = error_converter(statusInternal);

        if (status == CY_RSLT_SUCCESS)
        {
            free(*timer);
            *timer = NULL;
        }
    }

    return status;
}


/******************************************************
*                 Time
******************************************************/

cy_rslt_t cy_rtos_get_time(cy_time_t *tval)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint32_t tick_freq;

    if (tval == NULL)
        status = CY_RTOS_BAD_PARAM;
    else
    {
        /* Get Number of ticks per second */
        tick_freq = osKernelGetTickFreq();

        /* Convert ticks count to time in milliseconds */
        if (tick_freq != 0)
            *tval = (cy_time_t)((osKernelGetTickCount() * 1000LL) / tick_freq);
        else
            status = CY_RTOS_GENERAL_ERROR;
    }

    return status;
}

cy_rslt_t cy_rtos_delay_milliseconds(cy_time_t num_ms)
{
    cy_rslt_t status;
    cy_rtos_error_t statusInternal;

    statusInternal = osDelay(num_ms);
    status = error_converter(statusInternal);

    return status;
}


#if defined(__cplusplus)
}
#endif
