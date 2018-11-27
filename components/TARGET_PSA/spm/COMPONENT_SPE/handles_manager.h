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

#ifndef __MBED_HANDLE_MANAGER_H__
#define __MBED_HANDLE_MANAGER_H__

/* -------------------------------------- Includes ----------------------------------- */

#include "psa_defs.h"

#include <stdint.h>

/* -------------------------------- Handle Manager Module ---------------------------- */

/*
 * It generates and exposes a unique handle identifier (handle) per
 * handle memory (handle_mem) it receives from the user.
 * Then users can use the exposed handle identifier to relate to the "registered"
 * handle memory.
 *
 * Users can:
 * - Ask for a unique handle identifier for a given handle memory [handle_create]
 * - Ask for a pointer to the handle memory corresponding to a
 *   handle identifier [handle_get_mem]
 * - Remove a handle from the handle manager module [handle_destroy]
 *
 * Note:
 * Handle generation is done exclusively.
 * Once you got a handle, removing a handle or getting its memory can be
 * done non-exclusive.
 * The assumption is that only one context is dealing with a handle after it was
 * generated.
 */

/* --------------------------------- extern "C" wrapper ------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif



/* ------------------------------------ Definitions ---------------------------------- */

#define PSA_HANDLE_MGR_INVALID_HANDLE           ((uint32_t)PSA_NULL_HANDLE)

#define PSA_HANDLE_MGR_INVALID_FRIEND_OWNER     0       // Denoting invalid friend or invalid owner

// Handles manager pool indexes must be in range 0 - 0x7FFF.
// The reason for this limitation is that the index is stored in the upper 16 bits of a handle,
// and the most significant bit must be zero to keep handles non negative.
#define PSA_HANDLE_MGR_MAX_HANDLES_NUM          0x8000



/* -------------------------------------- Structs ------------------------------------ */

typedef struct psa_handle_item_t {

    psa_handle_t  handle;           /* The user exposed handle [unique identifier] */
    int32_t       handle_owner;     /* The partition id of the handle creator - allowed to get_mem() / destroy() */
    int32_t       handle_friend;    /* The partition id of a "friend" partition - allowed to get_mem() */
    void         *handle_mem;       /* Points to memory allocated by the user */

} psa_handle_item_t;


typedef struct psa_handle_manager_t {

    uint32_t           handle_generator;    /* A counter supplying handle numbers  */
    uint32_t           pool_size;           /* The maximum number of handles that pool can contain */
    psa_handle_item_t *handles_pool;        /* Holding couples of handles and their memory "blocks" */

} psa_handle_manager_t;


/*
handles_pool
    |
    |
    |
    -->  *--------------------------------------------------------------------------*
         |    handle      |    handle      |    handle      |       |       |  ...  |
         *--------------------------------------------------------------------------*
         |  handle_owner  |  handle_owner  |  handle_owner  |       |       |  ...  |
         *--------------------------------------------------------------------------*
         |  handle_friend |  handle_friend |  handle_friend |       |       |  ...  |
         *--------------------------------------------------------------------------*
         |  handle_mem    |  handle_mem    |  handle_mem    |       |       |  ...  |
         *--------------------------------------------------------------------------*
*/



/* ------------------------------------- Functions ----------------------------------- */


/*
 * @brief create unique handle identifier
 *
 * This function generates a unique handle identifier, and **couples** it with the received handle memory.
 * If there is no vacant space for the new handle, the function fails.
 *
 * @note This function is expected to pass since it is always coupled with memory pool allocation of the same size.
 *       In case memory pool allocation fails, this function should not be called.
 *       This function will panic on non vacant space use case.
 *
 * @param[in] handle_mgr   A pointer to the handle manager object.
 * @param[in] handle_mem   A pointer to a pre-allocated handle memory to get a handle identifier for
 * @param[in] friend_pid   The partition id which is allowed to `get_mem()` and `destroy()` in addition to the handle owner.
 *                          Use PSA_HANDLE_MGR_INVALID_FRIEND_OWNER to denote there is no friend partition.
 * @return The created handle identifier
 */
psa_handle_t psa_hndl_mgr_handle_create(psa_handle_manager_t *handle_mgr, void *handle_mem, int32_t friend_pid);


/*
 * @brief remove a handle from the handle manager.
 *
 * @param handle_mgr   A pointer to the handle manager object
 * @param handle       The handle to be removed
 */
void psa_hndl_mgr_handle_destroy(psa_handle_manager_t *handle_mgr, psa_handle_t handle);


/*
 * @brief dereference handle
 *
 * This function retrieves the pointer associated with the input <handle>.
 *
 * @note  This function will panic in case caller not allowed to dereference the memory
 *         or handler does not correspond to a valid existing handle
 *
 * @param handle_mgr   A pointer to the handle manager object.
 * @param handle       The handle for which you request the corresponding memory handle.
 * @return void*       A pointer to the memory corresponding to the handle.
 */
void *psa_hndl_mgr_handle_get_mem(psa_handle_manager_t *handle_mgr, psa_handle_t handle);


#ifdef __cplusplus
}
#endif

#endif  /* __MBED_HANDLE_MANAGER_H__ */
