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
#ifndef _CRYPTO_WRAPPER_H_
#define _CRYPTO_WRAPPER_H_

#include <stdint.h>
#include "include/crypto_defs.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef int (*init_t)(void);
typedef int (*deinit_t)(void);
typedef int (*algorithm_support_t)(int alg);
typedef int (*crypto_func_t)(crypto_indicator_t *indicator);
typedef int (*key_derive_t)(crypto_indicator_t *indicator, uint32_t *output_key_id);
typedef int (*generate_random_t)(uint8_t *odata, uint32_t odata_len);
typedef int (*ecdh_gen_key_pair_t)(crypto_indicator_t *indicator);
typedef int (*ecdh_gen_shared_secret_t)(crypto_indicator_t *indicator);

typedef int (*open_key_t)(uint32_t key_id);
typedef int (*close_key_t)(uint32_t key_id);
typedef int (*destroy_key_t)(uint32_t key_id);
typedef int (*export_public_key_t)(uint32_t key_id, uint8_t *key_buf, uint32_t buf_size, uint32_t *actual_size);
typedef int (*export_key_t)(uint32_t key_id, uint8_t *key_buf, uint32_t buf_size, uint32_t *actual_size);
typedef int (*import_key_t)(uint32_t *key_id, uint8_t *key_buf, uint32_t buf_size, KeyLifeTime lifetime);


typedef struct {
    init_t init;
    deinit_t deinit;
    algorithm_support_t algorithm_support;
    crypto_func_t crypto_func;
    key_derive_t key_derive;
    generate_random_t generate_random;
    ecdh_gen_key_pair_t ecdh_gen_key_pair;
    ecdh_gen_shared_secret_t ecdh_gen_shared_secret;

    open_key_t open_key;
    close_key_t close_key;
    destroy_key_t destroy_key;
    export_public_key_t export_public_key;
    export_key_t export_key;
    import_key_t import_key;

} crypto_wrapper_t;

#ifdef __cplusplus
}
#endif

#endif /* _CRYPTO_WRAPPER_H_ */
