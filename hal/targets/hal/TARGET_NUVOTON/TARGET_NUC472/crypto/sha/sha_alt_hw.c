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

#if defined(MBEDTLS_SHA1_C) || defined(MBEDTLS_SHA256_C) || defined(MBEDTLS_SHA512_C)

#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_SHA512_ALT)

#if defined(MBEDTLS_SHA1_ALT)
#include "sha1_alt.h"
#endif /* MBEDTLS_SHA1_ALT */

#if defined(MBEDTLS_SHA256_ALT)
#include "sha256_alt.h"
#endif /* MBEDTLS_SHA256_ALT */

#if defined(MBEDTLS_SHA512_ALT)
#include "sha512_alt.h"
#endif /* MBEDTLS_SHA512_ALT */

#include "nu_bitutil.h"
#include "mbed_assert.h"
#include "crypto-misc.h"

#include <string.h>

static void crypto_sha_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen);
static void crypto_sha_getinternstate(unsigned char output[], size_t olen);
static void crypto_sha_lastword(crypto_sha_context *ctx, int islast);

#endif /* MBEDTLS_SHA1_ALT || MBEDTLS_SHA256_ALT || MBEDTLS_SHA512_ALT */

#if defined(MBEDTLS_SHA1_ALT)

void mbedtls_sha1_hw_init(mbedtls_sha1_context *ctx)
{
    crypto_init();
    memset(&ctx->hw_ctx, 0, sizeof(ctx->hw_ctx));
}

void mbedtls_sha1_hw_free(mbedtls_sha1_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    crypto_zeroize(&ctx->hw_ctx, sizeof(ctx->hw_ctx));
}

void mbedtls_sha1_hw_clone(mbedtls_sha1_context *dst,
                        const mbedtls_sha1_context *src)
{
    dst->hw_ctx = src->hw_ctx;
}

void mbedtls_sha1_hw_starts(mbedtls_sha1_context *ctx)
{
    ctx->hw_ctx.total = 0;
    ctx->hw_ctx.lastword_size = 0;
    ctx->hw_ctx.blocksize = 64;
    ctx->hw_ctx.blocksize_mask = 0x3F;

    SHA_Open(SHA_MODE_SHA1, SHA_NO_SWAP);
    
    return;
}

void mbedtls_sha1_hw_update(mbedtls_sha1_context *ctx, const unsigned char *input, size_t ilen)
{
    crypto_sha_update(&ctx->hw_ctx, input, ilen);
}

void mbedtls_sha1_hw_finish(mbedtls_sha1_context *ctx, unsigned char output[20])
{
    crypto_sha_lastword(&ctx->hw_ctx, 1);
    crypto_sha_getinternstate(output, 20);
}

void mbedtls_sha1_hw_process(mbedtls_sha1_context *ctx, const unsigned char data[64])
{
    mbedtls_sha1_hw_update(ctx, data, 64);
}

#endif /* MBEDTLS_SHA1_ALT */

#if defined(MBEDTLS_SHA256_ALT)

void mbedtls_sha256_hw_init(mbedtls_sha256_context *ctx)
{
    crypto_init();
    memset(&ctx->hw_ctx, 0, sizeof(ctx->hw_ctx));
}

void mbedtls_sha256_hw_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    crypto_zeroize(&ctx->hw_ctx, sizeof(ctx->hw_ctx));
}

void mbedtls_sha256_hw_clone(mbedtls_sha256_context *dst,
                        const mbedtls_sha256_context *src)
{
    dst->hw_ctx = src->hw_ctx;
}

void mbedtls_sha256_hw_starts( mbedtls_sha256_context *ctx, int is224)
{
    ctx->hw_ctx.total = 0;
    ctx->hw_ctx.lastword_size = 0;
    ctx->hw_ctx.blocksize = 64;
    ctx->hw_ctx.blocksize_mask = 0x3F;
    ctx->hw_ctx.is224 = is224;

    SHA_Open(is224 ? SHA_MODE_SHA224 : SHA_MODE_SHA256, SHA_NO_SWAP);
    
    return;
}

void mbedtls_sha256_hw_update(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    crypto_sha_update(&ctx->hw_ctx, input, ilen);
}

void mbedtls_sha256_hw_finish(mbedtls_sha256_context *ctx, unsigned char output[32])
{
    crypto_sha_lastword(&ctx->hw_ctx, 1);
    crypto_sha_getinternstate(output, ctx->hw_ctx.is224 ? 28 : 32);
}

void mbedtls_sha256_hw_process(mbedtls_sha256_context *ctx, const unsigned char data[64])
{
    mbedtls_sha256_hw_update(ctx, data, 64);
}

#endif /* MBEDTLS_SHA256_ALT */

#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_SHA512_ALT)

void crypto_sha_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen)
{
    if (ilen == 0) {
        return;
    }
    
    crypto_sha_lastword(ctx, 0);

    ctx->total += (uint32_t) ilen;

    const unsigned char *in_pos = input;
    uint32_t rmn = ilen;
    uint32_t sha_ctl_start = (CRPT->SHA_CTL & ~(CRPT_SHA_CTL_DMALAST_Msk | CRPT_SHA_CTL_DMAEN_Msk)) | CRPT_SHA_CTL_START_Msk;
    
    while (rmn > 4) {
        CRPT->SHA_CTL = sha_ctl_start;
        
        uint32_t data = nu_get32_be(in_pos);
        while (! (CRPT->SHA_STS & CRPT_SHA_STS_DATINREQ_Msk));
        CRPT->SHA_DATIN = data;
        
        in_pos += 4;
        rmn -= 4;
    }
    
    MBED_ASSERT(rmn > 0 && rmn <= 4);
    unsigned char lastword_buf[4];
    memcpy(lastword_buf, in_pos, rmn);
    memset(lastword_buf + rmn, 0x00, 4 - rmn);
    ctx->lastword = nu_get32_be(lastword_buf);
    ctx->lastword_size = rmn;
}

void crypto_sha_getinternstate(unsigned char output[], size_t olen)
{
    uint32_t *in_pos = (uint32_t *) &CRPT->SHA_DGST0;
    unsigned char *out_pos = output;
    uint32_t rmn = olen;
    
    while (rmn) {
        uint32_t val = *in_pos ++;
        nu_set32_be(out_pos, val);
        out_pos += 4;
        rmn -= 4;
    }
}

void crypto_sha_lastword(crypto_sha_context *ctx, int islast)
{
    uint32_t sha_ctl_start = (CRPT->SHA_CTL & ~(CRPT_SHA_CTL_DMALAST_Msk | CRPT_SHA_CTL_DMAEN_Msk)) | CRPT_SHA_CTL_START_Msk;
    
    if (ctx->lastword_size) {
        if (islast) {
            uint32_t lastblock_size = ctx->total & ctx->blocksize_mask;
            if (lastblock_size == 0) {
                lastblock_size = ctx->blocksize;
            }
            CRPT->SHA_DMACNT = lastblock_size;
            CRPT->SHA_CTL = sha_ctl_start | CRPT_SHA_CTL_DMALAST_Msk;
        }
        else {
            CRPT->SHA_CTL = sha_ctl_start;
        }
        while (! (CRPT->SHA_STS & CRPT_SHA_STS_DATINREQ_Msk));
        CRPT->SHA_DATIN = ctx->lastword;
        
        if (islast) {
            while (CRPT->SHA_STS & CRPT_SHA_STS_BUSY_Msk);
        }
        
        ctx->lastword_size = 0;
    }
}

#endif /* MBEDTLS_SHA1_ALT || MBEDTLS_SHA256_ALT || MBEDTLS_SHA512_ALT */

#endif /* MBEDTLS_SHA1_C || MBEDTLS_SHA256_C || MBEDTLS_SHA512_C */
