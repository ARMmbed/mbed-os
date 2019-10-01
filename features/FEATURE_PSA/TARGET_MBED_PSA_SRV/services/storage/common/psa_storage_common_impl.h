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

#ifndef __PSA_STORAGE_COMMON_IMPL_H__
#define __PSA_STORAGE_COMMON_IMPL_H__

#include "psa/error.h"
#include "psa/storage_common.h"
#include "KVStore.h"

#ifdef   __cplusplus
extern "C"
{
#endif

typedef struct {
    uint32_t major;
    uint32_t minor;
} psa_storage_version_t;

typedef psa_status_t (*migrate_func_t)(mbed::KVStore *kvstore, const psa_storage_version_t *old_version, const psa_storage_version_t *new_version);

void psa_storage_handle_version(mbed::KVStore *kvstore, const char *version_key, const psa_storage_version_t *version,
                                migrate_func_t migrate_func);
psa_status_t psa_storage_set_impl(mbed::KVStore *kvstore, int32_t pid, psa_storage_uid_t uid, size_t data_length, const void *p_data, uint32_t kv_create_flags);
psa_status_t psa_storage_get_impl(mbed::KVStore *kvstore, int32_t pid, psa_storage_uid_t uid, size_t data_offset, size_t data_length, void *p_data, size_t *p_data_length);
psa_status_t psa_storage_get_info_impl(mbed::KVStore *kvstore, int32_t pid, psa_storage_uid_t uid, struct psa_storage_info_t *p_info, uint32_t *kv_get_flags);
psa_status_t psa_storage_remove_impl(mbed::KVStore *kvstore, int32_t pid, psa_storage_uid_t uid);
psa_status_t psa_storage_reset_impl(mbed::KVStore *kvstore);

#ifdef   __cplusplus
}
#endif


#endif // __PSA_STORAGE_COMMON_IMPL_H__
