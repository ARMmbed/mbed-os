/*
 *  mbed Microcontroller Library
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Copyright (C) 2019 Cypress Semiconductor Corporation
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     sha512_alt.c
 * \version  1.0
 *
 * \brief    Source file - wrapper for mbedtls SHA512 HW acceleration
 *
 */

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "config.h"
#endif

#if defined(MBEDTLS_SHA512_C)

#include "mbedtls/sha512.h"
#include "mbedtls/platform_util.h"

#include <string.h>

#if defined(MBEDTLS_SHA512_ALT)

/* Parameter validation macros based on platform_util.h */
#define SHA512_VALIDATE_RET(cond)                           \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_SHA512_BAD_INPUT_DATA )
#define SHA512_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

void mbedtls_sha512_init( mbedtls_sha512_context *ctx )
{
    SHA512_VALIDATE( ctx != NULL );

    cy_hw_sha_init(ctx, sizeof( mbedtls_sha512_context ));
}

void mbedtls_sha512_free( mbedtls_sha512_context *ctx )
{
    if (ctx == NULL)
        return;

    cy_hw_sha_free(ctx, sizeof( mbedtls_sha512_context ));
}

void mbedtls_sha512_clone( mbedtls_sha512_context *dst, const mbedtls_sha512_context *src )
{
    SHA512_VALIDATE( dst != NULL );
    SHA512_VALIDATE( src != NULL );

    cy_hw_sha_clone(dst, src, sizeof(mbedtls_sha512_context), &dst->hashState, &dst->shaBuffers);
}

/*
 * SHA-512 context setup
 */
int mbedtls_sha512_starts_ret( mbedtls_sha512_context *ctx, int is384)
{
    SHA512_VALIDATE_RET( ctx != NULL );
    SHA512_VALIDATE_RET( is384 == 0 || is384 == 1 );

    return cy_hw_sha_start(&ctx->obj, &ctx->hashState,
                           ( is384 == 0 ) ? CY_CRYPTO_MODE_SHA512 : CY_CRYPTO_MODE_SHA384,
                           &ctx->shaBuffers);
}

/*
 * SHA-512 process buffer
 */
int mbedtls_sha512_update_ret( mbedtls_sha512_context *ctx, const unsigned char *input, size_t ilen )
{
    SHA512_VALIDATE_RET( ctx != NULL );
    SHA512_VALIDATE_RET( ilen == 0 || input != NULL );

    if (ilen == 0)
        return (0);

    return cy_hw_sha_update(&ctx->obj, &ctx->hashState, input, ilen);
}

/*
 * SHA-512 final digest
 */
int mbedtls_sha512_finish_ret( mbedtls_sha512_context *ctx, unsigned char output[64] )
{
    SHA512_VALIDATE_RET( ctx != NULL );
    SHA512_VALIDATE_RET( (unsigned char *)output != NULL );

    return cy_hw_sha_finish(&ctx->obj, &ctx->hashState, output);
}

int mbedtls_internal_sha512_process( mbedtls_sha512_context *ctx, const unsigned char data[128] )
{
    SHA512_VALIDATE_RET( ctx != NULL );
    SHA512_VALIDATE_RET( (const unsigned char *)data != NULL );

    return cy_hw_sha_process(&ctx->obj, &ctx->hashState, data);
}

#endif /* MBEDTLS_SHA512_ALT */

#endif /* MBEDTLS_SHA512_C */
