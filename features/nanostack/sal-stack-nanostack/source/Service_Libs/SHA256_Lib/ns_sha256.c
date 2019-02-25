/*
 * Copyright (c) 2006-2018, Arm Limited and affiliates.
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
/*
 *  FIPS-180-2 compliant SHA-256 implementation
 *
 *  This file is derived from sha256.h in mbed TLS 2.3.0.
 */
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

#include "nsconfig.h"
#include "ns_sha256.h"

#ifndef NS_USE_EXTERNAL_MBED_TLS

#include <string.h>

/* Implementation that should never be optimized out by the compiler */
static void ns_zeroize(void *v, size_t n)
{
    volatile unsigned char *p = v;
    while (n--) {
        *p++ = 0;
    }
}

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i)                            \
(                                                       \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
        | ( (uint32_t) (b)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 3]       )             \
)
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

void ns_sha256_init(ns_sha256_context *ctx)
{
    memset(ctx, 0, sizeof(ns_sha256_context));
}

void ns_sha256_free(ns_sha256_context *ctx)
{
    ns_zeroize(ctx, sizeof(ns_sha256_context));
}

void ns_sha256_clone(ns_sha256_context *dst,
                     const ns_sha256_context *src)
{
    *dst = *src;
}

/*
 * SHA-256 context setup
 */
void ns_sha256_starts(ns_sha256_context *ctx)
{
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

#define  SHR(x,n) ((x & 0xFFFFFFFF) >> n)
#define ROTR(x,n) (SHR(x,n) | (x << (32 - n)))

#define S0(x) (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3))
#define S1(x) (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10))

#define S2(x) (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22))
#define S3(x) (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25))

#define F0(x,y,z) ((x & y) | (z & (x | y)))
#define F1(x,y,z) (z ^ (x & (y ^ z)))

#define R(t)                                              \
(                                                         \
    W[t & 15] += S1(W[(t -  2) & 15]) + W[(t - 7) & 15] + \
                 S0(W[(t - 15) & 15])                     \
)

#define P(a,b,c,d,e,f,g,h,x,K)                  \
{                                               \
    temp1 = h + S3(e) + F1(e,f,g) + K + x;      \
    temp2 = S2(a) + F0(a,b,c);                  \
    d += temp1; h = temp1 + temp2;              \
}

static void ns_sha256_process(ns_sha256_context *ctx, const unsigned char data[64])
{
    uint32_t temp1, temp2, W[16];
    uint32_t A[8];
    unsigned int i;

    for (i = 0; i < 8; i++) {
        A[i] = ctx->state[i];
    }

    for (i = 0; i < 64; i++) {
        uint32_t Wi;
        if (i < 16) {
            Wi = GET_UINT32_BE(W[i], data, 4 * i);
        } else {
            Wi = R(i);
        }

        P(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7], Wi, K[i]);

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

    for (i = 0; i < 8; i++) {
        ctx->state[i] += A[i];
    }
}

/*
 * SHA-256 process buffer
 */
void ns_sha256_update(ns_sha256_context *ctx, const void *inputv,
                      size_t ilen)
{
    const unsigned char *input = inputv;
    size_t fill;
    uint32_t left;

    if (ilen == 0) {
        return;
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
        ns_sha256_process(ctx, ctx->buffer);
        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while (ilen >= 64) {
        ns_sha256_process(ctx, input);
        input += 64;
        ilen  -= 64;
    }

    if (ilen > 0) {
        memcpy((void *)(ctx->buffer + left), input, ilen);
    }
}

/*
 * SHA-256 final digest
 */
void ns_sha256_finish_nbits(ns_sha256_context *ctx, void *outputv, unsigned obits)
{
    unsigned char *output = outputv;
    uint32_t last, padn;
    uint32_t high, low;
    unsigned char msglen[8];
    unsigned int i;

    high = (ctx->total[0] >> 29)
           | (ctx->total[1] <<  3);
    low  = (ctx->total[0] <<  3);

    PUT_UINT32_BE(high, msglen, 0);
    PUT_UINT32_BE(low,  msglen, 4);

    last = ctx->total[0] & 0x3F;
    padn = (last < 56) ? (56 - last) : (120 - last);

    /* Const compound literals need not designate distinct objects - this could
     * potentially save ROM over declaring the padding array.
     */
    ns_sha256_update(ctx, (const unsigned char[64]) {
        0x80
    }, padn);
    ns_sha256_update(ctx, msglen, 8);

    for (i = 0; i < 8 && obits > 0; i++, obits -= 32) {
        PUT_UINT32_BE(ctx->state[i], output, 4 * i);
    }
}

void ns_sha256_finish(ns_sha256_context *ctx, void *output)
{
    ns_sha256_finish_nbits(ctx, output, 256);
}

/*
 * output = SHA-256( input buffer )
 */
void ns_sha256_nbits(const void *input, size_t ilen,
                     void *output, unsigned obits)
{
    ns_sha256_context ctx;

    ns_sha256_init(&ctx);
    ns_sha256_starts(&ctx);
    ns_sha256_update(&ctx, input, ilen);
    ns_sha256_finish_nbits(&ctx, output, obits);
    ns_sha256_free(&ctx);
}

void ns_sha256(const void *input, size_t ilen,
               void *output)
{
    ns_sha256_nbits(input, ilen, output, 256);
}
#endif /* NS_USE_EXTERNAL_MBED_TLS */
