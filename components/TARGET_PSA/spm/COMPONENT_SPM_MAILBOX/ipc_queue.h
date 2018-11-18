/* Copyright (c) 2018 ARM Limited
 *
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
 */


#ifndef __PSA_MBED_IPC_QUEUE_H__
#define __PSA_MBED_IPC_QUEUE_H__


// Includes
// --------

#include "mbed_assert.h"
#include "cmsis_os2.h"
#include "cmsis_compiler.h"
#include "rtx_lib.h"


// IPC Queue Definitions
// ---------------------


#ifndef IPC_QUEUE_SLOTS
#define IPC_QUEUE_SLOTS             4
#endif

// Maximum number of queue items equals (IPC_QUEUE_SLOTS - 1).
// Therefore we enforce a minimum of 2 IPC_QUEUE_SLOTS
#if IPC_QUEUE_SLOTS <= 1
#undef IPC_QUEUE_SLOTS
#define IPC_QUEUE_SLOTS             2
#endif

#define IPC_QUEUE_BASE_MAGIC        0xCAFEBABE
#define IPC_QUEUE_PRODUCER_MAGIC    0xDAC0FFEE
#define IPC_QUEUE_CONSUMER_MAGIC    0xFACEB00C

#define IPC_QUEUE_WAIT_ON_FULL_MS   500
#define IPC_QUEUE_WAIT_ON_EMPTY_MS  500

#define IPC_QUEUE_SEM_MAX_COUNT     (1UL)   // Maximum number of available tokens for an IPC Queue semaphore
#define IPC_QUEUE_SEM_INITIAL_COUNT (0UL)   // Initial number of available tokens for an IPC Queue semaphore

// NOTE: STRUCT SIZE MUST BE 4 BYTES ALIGNED !!
typedef __PACKED_STRUCT ipc_queue_item_t {
    uint32_t  a;
    uint32_t  b;
    uint32_t  c;

} __ALIGNED(4) ipc_queue_item_t;
MBED_STATIC_ASSERT((sizeof(ipc_queue_item_t) % sizeof(uint32_t) == 0), "ipc_queue_item_t: Struct size must be 4 bytes aligned!");


// NOTE: STRUCT SIZE MUST BE 4 BYTES ALIGNED !!
typedef __PACKED_STRUCT ipc_base_queue_t {
    uint32_t           magic;
    volatile uint32_t  read_idx;
    volatile uint32_t  write_idx;
    ipc_queue_item_t   data[IPC_QUEUE_SLOTS];

} __ALIGNED(4) ipc_base_queue_t;
MBED_STATIC_ASSERT((sizeof(ipc_base_queue_t) % sizeof(uint32_t) == 0), "ipc_base_queue_t: Struct size must be 4 bytes aligned!");

typedef struct ipc_producer_queue_t {
    uint32_t                    magic;
    volatile __PACKED uint32_t  *read_idx;
    volatile __PACKED uint32_t  *write_idx;
    __PACKED ipc_queue_item_t   *data;
    osMutexId_t                 mutex;
    osSemaphoreId_t             full_queue_sem;

} ipc_producer_queue_t;

typedef struct ipc_consumer_queue_t {
    uint32_t                    magic;
    volatile __PACKED uint32_t  *read_idx;
    volatile __PACKED uint32_t  *write_idx;
    __PACKED ipc_queue_item_t   *data;
    osSemaphoreId_t             read_sem;

} ipc_consumer_queue_t;


// Event handling functions
// ------------------------

void on_new_item(void);
void on_vacancy(void);
void on_popped_item(ipc_queue_item_t item);


// IPC Queue API
// -------------

void ipc_producer_queue_init(ipc_producer_queue_t *queue,
                             ipc_base_queue_t     *base_queue_mem,
                             osMutexId_t           mutex,
                             osSemaphoreId_t       full_queue_sem
                            );

void ipc_consumer_queue_init(ipc_consumer_queue_t *queue,
                             ipc_base_queue_t     *base_queue_mem,
                             osSemaphoreId_t       read_sem
                            );

void ipc_queue_enqueue(ipc_producer_queue_t *queue,
                       ipc_queue_item_t      item_ptr
                      );

void ipc_queue_drain(ipc_consumer_queue_t *queue);


#endif  // __PSA_MBED_IPC_QUEUE_H__
