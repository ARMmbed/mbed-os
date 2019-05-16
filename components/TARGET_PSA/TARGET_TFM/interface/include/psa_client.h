/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __PSA_CLIENT_H__
#define __PSA_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/*********************** PSA Client Macros and Types *************************/

#define PSA_FRAMEWORK_VERSION  (0x0100)

#define PSA_VERSION_NONE       (0)

/* PSA response types */
#define PSA_SUCCESS            (0)
#define PSA_CONNECTION_REFUSED (INT32_MIN + 1)
#define PSA_CONNECTION_BUSY    (INT32_MIN + 2)
#define PSA_DROP_CONNECTION    (INT32_MIN)

/* PSA message handles */
#define PSA_NULL_HANDLE        ((psa_handle_t)0)

typedef int32_t psa_status_t;
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
 * \brief Retrieve the minor version of an RoT Service or indicate that it is
 *        not present on this system.
 *
 * \param[in] sid               ID of the RoT Service to query.
 *
 * \retval PSA_VERSION_NONE     The RoT Service is not implemented, or the
 *                              caller is not permitted to access the service.
 * \retval > 0                  The minor version of the implemented RoT
 *                              Service.
 */
uint32_t psa_version(uint32_t sid);

/**
 * \brief Connect to an RoT Service by its SID.
 *
 * \param[in] sid               ID of the RoT Service to connect to.
 * \param[in] minor_version     Requested version of the RoT Service.
 *
 * \retval > 0                  A handle for the connection.
 * \retval PSA_CONNECTION_REFUSED The SPM or RoT Service has refused the
 *                              connection.
 * \retval PSA_CONNECTION_BUSY  The SPM or RoT Service cannot make the
 *                              connection at the moment.
 * \retval "Does not return"    The RoT Service ID and version are not
 *                              supported, or the caller is not permitted to
 *                              access the service.
 */
psa_handle_t psa_connect(uint32_t sid, uint32_t minor_version);

/**
 * \brief Call an RoT Service on an established connection.
 *
 * \param[in] handle            A handle to an established connection.
 * \param[in] in_vec            Array of input \ref psa_invec structures.
 * \param[in] in_len            Number of input \ref psa_invec structures.
 * \param[in/out] out_vec       Array of output \ref psa_outvec structures.
 * \param[in] out_len           Number of output \ref psa_outvec structures.
 *
 * \retval >=0                  RoT Service-specific status value.
 * \retval <0                   RoT Service-specific error code.
 * \retval PSA_DROP_CONNECTION  The connection has been dropped by the RoT
 *                              Service. This indicates that either this or
 *                              a previous message was invalid.
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                           An invalid handle was passed.
 * \arg                           The connection is already handling a request.
 * \arg                           An invalid memory reference was provided.
 * \arg                           in_len + out_len > PSA_MAX_IOVEC.
 * \arg                           The message is unrecognized by the RoT
 *                                Service or incorrectly formatted.
 */
psa_status_t psa_call(psa_handle_t handle,
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
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                           An invalid handle was provided that is not
 *                                the null handle.
 * \arg                           The connection is handling a request.
 */
void psa_close(psa_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __PSA_CLIENT_H__ */
