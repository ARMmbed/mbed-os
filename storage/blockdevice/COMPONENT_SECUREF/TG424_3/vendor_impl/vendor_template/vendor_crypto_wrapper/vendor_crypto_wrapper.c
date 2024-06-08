/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
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
 
#include <stdbool.h>
#include <string.h>
#include "../../../JEDEC_security_HAL/crypto_wrapper.h"

#define KEY_HANDLE_NOT_LOADED (0)
#define MAC_SIZE
#define TAG_LENGTH
#define MAX_ROOT_KEY_NUM
#define MAX_SESSION_KEY_NUM
#define MAC_KEY_SIZE
#define ENC_KEY_SIZE


typedef struct {
    //TODO
}session_key_id_t;


static int crypto_wrapper_init(void)
{
    //TODO
}

static int crypto_wrapper_deinit(void)
{
    //TODO
}

static int crypto_wrapper_algorithm_support(int alg, int index)
{
    //TODO
}

static int crypto_wrapper_crypto_func(crypto_indicator_t *indicator)
{
    //TODO
}
static int crypto_wrapper_kdf(crypto_indicator_t *indicator,
                              uint32_t *output_key_id)
{
    //TODO
}

static int crypto_wrapper_generate_random(uint8_t *output, uint32_t output_size)
{
    //TODO
}

static int crypto_wrapper_ecdh_gen_key_pair(crypto_indicator_t *indicator)
{
    //TODO
}

static int crypto_wrapper_ecdh_gen_shared_secret(crypto_indicator_t *indicator)
{
    //TODO
}

static int crypto_wrapper_open_key(uint32_t root_key_id)
{
    //TODO
}

static int crypto_wrapper_close_key(uint32_t root_key_id)
{
    //TODO
}
static int crypto_wrapper_destroy_key(uint32_t key_id)
{
    //TODO
}

static int crypto_wrapper_export_public_key(uint32_t key_id, uint8_t *key_buf,
                                            uint32_t buf_size, uint32_t *actual_size)
{
    //TODO
}

static int crypto_wrapper_export_key(uint32_t key_id, uint8_t *key_buf,
                                     uint32_t buf_size, uint32_t *actual_size)
{
    //TODO
}

static int crypto_wrapper_import_key(uint32_t *key_id, uint8_t *key_buf,
                                     uint32_t key_size, KeyLifeTime lifetime)
{
    //TODO
}

crypto_wrapper_t vendor_crypto_wrapper = {
    .init = crypto_wrapper_init,
    .deinit = crypto_wrapper_deinit,
    .algorithm_support = crypto_wrapper_algorithm_support,
    .crypto_func = crypto_wrapper_crypto_func,
    .key_derive = crypto_wrapper_kdf,
    .generate_random = crypto_wrapper_generate_random,
    .ecdh_gen_key_pair = crypto_wrapper_ecdh_gen_key_pair,
    .ecdh_gen_shared_secret = crypto_wrapper_ecdh_gen_shared_secret,
    .open_key = crypto_wrapper_open_key,
    .close_key = crypto_wrapper_close_key,
    .destroy_key = crypto_wrapper_destroy_key,
    .export_public_key = crypto_wrapper_export_public_key,
    .export_key = crypto_wrapper_export_key,
    .import_key = crypto_wrapper_import_key,
};
