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

void crypto_sha_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen);
void crypto_sha_update_nobuf(crypto_sha_context *ctx, const unsigned char *input, size_t ilen, int islast);
void crypto_sha_getinternstate(unsigned char output[], size_t olen);

#endif /* MBEDTLS_SHA1_ALT || MBEDTLS_SHA256_ALT || MBEDTLS_SHA512_ALT */

#if defined(MBEDTLS_SHA1_C)
#if defined(MBEDTLS_SHA1_ALT)

void mbedtls_sha1_hw_init(crypto_sha_context *ctx)
{
    crypto_init();
    memset(ctx, 0, sizeof(crypto_sha_context));
}

void mbedtls_sha1_hw_free(crypto_sha_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    crypto_zeroize(ctx, sizeof(crypto_sha_context));
}

void mbedtls_sha1_hw_clone(crypto_sha_context *dst,
                        const crypto_sha_context *src)
{
    *dst = *src;
}

void mbedtls_sha1_hw_starts(crypto_sha_context *ctx)
{
    // NOTE: mbedtls may call mbedtls_shaXXX_starts multiple times and then call the ending mbedtls_shaXXX_finish. Guard from it.
    CRPT->SHA_CTL |= CRPT_SHA_CTL_STOP_Msk;
    
    ctx->total = 0;
    ctx->buffer_left = 0;
    ctx->blocksize = 64;
    ctx->blocksize_mask = 0x3F;

    SHA_Open(SHA_MODE_SHA1, SHA_NO_SWAP);
    
    // Ensure we have correct initial inernal states in SHA_DGST registers even though SHA H/W is not actually started.
    CRPT->SHA_CTL |= CRPT_SHA_CTL_START_Msk;
    
    return;
}

void mbedtls_sha1_hw_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen)
{
    crypto_sha_update(ctx, input, ilen);
}

void mbedtls_sha1_hw_finish(crypto_sha_context *ctx, unsigned char output[20])
{
    // H/W SHA cannot handle zero data well. Fall back to S/W SHA.
    if (ctx->total) {
        crypto_sha_update_nobuf(ctx, ctx->buffer, ctx->buffer_left, 1);
        ctx->buffer_left = 0;
        crypto_sha_getinternstate(output, 20);
    
        CRPT->SHA_CTL |= CRPT_SHA_CTL_STOP_Msk;
    }
    else {
        mbedtls_sha1_sw_context ctx_sw;
    
        mbedtls_sha1_sw_init(&ctx_sw);
        mbedtls_sha1_sw_starts(&ctx_sw);
        mbedtls_sha1_sw_finish(&ctx_sw, output);
        mbedtls_sha1_sw_free(&ctx_sw);
    }
}

void mbedtls_sha1_hw_process(crypto_sha_context *ctx, const unsigned char data[64])
{
    mbedtls_sha1_hw_update(ctx, data, 64);
}

#endif /* MBEDTLS_SHA1_ALT */
#endif /* MBEDTLS_SHA1_C */

#if defined(MBEDTLS_SHA256_C)
#if defined(MBEDTLS_SHA256_ALT)

void mbedtls_sha256_hw_init(crypto_sha_context *ctx)
{
    crypto_init();
    memset(ctx, 0, sizeof(crypto_sha_context));
}

void mbedtls_sha256_hw_free(crypto_sha_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    crypto_zeroize(ctx, sizeof(crypto_sha_context));
}

void mbedtls_sha256_hw_clone(crypto_sha_context *dst,
                        const crypto_sha_context *src)
{
    *dst = *src;
}

void mbedtls_sha256_hw_starts( crypto_sha_context *ctx, int is224)
{
    // NOTE: mbedtls may call mbedtls_shaXXX_starts multiple times and then call the ending mbedtls_shaXXX_finish. Guard from it.
    CRPT->SHA_CTL |= CRPT_SHA_CTL_STOP_Msk;
     
    ctx->total = 0;
    ctx->buffer_left = 0;
    ctx->blocksize = 64;
    ctx->blocksize_mask = 0x3F;
    ctx->is224 = is224;

    SHA_Open(is224 ? SHA_MODE_SHA224 : SHA_MODE_SHA256, SHA_NO_SWAP);
    
    // Ensure we have correct initial inernal states in SHA_DGST registers even though SHA H/W is not actually started.
    CRPT->SHA_CTL |= CRPT_SHA_CTL_START_Msk;
    
    return;
}

void mbedtls_sha256_hw_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen)
{
    crypto_sha_update(ctx, input, ilen);
}

void mbedtls_sha256_hw_finish(crypto_sha_context *ctx, unsigned char output[32])
{
    // H/W SHA cannot handle zero data well. Fall back to S/W SHA.
    if (ctx->total) {
        crypto_sha_update_nobuf(ctx, ctx->buffer, ctx->buffer_left, 1);
        ctx->buffer_left = 0;
        crypto_sha_getinternstate(output, ctx->is224 ? 28 : 32);
    
        CRPT->SHA_CTL |= CRPT_SHA_CTL_STOP_Msk;
    }
    else {
        mbedtls_sha256_sw_context ctx_sw;
    
        mbedtls_sha256_sw_init(&ctx_sw);
        mbedtls_sha256_sw_starts(&ctx_sw, ctx->is224);
        mbedtls_sha256_sw_finish(&ctx_sw, output);
        mbedtls_sha256_sw_free(&ctx_sw);
    }
}

void mbedtls_sha256_hw_process(crypto_sha_context *ctx, const unsigned char data[64])
{
    mbedtls_sha256_hw_update(ctx, data, 64);
}

#endif /* MBEDTLS_SHA256_ALT */
#endif /* MBEDTLS_SHA256_C */

#if defined(MBEDTLS_SHA1_C) || defined(MBEDTLS_SHA256_C) || defined(MBEDTLS_SHA512_C)
#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_SHA512_ALT)

void crypto_sha_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen)
{
    if (ilen == 0) {
        return;
    }
    
    size_t fill = ctx->blocksize - ctx->buffer_left;

    ctx->total += (uint32_t) ilen;

    if (ctx->buffer_left && ilen >= fill) {
        memcpy((void *) (ctx->buffer + ctx->buffer_left), input, fill);
        input += fill;
        ilen  -= fill;
        ctx->buffer_left += fill;
        if (ilen) {
            crypto_sha_update_nobuf(ctx, ctx->buffer, ctx->buffer_left, 0);
            ctx->buffer_left = 0;
        }
    }
    
    while (ilen > ctx->blocksize) {
        crypto_sha_update_nobuf(ctx, input, ctx->blocksize, 0);
        input += ctx->blocksize;
        ilen  -= ctx->blocksize;
    }

    if (ilen > 0) {
        memcpy((void *) (ctx->buffer + ctx->buffer_left), input, ilen);
        ctx->buffer_left += ilen;
    }
}

void crypto_sha_update_nobuf(crypto_sha_context *ctx, const unsigned char *input, size_t ilen, int islast)
{
    // Accept only:
    // 1. Last block which may be incomplete
    // 2. Non-last block which is complete
    MBED_ASSERT(islast || ilen == ctx->blocksize);
    
    const unsigned char *in_pos = input;
    int rmn = ilen;
    uint32_t sha_ctl_start = (CRPT->SHA_CTL & ~(CRPT_SHA_CTL_DMALAST_Msk | CRPT_SHA_CTL_DMAEN_Msk)) | CRPT_SHA_CTL_START_Msk;
    uint32_t sha_opmode = (CRPT->SHA_CTL & CRPT_SHA_CTL_OPMODE_Msk) >> CRPT_SHA_CTL_OPMODE_Pos;
    uint32_t DGST0_old, DGST1_old, DGST2_old, DGST3_old, DGST4_old, DGST5_old, DGST6_old, DGST7_old;
    
    while (rmn > 0) {
        CRPT->SHA_CTL = sha_ctl_start;
        
        uint32_t data = nu_get32_be(in_pos);
        if (rmn <= 4) { // Last word of a (in)complete block
            if (islast) {
                uint32_t lastblock_size = ctx->total & ctx->blocksize_mask;
                if (lastblock_size == 0) {
                    lastblock_size = ctx->blocksize;
                }
                CRPT->SHA_DMACNT = lastblock_size;
                CRPT->SHA_CTL = sha_ctl_start | CRPT_SHA_CTL_DMALAST_Msk;
            }
            else {
                switch (sha_opmode) {
                    case SHA_MODE_SHA256:
                        DGST7_old = CRPT->SHA_DGST7;
                    case SHA_MODE_SHA224:
                        DGST5_old = CRPT->SHA_DGST5;
                        DGST6_old = CRPT->SHA_DGST6;
                    case SHA_MODE_SHA1:
                        DGST0_old = CRPT->SHA_DGST0;
                        DGST1_old = CRPT->SHA_DGST1;
                        DGST2_old = CRPT->SHA_DGST2;
                        DGST3_old = CRPT->SHA_DGST3;
                        DGST4_old = CRPT->SHA_DGST4;
                }

                CRPT->SHA_CTL = sha_ctl_start;
            }
        }
        else {  // Non-last word of a complete block
            CRPT->SHA_CTL = sha_ctl_start;
        }
        while (! (CRPT->SHA_STS & CRPT_SHA_STS_DATINREQ_Msk));
        CRPT->SHA_DATIN = data;
        
        in_pos += 4;
        rmn -= 4;
    }
    
    if (islast) {   // Finish of last block
        while (CRPT->SHA_STS & CRPT_SHA_STS_BUSY_Msk);
    }
    else {  // Finish of non-last block
        // No H/W flag to indicate finish of non-last block process.
        // Values of SHA_DGSTx registers will change as last word of the block is input, so use it for judgement.
        int isfinish = 0;
        while (! isfinish) {
            switch (sha_opmode) {
                case SHA_MODE_SHA256:
                    if (DGST7_old != CRPT->SHA_DGST7) {
                        isfinish = 1;
                        break;
                    }
                case SHA_MODE_SHA224:
                    if (DGST5_old != CRPT->SHA_DGST5 || DGST6_old != CRPT->SHA_DGST6) {
                        isfinish = 1;
                        break;
                    }
                case SHA_MODE_SHA1:
                    if (DGST0_old != CRPT->SHA_DGST0 || DGST1_old != CRPT->SHA_DGST1 || DGST2_old != CRPT->SHA_DGST2 ||
                        DGST3_old != CRPT->SHA_DGST3 || DGST4_old != CRPT->SHA_DGST4) {
                        isfinish = 1;
                        break;
                    }
            }
        }
    }
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

#endif /* MBEDTLS_SHA1_ALT || MBEDTLS_SHA256_ALT || MBEDTLS_SHA512_ALT */
#endif /* MBEDTLS_SHA1_C || MBEDTLS_SHA256_C || MBEDTLS_SHA512_C */

#endif /* MBEDTLS_SHA1_C || MBEDTLS_SHA256_C || MBEDTLS_SHA512_C */
