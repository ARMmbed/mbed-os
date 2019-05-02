/*
* Copyright (c) 2018-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "psa_attest_inject_key.h"
#include "psa/crypto.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"

#define MINOR_VER 1

psa_status_t
psa_attestation_inject_key(const uint8_t *key_data,
                           size_t key_data_length,
                           psa_key_type_t type,
                           uint8_t *public_key_data,
                           size_t public_key_data_size,
                           size_t *public_key_data_length)
{
    psa_handle_t handle = PSA_NULL_HANDLE;
    psa_status_t call_error = PSA_SUCCESS;
    psa_invec in_vec[2] = {
        { &type, sizeof(type) },
        { key_data, key_data_length }
    };

    psa_outvec out_vec[2] = {
        { public_key_data, public_key_data_size },
        { public_key_data_length, sizeof(*public_key_data_length) }
    };

    handle = psa_connect(PSA_ATTEST_INJECT_KEY_ID, MINOR_VER);
    if (handle <= 0) {
        return (PSA_ERROR_COMMUNICATION_FAILURE);
    }

    call_error = psa_call(handle, in_vec, 2, out_vec, 2);

    psa_close(handle);
    return call_error;
}
