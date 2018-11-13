/* Copyright (c) 2017 ARM Limited
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

#ifndef __MBED_SPM_CLIENT_H__
#define __MBED_SPM_CLIENT_H__

/** @addtogroup SPM
 * The SPM (Secure Partition Manager) is responsible for isolating software in partitions,@n
 * managing the execution of software within partitions, and providing IPC between partitions.
 * @{
 */

/* -------------------------------------- Includes ----------------------------------- */

#include "psa_defs.h"

/* --------------------------------- extern "C" wrapper ------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Client-API
 *  The C interface for connecting to a Root of Trust Service and calling it.@n
 *  All functions are blocking.
 * @{
 */

/**
 * Retrieve the version of the PSA Framework API that is implemented.
 *
 * @note The PSA Framework API version is made up of the major and minor versions as follows:
 *      @code
 *      ((major_version << 8) | minor_version)
 *      @endcode
 * @return The PSA Framework API version
 */
uint32_t psa_framework_version(void);

/**
 * Retrieve the minor version of a Root of Trust Service by its SID.
 *
 * @param[in] sid The Root of Trust Service ID
 * @return Minor version of Root of Trust Service or PSA_VERSION_NONE if Root of Trust Service not present on the system.
 */
uint32_t psa_version(uint32_t sid);

/**
 * Connect to a Root of Trust Service by its SID.
 *
 * @note A minor version number must be provided to allow the Root of Trust Service to handle minor variations of the protocol.
 *
 * @param[in] sid          The Root of Trust Service ID.
 * @param[in] minor_version The minor version to be used for this connection.
 * @return A handle for the connection if greater than 0, else:@n
 *         @a PSA_CONNECTION_REFUSED if the Root of Trust Service cannot handle any more connections.@n
 */
psa_handle_t psa_connect(uint32_t sid, uint32_t minor_version);

/**
 * Call a connected Root of Trust Service.@n
 * The caller must provide an array of ::psa_invec_t structures as the input payload.
 *
 * @param[in]  handle   Handle for the connection.
 * @param[in]  in_vec   Array of ::psa_invec_t structures.
 * @param[in]  in_len   Number of ::psa_invec_t structures in in_vec. (At most ::PSA_MAX_IOVEC - out_len)
 * @param[out] out_vec  Array of ::psa_outvec_t structures for optional Root of Trust Service response.
 * @param[in]  out_len  Number of ::psa_outvec_t structures in out_vec. (At most ::PSA_MAX_IOVEC - in_len)
 * @return 0 for success or@n
 *         @a positive numbers for application-specific return code.
 *         @a negative number for application-specific error code.
 *         @a PSA_DROP_CONNECTION if the connection has been dropped by the RoT Service.
 */
psa_error_t psa_call(
    psa_handle_t handle,
    const psa_invec_t *in_vec,
    size_t in_len,
    const psa_outvec_t *out_vec,
    size_t out_len
    );

/**
 * Close a connection to a Root of Trust Service.
 * Sends the ::PSA_IPC_DISCONNECT message to the Root of Trust Service so it can clean up resources.
 *
 * @param[in] handle Handle for the connection.
 */
void psa_close(psa_handle_t handle);

/** @}*/ // end of Client-API group

#ifdef __cplusplus
}
#endif

/** @}*/ // end of SPM group

#endif  /* __MBED_SPM_CLIENT_H__ */
