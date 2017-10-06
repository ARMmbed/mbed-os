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
#include "crypto-misc.h"
#include "nu_bitutil.h"
#include "string.h"

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    if (crypto_sha_acquire()) {
        ctx->ishw = 1;
        mbedtls_sha256_hw_init(&ctx->hw_ctx);
    }
    else {
        ctx->ishw = 0;
        mbedtls_sha256_sw_init(&ctx->sw_ctx);
    }
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    if (ctx->ishw) {
        mbedtls_sha256_hw_free(&ctx->hw_ctx);
        crypto_sha_release();
    }
    else {
        mbedtls_sha256_sw_free(&ctx->sw_ctx);
    }
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst,
                        const mbedtls_sha256_context *src)
{
    if (src->ishw) {
        // Clone S/W ctx from H/W ctx
        dst->ishw = 0;
        dst->sw_ctx.total[0] = src->hw_ctx.total;
        dst->sw_ctx.total[1] = 0;
        {
            unsigned char output[32];
            crypto_sha_getinternstate(output, sizeof (output));
            unsigned char *output_pos = output;
            unsigned char *output_end = output + (sizeof (output) / sizeof (output[0]));
            uint32_t *state_pos = (uint32_t *) &(dst->sw_ctx.state[0]);
            while (output_pos != output_end) {
                *state_pos ++ = nu_get32_be(output_pos);
                output_pos += 4;
            }
        }
        memcpy(dst->sw_ctx.buffer, src->hw_ctx.buffer, src->hw_ctx.buffer_left);
        dst->sw_ctx.is224 = src->hw_ctx.is224;
        if (src->hw_ctx.buffer_left == src->hw_ctx.blocksize) {
            mbedtls_sha256_sw_process(&dst->sw_ctx, dst->sw_ctx.buffer);
        }
    }
    else {
        // Clone S/W ctx from S/W ctx
        dst->sw_ctx = src->sw_ctx;
    }
}

/*
 * SHA-256 context setup
 */
void mbedtls_sha256_starts(mbedtls_sha256_context *ctx, int is224)
{   
    if (ctx->ishw) {
        mbedtls_sha256_hw_starts(&ctx->hw_ctx, is224);
    }
    else {
        mbedtls_sha256_sw_starts(&ctx->sw_ctx, is224);
    }
}

/*
 * SHA-256 process buffer
 */
void mbedtls_sha256_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    if (ctx->ishw) {
        mbedtls_sha256_hw_update(&ctx->hw_ctx, input, ilen);
    }
    else {
        mbedtls_sha256_sw_update(&ctx->sw_ctx, input, ilen);
    }
}

/*
 * SHA-256 final digest
 */
void mbedtls_sha256_finish(mbedtls_sha256_context *ctx, unsigned char output[32])
{
    if (ctx->ishw) {
        mbedtls_sha256_hw_finish(&ctx->hw_ctx, output);
    }
    else {
        mbedtls_sha256_sw_finish(&ctx->sw_ctx, output);
    }
}

void mbedtls_sha256_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    if (ctx->ishw) {
        mbedtls_sha256_hw_process(&ctx->hw_ctx, data);
    }
    else {
        mbedtls_sha256_sw_process(&ctx->sw_ctx, data);
    }
}

#endif /* MBEDTLS_SHA256_ALT */
#endif /* MBEDTLS_SHA256_C */
