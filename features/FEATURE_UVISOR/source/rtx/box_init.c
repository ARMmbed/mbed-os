/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
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
#include "uvisor-lib/uvisor-lib.h"
#include "api/inc/pool_queue_exports.h"
#include "api/inc/rpc_exports.h"
#include "api/inc/uvisor_semaphore.h"
#include "mbed_interface.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <string.h>

/* Register the OS with uVisor */
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
extern uint32_t rt_suspend(void);

UVISOR_SET_PRIV_SYS_HOOKS(SVC_Handler, PendSV_Handler, SysTick_Handler, rt_suspend, __uvisor_semaphore_post);

extern RtxBoxIndex * const __uvisor_ps;

void __uvisor_initialize_rpc_queues(void)
{
    UvisorBoxIndex * const index = &__uvisor_ps->index;

    uvisor_pool_slot_t i;

    uvisor_rpc_outgoing_message_queue_t * rpc_outgoing_msg_queue = &(uvisor_rpc(index)->outgoing_message_queue);
    uvisor_rpc_incoming_message_queue_t * rpc_incoming_msg_queue = &(uvisor_rpc(index)->incoming_message_queue);
    uvisor_rpc_fn_group_queue_t * rpc_fn_group_queue = &(uvisor_rpc(index)->fn_group_queue);

    /* Initialize the outgoing RPC message queue. */
    if (uvisor_pool_queue_init(&rpc_outgoing_msg_queue->queue,
                               &rpc_outgoing_msg_queue->pool,
                               rpc_outgoing_msg_queue->messages,
                               sizeof(*rpc_outgoing_msg_queue->messages),
                               UVISOR_RPC_OUTGOING_MESSAGE_SLOTS,
                               UVISOR_POOL_QUEUE_BLOCKING)) {
        uvisor_error(USER_NOT_ALLOWED);
    }

    /* Initialize all the result semaphores. */
    for (i = 0; i < UVISOR_RPC_OUTGOING_MESSAGE_SLOTS; i++) {
        UvisorSemaphore * semaphore = &rpc_outgoing_msg_queue->messages[i].semaphore;
        if (__uvisor_semaphore_init(semaphore, 1)) {
            uvisor_error(USER_NOT_ALLOWED);
        }

        /* Semaphores are created with their value initialized to count. We
         * want the semaphore to start at zero. Decrement the semaphore, so it
         * starts with a value of zero. This will allow the first pend to
         * block. */
        if (__uvisor_semaphore_pend(semaphore, 0)) {
            uvisor_error(USER_NOT_ALLOWED);
        }
    }

    /* Initialize the incoming RPC message queues. */
    if (uvisor_pool_queue_init(&rpc_incoming_msg_queue->todo_queue,
                               &rpc_incoming_msg_queue->pool,
                               rpc_incoming_msg_queue->messages,
                               sizeof(*rpc_incoming_msg_queue->messages),
                               UVISOR_RPC_INCOMING_MESSAGE_SLOTS,
                               UVISOR_POOL_QUEUE_NON_BLOCKING)) {
        uvisor_error(USER_NOT_ALLOWED);
    }
    /* This is a double init of the pool. We need a function that just inits
     * the queue, not the pool, and init everybody separately. */
    if (uvisor_pool_queue_init(&rpc_incoming_msg_queue->done_queue,
                               &rpc_incoming_msg_queue->pool,
                               rpc_incoming_msg_queue->messages,
                               sizeof(*rpc_incoming_msg_queue->messages),
                               UVISOR_RPC_INCOMING_MESSAGE_SLOTS,
                               UVISOR_POOL_QUEUE_NON_BLOCKING)) {
        uvisor_error(USER_NOT_ALLOWED);
    }

    /* Initialize the function group pool. */
    if (uvisor_pool_queue_init(&rpc_fn_group_queue->queue,
                               &rpc_fn_group_queue->pool,
                               rpc_fn_group_queue->fn_groups,
                               sizeof(*rpc_fn_group_queue->fn_groups),
                               UVISOR_RPC_FN_GROUP_SLOTS,
                               UVISOR_POOL_QUEUE_BLOCKING)) {
        uvisor_error(USER_NOT_ALLOWED);
    }

    /* Initialize all the function group semaphores. */
    for (i = 0; i < UVISOR_RPC_FN_GROUP_SLOTS; i++) {
        UvisorSemaphore * semaphore = &rpc_fn_group_queue->fn_groups[i].semaphore;
        if (__uvisor_semaphore_init(semaphore, 1)) {
            uvisor_error(USER_NOT_ALLOWED);
        }

        /* Semaphores are created with their value initialized to count. We
         * want the semaphore to start at zero. Decrement the semaphore, so it
         * starts with a value of zero. This will allow the first pend to
         * block. */
        if (__uvisor_semaphore_pend(semaphore, 0)) {
            uvisor_error(USER_NOT_ALLOWED);
        }
    }
}

/* This function is called by uVisor in unprivileged mode. On this OS, we
 * create box main threads for the box. */
void __uvisor_lib_box_init(void * lib_config)
{
    osThreadId thread_id;
    osThreadDef_t * flash_thread_def = lib_config;
    osThreadDef_t thread_def;

    __uvisor_initialize_rpc_queues();

    /* Copy thread definition from flash to RAM. The thread definition is most
     * likely in flash, so we need to copy it to box-local RAM before we can
     * modify it. */
    memcpy(&thread_def, flash_thread_def, sizeof(thread_def));

    /* Note that the box main thread stack is separate from the box stack. This
     * is because the thread must be created to use a different stack than the
     * stack osCreateThread() is called from, as context information is saved
     * to the thread stack by the call to osCreateThread(). */
    /* Allocate memory for the main thread from the process heap (which is
     * private to the process). This memory is never freed, even if the box's
     * main thread exits. */
    thread_def.stack_pointer = malloc_p(thread_def.stacksize);

    if (thread_def.stack_pointer == NULL) {
        /* No process heap memory available */
        mbed_die();
    }

    thread_id = osThreadCreate(&thread_def, NULL);

    if (thread_id == NULL) {
        /* Failed to create thread */
        mbed_die();
    }
}
