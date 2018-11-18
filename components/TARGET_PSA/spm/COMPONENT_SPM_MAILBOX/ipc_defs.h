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


#ifndef __PSA_MBED_IPC_DEFS_H__
#define __PSA_MBED_IPC_DEFS_H__


// Includes
// --------

#include <stdint.h>

#include "mbed_assert.h"


// Definitions
// -----------

#define IPC_RES_SEM_MAX_COUNT       (1UL) // Maximum number of available tokens for an IPC result semaphore
#define IPC_RES_SEM_INITIAL_COUNT   (0UL) // Initial number of available tokens for an IPC result semaphore

#define ADDR_TABLE_MAGIC            0xDEADBEEF


// NOTE: STRUCT SIZE MUST BE 4 BYTES ALIGNED !!
typedef struct addr_table_t {
    uint32_t  magic;
    uintptr_t tx_queue_ptr;
    uintptr_t rx_queue_ptr;

} addr_table_t;
MBED_STATIC_ASSERT((sizeof(addr_table_t) % sizeof(uint32_t) == 0), "addr_table_t: Struct size must be 4 bytes aligned!");


#endif  // __PSA_MBED_IPC_DEFS_H__
