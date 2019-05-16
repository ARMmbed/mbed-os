/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_SVCALLS_H__
#define __TFM_SVCALLS_H__

/* Svcall for PSA Client APIs */

/**
 * \brief SVC handler for \ref psa_framework_version.
 *
 * \return version              The version of the PSA Framework implementation
 *                              that is providing the runtime services to the
 *                              caller.
 */
uint32_t tfm_svcall_psa_framework_version(void);

/**
 * \brief SVC handler for \ref psa_version.
 *
 * \param[in] args              Include all input arguments: sid.
 * \param[in] ns_caller         If 'non-zero', call from non-secure client.
 *                              Or from secure client.
 *
 * \retval PSA_VERSION_NONE     The RoT Service is not implemented, or the
 *                              caller is not permitted to access the service.
 * \retval > 0                  The minor version of the implemented RoT
 *                              Service.
 */
uint32_t tfm_svcall_psa_version(uint32_t *args, int32_t ns_caller);

/**
 * \brief SVC handler for \ref psa_connect.
 *
 * \param[in] args              Include all input arguments:
 *                              sid, minor_version.
 * \param[in] ns_caller         If 'non-zero', call from non-secure client.
 *                              Or from secure client.
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
psa_handle_t tfm_svcall_psa_connect(uint32_t *args, int32_t ns_caller);

/**
 * \brief SVC handler for \ref psa_call.
 *
 * \param[in] args              Include all input arguments:
 *                              handle, in_vec, in_len, out_vec, out_len.
 * \param[in] ns_caller         If 'non-zero', call from non-secure client.
 *                              Or from secure client.
 * \param[in] lr                EXC_RETURN value of the SVC.
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
psa_status_t tfm_svcall_psa_call(uint32_t *args, int32_t ns_caller,
                                 uint32_t lr);

/**
 * \brief SVC handler for \ref psa_close.
 *
 * \param[in] args              Include all input arguments: handle.
 * \param[in] ns_caller         If 'non-zero', call from non-secure client.
 *                              Or from secure client.
 *
 * \retval void                 Success.
 * \retval "Does not return"    The call is invalid, one or more of the
 *                              following are true:
 * \arg                           An invalid handle was provided that is not
 *                                the null handle.
 * \arg                           The connection is handling a request.
 */
void tfm_svcall_psa_close(uint32_t *args, int32_t ns_caller);

/**
 * \brief SVC handler for IPC functions
 *
 * \param[in] svc_num           SVC number
 * \param[in] ctx               Argument context
 * \param[in] lr                EXC_RETURN value of the SVC.
 *
 * \returns                     Return values from those who has,
 *                              or PSA_SUCCESS.
 */
int32_t SVC_Handler_IPC(tfm_svc_number_t svc_num, uint32_t *ctx, uint32_t lr);

#endif
