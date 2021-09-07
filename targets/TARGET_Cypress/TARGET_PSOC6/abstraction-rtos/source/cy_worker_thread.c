/***********************************************************************************************//**
 * \file cy_worker_thread.c
 *
 * \brief
 * Provides implementation for functions that allow creating/deleting worker
 * threads and deferring work to a worker thread.
 ***************************************************************************************************
 * \copyright
 * Copyright 2018-2021 Cypress Semiconductor Corporation
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
 **************************************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "cy_worker_thread.h"
#include "cy_utils.h"
#include "cyabs_rtos_internal.h"
#if defined(CY_USING_HAL)
#include "cyhal_system.h"

#if defined(__cplusplus)
extern "C"
{
#endif

// Info for dispatching a function call
typedef struct
{
    cy_worker_thread_func_t* work_func;
    void*                    arg;
} cy_worker_dispatch_info_t;


//--------------------------------------------------------------------------------------------------
// cy_worker_thread_func
//
/* Worker Thread to dispatch the events that added to the event queue.
 * It will wait indefinitely for a item to be queued and will terminate
 * when the NULL work function is queued by delete. It will process all
 * events before the terminating event.
 * @param   arg : pointer to @ref cy_worker_thread_info_t
 */
//--------------------------------------------------------------------------------------------------
static void cy_worker_thread_func(cy_thread_arg_t arg)
{
    cy_rslt_t                 result;
    cy_worker_dispatch_info_t dispatch_info;
    cy_worker_thread_info_t*  worker = (cy_worker_thread_info_t*)arg;

    while (1)
    {
        result = cy_rtos_get_queue(&worker->event_queue, &dispatch_info, CY_RTOS_NEVER_TIMEOUT,
                                   false);
        if (result == CY_RSLT_SUCCESS)
        {
            if (dispatch_info.work_func != NULL)
            {
                dispatch_info.work_func(dispatch_info.arg);
            }
            else
            {
                break;
            }
        }
    }
    cy_rtos_exit_thread();
}


//--------------------------------------------------------------------------------------------------
// cy_worker_thread_create
//--------------------------------------------------------------------------------------------------
cy_rslt_t cy_worker_thread_create(cy_worker_thread_info_t* new_worker,
                                  const cy_worker_thread_params_t* params)
{
    // Param check
    CY_ASSERT((params != NULL) && (new_worker != NULL));
    CY_ASSERT((params->stack == NULL) || (params->stack != NULL && params->stack_size != 0));

    // Start with a clean structure
    memset(new_worker, 0, sizeof(cy_worker_thread_info_t));

    cy_rslt_t result = cy_rtos_init_queue(&new_worker->event_queue,
                                          (params->num_entries != 0)
                                          ? params->num_entries
                                          : CY_WORKER_DEFAULT_ENTRIES,
                                          sizeof(cy_worker_dispatch_info_t));
    if (result == CY_RSLT_SUCCESS)
    {
        new_worker->state = CY_WORKER_THREAD_VALID;
        result            = cy_rtos_create_thread(&new_worker->thread,
                                                  cy_worker_thread_func,
                                                  (params->name != NULL)
                                                  ? params->name
                                                  : CY_WORKER_THREAD_DEFAULT_NAME,
                                                  params->stack,
                                                  params->stack_size,
                                                  params->priority,
                                                  (cy_thread_arg_t)new_worker);

        if (result != CY_RSLT_SUCCESS)
        {
            new_worker->state = CY_WORKER_THREAD_INVALID;
            cy_rtos_deinit_queue(&new_worker->event_queue);
        }
    }
    return result;
}


//--------------------------------------------------------------------------------------------------
// cy_worker_thread_delete
//--------------------------------------------------------------------------------------------------
cy_rslt_t cy_worker_thread_delete(cy_worker_thread_info_t* old_worker)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    uint32_t state = cyhal_system_critical_section_enter();
    if (old_worker->state != CY_WORKER_THREAD_INVALID)
    {
        // Don't allow terminating while cy_rtos_put_queue is running
        if (old_worker->state == CY_WORKER_THREAD_VALID)
        {
            // A terminating event is queued that will break the while loop
            // Note that this is ok because thread enqueue function will not
            // allow NULL as a valid value for the work function.
            old_worker->state = CY_WORKER_THREAD_TERMINATING;
            cyhal_system_critical_section_exit(state);
            cy_worker_dispatch_info_t dispatch_info = { NULL, NULL };
            bool in_isr = is_in_isr();
            result = cy_rtos_put_queue(&old_worker->event_queue, &dispatch_info, 0, in_isr);
            if (result != CY_RSLT_SUCCESS)
            {
                // Could not enqueue termination task, return to valid state
                state = cyhal_system_critical_section_enter();
                old_worker->state = CY_WORKER_THREAD_VALID;
                cyhal_system_critical_section_exit(state);

                return result;
            }
        }

        if (old_worker->state != CY_WORKER_THREAD_JOIN_COMPLETE)
        {
            cyhal_system_critical_section_exit(state);
            result = cy_rtos_join_thread(&old_worker->thread);
            if (result != CY_RSLT_SUCCESS)
            {
                return result;
            }
            state = cyhal_system_critical_section_enter();
            old_worker->state = CY_WORKER_THREAD_JOIN_COMPLETE;
        }

        if (old_worker->state != CY_WORKER_THREAD_INVALID)
        {
            cyhal_system_critical_section_exit(state);
            result = cy_rtos_deinit_queue(&old_worker->event_queue);
            if (result != CY_RSLT_SUCCESS)
            {
                return result;
            }
            state = cyhal_system_critical_section_enter();
            old_worker->state = CY_WORKER_THREAD_INVALID;
        }
    }

    cyhal_system_critical_section_exit(state);
    return result;
}


//--------------------------------------------------------------------------------------------------
// cy_worker_thread_enqueue
//--------------------------------------------------------------------------------------------------
cy_rslt_t cy_worker_thread_enqueue(cy_worker_thread_info_t* worker_info,
                                   cy_worker_thread_func_t* work_func, void* arg)
{
    CY_ASSERT(worker_info != NULL);
    CY_ASSERT(work_func != NULL);

    uint32_t state = cyhal_system_critical_section_enter();
    if ((worker_info->state != CY_WORKER_THREAD_VALID) &&
        (worker_info->state != CY_WORKER_THREAD_ENQUEUING))
    {
        cyhal_system_critical_section_exit(state);
        return CY_WORKER_THREAD_ERR_THREAD_INVALID;
    }
    worker_info->enqueue_count++;
    worker_info->state = CY_WORKER_THREAD_ENQUEUING;
    cyhal_system_critical_section_exit(state);

    cy_worker_dispatch_info_t dispatch_info = { work_func, arg };
    // Queue an event to be run by the worker thread
    bool in_isr = is_in_isr();
    cy_rslt_t result = cy_rtos_put_queue(&worker_info->event_queue, &dispatch_info, 0, in_isr);

    state = cyhal_system_critical_section_enter();
    worker_info->enqueue_count--;
    if (worker_info->enqueue_count == 0)
    {
        worker_info->state = CY_WORKER_THREAD_VALID;
    }
    cyhal_system_critical_section_exit(state);

    return result;
}


#if defined(__cplusplus)
}
#endif

#endif // defined(CY_USING_HAL)
