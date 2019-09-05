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
 * \file    crypto_common.h
 * \version 1.0
 *
 * \brief   Header file for common mbedtls acceleration functions
 *
 */

#if !defined(CRYPTO_COMMON_H)
#define CRYPTO_COMMON_H

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "config.h"
#endif

#include "mbedtls/ecp.h"

#include "cy_crypto_core_sha.h"

#include "cyhal_crypto_common.h"

/** CRYPTO object */
typedef struct {
#if defined(CY_IP_MXCRYPTO_INSTANCES) || defined(CPUSS_CRYPTO_PRESENT)
    CRYPTO_Type*                base;
    cyhal_resource_inst_t       resource;
    cyhal_crypto_feature_t      feature;
#endif
} cy_hw_crypto_t;

bool cy_hw_crypto_reserve(cy_hw_crypto_t *obj, cyhal_crypto_feature_t feature);
void cy_hw_crypto_release(cy_hw_crypto_t *obj);
void cy_hw_zeroize(void *data, uint32_t dataSize);

void cy_hw_sha_init(void *ctx, uint32_t ctxSize);
void cy_hw_sha_free(void *ctx, uint32_t ctxSize);

int cy_hw_sha_start (cy_hw_crypto_t *obj, cy_stc_crypto_sha_state_t *hashState,
                     cy_en_crypto_sha_mode_t shaMode, void *shaBuffers);

int cy_hw_sha_update(cy_hw_crypto_t *obj, cy_stc_crypto_sha_state_t *hashState,
                     const uint8_t *in, uint32_t inlen);

int cy_hw_sha_finish(cy_hw_crypto_t *obj, cy_stc_crypto_sha_state_t *hashState, 
                     uint8_t *output);

void cy_hw_sha_clone(void *ctxDst, const void *ctxSrc, uint32_t ctxSize,
                     cy_stc_crypto_sha_state_t *hashStateDst, void *shaBuffersDst);

int cy_hw_sha_process(cy_hw_crypto_t *obj, cy_stc_crypto_sha_state_t *hashState, 
                     const uint8_t *in);

#endif /* (CRYPTO_COMMON_H) */
