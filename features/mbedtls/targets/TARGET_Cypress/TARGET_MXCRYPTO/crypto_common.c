/*
 * mbed Microcontroller Library
 * Copyright (c) 2019 Cypress Semiconductor Corporation
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

/**
 * \file    crypto_common.c
 * \version 1.0
 *
 * \brief   Source file for common mbedtls acceleration functions
 *
 */

#include <string.h>

#include "crypto_common.h"
#include "cy_crypto_common.h"
#include "cy_crypto_core_hw.h"
#include "cy_crypto_core_mem.h"

#define CY_CRYPTO_CHECK_PARAM(cond)         do {                \
                                                if( !(cond) )   \
                                                    return;     \
                                            } while( 0 )

/*******************************************************************************
*       Crypto object manage functions
*******************************************************************************/
bool cy_hw_crypto_reserve(cy_hw_crypto_t *obj, cyhal_crypto_feature_t feature)
{
    cy_rslt_t status;
    CY_ASSERT( obj != NULL );

    status = cyhal_crypto_reserve(&(obj->base), &(obj->resource), feature);
    if (CY_RSLT_SUCCESS == status)
    {
        obj->feature = feature;
    }

    return (CY_RSLT_SUCCESS == status);
}

void cy_hw_crypto_release(cy_hw_crypto_t *obj)
{
    CY_ASSERT( obj != NULL );
    if (obj->resource.type == CYHAL_RSC_CRYPTO)
    {
        cyhal_crypto_free(obj->base, &(obj->resource), obj->feature);
    }
}

void cy_hw_zeroize(void *data, uint32_t dataSize)
{
    cy_hw_crypto_t obj;
    CY_CRYPTO_CHECK_PARAM( data != NULL );
    CY_CRYPTO_CHECK_PARAM( dataSize > 0u );

    if (cy_hw_crypto_reserve(&obj, CYHAL_CRYPTO_COMMON))
    {
        Cy_Crypto_Core_MemSet(obj.base, data, 0u, (uint16_t)dataSize);
    }
    cy_hw_crypto_release(&obj);
}

void cy_hw_sha_init(void *ctx, uint32_t ctxSize)
{
    CY_CRYPTO_CHECK_PARAM( ctx != NULL );
    CY_CRYPTO_CHECK_PARAM( ctxSize > 0u );

    cy_hw_zeroize(ctx, ctxSize);

    (void)cy_hw_crypto_reserve((cy_hw_crypto_t *)ctx, CYHAL_CRYPTO_COMMON);
}

void cy_hw_sha_free(void *ctx, uint32_t ctxSize)
{
    CY_CRYPTO_CHECK_PARAM( ctx != NULL );
    CY_CRYPTO_CHECK_PARAM( ctxSize > 0u);
         
    cy_hw_crypto_release((cy_hw_crypto_t *)ctx);
    cy_hw_zeroize(ctx, ctxSize);
}

int cy_hw_sha_start(cy_hw_crypto_t *obj, cy_stc_crypto_sha_state_t *hashState,
                    cy_en_crypto_sha_mode_t shaMode, void *shaBuffers)
{
    cy_en_crypto_status_t status;

    if ((hashState == NULL) || (shaBuffers == NULL))
        return (-1);

    status = Cy_Crypto_Core_Sha_Init(obj->base, hashState, shaMode, shaBuffers);
    if (CY_CRYPTO_SUCCESS != status)
        return (-1);

    status = Cy_Crypto_Core_Sha_Start(obj->base, hashState);
    if (CY_CRYPTO_SUCCESS != status)
        return (-1);

    return (0);
}

int cy_hw_sha_update(cy_hw_crypto_t *obj, cy_stc_crypto_sha_state_t *hashState,
                     const uint8_t *in, uint32_t inlen)
{
    cy_en_crypto_status_t status;

    if ((hashState == NULL) || (in == NULL))
        return (-1);

    if (hashState->blockSize == 0)
        return (-1);

    status = Cy_Crypto_Core_Sha_Update(obj->base, hashState, in, inlen);
    if (CY_CRYPTO_SUCCESS != status)
        return (-1);

    return (0);
}

int cy_hw_sha_finish(cy_hw_crypto_t *obj, cy_stc_crypto_sha_state_t *hashState, uint8_t *output)
{
    cy_en_crypto_status_t status;

    if ((hashState == NULL) || (output == NULL))
        return (-1);

    if (hashState->blockSize == 0)
        return (-1);

    status = Cy_Crypto_Core_Sha_Finish(obj->base, hashState, output);
    if (CY_CRYPTO_SUCCESS != status)
        return (-1);

    return (0);
}

int cy_hw_sha_process(cy_hw_crypto_t *obj, cy_stc_crypto_sha_state_t *hashState, const uint8_t *in)
{
    cy_en_crypto_status_t status;

    status = Cy_Crypto_Core_Sha_Update(obj->base, hashState, in, hashState->blockSize);
    if (CY_CRYPTO_SUCCESS != status)
        return (-1);

    return (0);
}

void cy_hw_sha_clone( void *ctxDst, const void *ctxSrc, uint32_t ctxSize,
                     cy_stc_crypto_sha_state_t *hashStateDst, void *shaBuffersDst)
{
    CY_CRYPTO_CHECK_PARAM( ctxDst != NULL );
    CY_CRYPTO_CHECK_PARAM( ctxSrc != NULL );
    CY_CRYPTO_CHECK_PARAM( hashStateDst != NULL );
    CY_CRYPTO_CHECK_PARAM( shaBuffersDst != NULL );

    Cy_Crypto_Core_MemCpy(((cy_hw_crypto_t *)ctxSrc)->base, ctxDst, ctxSrc, (uint16_t)ctxSize);
    Cy_Crypto_Core_Sha_Init(((cy_hw_crypto_t *)ctxSrc)->base, hashStateDst, (cy_en_crypto_sha_mode_t)hashStateDst->mode, shaBuffersDst);
}
