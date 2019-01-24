/* Copyright (c) 2017-2018 ARM Limited
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
#include "psa/internal_trusted_storage.h"
#include "test_pits.h"
#include "psa_test_its_reset_ifs.h"

psa_its_status_t test_psa_its_reset(void)
{
    psa_handle_t conn = psa_connect(TEST_PSA_ITS_RESET, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_status_t status = psa_call(conn, NULL, 0, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_its_status_t test_psa_its_set_ver(uint32_t major, uint32_t minor)
{
    psa_handle_t conn = psa_connect(TEST_PSA_ITS_SET_VER, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_invec_t msg[2] = {
        {&major, sizeof(major)},
        {&minor, sizeof(minor)}
    };

    psa_error_t status = psa_call(conn, msg, 2, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_its_status_t test_psa_its_get_ver(uint32_t *major, uint32_t *minor)
{
    psa_handle_t conn = psa_connect(TEST_PSA_ITS_GET_VER, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_outvec_t resp[2] = {
        {major, sizeof(*major)},
        {minor, sizeof(*minor)}
    };

    psa_error_t status = psa_call(conn, NULL, 0, resp, 2);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}

psa_its_status_t test_psa_its_deinit(void)
{
    psa_handle_t conn = psa_connect(TEST_PSA_ITS_DEINIT, 1);
    if (conn <= PSA_NULL_HANDLE) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_error_t status = psa_call(conn, NULL, 0, NULL, 0);
    if (status == PSA_DROP_CONNECTION) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    psa_close(conn);
    return status;
}
