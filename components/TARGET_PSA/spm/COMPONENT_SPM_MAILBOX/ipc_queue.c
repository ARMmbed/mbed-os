/* Copyright (c) 2018 ARM Limited
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


// Includes
// --------

#include "mbed_assert.h"
#include "ipc_queue.h"
#include "psa_defs.h"


// API Implmentation
// -----------------

void ipc_producer_queue_init( ipc_producer_queue_t *queue,
                              ipc_base_queue_t     *base_queue_mem,
                              osMutexId_t           mutex,
                              osSemaphoreId_t       full_queue_sem
                            )
{
    MBED_ASSERT(queue != NULL);
    MBED_ASSERT(base_queue_mem != NULL);
    MBED_ASSERT(base_queue_mem->magic == IPC_QUEUE_BASE_MAGIC);
    MBED_ASSERT(mutex != NULL);
    MBED_ASSERT(full_queue_sem != NULL);

    queue->magic          = IPC_QUEUE_PRODUCER_MAGIC;
    queue->read_idx       = &(base_queue_mem->read_idx);
    queue->write_idx      = &(base_queue_mem->write_idx);
    queue->data           = base_queue_mem->data;
    queue->mutex          = mutex;
    queue->full_queue_sem = full_queue_sem;
}

void ipc_consumer_queue_init( ipc_consumer_queue_t *queue,
                              ipc_base_queue_t     *base_queue_mem,
                              osSemaphoreId_t       read_sem
                            )
{
    MBED_ASSERT(queue != NULL);
    MBED_ASSERT(base_queue_mem != NULL);
    MBED_ASSERT(base_queue_mem->magic == IPC_QUEUE_BASE_MAGIC);
    MBED_ASSERT(read_sem != NULL);

    queue->magic          = IPC_QUEUE_CONSUMER_MAGIC;
    queue->read_idx       = &(base_queue_mem->read_idx);
    queue->write_idx      = &(base_queue_mem->write_idx);
    queue->data           = base_queue_mem->data;
    queue->read_sem       = read_sem;
}

void ipc_queue_enqueue( ipc_producer_queue_t *queue,
                        ipc_queue_item_t      queue_item
                      )
{
    MBED_ASSERT(queue != NULL);
    MBED_ASSERT(queue->magic == IPC_QUEUE_PRODUCER_MAGIC);

    osStatus_t os_status = osMutexAcquire(queue->mutex, osWaitForever);
    MBED_ASSERT(osOK == os_status);

    // While queue is full, wait on full_queue_sem
    while (((*(queue->write_idx) + 1) % IPC_QUEUE_SLOTS) == *(queue->read_idx)) {
        os_status = osSemaphoreAcquire(queue->full_queue_sem, IPC_QUEUE_WAIT_ON_FULL_MS);
        MBED_ASSERT((osOK == os_status) || (osErrorTimeout == os_status));
    }

    // Write data to queue (shallow copy)
    (queue->data)[*(queue->write_idx)] = queue_item;
    *(queue->write_idx) = ((*(queue->write_idx) + 1) % IPC_QUEUE_SLOTS);

    // If the queue was empty before the push, call the supplied CB function
    if (((*(queue->read_idx) + 1) % IPC_QUEUE_SLOTS) == *(queue->write_idx)) {
        on_new_item();
    }

    os_status = osMutexRelease(queue->mutex);
    MBED_ASSERT(osOK == os_status);
    PSA_UNUSED(os_status);
}

void ipc_queue_drain( ipc_consumer_queue_t *queue)
{
    MBED_ASSERT(queue != NULL);
    MBED_ASSERT(queue->magic == IPC_QUEUE_CONSUMER_MAGIC);

    // queue->read_sem is released when the queue becomes non empty
    osStatus_t os_status = osSemaphoreAcquire(queue->read_sem, IPC_QUEUE_WAIT_ON_EMPTY_MS);
    MBED_ASSERT((osOK == os_status) || (osErrorTimeout == os_status));
    PSA_UNUSED(os_status);

    // While queue is not empty, keep on popping queue items
    while (*(queue->read_idx) != *(queue->write_idx)) {

        // Pop an item from the queue (shallow copy)
        ipc_queue_item_t popped_item = (queue->data)[*(queue->read_idx)];
        *(queue->read_idx) = ((*(queue->read_idx) + 1) % IPC_QUEUE_SLOTS);

        // If queue was full before this pop, call the corresponding CB function
        if (((*(queue->write_idx) + 2) % IPC_QUEUE_SLOTS) == *(queue->read_idx)) {
            on_vacancy();
        }

        on_popped_item(popped_item);
    }
}
