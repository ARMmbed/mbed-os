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

/* -------------------------------------- Includes ----------------------------------- */

#include "psa_defs.h"
#include "cmsis_os2.h"
#include "mbed_atomic.h"
#include "spm_internal.h"
#include "spm_panic.h"
#include "handles_manager.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


/* ------------------------------------ Definitions ---------------------------------- */

#define PSA_HANDLE_MGR_HANDLE_INDEX_POS         16
#define PSA_HANDLE_MGR_HANDLE_INDEX_MSK         0xFFFF


/* ------------------------------------- Functions ----------------------------------- */


psa_handle_t psa_hndl_mgr_handle_create(psa_handle_manager_t *handle_mgr, void *handle_mem, int32_t friend_pid)
{
    // Make sanity checks on arguments
    SPM_ASSERT(handle_mgr != NULL);
    SPM_ASSERT(handle_mem != NULL);

    // Get active partition id - Needed for requester identification
    spm_partition_t *curr_part_ptr = get_active_partition();
    int32_t          current_pid   = ((curr_part_ptr != NULL) ? curr_part_ptr->partition_id : PSA_NSPE_IDENTIFIER);

    // Generate a new handle identifier
    uint32_t tmp_handle;
    do {
        tmp_handle = core_util_atomic_incr_u16(&(handle_mgr->handle_generator), 1);
    } while (tmp_handle == PSA_HANDLE_MGR_INVALID_HANDLE);
    psa_handle_t new_handle = PSA_NULL_HANDLE;

    // Look for a vacant space in handles pool for the generated handle
    for (uint32_t pool_ix = 0; pool_ix < handle_mgr->pool_size; pool_ix++) {

        psa_handle_t expected = PSA_NULL_HANDLE;

        // Write the handles pool index in the upper 16 bits of the handle
        psa_handle_t desired_handle = ((pool_ix << PSA_HANDLE_MGR_HANDLE_INDEX_POS) | tmp_handle);

        // Store the generated handle in the handles pool
        if (core_util_atomic_cas_s32(&(handle_mgr->handles_pool[pool_ix].handle),
                                     &expected,
                                     desired_handle
                                    )) {

            // Handle is successfully stored in handles pool
            new_handle = desired_handle;

            // Store the handle memory in the handles pool, "coupled" with the stored handle
            handle_mgr->handles_pool[pool_ix].handle_mem    = handle_mem;
            handle_mgr->handles_pool[pool_ix].handle_owner  = current_pid;
            handle_mgr->handles_pool[pool_ix].handle_friend = friend_pid;

            break;
        }

        // Occupied index in handles pool - continue looping
    }

    // Handle creation should only occur after a successful memory allocation
    // and is not expected to fail.
    SPM_ASSERT(new_handle != PSA_NULL_HANDLE);

    return new_handle;
}


void psa_hndl_mgr_handle_destroy(psa_handle_manager_t *handle_mgr, psa_handle_t handle)
{
    // Make sanity checks on arguments
    SPM_ASSERT(handle_mgr != NULL);
    SPM_ASSERT(handle != PSA_NULL_HANDLE);


    // Get the handle's index in the handles pool
    uint32_t pool_ix = ((handle >> PSA_HANDLE_MGR_HANDLE_INDEX_POS) & PSA_HANDLE_MGR_HANDLE_INDEX_MSK);
    if (pool_ix >= handle_mgr->pool_size) {
        SPM_PANIC("[ERROR] Handle's index [%lu] is bigger than handles pool size [%hu]! \n", pool_ix, handle_mgr->pool_size);
    }

    if (handle_mgr->handles_pool[pool_ix].handle != handle) {
        SPM_PANIC("[ERROR] Handle %ld is not found in expected index! \n", handle);
    }

    // Get active partition id - Needed for requester identification
    spm_partition_t *curr_part_ptr = get_active_partition();
    int32_t          current_pid   = ((curr_part_ptr != NULL) ? curr_part_ptr->partition_id : PSA_NSPE_IDENTIFIER);

    if ((handle_mgr->handles_pool[pool_ix].handle_owner != current_pid) &&
            (handle_mgr->handles_pool[pool_ix].handle_friend != current_pid)
       ) {
        SPM_PANIC("[ERROR] Request for destroy by non-owner or friend!\n");
    }

    handle_mgr->handles_pool[pool_ix].handle_owner  = PSA_HANDLE_MGR_INVALID_FRIEND_OWNER;
    handle_mgr->handles_pool[pool_ix].handle_friend = PSA_HANDLE_MGR_INVALID_FRIEND_OWNER;
    core_util_atomic_store_s32(&(handle_mgr->handles_pool[pool_ix].handle), PSA_NULL_HANDLE);
}


void *psa_hndl_mgr_handle_get_mem(psa_handle_manager_t *handle_mgr, psa_handle_t handle)
{
    SPM_ASSERT(handle_mgr != NULL);

    if (handle == PSA_NULL_HANDLE) {
        SPM_PANIC("[ERROR] Trying to get memory for an invalid handle! \n");
    }

    // Get the handle's index in the handles pool
    uint32_t pool_ix = ((handle >> PSA_HANDLE_MGR_HANDLE_INDEX_POS) & PSA_HANDLE_MGR_HANDLE_INDEX_MSK);
    if (pool_ix >= handle_mgr->pool_size) {
        SPM_PANIC("[ERROR] Handle's index [%lu] is bigger than handles pool size [%hu]! \n", pool_ix, handle_mgr->pool_size);
    }

    if (handle_mgr->handles_pool[pool_ix].handle != handle) {
        SPM_PANIC("[ERROR] Handle %ld is not found in expected index! \n", handle);
    }

    // Get active partition id - Needed for requester identification
    spm_partition_t *curr_part_ptr = get_active_partition();
    int32_t          current_pid   = ((curr_part_ptr != NULL) ? curr_part_ptr->partition_id : PSA_NSPE_IDENTIFIER);

    if ((current_pid != handle_mgr->handles_pool[pool_ix].handle_owner) &&
            (current_pid != handle_mgr->handles_pool[pool_ix].handle_friend)
       ) {
        SPM_PANIC("[ERROR] Request for handle memory is not allowed for this partition! \n");
    }

    // If a valid handle is "coupled" with a NULL handle memory then
    // it is an internal module error or memory was overwritten --> Assert
    SPM_ASSERT(handle_mgr->handles_pool[pool_ix].handle_mem != NULL);

    return handle_mgr->handles_pool[pool_ix].handle_mem;
}
