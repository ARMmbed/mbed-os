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

#ifndef PSA_CRYPTO_ACCESS_CONTROL_H
#define PSA_CRYPTO_ACCESS_CONTROL_H

#include <stdint.h>

#include "crypto_platform.h"

/* initialize the module, resets all tracked information */
void psa_crypto_access_control_init(void);

/* deinitialize the module, resets all tracked information */
void psa_crypto_access_control_destroy(void);

/* tracks and associates the key_handle with partition_id */
void psa_crypto_access_control_register_handle(psa_key_handle_t key_handle, int32_t partition_id);

/* removes tracking of the key_handle */
void psa_crypto_access_control_unregister_handle(psa_key_handle_t key_handle);

/* checks if the key_handle is associated with the partition_id, returns 0 is false otherwise 1 */
uint8_t psa_crypto_access_control_is_handle_permitted(psa_key_handle_t key_handle, int32_t partition_id);

#endif /* PSA_CRYPTO_ACCESS_CONTROL_H */
