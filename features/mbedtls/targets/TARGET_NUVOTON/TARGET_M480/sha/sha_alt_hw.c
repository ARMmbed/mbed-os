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
    CRPT->HMAC_CTL |= CRPT_HMAC_CTL_STOP_Msk;
    
    ctx->total = 0;
    ctx->buffer_left = 0;
    ctx->blocksize = 64;
    ctx->blocksize_mask = 0x3F;

    SHA_Open(SHA_MODE_SHA1, SHA_NO_SWAP, 0);
    
    // Ensure we have correct initial internal states in SHA_DGST registers even though SHA H/W is not actually started.
    CRPT->HMAC_CTL |= CRPT_HMAC_CTL_START_Msk;
    
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
    
        CRPT->HMAC_CTL |= CRPT_HMAC_CTL_STOP_Msk;
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
    CRPT->HMAC_CTL |= CRPT_HMAC_CTL_STOP_Msk;
     
    ctx->total = 0;
    ctx->buffer_left = 0;
    ctx->blocksize = 64;
    ctx->blocksize_mask = 0x3F;
    ctx->is224_384 = is224;

    SHA_Open(is224 ? SHA_MODE_SHA224 : SHA_MODE_SHA256, SHA_NO_SWAP, 0);
    
    // Ensure we have correct initial inernal states in SHA_DGST registers even though SHA H/W is not actually started.
    CRPT->HMAC_CTL |= CRPT_HMAC_CTL_START_Msk;
    
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
        crypto_sha_getinternstate(output, ctx->is224_384 ? 28 : 32);
    
        CRPT->HMAC_CTL |= CRPT_HMAC_CTL_STOP_Msk;
    }
    else {
        mbedtls_sha256_sw_context ctx_sw;
    
        mbedtls_sha256_sw_init(&ctx_sw);
        mbedtls_sha256_sw_starts(&ctx_sw, ctx->is224_384);
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


#if defined(MBEDTLS_SHA512_C)
#if defined(MBEDTLS_SHA512_ALT)

void mbedtls_sha512_hw_init(crypto_sha_context *ctx)
{
    crypto_init();
    memset(ctx, 0, sizeof(crypto_sha_context));
}

void mbedtls_sha512_hw_free(crypto_sha_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    crypto_zeroize(ctx, sizeof(crypto_sha_context));
}

void mbedtls_sha512_hw_clone(crypto_sha_context *dst,
                        const crypto_sha_context *src)
{
    *dst = *src;
}

void mbedtls_sha512_hw_starts( crypto_sha_context *ctx, int is384)
{
    // NOTE: mbedtls may call mbedtls_shaXXX_starts multiple times and then call the ending mbedtls_shaXXX_finish. Guard from it.
    CRPT->HMAC_CTL |= CRPT_HMAC_CTL_STOP_Msk;
     
    ctx->total = 0;
    ctx->buffer_left = 0;
    ctx->blocksize = 128;
    ctx->blocksize_mask = 0x7F;
    ctx->is224_384 = is384;

    SHA_Open(is384 ? SHA_MODE_SHA384 : SHA_MODE_SHA512, SHA_NO_SWAP, 0);
    
    // Ensure we have correct initial inernal states in SHA_DGST registers even though SHA H/W is not actually started.
    CRPT->HMAC_CTL |= CRPT_HMAC_CTL_START_Msk;
    
    return;
}

void mbedtls_sha512_hw_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen)
{
    crypto_sha_update(ctx, input, ilen);
}

void mbedtls_sha512_hw_finish(crypto_sha_context *ctx, unsigned char output[64])
{
    // H/W SHA cannot handle zero data well. Fall back to S/W SHA.
    if (ctx->total) {
        crypto_sha_update_nobuf(ctx, ctx->buffer, ctx->buffer_left, 1);
        ctx->buffer_left = 0;
        crypto_sha_getinternstate(output, ctx->is224_384 ? 48 : 64);
    
        CRPT->HMAC_CTL |= CRPT_HMAC_CTL_STOP_Msk;
    }
    else {
        mbedtls_sha512_sw_context ctx_sw;
    
        mbedtls_sha512_sw_init(&ctx_sw);
        mbedtls_sha512_sw_starts(&ctx_sw, ctx->is224_384);
        mbedtls_sha512_sw_finish(&ctx_sw, output);
        mbedtls_sha512_sw_free(&ctx_sw);
    }
}

void mbedtls_sha512_hw_process(crypto_sha_context *ctx, const unsigned char data[128])
{
    mbedtls_sha512_hw_update(ctx, data, 128);
}

#endif /* MBEDTLS_SHA512_ALT */
#endif /* MBEDTLS_SHA512_C */

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
    uint32_t sha_ctl_start = (CRPT->HMAC_CTL & ~(CRPT_HMAC_CTL_DMALAST_Msk | CRPT_HMAC_CTL_DMAEN_Msk | CRPT_HMAC_CTL_HMACEN_Msk)) | CRPT_HMAC_CTL_START_Msk;
    uint32_t sha_opmode = (CRPT->HMAC_CTL & CRPT_HMAC_CTL_OPMODE_Msk) >> CRPT_HMAC_CTL_OPMODE_Pos;
    uint32_t DGST0_old = 0, DGST1_old = 0, DGST2_old = 0, DGST3_old = 0, DGST4_old = 0, DGST5_old = 0, DGST6_old = 0, DGST7_old = 0,
        DGST8_old = 0, DGST9_old = 0, DGST10_old = 0, DGST11_old = 0, DGST12_old = 0, DGST13_old = 0, DGST14_old = 0, DGST15_old = 0;
    
    while (rmn > 0) {
        CRPT->HMAC_CTL = sha_ctl_start;
        
        uint32_t data = nu_get32_be(in_pos);
        if (rmn <= 4) { // Last word of a (in)complete block
            if (islast) {
                uint32_t lastblock_size = ctx->total & ctx->blocksize_mask;
                if (lastblock_size == 0) {
                    lastblock_size = ctx->blocksize;
                }
                CRPT->HMAC_DMACNT = lastblock_size;
                CRPT->HMAC_CTL = sha_ctl_start | CRPT_HMAC_CTL_DMALAST_Msk;
            }
            else {
                switch (sha_opmode) {
                    case SHA_MODE_SHA512:
                        DGST15_old = CRPT->HMAC_DGST[15];
                        DGST14_old = CRPT->HMAC_DGST[14];
                        DGST13_old = CRPT->HMAC_DGST[13];
                        DGST12_old = CRPT->HMAC_DGST[12];
                    case SHA_MODE_SHA384:
                        DGST11_old = CRPT->HMAC_DGST[11];
                        DGST10_old = CRPT->HMAC_DGST[10];
                        DGST9_old = CRPT->HMAC_DGST[9];
                        DGST8_old = CRPT->HMAC_DGST[8];
                    case SHA_MODE_SHA256:
                        DGST7_old = CRPT->HMAC_DGST[7];
                    case SHA_MODE_SHA224:
                        DGST5_old = CRPT->HMAC_DGST[5];
                        DGST6_old = CRPT->HMAC_DGST[6];
                    case SHA_MODE_SHA1:
                        DGST0_old = CRPT->HMAC_DGST[0];
                        DGST1_old = CRPT->HMAC_DGST[1];
                        DGST2_old = CRPT->HMAC_DGST[2];
                        DGST3_old = CRPT->HMAC_DGST[3];
                        DGST4_old = CRPT->HMAC_DGST[4];
                }

                CRPT->HMAC_CTL = sha_ctl_start;
            }
        }
        else {  // Non-last word of a complete block
            CRPT->HMAC_CTL = sha_ctl_start;
        }
        while (! (CRPT->HMAC_STS & CRPT_HMAC_STS_DATINREQ_Msk));
        CRPT->HMAC_DATIN = data;
        
        in_pos += 4;
        rmn -= 4;
    }
    
    if (islast) {   // Finish of last block
        while (CRPT->HMAC_STS & CRPT_HMAC_STS_BUSY_Msk);
    }
    else {  // Finish of non-last block
        // No H/W flag to indicate finish of non-last block process.
        // Values of SHA_DGSTx registers will change as last word of the block is input, so use it for judgement.
        int isfinish = 0;
        while (! isfinish) {
            switch (sha_opmode) {
                case SHA_MODE_SHA512:
                    if (DGST12_old != CRPT->HMAC_DGST[12] || DGST13_old != CRPT->HMAC_DGST[13] || DGST14_old != CRPT->HMAC_DGST[14] ||
                        DGST15_old != CRPT->HMAC_DGST[15]) {
                        isfinish = 1;
                        break;
                    }
                case SHA_MODE_SHA384:
                    if (DGST8_old != CRPT->HMAC_DGST[8] || DGST9_old != CRPT->HMAC_DGST[9] || DGST10_old != CRPT->HMAC_DGST[10] ||
                        DGST11_old != CRPT->HMAC_DGST[11]) {
                        isfinish = 1;
                        break;
                    }
                case SHA_MODE_SHA256:
                    if (DGST7_old != CRPT->HMAC_DGST[7]) {
                        isfinish = 1;
                        break;
                    }
                case SHA_MODE_SHA224:
                    if (DGST5_old != CRPT->HMAC_DGST[5] || DGST6_old != CRPT->HMAC_DGST[6]) {
                        isfinish = 1;
                        break;
                    }
                case SHA_MODE_SHA1:
                    if (DGST0_old != CRPT->HMAC_DGST[0] || DGST1_old != CRPT->HMAC_DGST[1] || DGST2_old != CRPT->HMAC_DGST[2] ||
                        DGST3_old != CRPT->HMAC_DGST[3] || DGST4_old != CRPT->HMAC_DGST[4]) {
                        isfinish = 1;
                        break;
                    }
            }
        }
    }
}

void crypto_sha_getinternstate(unsigned char output[], size_t olen)
{
    uint32_t *in_pos = (uint32_t *) &CRPT->HMAC_DGST[0];
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
