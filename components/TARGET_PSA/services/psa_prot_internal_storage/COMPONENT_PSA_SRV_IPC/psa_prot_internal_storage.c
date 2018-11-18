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

#include "spm_client.h"
#include "psa_prot_internal_storage.h"
#include "psa_its_ifs.h"

psa_its_status_t psa_its_set(uint32_t uid, uint32_t data_length, const void *p_data, psa_its_create_flags_t create_flags)
{
    if (!p_data && data_length) {
        return PSA_ITS_ERROR_BAD_POINTER;
    }

    psa_invec_t msg[3] = {
        { &uid, sizeof(uid) },
        { p_data, data_length },
        { &create_flags, sizeof(create_flags) }
    };

    psa_handle_t conn = psa_connect(PSA_ITS_SET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_error_t status = psa_call(conn, msg, 3, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_its_status_t psa_its_get(uint32_t uid, uint32_t data_offset, uint32_t data_length, void *p_data)
{
    if (!p_data && data_length) {
        return PSA_ITS_ERROR_BAD_POINTER;
    }

    psa_invec_t msg[2] = {
        { &uid, sizeof(uid) },
        { &data_offset, sizeof(data_offset) }
    };
    psa_outvec_t resp = { p_data, data_length };

    psa_handle_t conn = psa_connect(PSA_ITS_GET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_error_t status = psa_call(conn, msg, 2, &resp, 1);

    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_its_status_t psa_its_get_info(uint32_t uid, struct psa_its_info_t *p_info)
{
    if (!p_info) {
        return PSA_ITS_ERROR_BAD_POINTER;
    }

    struct psa_its_info_t info = { 0 };
    psa_invec_t msg = { &uid, sizeof(uid) };
    psa_outvec_t resp = { &info, sizeof(info) };
    psa_handle_t conn = psa_connect(PSA_ITS_INFO, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_error_t status = psa_call(conn, &msg, 1, &resp, 1);

    *p_info = info;

    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_its_status_t psa_its_remove(uint32_t uid)
{
    psa_invec_t msg = { &uid, sizeof(uid) };
    psa_handle_t conn = psa_connect(PSA_ITS_REMOVE, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_error_t status = psa_call(conn, &msg, 1, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}
