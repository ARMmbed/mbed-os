/* Copyright (c) 2019 ARM Limited
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

#include <cstring>
#include "KVMap.h"
#include "KVStore.h"
#include "kv_config.h"
#include "TDBStore.h"
#include "psa/protected_storage.h"
#include "psa_storage_common_impl.h"
#include "mbed_error.h"
#include "mbed_toolchain.h"

using namespace mbed;

#ifdef   __cplusplus
extern "C"
{
#endif

#define STR_EXPAND(tok)                 #tok
#define PS_VERSION_KEY "PSA_PS_VERSION"  // PS version entry identifier in TDBStore

// Global PID for protected storage, used when calling IMPL functions
#define PSA_PS_GLOBAL_PID        1

static KVStore *kvstore = NULL;
static bool initialized = false;

MBED_WEAK psa_status_t ps_version_migrate(KVStore *kvstore,
                                          const psa_storage_version_t *old_version, const psa_storage_version_t *new_version)
{
    (void)kvstore;
    (void)old_version;
    (void)new_version;
    return PSA_SUCCESS;
}


static void ps_init(void)
{
    int ret = kv_init_storage_config();
    if (ret) {
        // Can only happen due to system misconfiguration.
        // Thus considered as unrecoverable error for runtime.
        error("Failed initializing kvstore configuration\n");
    }
    KVMap &kv_map = KVMap::get_instance();
    psa_storage_version_t version = {PSA_PS_API_VERSION_MAJOR, PSA_PS_API_VERSION_MINOR};
    kvstore = kv_map.get_main_kv_instance(STR_EXPAND(MBED_CONF_STORAGE_DEFAULT_KV));
    KVStore *int_kvstore = kv_map.get_internal_kv_instance(STR_EXPAND(MBED_CONF_STORAGE_DEFAULT_KV));;
    if (!kvstore || !int_kvstore) {
        // Can only happen due to system misconfiguration.
        // Thus considered as unrecoverable error for runtime.
        error("Failed getting kvstore instance\n");
    }

    psa_storage_handle_version(kvstore, PS_VERSION_KEY, &version, ps_version_migrate);
    initialized = true;
}

// used from test only
void ps_deinit(void)
{
    kvstore = NULL;
    initialized = false;
}


psa_status_t psa_ps_set(psa_storage_uid_t uid, size_t data_length, const void *p_data, psa_storage_create_flags_t create_flags)
{
    if (!initialized) {
        ps_init();
    }

    if (create_flags & ~
            (PSA_STORAGE_FLAG_WRITE_ONCE | PSA_STORAGE_FLAG_NO_CONFIDENTIALITY | PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION)) {
        return PSA_ERROR_NOT_SUPPORTED;
    }

    // Assume confidentiality and replay protection are set by default
    uint32_t kv_create_flags = KVStore::REQUIRE_CONFIDENTIALITY_FLAG | KVStore::REQUIRE_REPLAY_PROTECTION_FLAG;

    if (create_flags & PSA_STORAGE_FLAG_NO_CONFIDENTIALITY) {
        kv_create_flags &= ~KVStore::REQUIRE_CONFIDENTIALITY_FLAG;
    }
    if (create_flags & PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION) {
        kv_create_flags &= ~KVStore::REQUIRE_REPLAY_PROTECTION_FLAG;
    }
    if (create_flags & PSA_STORAGE_FLAG_WRITE_ONCE) {
        kv_create_flags |= KVStore::WRITE_ONCE_FLAG;
    }

    return psa_storage_set_impl(kvstore, PSA_PS_GLOBAL_PID, uid, data_length, p_data, kv_create_flags);
}

psa_status_t psa_ps_get(psa_storage_uid_t uid, size_t data_offset, size_t data_length, void *p_data, size_t *p_data_length)
{
    if (!initialized) {
        ps_init();
    }

    return psa_storage_get_impl(kvstore, PSA_PS_GLOBAL_PID, uid, data_offset, data_length, p_data, p_data_length);
}

psa_status_t psa_ps_get_info(psa_storage_uid_t uid, struct psa_storage_info_t *p_info)
{
    psa_status_t ret;
    uint32_t kv_get_flags;

    if (!initialized) {
        ps_init();
    }

    ret = psa_storage_get_info_impl(kvstore, PSA_PS_GLOBAL_PID, uid, p_info, &kv_get_flags);

    if ((kv_get_flags & ~KVStore::REQUIRE_CONFIDENTIALITY_FLAG) == kv_get_flags) {
        p_info->flags |= PSA_STORAGE_FLAG_NO_CONFIDENTIALITY;
    }
    if ((kv_get_flags & ~KVStore::REQUIRE_REPLAY_PROTECTION_FLAG) == kv_get_flags) {
        p_info->flags |= PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION;
    }

    return ret;
}

psa_status_t psa_ps_remove(psa_storage_uid_t uid)
{
    if (!initialized) {
        ps_init();
    }

    return psa_storage_remove_impl(kvstore, PSA_PS_GLOBAL_PID, uid);
}

extern "C" psa_status_t psa_ps_reset()
{
    // Do not call its_init here to avoid version check before reset
    int ret = kv_init_storage_config();
    if (ret) {
        // Can only happen due to system misconfiguration.
        // Thus considered as unrecoverable error for runtime.
        error("Failed initializing kvstore configuration\n");
    }

    KVMap &kv_map = KVMap::get_instance();
    kvstore = kv_map.get_main_kv_instance(STR_EXPAND(MBED_CONF_STORAGE_DEFAULT_KV));
    if (!kvstore) {
        // Can only happen due to system misconfiguration.
        // Thus considered as unrecoverable error for runtime.
        error("Failed getting kvstore instance\n");
    }

    return psa_storage_reset_impl(kvstore);
}

#ifdef   __cplusplus
}
#endif
