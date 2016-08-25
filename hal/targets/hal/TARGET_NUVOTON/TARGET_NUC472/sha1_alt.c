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

#if defined(MBEDTLS_SHA1_C)
#if defined(MBEDTLS_SHA1_ALT)

#include "sha1_alt.h"
#include "crypto.h"

void mbedtls_sha1_init(mbedtls_sha1_context *ctx)
{
    if (crypto_sha_hw_acquire()) {
        ctx->mbedtls_sha1_init = mbedtls_sha1_hw_init;
        ctx->mbedtls_sha1_free = mbedtls_sha1_hw_free;
        ctx->mbedtls_sha1_clone = mbedtls_sha1_hw_clone;
        ctx->mbedtls_sha1_starts = mbedtls_sha1_hw_starts;
        ctx->mbedtls_sha1_update = mbedtls_sha1_hw_update;
        ctx->mbedtls_sha1_finish = mbedtls_sha1_hw_finish;
        ctx->mbedtls_sha1_process = mbedtls_sha1_hw_process;
    }
    else {
        ctx->mbedtls_sha1_init = mbedtls_sha1_sw_init;
        ctx->mbedtls_sha1_free = mbedtls_sha1_sw_free;
        ctx->mbedtls_sha1_clone = mbedtls_sha1_sw_clone;
        ctx->mbedtls_sha1_starts = mbedtls_sha1_sw_starts;
        ctx->mbedtls_sha1_update = mbedtls_sha1_sw_update;
        ctx->mbedtls_sha1_finish = mbedtls_sha1_sw_finish;
        ctx->mbedtls_sha1_process = mbedtls_sha1_sw_process;
    }
    
    ctx->mbedtls_sha1_init(ctx);
}

void mbedtls_sha1_free(mbedtls_sha1_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    ctx->mbedtls_sha1_free(ctx);
    
    if (ctx->mbedtls_sha1_init == mbedtls_sha1_hw_init) {
        crypto_sha_hw_release();
    }
}

void mbedtls_sha1_clone(mbedtls_sha1_context *dst,
                        const mbedtls_sha1_context *src)
{
    *dst = *src;
}

/*
 * SHA-1 context setup
 */
void mbedtls_sha1_starts(mbedtls_sha1_context *ctx)
{
    ctx->mbedtls_sha1_starts(ctx);
    
    return;
}

/*
 * SHA-1 process buffer
 */
void mbedtls_sha1_update(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    ctx->mbedtls_sha1_update(ctx, input, ilen);
}

/*
 * SHA-1 final digest
 */
void mbedtls_sha1_finish(mbedtls_sha1_context *ctx, unsigned char output[20])
{
    ctx->mbedtls_sha1_finish(ctx, output);
}

void mbedtls_sha1_process(mbedtls_sha1_context *ctx, const unsigned char data[64])
{
    ctx->mbedtls_sha1_process(ctx, data);
}

#endif /* MBEDTLS_SHA1_ALT */
#endif /* MBEDTLS_SHA1_C */
