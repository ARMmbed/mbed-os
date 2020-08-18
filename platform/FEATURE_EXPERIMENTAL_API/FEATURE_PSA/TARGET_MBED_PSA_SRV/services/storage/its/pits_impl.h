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

#include "psa/error.h"
#include "psa/storage_common.h"

#ifdef   __cplusplus
extern "C"
{
#endif

psa_status_t psa_its_set_impl(int32_t pid, psa_storage_uid_t uid, size_t data_length, const void *p_data, psa_storage_create_flags_t create_flags);
psa_status_t psa_its_get_impl(int32_t pid, psa_storage_uid_t uid, size_t data_offset, size_t data_length, void *p_data, size_t *p_data_length);
psa_status_t psa_its_get_info_impl(int32_t pid, psa_storage_uid_t uid, struct psa_storage_info_t *p_info);
psa_status_t psa_its_remove_impl(int32_t pid, psa_storage_uid_t uid);
psa_status_t psa_its_reset_impl();

#ifdef   __cplusplus
}
#endif

#endif // __PITS_IMPL_H__
