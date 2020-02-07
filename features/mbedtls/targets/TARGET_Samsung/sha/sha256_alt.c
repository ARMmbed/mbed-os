/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/


#include "mbedtls/sha256.h"
#include "mbedtls/platform_util.h"

#if defined(MBEDTLS_SHA256_C)
#if defined(MBEDTLS_SHA256_ALT)
#include "mb_cmd_hash.h"
#define SHA256_VALIDATE_RET(cond)                           \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_SHA256_BAD_INPUT_DATA )
#define SHA256_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i)                            \
do {                                                    \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
        | ( (uint32_t) (b)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 3]       );            \
} while( 0 )
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
do {                                                    \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
} while( 0 )
#endif


#include "string.h"
#include "sss_common.h"
#include "mb_cmd_system.h"

int mbedtls_sha256_sw_finish_ret(mbedtls_sha256_context *ctx, unsigned char output[32]);
int mbedtls_sha256_sw_update_ret(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen);

void mbedtls_sha256_init(mbedtls_sha256_context *ctx)
{
    memset(ctx, 0, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_free(mbedtls_sha256_context *ctx)
{
    if (ctx == NULL) {
        return;
    }
    memset(ctx, 0, sizeof(mbedtls_sha256_context));
}

void mbedtls_sha256_clone(mbedtls_sha256_context *dst,
                          const mbedtls_sha256_context *src)
{
    // Corner case: Destination/source contexts are the same
    if (dst == src) {
        return;
    }

    SHA256_VALIDATE(dst != NULL);
    SHA256_VALIDATE(src != NULL);

    memcpy(dst, src, sizeof(mbedtls_sha256_context));
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts_ret(mbedtls_sha256_context *ctx, int is224)
{
    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET(is224 == 0 || is224 == 1);

    ctx->total[0] = 0;
    ctx->total[1] = 0;

    if (is224 == 0) {
        ctx->is224 = 0;
        memset(ctx, 0, sizeof(mbedtls_sha256_context));
    } else {
        /* SHA-224 */
        ctx->state[0] = 0xC1059ED8;
        ctx->state[1] = 0x367CD507;
        ctx->state[2] = 0x3070DD17;
        ctx->state[3] = 0xF70E5939;
        ctx->state[4] = 0xFFC00B31;
        ctx->state[5] = 0x68581511;
        ctx->state[6] = 0x64F98FA7;
        ctx->state[7] = 0xBEFA4FA4;
        ctx->is224 = is224;
    }

    // No any of H/W access, always return OK
    return 0;
}

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update_ret(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    if (ctx->is224) {
        mbedtls_sha256_sw_update_ret(ctx, input, ilen);
    } else {
        if (ilen > MAX_MB_HASH_BLOCK_BLEN || (ctx->totals + ilen) > MAX_MB_HASH_BLOCK_BLEN || ctx->totals > MAX_MB_HASH_BLOCK_BLEN) {
            // H/W SHA has limitation to seperated API with oversized message.
            // fall back to S/W SHA-256
            if (ctx->totals == 0 || ctx->hw == 1) {
                ctx->total[0] = 0;
                ctx->total[1] = 0;
                /* SHA-256 */
                ctx->state[0] = 0x6A09E667;
                ctx->state[1] = 0xBB67AE85;
                ctx->state[2] = 0x3C6EF372;
                ctx->state[3] = 0xA54FF53A;
                ctx->state[4] = 0x510E527F;
                ctx->state[5] = 0x9B05688C;
                ctx->state[6] = 0x1F83D9AB;
                ctx->state[7] = 0x5BE0CD19;
            }
            ctx->totals += ilen;
            //in case, H/W -> S/W fallback case
            if ((ctx->totals + ilen) > MAX_MB_HASH_BLOCK_BLEN && ctx->hw == 1) {
                mbedtls_sha512_sw_update_ret(ctx, ctx->sbuf, ctx->pstMessage.u32DataByteLen);
            }
            ctx->hw = 0;
            mbedtls_sha256_sw_update_ret(ctx, input, ilen);
        } else { //less than MAX_MB_HASH_BLOCK_BLEN size will handle with H/W
            // SHA-256 handle by SSS H/W
            memcpy(ctx->sbuf + ctx->pstMessage.u32DataByteLen, input, ilen);
            ctx->pstMessage.u32DataByteLen += ilen;
            ctx->totals += ilen; //in case the block size increased incrementally. (3, 20, 256..)
        }
    }

    // No any of H/W access, always return OK
    return 0;
}

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish_ret(mbedtls_sha256_context *ctx, unsigned char output[32])
{
    if (ctx->is224 || ctx->totals > MAX_MB_HASH_BLOCK_BLEN) {
        mbedtls_sha256_sw_finish_ret(ctx, output);
    } else {
        int ret = FAIL;
        unsigned int object_id;
        unsigned int block_byte_len;

        ctx->pstDigest.pu08Data = output; /* assign output buffer */

        stOCTET_STRING stHASH_Input;
        //! step 0 : clear Mailbox
        ret = mb_system_clear(CLEAR_TYPE_MAILBOX);

        if (ret != SSSR_SUCCESS) {
            return ret;
        }

        //! assign hash_byte_len to compare returned result from sss_fw after hash operation
        object_id = OID_SHA2_256;
        block_byte_len = 64;

        //! step 1 : set message length parameter to SSS
        ret = mb_hash_init(&ctx->pstMessage, object_id);

        if (ret != SSSR_SUCCESS) {
            return ret;
        }

        //! step 2 : set message block to SSS
        ctx->pstMessage.pu08Data = ctx->sbuf;
        stHASH_Input.pu08Data = ctx->pstMessage.pu08Data;
        stHASH_Input.u32DataByteLen = ctx->pstMessage.u32DataByteLen;

        ret = mb_hash_update(&stHASH_Input, block_byte_len);

        if (ret != SSSR_SUCCESS) {
            return ret;
        }

        //! step 3 : get hash result from SSS
        ret = mb_hash_final(&stHASH_Input, &ctx->pstDigest);
        if (ret != SSSR_SUCCESS) {
            return ret;
        }

    }
    return 0;
}

static const uint32_t K[] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
    0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
    0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
    0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
    0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
    0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
    0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
    0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
    0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
};

#define  SHR(x,n) (((x) & 0xFFFFFFFF) >> (n))
#define ROTR(x,n) (SHR(x,n) | ((x) << (32 - (n))))

#define S0(x) (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3))
#define S1(x) (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))

#define F0(x,y,z) (((x) & (y)) | ((z) & ((x) | (y))))
#define F1(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))

#define R(t)                                    \
    (                                           \
        W[t] = S1(W[(t) -  2]) + W[(t) -  7] +  \
               S0(W[(t) - 15]) + W[(t) - 16]    \
    )

#define P(a,b,c,d,e,f,g,h,x,K)                          \
    do                                                  \
    {                                                   \
        temp1 = (h) + S3(e) + F1((e),(f),(g)) + (K) + (x);      \
        temp2 = S2(a) + F0((a),(b),(c));                        \
        (d) += temp1; (h) = temp1 + temp2;              \
    } while( 0 )

int mbedtls_internal_sha256_process(mbedtls_sha256_context *ctx,
                                    const unsigned char data[64])
{
    uint32_t temp1, temp2, W[64];
    uint32_t A[8];
    unsigned int i;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET((const unsigned char *)data != NULL);

    for (i = 0; i < 8; i++) {
        A[i] = ctx->state[i];
    }

#if defined(MBEDTLS_SHA256_SMALLER)
    for (i = 0; i < 64; i++) {
        if (i < 16) {
            GET_UINT32_BE(W[i], data, 4 * i);
        } else {
            R(i);
        }

        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i], K[i]);

        temp1 = A[7];
        A[7] = A[6];
        A[6] = A[5];
        A[5] = A[4];
        A[4] = A[3];
        A[3] = A[2];
        A[2] = A[1];
        A[1] = A[0];
        A[0] = temp1;
    }
#else /* MBEDTLS_SHA256_SMALLER */
    for (i = 0; i < 16; i++) {
        GET_UINT32_BE(W[i], data, 4 * i);
    }

    for (i = 0; i < 16; i += 8) {
        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], W[i + 0], K[i + 0]);
        P(A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], W[i + 1], K[i + 1]);
        P(A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], W[i + 2], K[i + 2]);
        P(A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], W[i + 3], K[i + 3]);
        P(A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], W[i + 4], K[i + 4]);
        P(A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], W[i + 5], K[i + 5]);
        P(A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], W[i + 6], K[i + 6]);
        P(A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], W[i + 7], K[i + 7]);
    }

    for (i = 16; i < 64; i += 8) {
        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], R(i + 0), K[i + 0]);
        P(A[7], A[0], A[1], A[2], A[3], A[4], A[5], A[6], R(i + 1), K[i + 1]);
        P(A[6], A[7], A[0], A[1], A[2], A[3], A[4], A[5], R(i + 2), K[i + 2]);
        P(A[5], A[6], A[7], A[0], A[1], A[2], A[3], A[4], R(i + 3), K[i + 3]);
        P(A[4], A[5], A[6], A[7], A[0], A[1], A[2], A[3], R(i + 4), K[i + 4]);
        P(A[3], A[4], A[5], A[6], A[7], A[0], A[1], A[2], R(i + 5), K[i + 5]);
        P(A[2], A[3], A[4], A[5], A[6], A[7], A[0], A[1], R(i + 6), K[i + 6]);
        P(A[1], A[2], A[3], A[4], A[5], A[6], A[7], A[0], R(i + 7), K[i + 7]);
    }
#endif /* MBEDTLS_SHA256_SMALLER */

    for (i = 0; i < 8; i++) {
        ctx->state[i] += A[i];
    }

    return 0;
}


int mbedtls_sha256_sw_finish_ret(mbedtls_sha256_context *ctx, unsigned char output[32])
{
    int ret;
    uint32_t used;
    uint32_t high, low;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET((unsigned char *)output != NULL);

    /*
     * Add padding: 0x80 then 0x00 until 8 bytes remain for the length
     */
    used = ctx->total[0] & 0x3F;

    ctx->buffer[used++] = 0x80;

    if (used <= 56) {
        /* Enough room for padding + length in current block */
        memset(ctx->buffer + used, 0, 56 - used);
    } else {
        /* We'll need an extra block */
        memset(ctx->buffer + used, 0, 64 - used);

        if ((ret = mbedtls_internal_sha256_process(ctx, ctx->buffer)) != 0) {
            return (ret);
        }

        memset(ctx->buffer, 0, 56);
    }

    /*
     * Add message length
     */
    high = (ctx->total[0] >> 29)
           | (ctx->total[1] <<  3);
    low  = (ctx->total[0] <<  3);

    PUT_UINT32_BE(high, ctx->buffer, 56);
    PUT_UINT32_BE(low,  ctx->buffer, 60);

    if ((ret = mbedtls_internal_sha256_process(ctx, ctx->buffer)) != 0) {
        return (ret);
    }

    /*
     * Output final state
     */
    PUT_UINT32_BE(ctx->state[0], output,  0);
    PUT_UINT32_BE(ctx->state[1], output,  4);
    PUT_UINT32_BE(ctx->state[2], output,  8);
    PUT_UINT32_BE(ctx->state[3], output, 12);
    PUT_UINT32_BE(ctx->state[4], output, 16);
    PUT_UINT32_BE(ctx->state[5], output, 20);
    PUT_UINT32_BE(ctx->state[6], output, 24);

    if (ctx->is224 == 0) {
        PUT_UINT32_BE(ctx->state[7], output, 28);
    }
    return 0;
}

int mbedtls_sha256_sw_update_ret(mbedtls_sha256_context *ctx, const unsigned char *input, size_t ilen)
{
    //process SW SHA224
    int ret;
    size_t fill;
    uint32_t left;

    SHA256_VALIDATE_RET(ctx != NULL);
    SHA256_VALIDATE_RET(ilen == 0 || input != NULL);

    if (ilen == 0) {
        return (0);
    }

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if (ctx->total[0] < (uint32_t) ilen) {
        ctx->total[1]++;
    }

    if (left && ilen >= fill) {
        memcpy((void *)(ctx->buffer + left), input, fill);

        if ((ret = mbedtls_internal_sha256_process(ctx, ctx->buffer)) != 0) {
            return (ret);
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= 64) {
        if ((ret = mbedtls_internal_sha256_process(ctx, input)) != 0) {
            return (ret);
        }

        input += 64;
        ilen  -= 64;
    }

    if (ilen > 0) {
        memcpy((void *)(ctx->buffer + left), input, ilen);
    }

    return 0;
}

#endif /* MBEDTLS_SHA256_ALT */
#endif /* MBEDTLS_SHA256_C */
