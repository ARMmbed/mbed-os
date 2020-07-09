/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "os_wrapper/mutex.h"

#include "psa/client.h"
#include "psa/error.h"
#include "tfm_api.h"
#include "tfm_multi_core_api.h"
#include "tfm_ns_mailbox.h"

/*
 * TODO
 * Currently, force all the non-secure client to share the same ID.
 *
 * It requires a more clear mechanism to synchronize the non-secure client
 * ID with SPE in dual core scenario.
 * In current design, the value is transferred to SPE via mailbox message.
 * A dedicated routine to receive the non-secure client information in
 * TF-M core/SPM in dual core scenario should be added besides current
 * implementation for single Armv8-M.
 * The non-secure client identification is shared with SPE in
 * single Armv8-M scenario via CMSIS TrustZone context management API,
 * which may not work in dual core scenario.
 */
#define NON_SECURE_CLIENT_ID            (1)

/*
 * TODO
 * Require a formal definition of errors related to mailbox in PSA client call.
 */
#define PSA_INTER_CORE_COMM_ERR         (INT32_MIN + 0xFF)

static void mailbox_wait_reply(mailbox_msg_handle_t handle)
{
    /*
     * If the system can support multiple outstanding NS PSA Client calls, call
     * tfm_ns_mailbox_wait_reply() to sleep and wait for reply. The NS side
     * should implement tfm_ns_mailbox_hal_wait_reply() and wake-up mechanism.
     * Otherwise, by default, call tfm_ns_mailbox_is_msg_replied() to simply
     * poll the reply status of the mailbox message of current thread.
     */
#ifdef TFM_MULTI_CORE_MULTI_CLIENT_CALL
    tfm_ns_mailbox_wait_reply(handle);
#else
    while (!tfm_ns_mailbox_is_msg_replied(handle)) {
    }
#endif
}

/**** API functions ****/

uint32_t psa_framework_version(void)
{
    struct psa_client_params_t params;
    mailbox_msg_handle_t handle;
    uint32_t version;
    int32_t ret;

    if (tfm_ns_multi_core_lock_acquire() != OS_WRAPPER_SUCCESS) {
        return PSA_VERSION_NONE;
    }

    handle = tfm_ns_mailbox_tx_client_req(MAILBOX_PSA_FRAMEWORK_VERSION,
                                          &params, NON_SECURE_CLIENT_ID);
    if (handle < 0) {
        tfm_ns_multi_core_lock_release();
        return PSA_VERSION_NONE;
    }

    mailbox_wait_reply(handle);

    ret = tfm_ns_mailbox_rx_client_reply(handle, (int32_t *)&version);
    if (ret != MAILBOX_SUCCESS) {
        version = PSA_VERSION_NONE;
    }

    if (tfm_ns_multi_core_lock_release() != OS_WRAPPER_SUCCESS) {
        return PSA_VERSION_NONE;
    }

    return version;
}

uint32_t psa_version(uint32_t sid)
{
    struct psa_client_params_t params;
    mailbox_msg_handle_t handle;
    uint32_t version;
    int32_t ret;

    params.psa_version_params.sid = sid;

    if (tfm_ns_multi_core_lock_acquire() != OS_WRAPPER_SUCCESS) {
        return PSA_VERSION_NONE;
    }

    handle = tfm_ns_mailbox_tx_client_req(MAILBOX_PSA_VERSION, &params,
                                          NON_SECURE_CLIENT_ID);
    if (handle < 0) {
        tfm_ns_multi_core_lock_release();
        return PSA_VERSION_NONE;
    }

    mailbox_wait_reply(handle);

    ret = tfm_ns_mailbox_rx_client_reply(handle, (int32_t *)&version);
    if (ret != MAILBOX_SUCCESS) {
        version = PSA_VERSION_NONE;
    }

    if (tfm_ns_multi_core_lock_release() != OS_WRAPPER_SUCCESS) {
        return PSA_VERSION_NONE;
    }

    return version;
}

psa_handle_t psa_connect(uint32_t sid, uint32_t version)
{
    struct psa_client_params_t params;
    mailbox_msg_handle_t handle;
    psa_handle_t psa_handle;
    int32_t ret;

    params.psa_connect_params.sid = sid;
    params.psa_connect_params.version = version;

    if (tfm_ns_multi_core_lock_acquire() != OS_WRAPPER_SUCCESS) {
        return PSA_NULL_HANDLE;
    }

    handle = tfm_ns_mailbox_tx_client_req(MAILBOX_PSA_CONNECT, &params,
                                          NON_SECURE_CLIENT_ID);
    if (handle < 0) {
        tfm_ns_multi_core_lock_release();
        return PSA_NULL_HANDLE;
    }

    mailbox_wait_reply(handle);

    ret = tfm_ns_mailbox_rx_client_reply(handle, (int32_t *)&psa_handle);
    if (ret != MAILBOX_SUCCESS) {
        psa_handle = PSA_NULL_HANDLE;
    }

    if (tfm_ns_multi_core_lock_release() != OS_WRAPPER_SUCCESS) {
        return PSA_NULL_HANDLE;
    }

    return psa_handle;
}

psa_status_t psa_call(psa_handle_t handle, int32_t type,
                      const psa_invec *in_vec, size_t in_len,
                      psa_outvec *out_vec, size_t out_len)
{
    struct psa_client_params_t params;
    mailbox_msg_handle_t msg_handle;
    int32_t ret;
    psa_status_t status;

    params.psa_call_params.handle = handle;
    params.psa_call_params.type = type;
    params.psa_call_params.in_vec = in_vec;
    params.psa_call_params.in_len = in_len;
    params.psa_call_params.out_vec = out_vec;
    params.psa_call_params.out_len = out_len;

    if (tfm_ns_multi_core_lock_acquire() != OS_WRAPPER_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    msg_handle = tfm_ns_mailbox_tx_client_req(MAILBOX_PSA_CALL, &params,
                                              NON_SECURE_CLIENT_ID);
    if (msg_handle < 0) {
        tfm_ns_multi_core_lock_release();
        return PSA_INTER_CORE_COMM_ERR;
    }

    mailbox_wait_reply(msg_handle);

    ret = tfm_ns_mailbox_rx_client_reply(msg_handle, (int32_t *)&status);
    if (ret != MAILBOX_SUCCESS) {
        status = PSA_INTER_CORE_COMM_ERR;
    }

    if (tfm_ns_multi_core_lock_release() != OS_WRAPPER_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    return status;
}

void psa_close(psa_handle_t handle)
{
    struct psa_client_params_t params;
    mailbox_msg_handle_t msg_handle;
    int32_t reply;

    params.psa_close_params.handle = handle;

    if (tfm_ns_multi_core_lock_acquire() != OS_WRAPPER_SUCCESS) {
        return;
    }

    msg_handle = tfm_ns_mailbox_tx_client_req(MAILBOX_PSA_CLOSE, &params,
                                              NON_SECURE_CLIENT_ID);
    if (msg_handle < 0) {
        tfm_ns_multi_core_lock_release();
        return;
    }

    mailbox_wait_reply(msg_handle);

    (void)tfm_ns_mailbox_rx_client_reply(msg_handle, &reply);

    tfm_ns_multi_core_lock_release();
}
