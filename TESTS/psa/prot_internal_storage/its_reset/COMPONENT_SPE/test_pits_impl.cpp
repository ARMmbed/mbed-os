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
#include "kv_config.h"
#include "KVMap.h"
#include "KVStore.h"
#include "mbed_error.h"

#ifdef   __cplusplus
extern "C"
{
#endif

using namespace mbed;

#define STR_EXPAND(tok)     #tok

psa_its_status_t test_psa_its_reset_impl(void)
{
    psa_its_status_t status = PSA_ITS_SUCCESS;

    int kv_status = kv_init_storage_config();
    if (kv_status != MBED_SUCCESS) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    KVMap &kv_map = KVMap::get_instance();
    KVStore *kvstore = kv_map.get_main_kv_instance(STR_EXPAND(MBED_CONF_STORAGE_DEFAULT_KV));
    if (!kvstore) {
        return PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    if (kvstore->reset() != MBED_SUCCESS) {
        status = PSA_ITS_ERROR_STORAGE_FAILURE;
    }

    return status;
}

#ifdef   __cplusplus
}
#endif
