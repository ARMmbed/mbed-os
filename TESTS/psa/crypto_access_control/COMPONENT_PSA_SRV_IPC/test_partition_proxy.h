/*
 * Copyright (c) 2019, Arm Limited and affiliates
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

#ifndef TEST_PARTITION_PROXY_H
#define TEST_PARTITION_PROXY_H

#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

psa_status_t test_partition_crypto_get_key_attributes(
    psa_key_handle_t key_handle, psa_key_attributes_t *attributes);

psa_status_t test_partition_crypto_generate_key(
    const psa_key_attributes_t *attributes, psa_key_handle_t *key_handle);

psa_status_t test_partition_crypto_open_key(
    psa_key_id_t key_id, psa_key_handle_t *key_handle);

psa_status_t test_partition_crypto_close_key(psa_key_handle_t key_handle);

psa_status_t test_partition_crypto_destroy_key(psa_key_handle_t key_handle);

psa_status_t test_partition_crypto_import_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_data,
    size_t key_data_size,
    psa_key_handle_t *key_handle);

#ifdef __cplusplus
}
#endif

#endif /* TEST_PARTITION_PROXY_H */
