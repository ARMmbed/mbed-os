/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA256_C)
#if defined(MBEDTLS_SHA256_ALT)

#include "sha256_alt.h"
#include "crypto.h"

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    if (crypto_sha_hw_acquire()) {
        ctx->mbedtls_sha256_init = mbedtls_sha256_hw_init;
        ctx->mbedtls_sha256_free = mbedtls_sha256_hw_free;
        ctx->mbedtls_sha256_clone = mbedtls_sha256_hw_clone;
        ctx->mbedtls_sha256_starts = mbedtls_sha256_hw_starts;
        ctx->mbedtls_sha256_update = mbedtls_sha256_hw_update;
        ctx->mbedtls_sha256_finish = mbedtls_sha256_hw_finish;
        ctx->mbedtls_sha256_process = mbedtls_sha256_hw_process;
    }
    else {
        ctx->mbedtls_sha256_init = mbedtls_sha256_sw_init;
        ctx->mbedtls_sha256_free = mbedtls_sha256_sw_free;
        ctx->mbedtls_sha256_clone = mbedtls_sha256_sw_clone;
        ctx->mbedtls_sha256_starts = mbedtls_sha256_sw_starts;
        ctx->mbedtls_sha256_update = mbedtls_sha256_sw_update;
        ctx->mbedtls_sha256_finish = mbedtls_sha256_sw_finish;
        ctx->mbedtls_sha256_process = mbedtls_sha256_sw_process;
    }
    
    ctx->mbedtls_sha256_init(ctx);
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    ctx->mbedtls_sha256_free(ctx);
    
    if (ctx->mbedtls_sha256_init == mbedtls_sha256_hw_init) {
        crypto_sha_hw_release();
    }
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst,
                        const mbedtls_sha256_context *src)
{
    *dst = *src;
}

/*
 * SHA-256 context setup
 */
void mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int is224)
{   
    ctx->mbedtls_sha256_starts(ctx, is224);
    
    return;
}

/*
 * SHA-256 process buffer
 */
void mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    ctx->mbedtls_sha256_update(ctx, input, ilen);
}

/*
 * SHA-256 final digest
 */
void mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char output[20])
{
    ctx->mbedtls_sha256_finish(ctx, output);
}

void mbedtls_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    ctx->mbedtls_sha256_process(ctx, data);
}

#endif /* MBEDTLS_SHA256_ALT */
#endif /* MBEDTLS_SHA256_C */
