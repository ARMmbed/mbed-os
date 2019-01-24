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
#include <string.h>
#include <stdlib.h>
#include "psa_prot_internal_storage.h"
#include "test_pits_impl.h"
#include "pits_impl.h"
#include "pits_version_impl.h"
#include "kv_config.h"
#include "KVMap.h"
#include "KVStore.h"
#include "mbed_error.h"

using namespace mbed;

#ifdef   __cplusplus
extern "C"
{
#endif

void its_deinit(void);

static KVStore *get_instance(void)
{
    int kv_status = kv_init_storage_config();
    if (kv_status != MBED_SUCCESS) {
        return NULL;
    }

    KVMap &kv_map = KVMap::get_instance();
    return kv_map.get_internal_kv_instance(STR_EXPAND(MBED_CONF_STORAGE_DEFAULT_KV));
}

psa_its_status_t test_psa_its_reset_impl(void)
{
    psa_its_status_t status = PSA_ITS_SUCCESS;
    KVStore *kvstore = get_instance();
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    if (kvstore->reset() != MBED_SUCCESS) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return status;
}

psa_its_status_t test_psa_its_set_ver_impl(uint32_t major, uint32_t minor)
{
    its_version_t ver = {major, minor};
    psa_its_status_t status = PSA_ITS_SUCCESS;
    KVStore *kvstore = get_instance();
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    if (kvstore->set(ITS_VERSION_KEY, &ver, sizeof(ver), 0) != MBED_SUCCESS) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return status;
}

psa_its_status_t test_psa_its_get_ver_impl(uint32_t *major, uint32_t *minor)
{
    its_version_t ver = {0, 0};
    psa_its_status_t status = PSA_ITS_SUCCESS;
    KVStore *kvstore = get_instance();
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    if (kvstore->get(ITS_VERSION_KEY, &ver, sizeof(ver)) != MBED_SUCCESS) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    } else {
        *major = ver.major;
        *minor = ver.minor;
    }

    return status;
}

psa_its_status_t test_psa_its_deinit_impl(void)
{
    its_deinit();
    return PSA_ITS_SUCCESS;
}



#ifdef   __cplusplus
}
#endif
