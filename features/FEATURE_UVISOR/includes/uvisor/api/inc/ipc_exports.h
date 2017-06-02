/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
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
#ifndef __UVISOR_API_IPC_EXPORTS_H__
#define __UVISOR_API_IPC_EXPORTS_H__

#include "api/inc/pool_queue_exports.h"
#include "api/inc/uvisor_spinlock_exports.h"
#include "api/inc/vmpu_exports.h"
#include <stddef.h>

/* Use the invalid box ID to mean "receive from any" box. */
#define UVISOR_BOX_ID_ANY UVISOR_BOX_ID_INVALID

#define UVISOR_IPC_SEND_SLOTS 16
#define UVISOR_IPC_RECV_SLOTS 16

/* The value UVISOR_IPC_INVALID_TOKEN is defined to should be useful as a null
 * token, and preferably not having any other legitimate use. Due to the
 * internal bitfield representation of tokens, it makes a lot of sense to use 0
 * as the null token. ipc_allocate_token() returns 0 if no tokens are
 * available. Freeing 0 bits means nothing ('&= ~0' is a no-op). No other value
 * makes as much sense to use as 0. If tokens become represented internally as
 * something other than a bitfield, it would make sense to reconsider the value
 * used here. */
#define UVISOR_IPC_INVALID_TOKEN 0

typedef enum uvisor_ipc_io_state {
    UVISOR_IPC_IO_STATE_INVALID,
    UVISOR_IPC_IO_STATE_IDLE,
    UVISOR_IPC_IO_STATE_READY_TO_SEND, /* Enqueued and waiting for delivery */
    UVISOR_IPC_IO_STATE_READY_TO_RECV, /* Enqueued and waiting for delivery */
    UVISOR_IPC_IO_STATE_VALID, /* uVisor has copied the message */
} uvisor_ipc_io_state_t;

/* IPC Descriptor Structure */
/* When sending:
 * @param[in]  box_id  the ID of the destination box
 * @param[in]  port    the port to send the message to
 * @param[in]  len     the length of the message
 * @param[out] token   a token that can be used to wait at a later time for
 *                     the send to complete
 */
/* When receiving before a message has been received:
 * @param[in]   box_id  an ID of a box that is allowed to send to this box, or
 *                      UVISOR_BOX_ID_ANY to allow messages from any box
 * @param[in]   port    the port to listen for messages on
 * @param[in]   len     the maximum length of message to receive
 * @param[out]  token   a token that can be used to wait at a later time for
 *                      the recv to complete
 *
 * When receiving after a message has been received:
 * @param[out]  box_id  the box ID of the sender
 * @param[out]  port    the port the message arrived on
 * @param[out]  len     the length of the message
 * @param[out]  token   not modified
 */
typedef struct uvisor_ipc_desc {
    int box_id;
    size_t port;
    size_t len;
    uint32_t token;
} uvisor_ipc_desc_t;

/* IPC IO Request Structure */
typedef struct uvisor_ipc_io {
    uvisor_ipc_desc_t * desc;
    void * msg;
    uvisor_ipc_io_state_t state;
} uvisor_ipc_io_t;

#define UVISOR_IPC_SEND_TYPE(slots) \
    struct { \
        uvisor_pool_queue_t queue; \
        uvisor_pool_t pool; \
        uvisor_pool_queue_entry_t entries[slots]; \
        uvisor_ipc_io_t io[slots]; \
    }

#define UVISOR_IPC_RECV_TYPE(slots) \
    struct { \
        uvisor_pool_queue_t queue; \
        uvisor_pool_t pool; \
        uvisor_pool_queue_entry_t entries[slots]; \
        uvisor_ipc_io_t io[slots]; \
    }

typedef UVISOR_IPC_SEND_TYPE(UVISOR_IPC_SEND_SLOTS) uvisor_ipc_send_queue_t;
typedef UVISOR_IPC_RECV_TYPE(UVISOR_IPC_RECV_SLOTS) uvisor_ipc_recv_queue_t;

typedef struct uvisor_ipc {
    uvisor_ipc_send_queue_t send_queue;
    uvisor_ipc_recv_queue_t recv_queue;
    UvisorSpinlock tokens_lock; /* Protect access to tokens. */
    uint32_t allocated_tokens; /* Endpoints read and write. */
    uint32_t completed_tokens; /* uVisor and endpoints read and write. */
} uvisor_ipc_t;

static inline uvisor_ipc_t * uvisor_ipc(UvisorBoxIndex * const index)
{
    return (uvisor_ipc_t *) index->bss.address_of.ipc;
}

#endif
