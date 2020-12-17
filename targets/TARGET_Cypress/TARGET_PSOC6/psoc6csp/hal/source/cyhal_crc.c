/*******************************************************************************
* File Name: cyhal_crc.c
*
* Description:
* Provides a high level interface for interacting with the Cypress CRC. This is
* a wrapper around the lower level PDL API.
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

#include "cy_crypto_core_crc.h"
#include "cyhal_crypto_common.h"
#include "cyhal_crc_impl.h"
#include "cyhal_hwmgr.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C"
{
#endif

/*******************************************************************************
*       Functions
*******************************************************************************/
cy_rslt_t cyhal_crc_init(cyhal_crc_t *obj)
{
    CY_ASSERT(NULL != obj);
    // Note: this takes care of enabling the Crypto block
    return cyhal_crypto_reserve(&(obj->base), &(obj->resource), CYHAL_CRYPTO_CRC);
}

void cyhal_crc_free(cyhal_crc_t *obj)
{
    CY_ASSERT(NULL != obj || obj->resource.type != CYHAL_RSC_CRYPTO);
    if (obj->resource.type != CYHAL_RSC_INVALID)
    {
        cyhal_crypto_free(obj->base, &(obj->resource), CYHAL_CRYPTO_CRC);
    }
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXCRYPTO) */
