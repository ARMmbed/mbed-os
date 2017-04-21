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
#ifndef __UVISOR_API_RPC_EXPORTS_H__
#define __UVISOR_API_RPC_EXPORTS_H__

#include "api/inc/pool_queue_exports.h"
#include "api/inc/uvisor_semaphore_exports.h"
#include "api/inc/rpc_gateway_exports.h"
#include "api/inc/vmpu_exports.h"

typedef uint32_t (*TFN_Ptr)(uint32_t, uint32_t, uint32_t, uint32_t);

#define UVISOR_RESULT_SLOT_BITS 10
#define UVISOR_RESULT_SLOT_MASK ((1 << UVISOR_RESULT_SLOT_BITS) - 1)

#define UVISOR_RESULT_COUNTER_MASK (0xFFFFFFFFUL << UVISOR_RESULT_SLOT_BITS)

/* Increment by 2 because we never want to overflow into the invalid value. */
#define UVISOR_RESULT_COUNTER_INCREMENT (2 << UVISOR_RESULT_SLOT_BITS)

#define UVISOR_RESULT_INVALID_COUNTER (UVISOR_RESULT_COUNTER_MASK)

/* This is the token to wait on for the result of an asynchronous RPC. */
typedef uint32_t uvisor_rpc_result_t;

static inline uvisor_pool_slot_t uvisor_result_slot(uvisor_rpc_result_t result)
{
    return result & UVISOR_RESULT_SLOT_MASK;
}

static inline uint32_t uvisor_result_counter(uvisor_rpc_result_t result)
{
    return result & UVISOR_RESULT_COUNTER_MASK;
}

static inline uvisor_rpc_result_t uvisor_result_build(uint32_t counter, uvisor_pool_slot_t slot)
{
    return (counter & UVISOR_RESULT_COUNTER_MASK) | (slot & UVISOR_RESULT_SLOT_MASK);
}

typedef enum {
    /* Who sets this value for caller (outgoing queue), Who sets this value for (incoming queue) callee. */
    UVISOR_RPC_MESSAGE_STATE_INVALID, /* nobody, nobody */
    UVISOR_RPC_MESSAGE_STATE_IDLE, /* caller receive function before freeing, uvisor when delivers back */
    UVISOR_RPC_MESSAGE_STATE_READY_TO_SEND, /* send function, nobody */
    UVISOR_RPC_MESSAGE_STATE_SENT, /* uvisor, uvisor */
    UVISOR_RPC_MESSAGE_STATE_DONE, /* waitfor_fngroup function, uvisor when delivers back */
} uvisor_rpc_message_state_t;

typedef struct uvisor_rpc_message {
    /* NOTE: These are set by the caller, and read by the callee. */
    uint32_t p0;
    uint32_t p1;
    uint32_t p2;
    uint32_t p3;

    const TRPCGateway * gateway;

    /* The box ID of the other box. For callers, this is the destination box
     * ID. For callees, this is the source box ID. */
    int other_box_id;

    /* The semaphore to post to when a result is ready */
    UvisorSemaphore semaphore;

    /* This cookie keeps track of which result to wait for. It changes
     * atomically to an invalid cookie when being waited on, to prevent
     * multiple waits for the same result. */
    uvisor_rpc_result_t wait_cookie;

    /* This is an extra copy of the above cookie, used by uVisor to verify that
     * a certain result matches a certain caller. This identifies to uVisor
     * which RPC it should complete. uVisor must verify this information of
     * course, to see if this box is currently being called into and is allowed
     * to complete the RPC. */
    uvisor_rpc_result_t match_cookie;

    uvisor_rpc_message_state_t state;

    uint32_t result;
} uvisor_rpc_message_t;

typedef struct uvisor_rpc_fn_group {
    /* A pointer to the function group */
    TFN_Ptr const * fn_ptr_array;
    size_t fn_count;

    /* The semaphore to wait on for this function group */
    UvisorSemaphore semaphore;
} uvisor_rpc_fn_group_t;

#define UVISOR_RPC_OUTGOING_MESSAGE_SLOTS (8)

#define UVISOR_RPC_INCOMING_MESSAGE_SLOTS (8)

#define UVISOR_RPC_FN_GROUP_SLOTS (8)

#define UVISOR_RPC_OUTGOING_MESSAGE_TYPE(slots) \
    struct { \
        uvisor_pool_queue_t queue; \
        uvisor_pool_t pool; \
        uvisor_pool_queue_entry_t entries[slots]; \
        uvisor_rpc_message_t messages[slots]; \
    }

#define UVISOR_RPC_INCOMING_MESSAGE_TYPE(slots) \
    struct { \
        uvisor_pool_queue_t todo_queue; \
        uvisor_pool_queue_t done_queue; \
        uvisor_pool_t pool; \
        uvisor_pool_queue_entry_t entries[slots]; \
        uvisor_rpc_message_t messages[slots]; \
    }

#define UVISOR_RPC_FN_GROUP_TYPE(slots) \
    struct { \
        uvisor_pool_queue_t queue; \
        uvisor_pool_t pool; \
        uvisor_pool_queue_entry_t entries[slots]; \
        uvisor_rpc_fn_group_t fn_groups[slots]; \
    }

typedef UVISOR_RPC_OUTGOING_MESSAGE_TYPE(UVISOR_RPC_OUTGOING_MESSAGE_SLOTS) uvisor_rpc_outgoing_message_queue_t;
typedef UVISOR_RPC_INCOMING_MESSAGE_TYPE(UVISOR_RPC_INCOMING_MESSAGE_SLOTS) uvisor_rpc_incoming_message_queue_t;
typedef UVISOR_RPC_FN_GROUP_TYPE(UVISOR_RPC_FN_GROUP_SLOTS) uvisor_rpc_fn_group_queue_t;

typedef struct uvisor_rpc_t {
    /* Outgoing message queue */
    uvisor_rpc_outgoing_message_queue_t outgoing_message_queue;

    /* Incoming message queue */
    uvisor_rpc_incoming_message_queue_t incoming_message_queue;

    /* Function group queue */
    uvisor_rpc_fn_group_queue_t fn_group_queue;

    /* Counter to avoid waiting on the same RPC result twice by accident. */
    uint32_t result_counter;
} uvisor_rpc_t;

static inline uvisor_rpc_t * uvisor_rpc(UvisorBoxIndex * const index)
{
    return (uvisor_rpc_t *) index->bss.address_of.rpc;
}

#endif
