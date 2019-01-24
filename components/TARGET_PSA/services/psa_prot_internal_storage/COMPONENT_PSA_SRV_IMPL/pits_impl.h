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

#ifndef __PITS_IMPL_H__
#define __PITS_IMPL_H__

#include "psa/internal_trusted_storage.h"

#ifdef   __cplusplus
extern "C"
{
#endif

#define PITS_DATA_PTR_AT_OFFSET(ptr, offset) ((void *)(((uintptr_t)ptr) + ((uintptr_t)offset)))
#define STR_EXPAND(tok)                 #tok
#define PSA_ITS_FLAGS_BIT_MASK (PSA_ITS_FLAG_NONE | PSA_ITS_FLAG_WRITE_ONCE)

psa_its_status_t psa_its_set_impl(int32_t pid, psa_its_uid_t uid, uint32_t data_length, const void *p_data, psa_its_create_flags_t create_flags);
psa_its_status_t psa_its_get_impl(int32_t pid, psa_its_uid_t uid, uint32_t data_offset, uint32_t data_length, void *p_data);
psa_its_status_t psa_its_get_info_impl(int32_t pid, psa_its_uid_t uid, struct psa_its_info_t *p_info);
psa_its_status_t psa_its_remove_impl(int32_t pid, psa_its_uid_t uid);
psa_its_status_t psa_its_reset_impl();

#ifdef   __cplusplus
}
#endif

#endif // __PITS_IMPL_H__
