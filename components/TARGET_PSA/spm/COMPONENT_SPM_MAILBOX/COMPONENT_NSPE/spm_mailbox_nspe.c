/* Copyright (c) 2017-2018 ARM Limited
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

#include "cmsis_os2.h"
#include "psa_defs.h"
#include "mbed_assert.h"
#include "ipc_queue.h"
#include "ipc_defs.h"
#include "spm_api.h"

static os_mutex_t queue_mutex_storage;
static os_semaphore_t full_sema;
static os_semaphore_t read_sema;

static const osMutexAttr_t queue_mutex_attr = {
    .name      = "Q_MUT",
    .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
    .cb_mem    = &queue_mutex_storage,
    .cb_size   = sizeof(queue_mutex_storage)
};

// Full queue semaphore attributes for the consumer queue
static const osSemaphoreAttr_t full_sem_attr = {
    .name      = "Q_W_SEM",
    .attr_bits = 0,
    .cb_mem    = &full_sema,
    .cb_size   = sizeof(full_sema)
};

// Read semaphore attributes for the consumer queue
static const osSemaphoreAttr_t read_sem_attr = {
    .name      = "Q_R_SEM",
    .attr_bits = 0,
    .cb_mem    = &read_sema,
    .cb_size   = sizeof(read_sema)
};

static ipc_producer_queue_t _prod_queue;
ipc_producer_queue_t *prod_queue = &_prod_queue;
static ipc_consumer_queue_t _cons_queue;
ipc_consumer_queue_t *cons_queue = &_cons_queue;


void on_new_item(void)
{
    spm_hal_mailbox_notify();
}

void on_vacancy(void)
{
    spm_hal_mailbox_notify();
}

void on_popped_item(ipc_queue_item_t item)
{
    osStatus_t os_status = osSemaphoreRelease((osSemaphoreId_t)(item.b));
    MBED_ASSERT(osOK == os_status);
    PSA_UNUSED(os_status);
}


void spm_ipc_mailbox_init(void)
{
    // Initialization by data from shared memory
    // -----------------------------------------

    // This table is holding addresses of the platform's shared memory.
    addr_table_t *shared_addr_table_ptr = (addr_table_t *)PSA_SHARED_RAM_START;
    MBED_ASSERT(shared_addr_table_ptr->magic == ADDR_TABLE_MAGIC);

    ipc_base_queue_t *tx_queue_mem_ptr = (ipc_base_queue_t *)(shared_addr_table_ptr->tx_queue_ptr);
    MBED_ASSERT(tx_queue_mem_ptr->magic == IPC_QUEUE_BASE_MAGIC);

    ipc_base_queue_t *rx_queue_mem_ptr = (ipc_base_queue_t *)(shared_addr_table_ptr->rx_queue_ptr);
    MBED_ASSERT(rx_queue_mem_ptr->magic == IPC_QUEUE_BASE_MAGIC);

    osMutexId_t queue_mutex = osMutexNew(&queue_mutex_attr);
    MBED_ASSERT(queue_mutex != NULL);    // TODO: Panic instead

    osSemaphoreId_t full_queue_sem = osSemaphoreNew(IPC_QUEUE_SEM_MAX_COUNT, IPC_QUEUE_SEM_INITIAL_COUNT, &full_sem_attr);
    MBED_ASSERT(full_queue_sem != NULL);    // TODO: Panic instead

    osSemaphoreId_t queue_read_sem = osSemaphoreNew(IPC_QUEUE_SEM_MAX_COUNT, IPC_QUEUE_SEM_INITIAL_COUNT, &read_sem_attr);
    MBED_ASSERT(queue_read_sem != NULL);    // TODO: Panic instead

    ipc_producer_queue_init(prod_queue, tx_queue_mem_ptr, queue_mutex, full_queue_sem);
    ipc_consumer_queue_init(cons_queue, rx_queue_mem_ptr, queue_read_sem);
}

void spm_mailbox_irq_callback(void)
{
    osStatus_t os_status = osSemaphoreRelease(prod_queue->full_queue_sem);
    MBED_ASSERT((osOK == os_status) || (osErrorResource == os_status));

    os_status = osSemaphoreRelease(cons_queue->read_sem);
    MBED_ASSERT((osOK == os_status) || (osErrorResource == os_status));

    PSA_UNUSED(os_status);
}

/*******************************************************************************
* Function Name: psa_spm_mailbox_dispatcher
*******************************************************************************/
void psa_spm_mailbox_dispatcher(void *arg)
{
    while (true) {
        ipc_queue_drain(cons_queue);
    }
}
