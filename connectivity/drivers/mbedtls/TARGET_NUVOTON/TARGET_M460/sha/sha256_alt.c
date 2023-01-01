/*
 * Copyright (c) 2022, Nuvoton Technology Corporation
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

#include "mbedtls/sha256.h"

#if defined(MBEDTLS_SHA256_C)
#if defined(MBEDTLS_SHA256_ALT)

#include "crypto-misc.h"
#include "nu_bitutil.h"
#include "string.h"

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    crypto_sha_init(&ctx->hw_ctx, SHA_MODE_SHA256);
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    crypto_sha_free(&ctx->hw_ctx);
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst,
                          const mbedtls_sha256_context *src)
{
    // Corner case: Destination/source contexts are the same
    if (dst == src) {
        return;
    }

    memcpy(dst, src, sizeof(mbedtls_sha256_context));
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts_ret(mbedtls_sha256_context *ctx, int is224)
{
    if (ctx == NULL) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }

    return crypto_sha_starts(&ctx->hw_ctx, is224);
}

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update_ret(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    if (ctx == NULL || (input == NULL && ilen == 0)) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }

    return crypto_sha_update(&ctx->hw_ctx, input, ilen);
}

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish_ret(mbedtls_sha256_context *ctx, unsigned char output[32])
{
    if (ctx == NULL || output == NULL) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }

    return crypto_sha_finish(&ctx->hw_ctx, output, ctx->hw_ctx.is224_384 ? 28 : 32);
}

int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    if (ctx == NULL || data == NULL) {
        return MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
    }

    return crypto_sha_update(&ctx->hw_ctx, data, 64);
}

#endif /* MBEDTLS_SHA256_ALT */
#endif /* MBEDTLS_SHA256_C */
