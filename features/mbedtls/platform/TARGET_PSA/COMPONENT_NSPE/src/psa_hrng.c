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

#include "trng_api.h"
#include "crypto.h"

#if DEVICE_TRNG

__WEAK void trng_init(trng_t *obj)
{
    (void)(obj);
}


__WEAK void trng_free(trng_t *obj)
{
    (void)(obj);
}

__WEAK int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    if (output == NULL){
        return -1;
    }

    if(psa_crypto_init() != PSA_SUCCESS) {
        return -1;
    }

    if (psa_generate_random(output, length) != PSA_SUCCESS) {
        return -1;
    }

    mbedtls_psa_crypto_free();

    ((void)(obj));
    ((void)(output_length));

    return 0;
}

#endif  // DEVICE_TRNG
