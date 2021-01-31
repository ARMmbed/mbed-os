/***************************************************************************//**
* \file cy_crypto_core_mem_v2.h
* \version 2.40
*
* \brief
*  This file provides the headers for the string management API
*  in the Crypto driver.
*
********************************************************************************
* Copyright 2016-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/


#if !defined(CY_CRYPTO_CORE_MEM_V2_H)
#define CY_CRYPTO_CORE_MEM_V2_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

void Cy_Crypto_Core_V2_MemCpy(CRYPTO_Type *base,
                               void* dst, void const *src, uint16_t size);
void Cy_Crypto_Core_V2_MemSet(CRYPTO_Type *base,
                               void* dst, uint8_t data, uint16_t size);
uint32_t Cy_Crypto_Core_V2_MemCmp(CRYPTO_Type *base,
                               void const *src0, void const *src1, uint16_t size);
void Cy_Crypto_Core_V2_MemXor(CRYPTO_Type *base, void* dst,
                               void const *src0, void const *src1, uint16_t size);

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_MEM_V2_H) */


/* [] END OF FILE */
