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

#if defined(DEVICE_TRNG)

#include "secure_time_crypto.h"
#include "secure_time_client_spe.h"
#include "mbed_error.h"

#include "mbedtls/pk.h"
#include "mbedtls/md.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

/*
 * Structure containing contexts for random number generation.
 */
typedef struct secure_time_random_ctx {
    mbedtls_ctr_drbg_context ctr_drbg_ctx; /* CTR_DRBG context structure. */
    mbedtls_entropy_context entropy_ctx;   /* Entropy context structure. */
} secure_time_random_ctx_t;

secure_time_random_ctx_t *random_ctx = NULL;

static void random_ctx_init(secure_time_random_ctx_t *ctx)
{
    int rc = SECURE_TIME_SUCCESS;

    mbedtls_entropy_init(&(ctx->entropy_ctx));
    mbedtls_ctr_drbg_init(&(ctx->ctr_drbg_ctx));
    rc = mbedtls_ctr_drbg_seed(
        &(ctx->ctr_drbg_ctx),
        mbedtls_entropy_func,
        &(ctx->entropy_ctx),
        0,
        0
        );
    if (SECURE_TIME_SUCCESS != rc) {
        error("mbedtls_ctr_drbg_seed() failed! (rc=%d)", rc);
    }
}

static mbedtls_md_type_t md_type_from_signature_alg(SignatureAlg alg)
{
    switch(alg) {
        case SIGNATURE_ALG_SHA256_ECDSA:
            return MBEDTLS_MD_SHA256;
        default:
            return MBEDTLS_MD_NONE;
    }
}

static mbedtls_pk_type_t pk_type_from_signature_alg(SignatureAlg alg)
{
    switch(alg) {
        case SIGNATURE_ALG_SHA256_ECDSA:
            return MBEDTLS_PK_ECDSA;
        default:
            return MBEDTLS_PK_NONE;
    }
}

static void calculate_hash(
    const void *data,
    size_t data_size,
    mbedtls_md_type_t md_type,
    uint8_t *hash
    )
{
    int rc = SECURE_TIME_SUCCESS;
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(md_type);
    if (NULL == md_info) {
        error("mbedtls_md_info_from_type() returned NULL!");
    }

    rc = mbedtls_md(md_info, (const unsigned char *)data, data_size, hash);
    if (SECURE_TIME_SUCCESS != rc) {
        error("mbedtls_md() failed! (rc=%d)", rc);
    }
}

int32_t secure_time_verify_signature(
    const void *data,
    size_t data_size,
    const void *sign,
    size_t sign_size,
    const void *pubkey,
    size_t pubkey_size
    )
{
    int rc = SECURE_TIME_SUCCESS;
    uint8_t hash[MBEDTLS_MD_MAX_SIZE] = {0};
    mbedtls_pk_context pubkey_ctx = {0};

    mbedtls_md_type_t md_type = md_type_from_signature_alg(SIGNATURE_ALG_SHA256_ECDSA);
    mbedtls_pk_type_t pk_type = pk_type_from_signature_alg(SIGNATURE_ALG_SHA256_ECDSA);
    if ((MBEDTLS_MD_NONE == md_type) || (MBEDTLS_PK_NONE == pk_type)) {
        error("Failed to determine the signature algorithm!");
    }

    calculate_hash(data, data_size, md_type, hash);
    mbedtls_pk_init(&pubkey_ctx);

    rc = mbedtls_pk_parse_public_key(&pubkey_ctx, pubkey, pubkey_size);
    if (0 != rc) {
        error("Failed to parse public key! (rc=%d)", rc);
    }

    if (!mbedtls_pk_can_do(&pubkey_ctx, pk_type)) {
        error("Unable to verify signature");
    }

    rc = mbedtls_pk_verify(
        &pubkey_ctx,
        md_type,
        hash,
        0,
        (const unsigned char *)sign,
        sign_size
        );
    if (SECURE_TIME_SUCCESS != rc) {
        rc = SECURE_TIME_SIGNATURE_VERIFICATION_FAILED;
    }
    mbedtls_pk_free(&pubkey_ctx);

    return rc;
}

void secure_time_generate_random_bytes(size_t size, void *random_buf)
{
    int rc = SECURE_TIME_SUCCESS;

    if (NULL == random_ctx) {
        random_ctx = (secure_time_random_ctx_t *)malloc(sizeof(*random_ctx));
        if (NULL == random_ctx) {
            error("Failed to allocate memory for random_ctx!");
        }
        random_ctx_init(random_ctx);
    }

    rc = mbedtls_ctr_drbg_random(&(random_ctx->ctr_drbg_ctx), (unsigned char *)random_buf, size);
    if (SECURE_TIME_SUCCESS != rc) {
        error("mbedtls_ctr_drbg_random() failed! (rc=%d)", rc);
    }
}

#endif // defined(DEVICE_TRNG)
