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

#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"

#if defined(MBEDTLS_SHA1_C) || defined(MBEDTLS_SHA256_C) || defined(MBEDTLS_SHA512_C)
#if defined(MBEDTLS_SHA1_ALT) || defined(MBEDTLS_SHA256_ALT) || defined(MBEDTLS_SHA512_ALT)

#include "nu_bitutil.h"
#include "nu_timer.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "crypto-misc.h"

#include <string.h>

/* SHA context for DMA */
static struct {
    MBED_ALIGN(4)
    uint8_t block_imd[NU_CRYPTO_SHA_MAXSIZE_DMABLOCKRUN_BYTES]; // Intermediate data block in DMA mode
    uint32_t fbinfo_imd[NU_CRYPTO_SHA_MAXSIZE_FBINFO_WORDS];    // Intermediate feedback information in DMA cascade mode

    const uint8_t *block_act;   // Actual of above
    uint32_t *fbinfo_act;       // Actual of above
} dma_ctx_inst;

/* Manage SHA H/W
 *
 * We follow the rules below to manage SHA H/W to share among contexts:
 * 1. Go DMA cascade mode to support context save & restore.
 * 2. Initialize/un-initialize crypto H/W at first context init/last context free.
 * 3. All code involving SHA H/W are in between crypto_sha_acquire()/crypto_sha_release().
 * 4. Interrupt is enabled (SHA_ENABLE_INT()/SHA_DISABLE_INT()) only during real SHA H/W operation.
 */

void crypto_sha_init(crypto_sha_context *ctx, uint32_t type)
{
    MBED_ASSERT(type == SHA_MODE_SHA1 ||
                type == SHA_MODE_SHA256 ||
                type == SHA_MODE_SHA512);

    /* Clean SHA context */
    crypto_zeroize(ctx, sizeof(*ctx));

    /* SHA operation mode */
    ctx->type = type;
    ctx->opmode = type;

    /* Mbed TLS error code for the SHA type */
    switch (type) {
    case SHA_MODE_SHA1:
        ctx->digestsize = 20;
        ctx->err_hw_accel = MBEDTLS_ERR_SHA1_HW_ACCEL_FAILED;
        ctx->err_bad_input = MBEDTLS_ERR_SHA1_BAD_INPUT_DATA;
        break;

    case SHA_MODE_SHA256:
        ctx->digestsize = 32;
        ctx->err_hw_accel = MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED;
        ctx->err_bad_input = MBEDTLS_ERR_SHA256_BAD_INPUT_DATA;
        break;

    case SHA_MODE_SHA512:
        ctx->digestsize = 64;
        ctx->err_hw_accel = MBEDTLS_ERR_SHA512_HW_ACCEL_FAILED;
        ctx->err_bad_input = MBEDTLS_ERR_SHA512_BAD_INPUT_DATA;
        break;

    default:
        MBED_ASSERT(0);
    }

    /* Acquire ownership of SHA H/W */
    crypto_sha_acquire();

    /* SHA H/W DMA buffer has the following requirements:
     * (1) Word-aligned buffer base address
     * (2) Word-aligned buffer size
     * (3) Located in 0x20000000-0x2FFFFFFF region */
    if ((! crypto_dma_buff_compat(dma_ctx_inst.block_imd, sizeof(dma_ctx_inst.block_imd), 4)) ||
        (! crypto_dma_buff_compat(dma_ctx_inst.fbinfo_imd, sizeof(dma_ctx_inst.fbinfo_imd), 4))) {
        error("Buffer for SHA H/W DMA requires to be word-aligned and located in 0x20000000-0x2FFFFFFF region.");
    }

    /* Initialize crypto module */
    crypto_init();

    /* Release ownership of SHA H/W */
    crypto_sha_release();
}

void crypto_sha_free(crypto_sha_context *ctx)
{
    if (ctx == NULL) {
        return;
    }

    /* Acquire ownership of SHA H/W */
    crypto_sha_acquire();

    /* Un-initialize crypto module */
    crypto_uninit();

    /* Release ownership of SHA H/W */
    crypto_sha_release();

    /* Clean SHA context */
    crypto_zeroize(ctx, sizeof(*ctx));
}

int crypto_sha_starts(crypto_sha_context *ctx, int is224_384)
{
    MBED_ASSERT(ctx != NULL);

    /* NOTE: mbedtls may call mbedtls_shaXXX_starts multiple times and then call the ending mbedtls_shaXXX_finish. Guard from it. */
    /* NOTE: In DMA cascade mode, re-configure SHA H/W instead of at start */

    ctx->total = 0;
    ctx->buffer_left = 0;

    switch (ctx->type) {
    case SHA_MODE_SHA1:
        ctx->blocksize = 64;
        ctx->blocksize_mask = 0x3F;
        break;

    case SHA_MODE_SHA256:
        ctx->blocksize = 64;
        ctx->blocksize_mask = 0x3F;
        ctx->opmode = is224_384 ? SHA_MODE_SHA224 : SHA_MODE_SHA256;
        ctx->digestsize = is224_384 ? 28 : 32;
        break;

    case SHA_MODE_SHA512:
        ctx->blocksize = 128;
        ctx->blocksize_mask = 0x7F;
        ctx->opmode = is224_384 ? SHA_MODE_SHA384 : SHA_MODE_SHA512;
        ctx->digestsize = is224_384 ? 48 : 64;
        break;

    default:
        MBED_ASSERT(0);
    }

    ctx->is224_384 = is224_384;

    return 0;
}

int crypto_sha_update(crypto_sha_context *ctx, const unsigned char *input, size_t ilen)
{
    MBED_ASSERT(ctx != NULL);

    if (ilen == 0) {
        return 0;
    }

    MBED_ASSERT(input != NULL);

    int rc = 0;

    /* One complete block = above left + new input */
    size_t fill = ctx->blocksize - ctx->buffer_left;
    if (ctx->buffer_left && ilen >= fill) {
        memcpy((void *) (ctx->buffer + ctx->buffer_left), input, fill);
        input += fill;
        ilen -= fill;
        ctx->buffer_left += fill;
        MBED_ASSERT(ctx->buffer_left == ctx->blocksize);
        rc = crypto_sha_update_nobuf(ctx, ctx->buffer, ctx->buffer_left, (ctx->total == 0), 0);
        if (rc != 0) {
            return rc;
        }
        ctx->total += ctx->buffer_left;
        ctx->buffer_left = 0;
    }

    /* Complete blocks in block runs */
    while (ilen >= NU_CRYPTO_SHA_MAXSIZE_DMABLOCKRUN_BYTES) {
        rc = crypto_sha_update_nobuf(ctx, input, NU_CRYPTO_SHA_MAXSIZE_DMABLOCKRUN_BYTES, (ctx->total == 0), 0);
        if (rc != 0) {
            return rc;
        }
        ctx->total += NU_CRYPTO_SHA_MAXSIZE_DMABLOCKRUN_BYTES;
        input += NU_CRYPTO_SHA_MAXSIZE_DMABLOCKRUN_BYTES;
        ilen  -= NU_CRYPTO_SHA_MAXSIZE_DMABLOCKRUN_BYTES;
    }

    /* Complete blocks modulus block run */
    fill = ilen - (ilen % ctx->blocksize);
    MBED_ASSERT((fill % ctx->blocksize) == 0);
    if (fill) {
        rc = crypto_sha_update_nobuf(ctx, input, fill, (ctx->total == 0), 0);
        if (rc != 0) {
            return rc;
        }
        ctx->total += fill;
        input += fill;
        ilen  -= fill;
    }

    /* Left incomplete block */
    if (ilen > 0) {
        MBED_ASSERT(ilen < ctx->blocksize);
        fill = ilen;
        memcpy((void *) (ctx->buffer + ctx->buffer_left), input, fill);
        input += fill;
        ilen -= fill;
        ctx->buffer_left += fill;
    }

    MBED_ASSERT(ilen == 0);

    return rc;
}

int crypto_sha_update_nobuf(crypto_sha_context *ctx, const unsigned char *input, size_t ilen, int isfirst, int islast)
{
    MBED_ASSERT(ctx != NULL);

    /* Accept only:
     * 1. Last block which may be incomplete
     * 2. Non-last block(s) which must be complete */
    MBED_ASSERT(islast || (ilen % ctx->blocksize) == 0);

    int rc = 0;

    /* Acquire ownership of SHA H/W */
    crypto_sha_acquire();

    /* Enable SHA interrupt */
    SHA_ENABLE_INT(CRPT);

    /* Use intermediate buffer when incompatible with SHA H/W DMA buffer */
    if (!crypto_dma_buff_compat(input, ilen, 1)) {
        memcpy(dma_ctx_inst.block_imd, input, ilen);
        dma_ctx_inst.block_act = dma_ctx_inst.block_imd;
    } else {
        dma_ctx_inst.block_act = input;
    }
    if (!crypto_dma_buff_compat(ctx->fbinfo, sizeof(ctx->fbinfo), 4)) {
        memcpy(dma_ctx_inst.fbinfo_imd, ctx->fbinfo, sizeof(ctx->fbinfo));
        dma_ctx_inst.fbinfo_act = dma_ctx_inst.fbinfo_imd;
    } else {
        dma_ctx_inst.fbinfo_act = ctx->fbinfo;
    }

    SHA_Open(CRPT, ctx->opmode, SHA_IN_SWAP, 0);
    SHA_SetDMATransfer(CRPT, (uint32_t) dma_ctx_inst.block_act, ilen);

    /* Address of feedback information input/output */
    CRPT->HMAC_FBADDR = (uint32_t) dma_ctx_inst.fbinfo_act;

    /* Continue above and re-configure further */
    uint32_t hmac_ctl = CRPT->HMAC_CTL |
        CRPT_HMAC_CTL_START_Msk |                       // Start SHA/HMAC engine
        (isfirst ? CRPT_HMAC_CTL_DMAFIRST_Msk : 0) |    // SHA/HMAC first block in cascade function
        (islast ? CRPT_HMAC_CTL_DMALAST_Msk : 0) |      // SHA/HMAC last block
        CRPT_HMAC_CTL_DMACSCAD_Msk |                    // Enable SHA/HMAC engine DMA with cascade mode
        CRPT_HMAC_CTL_DMAEN_Msk |                       // Enable SHA/HMAC engine DMA
        (isfirst ? 0 : CRPT_HMAC_CTL_FBIN_Msk) |        // Feedback input to SHA/HMAC via DMA automatically
                                                        // FBIN doesn't make sense for the first block, on which
                                                        // SHA H/W isn't fool-proof. Enable FBIN for the first block
                                                        // will crash SHA H/W.
        (islast ? 0 : CRPT_HMAC_CTL_FBOUT_Msk) |        // Feedback output from SHA/HMAC via DMA automatically
                                                        // FBOUT is unnecessary for the last block.
        0;

    crypto_sha_prestart();
    CRPT->HMAC_CTL = hmac_ctl;
    rc = crypto_sha_wait() ? 0 : ctx->err_hw_accel;

    /* On using intermediate buffer, restore output feedback information */
    if (rc == 0 &&
        (uint32_t) ctx->fbinfo != (uint32_t) dma_ctx_inst.fbinfo_act) {
        memcpy(ctx->fbinfo, dma_ctx_inst.fbinfo_act, sizeof(ctx->fbinfo));
    }

    /* Disable SHA interrupt */
    SHA_DISABLE_INT(CRPT);

    /* Release ownership of SHA H/W */
    crypto_sha_release();

    return rc;
}

int crypto_sha_finish(crypto_sha_context *ctx, unsigned char output[], size_t olen)
{
    MBED_ASSERT(ctx != NULL);
    MBED_ASSERT(output != NULL);
    MBED_ASSERT(olen != 0 && olen == ctx->digestsize);

    int rc = 0;

    /* NOTE: Per real test, SHA H/W can support zero data, so we needn't
     *       special handling for it. */
    rc = crypto_sha_update_nobuf(ctx, ctx->buffer, ctx->buffer_left, (ctx->total == 0), 1);
    if (rc != 0) {
        return rc;
    }
    ctx->total += ctx->buffer_left;
    ctx->buffer_left = 0;
    rc = crypto_sha_getdigest(output, ctx->digestsize);
    if (rc != 0) {
        return rc;
    }

    return rc;
}

int crypto_sha_getdigest(unsigned char output[], size_t olen)
{
    MBED_ASSERT(output != NULL);
    MBED_ASSERT(olen != 0 && (olen % 4) == 0);

    uint32_t *in_pos = (uint32_t *) &CRPT->HMAC_DGST[0];
    unsigned char *out_pos = output;
    uint32_t rmn = olen;

    while (rmn) {
        uint32_t val = *in_pos ++;
        nu_set32_be(out_pos, val);
        out_pos += 4;
        rmn -= 4;
    }

    return 0;
}

#endif /* MBEDTLS_SHA1_ALT || MBEDTLS_SHA256_ALT || MBEDTLS_SHA512_ALT */
#endif /* MBEDTLS_SHA1_C || MBEDTLS_SHA256_C || MBEDTLS_SHA512_C */
