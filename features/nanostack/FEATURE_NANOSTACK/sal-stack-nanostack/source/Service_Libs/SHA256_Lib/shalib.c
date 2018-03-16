/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "common_functions.h"
#include <stdint.h>
#include <string.h>
#include "shalib.h"
#include "ns_sha256.h"

#define SHALIB_OPAD 0x5c
#define SHALIB_IPAD 0x36

static void SHALIB_presecret_set(uint8_t *buffer, uint8_t operation);
static void SHALIB_HMAC_inpad(void);

static prf_sec_param_t prf_sec;

static ns_sha256_context hmac_sha_ctx;


static void SHALIB_presecret_set(uint8_t *buffer, uint8_t operation)
{
    uint8_t i;
    memset(buffer, 0, 64);
    memcpy(buffer, prf_sec.secret, prf_sec.sec_len);

    for (i = 0; i < 64; i++) {
        buffer[i] ^= operation;
    }
}


void SHALIB_init_HMAC(const uint8_t *secret, uint8_t sec_len)
{
    prf_sec.sec_len  = sec_len;
    prf_sec.secret = secret;
    SHALIB_HMAC_inpad();
}

static void SHALIB_HMAC_inpad(void)
{
    uint8_t Kipad[64];
    SHALIB_presecret_set(Kipad, SHALIB_IPAD);
    ns_sha256_init(&hmac_sha_ctx);
    ns_sha256_starts(&hmac_sha_ctx);
    ns_sha256_update(&hmac_sha_ctx, Kipad, 64);
}

void SHALIB_push_data_HMAC(const void *data, uint16_t len)
{
    ns_sha256_update(&hmac_sha_ctx, data, len);
}

void SHALIB_finish_HMAC(void *buffer, uint8_t len)
{
    {
        uint8_t hash[32];

        ns_sha256_finish(&hmac_sha_ctx, hash);
        ns_sha256_starts(&hmac_sha_ctx);
        {
            uint8_t Kopad[64];
            SHALIB_presecret_set(Kopad, SHALIB_OPAD);
            ns_sha256_update(&hmac_sha_ctx, Kopad, 64);
        }
        ns_sha256_update(&hmac_sha_ctx, hash, 32);
    }
    ns_sha256_finish_nbits(&hmac_sha_ctx, buffer, len * 32);
    ns_sha256_free(&hmac_sha_ctx);
}


prf_sec_param_t *shalib_prf_param_get(void)
{
    return &prf_sec;
}

void shalib_prf_calc(void *output, uint_fast16_t nwords)
{
    uint8_t *outptr = output;
    uint8_t A[32];

    if (nwords == 0) {
        return;
    }

    /* Compute initial A(1) = HMAC(secret, label + seed) */
    SHALIB_HMAC_inpad();
    SHALIB_push_data_HMAC(prf_sec.label, strlen(prf_sec.label));
    SHALIB_push_data_HMAC(prf_sec.seed, prf_sec.seedlen);
    SHALIB_finish_HMAC(A, 8);

    for (;;) {
        /* Output PRF(i) = HMAC(secret, A(i) + label + seed) */
        SHALIB_HMAC_inpad();
        SHALIB_push_data_HMAC(A, 32);
        SHALIB_push_data_HMAC(prf_sec.label, strlen(prf_sec.label));
        SHALIB_push_data_HMAC(prf_sec.seed, prf_sec.seedlen);
        unsigned words_this_time = nwords > 8 ? 8 : nwords;
        SHALIB_finish_HMAC(outptr, words_this_time);
        outptr += words_this_time * 4;
        nwords -= words_this_time;
        if (nwords == 0) {
            break;
        }

        /* Compute A(i+1) = HMAC(secret, A(i)) */
        SHALIB_HMAC_inpad();
        SHALIB_push_data_HMAC(A, 32);
        SHALIB_finish_HMAC(A, 8);
    }
}
