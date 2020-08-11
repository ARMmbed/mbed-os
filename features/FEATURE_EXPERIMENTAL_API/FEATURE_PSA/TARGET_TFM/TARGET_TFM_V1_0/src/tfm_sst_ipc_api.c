/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/protected_storage.h"

#include "tfm_ns_interface.h"
#include "psa_manifest/sid.h"

#define IOVEC_LEN(x) (uint32_t)(sizeof(x)/sizeof(x[0]))

psa_status_t psa_ps_set(psa_storage_uid_t uid,
                        size_t data_length,
                        const void *p_data,
                        psa_storage_create_flags_t create_flags)
{
    psa_status_t status;
    psa_handle_t handle;

    psa_invec in_vec[] = {
        { .base = &uid,   .len = sizeof(uid) },
        { .base = p_data, .len = data_length },
        { .base = &create_flags, .len = sizeof(create_flags) }
    };

    handle = psa_connect(TFM_SST_SET_SID, TFM_SST_SET_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec),
                      NULL, 0);

    psa_close(handle);

    /* A parameter with a buffer pointer pointer that has data length longer
     * than maximum permitted is treated as a secure violation.
     * TF-M framework rejects the request with TFM_ERROR_INVALID_PARAMETER.
     */
    if (status == (psa_status_t)TFM_ERROR_INVALID_PARAMETER) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    return status;
}

psa_status_t psa_ps_get(psa_storage_uid_t uid,
                        size_t data_offset,
                        size_t data_size,
                        void *p_data,
                        size_t *p_data_length)
{
    psa_status_t status;
    psa_handle_t handle;

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) },
        { .base = &data_offset, .len = sizeof(data_offset) }
    };

    psa_outvec out_vec[] = {
        { .base = p_data, .len = data_size }
    };

    if (p_data_length == NULL) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    handle = psa_connect(TFM_SST_GET_SID, TFM_SST_GET_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), out_vec,
                      IOVEC_LEN(out_vec));

    psa_close(handle);

    *p_data_length = out_vec[0].len;

    return status;
}

psa_status_t psa_ps_get_info(psa_storage_uid_t uid,
                             struct psa_storage_info_t *p_info)
{
    psa_status_t status;
    psa_handle_t handle;

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) }
    };

    psa_outvec out_vec[] = {
        { .base = p_info, .len = sizeof(*p_info) }
    };

    handle = psa_connect(TFM_SST_GET_INFO_SID, TFM_SST_GET_INFO_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), out_vec,
                      IOVEC_LEN(out_vec));

    psa_close(handle);

    return status;
}

psa_status_t psa_ps_remove(psa_storage_uid_t uid)
{
    psa_status_t status;
    psa_handle_t handle;

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) }
    };


    handle = psa_connect(TFM_SST_REMOVE_SID, TFM_SST_REMOVE_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec),
                      NULL, 0);

    psa_close(handle);

    return status;
}

psa_status_t psa_ps_create(psa_storage_uid_t uid, size_t size,
                           psa_storage_create_flags_t create_flags)
{
    (void)uid;
    (void)size;
    (void)create_flags;

    return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t psa_ps_set_extended(psa_storage_uid_t uid, size_t data_offset,
                                 size_t data_length, const void *p_data)
{
    (void)uid;
    (void)data_offset;
    (void)data_length;
    (void)p_data;

    return PSA_ERROR_NOT_SUPPORTED;
}

uint32_t psa_ps_get_support(void)
{
    /* Initialise support_flags to a sensible default, to avoid returning an
     * uninitialised value in case the secure function fails.
     */
    uint32_t support_flags = 0;
    psa_handle_t handle;

    psa_outvec out_vec[] = {
        { .base = &support_flags, .len = sizeof(support_flags) }
    };

    /* The PSA API does not return an error, so any error from TF-M is
     * ignored.
     */
    handle = psa_connect(TFM_SST_GET_SUPPORT_SID, TFM_SST_GET_SUPPORT_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return support_flags;
    }

    (void)psa_call(handle, PSA_IPC_CALL, NULL, 0, out_vec, IOVEC_LEN(out_vec));

    psa_close(handle);

    return support_flags;
}
