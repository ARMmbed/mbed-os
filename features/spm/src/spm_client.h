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

#ifndef __UVISOR_MBED_SPM_CLIENT_H__
#define __UVISOR_MBED_SPM_CLIENT_H__

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
 *  The C interface for connecting to a Secure Function and calling it.@n
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
 * Retrieve the minor version of a Secure Function by its SFID.
 *
 * @param[in] sfid The Secure Function ID
 * @return Minor version of Secure Function or PSA_VERSION_NONE if Secure Function not present on the system.
 */
uint32_t psa_version(uint32_t sfid);

/**
 * Connect to a Secure Function by its SFID.
 *
 * @note A minor version number must be provided to allow the Secure Function to handle minor variations of the protocol.
 *
 * @param[in] sfid          The Secure Function ID.
 * @param[in] minor_version The minor version to be used for this connection.
 * @return A handle for the connection if greater than 0, else one of the following errors:@n
 *         @a PSA_CONNECTION_REFUSED_BUSY if the Secure Function cannot handle any more connections.@n
 *         @a PSA_CONNECTION_REFUSED_PERM if the Secure Function does not exist or access is denied.
 */
psa_handle_t psa_connect(uint32_t sfid, uint32_t minor_version);

/**
 * Call a connected Secure Function.@n
 * The caller must provide an array of ::iovec_t structures as the input payload.
 *
 * TODO: Need to align again when API definitions are updated in spec
 *
 * @param[in]  handle   Handle for the connection.
 * @param[in]  tx_iovec Array of ::iovec_t structures.
 * @param[in]  tx_len   Number of ::iovec_t structures in tx_iovec. (Must be greater than 0 and less than 4)
 * @param[out] rx_buf  Receive buffer for optional Secure function response.
 * @param[in]  rx_len  Length in bytes of rx_buf.
 * @return Positive numbers with an application-specific return code or negative numbers on error:@n
 *         @a PSA_INVALID_MEM if the specified buffers in tx_iovec or rx_buf contain an invalid memory address
 *                            (pointing to non-existent memory or lack of permission for this memory).
 *         @a PSA_MSG_PAYLOAD_TOO_LARGE/PSA_MSG_PAYLOAD_TOO_SMALL if there is an issue with the payload size.@n
 *         @a PSA_INVALID_HANDLE if the handle is not valid.@n
 *         @a PSA_RESPONSE_PAYLOAD_UNSUPPORTED if the caller specified a response buffer but the Secure Function does not send response payloads.
 */
psa_error_t psa_call(
    psa_handle_t handle,
    const iovec_t *tx_iovec,
    size_t tx_len,
    void *rx_buf,
    size_t rx_len
    );

/**
 * Close a connection to a Secure Function.
 * Sends the ::PSA_IPC_MSG_TYPE_DISCONNECT message to the Secure Function so it can clean up resources.
 *
 * @param[in] handle Handle for the connection.
 * @return PSA_SUCCESS or negative error code if failed.
 */
psa_error_t psa_close(psa_handle_t handle);

/** @}*/ // end of Client-API group

#ifdef __cplusplus
}
#endif

/** @}*/ // end of SPM group

#endif  /* __UVISOR_MBED_SPM_CLIENT_H__ */
