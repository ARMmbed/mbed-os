/***********************************************************************************************//**
 * \file cy_worker_thread.h
 *
 * \brief
 * Defines the interface for the worker thread utility. Provides prototypes for
 * functions that allow creating/deleting worker threads and queueing work to
 * a worker thread.
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "cy_result.h"
#include "cyabs_rtos.h"

/**
 * \addtogroup group_worker_thread_util Worker Thread Utility
 * \{
 * Worker thread utility that allows functions to be run a different thread context.
 * This utility can be used to delegate work that is not timing critical. For example,
 * scheduling work in interrupt handlers to keep handler execution times low or if some
 * work needs to be done at a different priority.
 */

/**< Default worker thread name */
#define CY_WORKER_THREAD_DEFAULT_NAME               "CYWorker"
/** Default number of work items in the queue */
#define CY_WORKER_DEFAULT_ENTRIES                   (16)

/** Additional work cannot be enqueued because the worker thread has been terminated.
 * This can occur if \ref cy_worker_thread_create was not called or \ref cy_worker_thread_delete was
 * called before calling \ref cy_worker_thread_enqueue
 */
#define CY_WORKER_THREAD_ERR_THREAD_INVALID          \
    CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_ABSTRACTION_OS, 32)

/** Worker thread function call prototype  */
typedef void (cy_worker_thread_func_t)(void* arg);

/** Thread state enumeration */
typedef enum
{
    CY_WORKER_THREAD_INVALID,       /**< Worker Thread is in invalid state      */
    CY_WORKER_THREAD_VALID,         /**< Worker Thread is in valid state        */
    CY_WORKER_THREAD_ENQUEUING,     /**< Worker Thread is adding to the queue   */
    CY_WORKER_THREAD_TERMINATING,   /**< Worker Thread is starting to terminate */
    CY_WORKER_THREAD_JOIN_COMPLETE  /**< Worker Thread join is complete         */
} cy_worker_thread_state_t;

/** Worker Thread Parameters. */
typedef struct
{
    cy_thread_priority_t priority;     /**< Requested thread priority. */
    uint32_t             stack_size;   /**< Size of stack for new thread.
                                            Note that this must be atleast CY_RTOS_MIN_STACK_SIZE */
    uint8_t*             stack;        /**< Pointer to stack. If this is NULL a stack of
                                            size \ref stack_size will be allocated. */
    const char*          name;         /**< Thread name. If set to NULL,
                                            \ref CY_WORKER_THREAD_DEFAULT_NAME will be used. */
    uint32_t             num_entries;  /**< Maximum number of enteries the worker thread can queue.
                                            If set to 0, \ref CY_WORKER_DEFAULT_ENTRIES
                                            will be used.       */
} cy_worker_thread_params_t;

/** Worker Thread Information. */
typedef struct
{
    cy_queue_t               event_queue;    /**< Event Queue for this thread */
    uint32_t                 enqueue_count;  /**< Number of conccurent enqueue requests */
    cy_thread_t              thread;         /**< Thread object               */
    cy_worker_thread_state_t state;          /**< State of the worker thread  */
} cy_worker_thread_info_t;

/** Create worker thread to handle running callbacks in a separate thread.
 *
 * @note Calling this function twice on the same thread object ( \ref cy_worker_thread_info_t)
 * without
 * calling \ref cy_worker_thread_delete will cause memory leakage.
 *
 * @param[out] new_worker   pointer to cy_worker_thread_info_t structure to be filled when created.
 * @param[in]  params       pointer to requested parameters for starting worker thread.
 *
 * @return The status of the worker thread creation request.
 */
cy_rslt_t cy_worker_thread_create(cy_worker_thread_info_t* new_worker,
                                  const cy_worker_thread_params_t* params);

/** Delete worker thread.
 *
 * @note This function will wait for the thread to complete all pending work in the
 * queue and exit before returning.
 *
 * @param[in] old_worker    pointer to cy_worker_thread_info_t structure to be deleted.
 *
 * @return The status of the deletion of the worker thread.
 */
cy_rslt_t cy_worker_thread_delete(cy_worker_thread_info_t* old_worker);

/** Queue work on a worker thread.
 *
 * Call the given function in the worker thread context.
 *
 * @note If the thread priority is below that of the current thread, you must yield to allow
 * the worker thread to run. This can be done by calling \ref cy_rtos_delay_milliseconds or
 * by waiting on an RTOS object in all higher priority threads.
 *
 * @param[in] worker_info    pointer to worker_thread used to run function
 * @param[in] work_func      function to run
 * @param[in] arg            opaque arg to be used in function call
 *
 * @return The status of the queueing of work.
 */
cy_rslt_t cy_worker_thread_enqueue(cy_worker_thread_info_t* worker_info,
                                   cy_worker_thread_func_t* work_func, void* arg);

/** @} */

#ifdef __cplusplus
}
#endif

/** \} group_abstraction_resource */
