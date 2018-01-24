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

/* -------------------------------------- Includes ----------------------------------- */

#include "psa_defs.h"
#include "cmsis_os2.h"
#include "mbed_critical.h"
#include "spm_internal.h"
#include "spm_panic.h"
#include "handles_manager.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>



/* -------------------------------- Handle Manager Module ---------------------------- */

/* The Handle Manager Module manages handles.
 *
 * It basically generates and exposes a unique handle identifier [handle] per
 * handle memory [handle_mem] it receives from the user.
 * Then users can use the exposed handle identifier to relate to the "registered"
 * handle memory.
 *
 * Users can:
 * - Initiate the module
 * - Ask for a unique handle identifier for a given handle memory [handle_create]
 * - Ask for a pointer to the handle memory corresponding to a
 *   handle identifier [handle_get_mem]
 * - Remove a handle from the handle manager module [handle_destroy]
 *
 * Note:
 * Handles generation is done exclusively.
 * Once we got a handle, removing a handle or getting its memory can be
 * done non-exclusive.
 * The assumption is that only one context is dealing with a handle after it was
 * generated.
 */

/* ------------------------------------- Functions ----------------------------------- */

/**********************************************************************************************************************************
 * Function   : psa_hndl_mgr_init
 *
 * Description: This function initializes a new handle manager object.
 *              It should be called by users who want to use the handle manager module.
 *
 * Parameters : handle_mgr         - [OUT] A pointer to a handle manager object, to be filled in with handle manager instance data.
 *                                         Should be allocated by the caller.
 *              handle_mgr_storage - [IN]  A pointer to the the module storage [the handles pool].
 *                                         Should be allocated by the caller.
 *              storage_size       - [IN]  Number of elements in <handle_mgr_storage>.
 *
 * Return     : Void
 *********************************************************************************************************************************/
void psa_hndl_mgr_init(psa_handle_manager_t *handle_mgr, psa_handle_item_t *handle_mgr_storage, size_t storage_size)
{
    /* Make sanity checks on arguments */

    SPM_ASSERT(handle_mgr != NULL);
    SPM_ASSERT(handle_mgr_storage != NULL);
    SPM_ASSERT(storage_size != 0);

    /* Reset and validate handle manager object */

    memset(handle_mgr_storage, 0, sizeof(*handle_mgr_storage));
    memset(handle_mgr, 0, sizeof(*handle_mgr));

    for (uint32_t idx = 0; idx < storage_size; idx++) {

        handle_mgr_storage[idx].handle_owner  = PSA_HANDLE_MGR_INVALID_FRIEND_OWNER;
        handle_mgr_storage[idx].handle_friend = PSA_HANDLE_MGR_INVALID_FRIEND_OWNER;
    }

    handle_mgr->handle_generator = PSA_HANDLE_MGR_INVALID_HANDLE;
    handle_mgr->pool_size        = storage_size;
    handle_mgr->handles_pool     = handle_mgr_storage;
}


/**********************************************************************************************************************************
 * Function   : psa_hndl_mgr_handle_create
 *
 * Description: This function generates a unique handle identifier, and "couples" it with the received handle memory.
 *              If there is no vacant space for the new handle, the function fails.
 *
 * Parameters : handle_mgr - [IN]  A pointer to the handle manager object
 *              handle_mem - [IN]  A pointer to a pre-allocated handle memory to get a handle identifier for
 *              friend_pid - [IN]  The partition id which is allowed to get_mem() in addition to the handle owner.
 *                                 Use PSA_HANDLE_MGR_INVALID_FRIEND_OWNER to denote there is no friend partition.
 *              handle     - [OUT] A pointer to a handle, to be set with the created handle identifier
 *
 * Return     : PSA_STATUS_SUCCESS on success. Error code on failure.
 *********************************************************************************************************************************/
error_t psa_hndl_mgr_handle_create(psa_handle_manager_t *handle_mgr, void *handle_mem, int32_t friend_pid, psa_handle_t *handle)
{
    uint32_t new_handle = PSA_HANDLE_MGR_INVALID_HANDLE;


    /* Make sanity checks on arguments */
    SPM_ASSERT(handle_mgr != NULL);
    SPM_ASSERT(handle_mem != NULL);
    SPM_ASSERT(handle != NULL);


    *handle = PSA_NULL_HANDLE;


    /* Get active partition id - Needed for requester identification */
    partition_t *curr_part_ptr = active_partition_get();
    int32_t      current_pid   = ((curr_part_ptr != NULL) ? curr_part_ptr->partition_id : PSA_NSPE_IDENTIFIER);
    uint32_t expected = INT32_MAX;
    /* Avoid passing INT32_MAX. Start again from 0 if reached.
     * The reason for that is that SPM which uses the handles manager will relate to a negative int32_t integer
     * as an error code and not as a valid handle.
     * */
    core_util_atomic_cas_u32( (uint32_t *)( &(handle_mgr->handle_generator) ),
                            &expected,
                            PSA_HANDLE_MGR_INVALID_HANDLE
                          );

    /* Generate a new handle identifier */
    new_handle = core_util_atomic_incr_u32(&(handle_mgr->handle_generator), 1);

    /* Look for a vacant space in handles pool for the generated handle */
    for(uint32_t pool_ix = 0; pool_ix < handle_mgr->pool_size; pool_ix++) {
        expected = PSA_HANDLE_MGR_INVALID_HANDLE;
        /* Store the generated handle in the handles pool */
        if(core_util_atomic_cas_u32( (uint32_t *)( &(handle_mgr->handles_pool[pool_ix].handle) ),
                                    &expected,
                                    new_handle
                                 )) {

            /* Handle is successfully stored in handles pool */

            /* Store the handle memory in the handles pool, "coupled" with the stored handle  */
            handle_mgr->handles_pool[pool_ix].handle_mem    = handle_mem;
            handle_mgr->handles_pool[pool_ix].handle_owner  = current_pid;
            handle_mgr->handles_pool[pool_ix].handle_friend = friend_pid;

            *handle = new_handle;

            return PSA_SUCCESS;
        }

        /* Occupied index in handles pool - continue looping */
    }


    /* No vacant space for new handle. No worries about the generated handle - it will not be used */

    return PSA_GENERIC_ERROR;
}


/**********************************************************************************************************************************
 * Function   : psa_hndl_mgr_handle_destroy
 *
 * Description: This function removes a handle from the handle manager.
 *
 * Parameters : handle_mgr - [IN]  A pointer to the handle manager object
 *              handle     - [IN]  The handle to be removed
 *
 * Return     : Void
 *********************************************************************************************************************************/
void psa_hndl_mgr_handle_destroy(psa_handle_manager_t *handle_mgr, psa_handle_t handle)
{
    /* Make sanity checks on arguments */
    SPM_ASSERT(handle_mgr != NULL);
    SPM_ASSERT(handle != PSA_NULL_HANDLE);


    /* Get active partition id - Needed for requester identification */
    partition_t *curr_part_ptr = active_partition_get();
    int32_t      current_pid   = ((curr_part_ptr != NULL) ? curr_part_ptr->partition_id : PSA_NSPE_IDENTIFIER);


    /* Look for <handle> in handles pool */
    for(uint32_t pool_ix = 0; pool_ix < handle_mgr->pool_size; pool_ix++) {

        if(handle_mgr->handles_pool[pool_ix].handle == handle) {

            if(handle_mgr->handles_pool[pool_ix].handle_owner != current_pid) {

                // The SPM_PANIC() macro will exit the program
                SPM_PANIC("[ERROR] Request for destroy by non-owner!\n");
            }

            /* Handle found in handles pool */

            handle_mgr->handles_pool[pool_ix].handle = PSA_NULL_HANDLE;
            handle_mgr->handles_pool[pool_ix].handle_owner  = PSA_HANDLE_MGR_INVALID_FRIEND_OWNER;
            handle_mgr->handles_pool[pool_ix].handle_friend = PSA_HANDLE_MGR_INVALID_FRIEND_OWNER;

            return;
        }
    }


    // Handle not found in handles pool - the SPM_PANIC() macro will exit the program

    SPM_PANIC("[ERROR] Handle not found %d! \n", (int)handle);
}


/**********************************************************************************************************************************
 * Function   : psa_hndl_mgr_handle_get_mem
 *
 * Description: This function looks for the handle memory corresponding to <handle>.
 *              If it is not found in the handles pool, the function fails.
 *
 * Parameters : handle_mgr - [IN]  A pointer to the handle manager object
 *              handle     - [IN]  The handle for which we request the corresponding memory handle
 *              handle_mem - [OUT] A pointer to pointer to be set with the requested memory handle
 *
 * Return     : Void
 *********************************************************************************************************************************/
void psa_hndl_mgr_handle_get_mem(psa_handle_manager_t *handle_mgr, psa_handle_t handle, void **handle_mem)
{
    /* Make sanity checks on arguments */

    SPM_ASSERT(handle_mgr != NULL);
    SPM_ASSERT(handle_mem != NULL);

    if(handle == PSA_NULL_HANDLE) {

        // The SPM_PANIC() macro will exit the program
        SPM_PANIC("[ERROR] Trying to get memory for an invalid handle! \n");
    }


    /* Get active partition id - Needed for requester identification */
    partition_t *curr_part_ptr = active_partition_get();
    int32_t      current_pid   = ((curr_part_ptr != NULL) ? curr_part_ptr->partition_id : PSA_NSPE_IDENTIFIER);


    *handle_mem = NULL;


    /* Look for <handle> in handles pool */
    for(uint32_t pool_ix = 0; pool_ix < handle_mgr->pool_size; pool_ix++) {

        if(handle_mgr->handles_pool[pool_ix].handle == handle) {

            /* Handle found in handles pool */

            if( (current_pid != handle_mgr->handles_pool[pool_ix].handle_owner) &&
                (current_pid != handle_mgr->handles_pool[pool_ix].handle_friend)
              ) {

                // The SPM_PANIC() macro will exit the program
                SPM_PANIC("[ERROR] Request for handle memory is not allowed for this partition! \n");
            }

            *handle_mem = handle_mgr->handles_pool[pool_ix].handle_mem;

            /* If a valid handle is "coupled" with a NULL handle memory then
             * it is an internal module error or memory was overwritten --> Assert */
            SPM_ASSERT(*handle_mem != NULL);

            return;
        }
    }


    // Handle not found in handles pool - the SPM_PANIC() macro will exit the program

    SPM_PANIC("[ERROR] Handle %d not found! \n", (int)handle);
}

