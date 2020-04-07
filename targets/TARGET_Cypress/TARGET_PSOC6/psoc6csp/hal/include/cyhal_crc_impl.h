/***************************************************************************//**
 * \file cyhal_crc_impl.h
*
* Description:
* Provides a high level interface for interacting with the Cypress CRC accelerator.
* This is a wrapper around the lower level PDL API.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#pragma once

#include "cyhal_crc.h"
#include "cyhal_hwmgr.h"
#include "cy_utils.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

// This helper function mirrors the definition of cyhal_crc_start
__STATIC_INLINE cy_rslt_t cyhal_crc_start_internal(cyhal_crc_t *obj, const crc_algorithm_t *algorithm)
{
    CY_ASSERT(NULL != obj);
    if(NULL == algorithm)
        return CYHAL_CRC_RSLT_ERR_BAD_ARGUMENT;

    obj->crc_width = algorithm->width;
    return Cy_Crypto_Core_Crc_CalcInit(obj->base,
                                algorithm->width,
                                algorithm->polynomial,
                                algorithm->dataReverse ? 1u : 0u,
                                algorithm->dataXor,
                                algorithm->remReverse ? 1u : 0u,
                                algorithm->remXor,
                                algorithm->lfsrInitState);
}

#define cyhal_crc_start(obj, algorithm) cyhal_crc_start_internal(obj, algorithm)

// This helper function mirrors the definition of cyhal_crc_compute
__STATIC_INLINE cy_rslt_t cyhal_crc_compute_internal(const cyhal_crc_t *obj, const uint8_t *data, size_t length)
{
    CY_ASSERT(NULL != obj);
    if(NULL == data || 0 == length)
        return CYHAL_CRC_RSLT_ERR_BAD_ARGUMENT;

    return Cy_Crypto_Core_Crc_CalcPartial(obj->base, data, length);
}

#define cyhal_crc_compute(obj, data, length) cyhal_crc_compute_internal(obj, data, length)

// This helper function mirrors the definition of cyhal_crc_finish
__STATIC_INLINE cy_rslt_t cyhal_crc_finish_internal(const cyhal_crc_t *obj, uint32_t *crc)
{
    CY_ASSERT(NULL != obj);
    if(NULL == crc)
        return CYHAL_CRC_RSLT_ERR_BAD_ARGUMENT;

    return Cy_Crypto_Core_Crc_CalcFinish(obj->base, obj->crc_width, crc);
}

#define cyhal_crc_finish(obj, crc) cyhal_crc_finish_internal(obj, crc)

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* defined(CY_IP_MXCRYPTO) */
