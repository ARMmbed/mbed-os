/* Copyright (c) 2017-2020 ARM Limited
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

#if defined(FEATURE_TFM)
#include "interface/include/psa_client.h"
#else

#ifndef __MBED_OS_DEFAULT_PSA_CLIENT_API_H__
#define __MBED_OS_DEFAULT_PSA_CLIENT_API_H__

#include <stddef.h>
#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(UINT32_MAX)
#define UINT32_MAX ((uint32_t)-1)
#endif

#if !defined(INT32_MIN)
#define INT32_MIN   (-0x7fffffff - 1)
#endif

#define PSA_FRAMEWORK_VERSION   (0x0100) /**< Version of the PSA Framework API. */
#define PSA_VERSION_NONE        (0L)     /**< Identifier for an unimplemented Root of Trust (RoT) Service. */
#define PSA_CONNECTION_REFUSED  (INT32_MIN + 1)   /**< The return value from psa_connect() if the RoT Service or SPM was unable to establish a connection.*/
#define PSA_CONNECTION_BUSY     (INT32_MIN + 2)   /**< The return value from psa_connect() if the RoT Service rejects the connection for a transient reason.*/
#define PSA_DROP_CONNECTION     (INT32_MIN)       /**< The result code in a call to psa_reply() to indicate a nonrecoverable error in the client.*/
#define PSA_NULL_HANDLE         ((psa_handle_t)0)   /**< Denotes an invalid handle.*/

typedef int32_t psa_handle_t;

typedef struct psa_invec {
    const void *base;   /**< Starting address of the buffer.*/
    size_t len;         /**< Length in bytes of the buffer.*/
} psa_invec;


typedef struct psa_outvec {
    void *base;      /**< Starting address of the buffer.*/
    size_t len;      /**< Length in bytes of the buffer.*/
} psa_outvec;

/**
 * \brief Retrieve the version of the PSA Framework API that is implemented.
 *
 * \return version              The version of the PSA Framework implementation
 *                              that is providing the runtime services to the
 *                              caller. The major and minor version are encoded
 *                              as follows:
 * \arg                           version[15:8] -- major version number.
 * \arg                           version[7:0]  -- minor version number.
 */
uint32_t psa_framework_version(void);

/**
 * \brief Retrieve the version of an RoT Service or indicate that it is not
 *        present on this system.
 *
 * \param[in] sid               ID of the RoT Service to query.
 *
 * \retval PSA_VERSION_NONE     The RoT Service is not implemented, or the
 *                              caller is not permitted to access the service.
 * \retval > 0                  The version of the implemented RoT Service.
 */
uint32_t psa_version(uint32_t sid);

/**
 * \brief Connect to an RoT Service by its SID.
 *
 * \param[in] sid               ID of the RoT Service to connect to.
 * \param[in] version           Requested version of the RoT Service.
 *
 * \retval > 0                  A handle for the connection.
 * \retval PSA_ERROR_CONNECTION_REFUSED The SPM or RoT Service has refused the
 *                              connection.
 * \retval PSA_ERROR_CONNECTION_BUSY The SPM or RoT Service cannot make the
 *                              connection at the moment.
 * \retval "PROGRAMMER ERROR"   The call is a PROGRAMMER ERROR if one or more
 *                              of the following are true:
 * \arg                           The RoT Service ID is not present.
 * \arg                           The RoT Service version is not supported.
 * \arg                           The caller is not allowed to access the RoT
 *                                service.
 */
psa_handle_t psa_connect(uint32_t sid, uint32_t version);

/**
 * \brief Call an RoT Service on an established connection.
 *
 * \param[in] handle            A handle to an established connection.
 * \param[in] type              The reuqest type.
 *                              Must be zero( \ref PSA_IPC_CALL) or positive.
 * \param[in] in_vec            Array of input \ref psa_invec structures.
 * \param[in] in_len            Number of input \ref psa_invec structures.
 * \param[in/out] out_vec       Array of output \ref psa_outvec structures.
 * \param[in] out_len           Number of output \ref psa_outvec structures.
 *
 * \retval >=0                  RoT Service-specific status value.
 * \retval <0                   RoT Service-specific error code.
 * \retval PSA_ERROR_PROGRAMMER_ERROR The connection has been terminated by the
 *                              RoT Service. The call is a PROGRAMMER ERROR if
 *                              one or more of the following are true:
 * \arg                           An invalid handle was passed.
 * \arg                           The connection is already handling a request.
 * \arg                           type < 0.
 * \arg                           An invalid memory reference was provided.
 * \arg                           in_len + out_len > PSA_MAX_IOVEC.
 * \arg                           The message is unrecognized by the RoT
 *                                Service or incorrectly formatted.
 */
psa_status_t psa_call(psa_handle_t handle, int32_t type,
                      const psa_invec *in_vec,
                      size_t in_len,
                      psa_outvec *out_vec,
                      size_t out_len);

/**
 * \brief Close a connection to an RoT Service.
 *
 * \param[in] handle            A handle to an established connection, or the
 *                              null handle.
 *
 * \retval void                 Success.
 * \retval "PROGRAMMER ERROR"   The call is a PROGRAMMER ERROR if one or more
 *                              of the following are true:
 * \arg                           An invalid handle was provided that is not
 *                                the null handle.
 * \arg                           The connection is currently handling a
 *                                request.
 */
void psa_close(psa_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif // __MBED_OS_DEFAULT_PSA_CLIENT_API_H__
#endif
