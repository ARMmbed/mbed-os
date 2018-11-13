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

#ifndef __MBED_HANDLE_MANAGER_H__
#define __MBED_HANDLE_MANAGER_H__

/* -------------------------------------- Includes ----------------------------------- */

#include "psa_defs.h"

#include <stdint.h>



/* --------------------------------- extern "C" wrapper ------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif



/* ------------------------------------ Definitions ---------------------------------- */

#define PSA_HANDLE_MGR_INVALID_HANDLE           ((uint32_t)PSA_NULL_HANDLE)

#define PSA_HANDLE_MGR_INVALID_FRIEND_OWNER     0       // Denoting invalid friend or invalid owner

#define PSA_HANDLE_MGR_MAX_HANDLES_NUM          0x8000  // Handles manager pool indexes must be in range 0 - 0x7FFF.
                                                        // The reason for this limitation is that the index is stored in the upper 16 bits of a handle,
                                                        // and the most significant bit must be zero to keep handles non negative.



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

psa_handle_t psa_hndl_mgr_handle_create(psa_handle_manager_t *handle_mgr, void *handle_mem, int32_t friend_pid);
void psa_hndl_mgr_handle_destroy(psa_handle_manager_t *handle_mgr, psa_handle_t handle);
void *psa_hndl_mgr_handle_get_mem(psa_handle_manager_t *handle_mgr, psa_handle_t handle);


#ifdef __cplusplus
}
#endif

#endif  /* __MBED_HANDLE_MANAGER_H__ */
