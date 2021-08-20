/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_API_H__
#define __TFM_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "psa/client.h"

#define TFM_INVALID_CLIENT_ID 0

/**
 * \brief Checks if the provided client ID is a secure client ID.
 *
 * \param[in] client_id         Client ID to check.
 *
 * \retval 1                    Client ID is secure.
 * \retval 0                    Client ID is non-secure.
 */
#define TFM_CLIENT_ID_IS_S(client_id)  ((client_id)>0)

/**
 * \brief Checks if the provided client ID is a non-secure client ID.
 *
 * \param[in] client_id         Client ID to check.
 *
 * \retval 1                    Client ID is non-secure.
 * \retval 0                    Client ID is secure.
 */
#define TFM_CLIENT_ID_IS_NS(client_id) ((client_id)<0)

/* The mask used for timeout values */
#define PSA_TIMEOUT_MASK        PSA_BLOCK

/* FixMe: sort out DEBUG compile option and limit return value options
 * on external interfaces */
enum tfm_status_e
{
    TFM_SUCCESS = 0,
    TFM_PARTITION_BUSY,
    TFM_ERROR_SECURE_DOMAIN_LOCKED,
    TFM_ERROR_INVALID_PARAMETER,
    TFM_ERROR_PARTITION_NON_REENTRANT,
    TFM_ERROR_NS_THREAD_MODE_CALL,
    TFM_ERROR_NOT_INITIALIZED,
    TFM_ERROR_NO_ACTIVE_PARTITION,
    TFM_ERROR_INVALID_EXC_MODE,
    TFM_SECURE_LOCK_FAILED,
    TFM_SECURE_UNLOCK_FAILED,
    TFM_ERROR_GENERIC = 0x1F,
};

/*
 * Structure to package type, in_len and out_len, it is mainly used for
 * psa_call.
 */
struct tfm_control_parameter_t {
   int32_t type;
   size_t in_len;
   size_t out_len;
};

/********************* Secure function declarations ***************************/

/**
 * \brief Assign client ID to the current TZ context.
 *
 * \param[in] ns_client_id      The client ID to be assigned to the current
 *                              context.
 * \retval TFM_SUCCESS          The client ID assigned successfully.
 * \retval error code           The client ID assignment failed, an error code
 *                              returned according to \ref tfm_status_e.
 * \note This function have to be called from handler mode.
 */
enum tfm_status_e tfm_register_client_id (int32_t ns_client_id);

/**
 * \brief Retrieve the version of the PSA Framework API that is implemented.
 *
 * \return The version of the PSA Framework.
 */
uint32_t tfm_psa_framework_version_veneer(void);

/**
 * \brief Return version of secure function provided by secure binary.
 *
 * \param[in] sid               ID of secure service.
 *
 * \return Version number of secure function.
 */
uint32_t tfm_psa_version_veneer(uint32_t sid);

/**
 * \brief Connect to secure function.
 *
 * \param[in] sid               ID of secure service.
 * \param[in] version           Version of SF requested by client.
 *
 * \return Returns handle to connection.
 */
psa_handle_t tfm_psa_connect_veneer(uint32_t sid, uint32_t version);

/**
 * \brief Call a secure function referenced by a connection handle.
 *
 * \param[in] handle            Handle to connection.
 * \param[in] ctrl_param        Parameter structure, includes request type,
 *                              in_num and out_num.
 * \param[in] in_vec            Array of input \ref psa_invec structures.
 * \param[in,out] out_vec       Array of output \ref psa_outvec structures.
 *
 * \return Returns \ref psa_status_t status code.
 */
psa_status_t tfm_psa_call_veneer(psa_handle_t handle,
                               const struct tfm_control_parameter_t *ctrl_param,
                               const psa_invec *in_vec,
                               psa_outvec *out_vec);

/**
 * \brief Close connection to secure function referenced by a connection handle.
 *
 * \param[in] handle            Handle to connection
 */
void tfm_psa_close_veneer(psa_handle_t handle);

/***************** End Secure function declarations ***************************/

#ifdef __cplusplus
}
#endif

#endif /* __TFM_API_H__ */
