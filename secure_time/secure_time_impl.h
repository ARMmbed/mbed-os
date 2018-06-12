/* Copyright (c) 2018 ARM Limited
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

#ifndef __SECURE_TIME_IMPL_H__
#define __SECURE_TIME_IMPL_H__

#include "secure_time_client_spe.h"

#ifdef __cplusplus
extern "C" {
#endif

/**< Maximum time a nonce is valid after generation.*/
#define SECURE_TIME_NONCE_TIMEOUT_SECONDS           (300UL)

int32_t secure_time_set_trusted_init_impl(uint64_t *nonce);

int32_t secure_time_set_trusted_commit_impl(const void *blob, size_t blob_size);

int32_t secure_time_set_impl(uint64_t new_time);

uint64_t secure_time_get_impl(void);

int32_t secure_time_set_stored_public_key_impl(const void* pubkey, size_t key_size);

int32_t secure_time_get_stored_public_key_impl(
    void *pubkey,
    size_t size,
    size_t *actual_size
);

int32_t secure_time_get_stored_public_key_size_impl(size_t *actual_size);

#ifdef __cplusplus
}
#endif

#endif  // __SECURE_TIME_IMPL_H__
