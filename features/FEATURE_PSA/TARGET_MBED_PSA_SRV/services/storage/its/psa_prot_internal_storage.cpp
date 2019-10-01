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

#include <stdlib.h>
#include <string.h>

#include "psa/internal_trusted_storage.h"
#include "psa/storage_common.h"
#include "pits_impl.h"
#include "kv_config.h"
#include "mbed_error.h"

// In EMUL world, there is no real partitioning, which makes the source partition irrelevant.
// So here we set a global pid value to be used for when calling IMPL functions
#define PSA_ITS_EMUL_PID        1

psa_status_t psa_its_set(psa_storage_uid_t uid, size_t data_length, const void *p_data, psa_storage_create_flags_t create_flags)
{
    if (!p_data && data_length) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    // KVStore initiation:
    // - In EMUL (non-secure single core) we do it here since we don't have another context to do it inside.
    // - Repeating calls has no effect
    int kv_status = kv_init_storage_config();
    if (kv_status != MBED_SUCCESS) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    psa_status_t res = psa_its_set_impl(PSA_ITS_EMUL_PID, uid, data_length, p_data, create_flags);

    return res;
}

psa_status_t psa_its_get(psa_storage_uid_t uid, size_t data_offset, size_t data_length, void *p_data, size_t *p_data_length)
{
    if ((!p_data && data_length) || !p_data_length) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    // KVStore initiation:
    // - In EMUL (non-secure single core) we do it here since we don't have another context to do it inside.
    // - Repeating calls has no effect
    int kv_status = kv_init_storage_config();
    if (kv_status != MBED_SUCCESS) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return psa_its_get_impl(PSA_ITS_EMUL_PID, uid, data_offset, data_length, p_data, p_data_length);
}

psa_status_t psa_its_get_info(psa_storage_uid_t uid, struct psa_storage_info_t *p_info)
{
    if (!p_info) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    // KVStore initiation:
    // - In EMUL (non-secure single core) we do it here since we don't have another context to do it inside.
    // - Repeating calls has no effect
    int kv_status = kv_init_storage_config();
    if (kv_status != MBED_SUCCESS) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return psa_its_get_info_impl(PSA_ITS_EMUL_PID, uid, p_info);
}

psa_status_t psa_its_remove(psa_storage_uid_t uid)
{
    // KVStore initiation:
    // - In EMUL (non-secure single core) we do it here since we don't have another context to do it inside.
    // - Repeating calls has no effect
    int kv_status = kv_init_storage_config();
    if (kv_status != MBED_SUCCESS) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return psa_its_remove_impl(PSA_ITS_EMUL_PID, uid);
}

extern "C" psa_status_t psa_its_reset()
{
    // KVStore initiation:
    // - In EMUL (non-secure single core) we do it here since we don't have another context to do it inside.
    // - Repeating calls has no effect
    int kv_status = kv_init_storage_config();
    if (kv_status != MBED_SUCCESS) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return psa_its_reset_impl();
}
