/** @file
 * Copyright (c) 2018, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifdef COMPONENT_PSA_SRV_IPC

#include "pal_common.h"
#include "pal_client_api_intf.h"

/**
 * @brief   - Retrieve the version of the PSA Framework API that is implemented.
 * This is a wrapper API for psa_framework_version API.
 * @param    - void
 * @return   - The PSA Framework API version.
 *             Note - Return PAL_STATUS_ERROR if PSA IPC is not implemented.
 */

uint32_t pal_ipc_framework_version(void)
{
    return (psa_framework_version());
}

/**
 * @brief   - Retrieve the minor version of a Root of Trust Service by its SID.
 * This is a wrapper API for the psa_version API.
 * @param - sid The Root of Trust Service ID
 * @return - Minor version of Root of Trust Service or PSA_VERSION_NONE if Root of Trust
 *           Service not present on the system.
 *           Note - Return PAL_STATUS_ERROR if PSA IPC is not implemented.
 */

uint32_t pal_ipc_version(uint32_t sid)
{
    return (psa_version(sid));
}

/**
 * @brief   - Connect to given sid.
 *            This is a wrapper API for the psa_connect API.
 * @param   - sid : RoT service id
 * @param   - minor_version : minor_version of RoT service
 * @return  - psa_handle_t : return connection handle
 *            Note - Return PSA_NULL_HANDLE if PSA IPC is not implemented.
 */

psa_handle_t pal_ipc_connect(uint32_t sid, uint32_t minor_version)
{
    return (psa_connect(sid, minor_version));
}

/**
 * @brief Call a connected Root of Trust Service.
 * This is a wrapper API for the psa_call API.
 * The caller must provide an array of ::psa_invec_t structures as the input payload.
 *
 * @param  -handle   Handle for the connection.
 * @param  -in_vec   Array of psa_invec structures.
 * @param  -in_len   Number of psa_invec structures in in_vec.
 * @param  -out_vec  Array of psa_outvec structures for optional Root of Trust Service response.
 * @param  -out_len  Number of psa_outvec structures in out_vec.
 * @return -psa_status_t
 *          Note - Return -1 if PSA IPC is not implemented.
 */

psa_status_t pal_ipc_call(psa_handle_t handle,
                         const psa_invec *in_vec,
                         size_t in_len,
                         psa_outvec *out_vec,
                         size_t out_len)
{
    return (psa_call(handle, in_vec, in_len, out_vec, out_len));
}

/**
 * @brief Close a connection to a Root of Trust Service.
 * This is a wrapper API for the psa_close API.
 * Sends the PSA_IPC_DISCONNECT message to the Root of Trust Service so it can clean up resources.
 *
 * @param  - handle Handle for the connection.
 * @return - void
 */

void pal_ipc_close(psa_handle_t handle)
{
    psa_close(handle);
}

#endif
