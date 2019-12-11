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
 * \file     sha1_alt.c
 * \version  1.0
 *
 * \brief    Source file - wrapper for mbedtls SHA1 HW acceleration
 *
 */


#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "config.h"
#endif

#if defined(MBEDTLS_SHA1_C)

#include "mbedtls/sha1.h"
#include "mbedtls/platform_util.h"

#include <string.h>

#if defined(MBEDTLS_SHA1_ALT)

/* Parameter validation macros based on platform_util.h */
#define SHA1_VALIDATE_RET(cond)                             \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_SHA1_BAD_INPUT_DATA )
#define SHA1_VALIDATE(cond)  MBEDTLS_INTERNAL_VALIDATE( cond )

void mbedtls_sha1_init( mbedtls_sha1_context *ctx )
{
    SHA1_VALIDATE( ctx != NULL );

    cy_hw_sha_init(ctx, sizeof( mbedtls_sha1_context ));
}

void mbedtls_sha1_free( mbedtls_sha1_context *ctx )
{
    if (ctx == NULL)
        return;

    cy_hw_sha_free(ctx, sizeof( mbedtls_sha1_context ));
}

void mbedtls_sha1_clone( mbedtls_sha1_context *dst, const mbedtls_sha1_context *src )
{
    SHA1_VALIDATE( dst != NULL );
    SHA1_VALIDATE( src != NULL );

    cy_hw_sha_clone(dst, src, sizeof(mbedtls_sha1_context), &dst->hashState, &dst->shaBuffers);
}

/*
 * SHA-1 context setup
 */
int mbedtls_sha1_starts_ret( mbedtls_sha1_context *ctx )
{
    SHA1_VALIDATE_RET( ctx != NULL );

    return cy_hw_sha_start(&ctx->obj, &ctx->hashState, CY_CRYPTO_MODE_SHA1, &ctx->shaBuffers);
}

/*
 * SHA-1 process buffer
 */
int mbedtls_sha1_update_ret( mbedtls_sha1_context *ctx,
                             const unsigned char *input,
                             size_t ilen )
{
    SHA1_VALIDATE_RET( ctx != NULL );
    SHA1_VALIDATE_RET( ilen == 0 || input != NULL );

    if (ilen == 0)
        return (0);

    return cy_hw_sha_update(&ctx->obj, &ctx->hashState, input, ilen);
}

/*
 * SHA-1 final digest
 */
int mbedtls_sha1_finish_ret( mbedtls_sha1_context *ctx, unsigned char output[20])
{
    SHA1_VALIDATE_RET( ctx != NULL );
    SHA1_VALIDATE_RET( (unsigned char *)output != NULL );

    return cy_hw_sha_finish(&ctx->obj, &ctx->hashState, output);
}

int mbedtls_internal_sha1_process( mbedtls_sha1_context *ctx, const unsigned char data[64] )
{
    SHA1_VALIDATE_RET( ctx != NULL );
    SHA1_VALIDATE_RET( (const unsigned char *)data != NULL );

    return cy_hw_sha_process(&ctx->obj, &ctx->hashState, data);
}

#endif /* MBEDTLS_SHA1_ALT */

#endif /* MBEDTLS_SHA1_C */
