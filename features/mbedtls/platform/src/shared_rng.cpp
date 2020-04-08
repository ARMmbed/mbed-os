/*
 *  shared_rng.cpp
 *
 *  Copyright (C) 2019-2020, Arm Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "shared_rng.h"

#if defined(MBEDTLS_SSL_CONF_RNG)

#include "mbed_trace.h"

#define TRACE_GROUP "SRNG"

mbedtls_hmac_drbg_context global_hmac_drbg;
mbedtls_entropy_context global_entropy;
static bool is_initialized = false;

int init_global_rng()
{
    mbedtls_entropy_init(&global_entropy);
    mbedtls_hmac_drbg_init(&global_hmac_drbg);

    int ret = mbedtls_hmac_drbg_seed(&global_hmac_drbg,
                                     mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                                     mbedtls_entropy_func, &global_entropy, NULL, 0);

    if (ret != 0) {
        tr_error(" init_global_rng failed! mbedtls_hmac_drbg_seed returned -0x%x", -ret);
        free_global_rng();
    } else {
        is_initialized = true;
    }

    return ret;
}

void free_global_rng()
{
    mbedtls_entropy_free(&global_entropy);
    mbedtls_hmac_drbg_free(&global_hmac_drbg);
    is_initialized = false;
}

int global_rng( void *ctx, unsigned char *dst, size_t len )
{
    if (!is_initialized) {
        return MBED_SHARED_RNG_NOT_INITIALIZED;
    }
    return mbedtls_hmac_drbg_random(&global_hmac_drbg, dst, len);
}

mbedtls_hmac_drbg_context *get_global_hmac_drbg()
{
    return &global_hmac_drbg;
}

mbedtls_entropy_context *get_global_entropy()
{
    return &global_entropy;
}

#endif // MBEDTLS_SSL_CONF_RNG
