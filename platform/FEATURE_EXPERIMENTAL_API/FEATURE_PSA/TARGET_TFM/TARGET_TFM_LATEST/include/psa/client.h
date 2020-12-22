/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PSA_CLIENT_H__
#define __PSA_CLIENT_H__

#include <stddef.h>
#include <stdint.h>

#include "psa/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************** PSA Client Macros and Types *************************/

/**
 * The version of the PSA Framework API that is being used to build the calling
 * firmware.
 */
#define PSA_FRAMEWORK_VERSION       (0x0100u)

/**
 * Return value from psa_version() if the requested RoT Service is not present
 * in the system.
 */
#define PSA_VERSION_NONE            (0u)

/**
 * The zero-value null handle can be assigned to variables used in clients and
 * RoT Services, indicating that there is no current connection or message.
 */
#define PSA_NULL_HANDLE             ((psa_handle_t)0)

/**
 * Tests whether a handle value returned by psa_connect() is valid.
 */
#define PSA_HANDLE_IS_VALID(handle) ((psa_handle_t)(handle) > 0)

/**
 * Converts the handle value returned from a failed call psa_connect() into
 * an error code.
 */
#define PSA_HANDLE_TO_ERROR(handle) ((psa_status_t)(handle))

/**
 * Maximum number of input and output vectors for a request to psa_call().
 */
#define PSA_MAX_IOVEC               (4u)

/**
 * An IPC message type that indicates a generic client request.
 */
#define PSA_IPC_CALL                (0)

typedef int32_t psa_handle_t;

/**
 * A read-only input memory region provided to an RoT Service.
 */
typedef struct psa_invec {
    const void *base;           /*!< the start address of the memory buffer */
    size_t len;                 /*!< the size in bytes                      */
} psa_invec;

/**
 * A writable output memory region provided to an RoT Service.
 */
typedef struct psa_outvec {
    void *base;                 /*!< the start address of the memory buffer */
    size_t len;                 /*!< the size in bytes                      */
} psa_outvec;

/*************************** PSA Client API **********************************/

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

#endif /* __PSA_CLIENT_H__ */
