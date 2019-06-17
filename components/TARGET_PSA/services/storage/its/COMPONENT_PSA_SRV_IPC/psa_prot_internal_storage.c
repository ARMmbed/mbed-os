/* Copyright (c) 2018 ARM Limited
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

#include "psa/client.h"
#include "psa/storage_common.h"
#include "psa/internal_trusted_storage.h"
#include "psa_manifest/sid.h"

psa_status_t psa_its_set(psa_storage_uid_t uid, size_t data_length, const void *p_data, psa_storage_create_flags_t create_flags)
{
    if (!p_data && data_length) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_invec msg[3] = {
        { &uid, sizeof(uid) },
        { p_data, data_length },
        { &create_flags, sizeof(create_flags) }
    };

    psa_handle_t conn = psa_connect(PSA_ITS_SET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    psa_status_t status = psa_call(conn, msg, 3, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_status_t psa_its_get(psa_storage_uid_t uid, size_t data_offset, size_t data_length, void *p_data, size_t *p_data_length)
{
    size_t actual_size = 0;

    if ((!p_data && data_length) || !p_data_length) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    psa_invec msg[2] = {
        { &uid, sizeof(uid) },
        { &data_offset, sizeof(data_offset) }
    };

    psa_outvec resp[2] = {
        { p_data, data_length },
        { &actual_size, sizeof(actual_size) }
    };

    psa_handle_t conn = psa_connect(PSA_ITS_GET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    psa_status_t status = psa_call(conn, msg, 2, resp, 2);

    *p_data_length = actual_size;

    psa_close(conn);
    return status;
}

psa_status_t psa_its_get_info(psa_storage_uid_t uid, struct psa_storage_info_t *p_info)
{
    if (!p_info) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    struct psa_storage_info_t info = { 0, PSA_STORAGE_FLAG_NONE };
    psa_invec msg = { &uid, sizeof(uid) };
    psa_outvec resp = { &info, sizeof(info) };
    psa_handle_t conn = psa_connect(PSA_ITS_INFO, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    psa_status_t status = psa_call(conn, &msg, 1, &resp, 1);

    *p_info = info;

    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_status_t psa_its_remove(psa_storage_uid_t uid)
{
    psa_invec msg = { &uid, sizeof(uid) };
    psa_handle_t conn = psa_connect(PSA_ITS_REMOVE, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    psa_status_t status = psa_call(conn, &msg, 1, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_status_t psa_its_reset()
{
    psa_handle_t conn = psa_connect(PSA_ITS_RESET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    psa_status_t status = psa_call(conn, NULL, 0, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}
