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

#ifndef SPM_MESSAGES_H
#define SPM_MESSAGES_H

#include "cmsis_compiler.h"
#include "cmsis_os2.h"
#include "psa_defs.h"

// All spm_pending_*_msg structs below are packed since in a dual processor
// solution they are used in both processors

/*
 * Structure containing data sent from NSPE for ROT_SRV call.
 */
typedef __PACKED_STRUCT spm_pending_call_msg {
    const psa_invec_t *in_vec; /* Invecs sent.*/
    uint32_t in_vec_size; /* Number of Invecs sent.*/
    const psa_outvec_t *out_vec; /* Outvecs for response.*/
    uint32_t out_vec_size; /* Number of Outvecs for response.*/
    psa_error_t rc; /* Return code to be filled by the Root of Trust Service.*/
    osSemaphoreId_t completion_sem_id; /* Semaphore to be released at the end of execution */
} __ALIGNED(4) spm_pending_call_msg_t;

/*
 * Structure containing data sent from NSPE for connection.
 */
typedef __PACKED_STRUCT spm_pending_connect_msg {
    uint32_t min_version; /* Minor version of the Root of Trust Service interface.*/
    psa_error_t rc; /* Return code to be filled by the Root of Trust Service.*/
    osSemaphoreId_t completion_sem_id; /* Semaphore to be released at the end of execution */
} __ALIGNED(4) spm_pending_connect_msg_t;


/*
 * Structure containing data sent from NSPE for RoT-Service version query.
 */
typedef __PACKED_STRUCT spm_pending_version_msg {
    uint32_t rc; /* Return code to be filled by the Root of Trust Service.*/
    osSemaphoreId_t completion_sem_id; /* Semaphore to be released at the end of execution */
} __ALIGNED(4) spm_pending_version_msg_t;

/*
 * Structure containing data sent from NSPE for closing a connection.
 */
typedef __PACKED_STRUCT spm_pending_close_msg {
    psa_handle_t handle;               /* Handle of channel to be closed */
    osSemaphoreId_t completion_sem_id; /* Semaphore to be released at the end of execution */
} __ALIGNED(4) spm_pending_close_msg_t;

#endif // SPM_MESSAGES_H
