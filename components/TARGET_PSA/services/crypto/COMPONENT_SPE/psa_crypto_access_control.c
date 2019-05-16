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

#include <string.h>

#include "psa/client.h"
#include "psa/service.h"
#include "psa_crypto_access_control.h"
#include "psa_crypto_core.h"
#include "psa_crypto_slot_management.h"

typedef struct psa_crypto_access_control_s {
    psa_key_handle_t key_handle;
    int32_t partition_id;
} psa_crypto_access_control_t;

static psa_crypto_access_control_t crypto_access_control_arr[PSA_KEY_SLOT_COUNT];

static inline void psa_crypto_access_control_reset()
{
    memset(crypto_access_control_arr, 0, sizeof(crypto_access_control_arr));
}

void psa_crypto_access_control_init(void)
{
    psa_crypto_access_control_reset();
}

void psa_crypto_access_control_destroy(void)
{
    psa_crypto_access_control_reset();
}

void psa_crypto_access_control_register_handle(psa_key_handle_t key_handle, int32_t partition_id)
{
    for (size_t i = 0; i < PSA_KEY_SLOT_COUNT; i++) {
        if (crypto_access_control_arr[i].key_handle == 0 &&
                crypto_access_control_arr[i].partition_id == 0) {
            crypto_access_control_arr[i].key_handle = key_handle;
            crypto_access_control_arr[i].partition_id = partition_id;
            return;
        }
    }

    SPM_PANIC("psa_crypto_access_control_register_handle failed");
}

void psa_crypto_access_control_unregister_handle(psa_key_handle_t key_handle)
{
    for (size_t i = 0; i < PSA_KEY_SLOT_COUNT; i++) {
        if (crypto_access_control_arr[i].key_handle == key_handle) {
            crypto_access_control_arr[i].key_handle = 0;
            crypto_access_control_arr[i].partition_id = 0;
            return;
        }
    }

    SPM_PANIC("psa_crypto_access_control_unregister_handle failed");
}

uint8_t psa_crypto_access_control_is_handle_permitted(psa_key_handle_t key_handle, int32_t partition_id)
{
    for (size_t i = 0; i < PSA_KEY_SLOT_COUNT; i++) {
        if (crypto_access_control_arr[i].key_handle == key_handle &&
                crypto_access_control_arr[i].partition_id == partition_id) {
            return 1;
        }
    }

    return 0;
}
