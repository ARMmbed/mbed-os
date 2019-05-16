/***************************************************************************//**
* \file cy_crypto_core_sha_v2.h
* \version 2.20
*
* \brief
*  This file provides constants and function prototypes
*  for the API for the SHA method in the Crypto block driver.
*
********************************************************************************
* Copyright 2016-2019 Cypress Semiconductor Corporation
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


#if !defined(CY_CRYPTO_CORE_SHA_V2_H)
#define CY_CRYPTO_CORE_SHA_V2_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_SHA == 1)

#include "cy_syslib.h"

/** \cond INTERNAL */

cy_en_crypto_status_t Cy_Crypto_Core_V2_Sha_Init(CRYPTO_Type *base,
                                cy_stc_crypto_sha_state_t *hashState,
                                cy_en_crypto_sha_mode_t mode,
                                void *shaBuffers);

cy_en_crypto_status_t Cy_Crypto_Core_V2_Sha_Start(CRYPTO_Type *base, cy_stc_crypto_sha_state_t *hashState);

cy_en_crypto_status_t Cy_Crypto_Core_V2_Sha_Update(CRYPTO_Type *base,
                                cy_stc_crypto_sha_state_t *hashState,
                                uint8_t const *message,
                                uint32_t messageSize);

cy_en_crypto_status_t Cy_Crypto_Core_V2_Sha_Finish(CRYPTO_Type *base,
                                cy_stc_crypto_sha_state_t *hashState,
                                uint8_t *digest);

cy_en_crypto_status_t Cy_Crypto_Core_V2_Sha_Free(CRYPTO_Type *base, cy_stc_crypto_sha_state_t *hashState);

cy_en_crypto_status_t Cy_Crypto_Core_V2_Sha(CRYPTO_Type *base,
                                uint8_t const *message,
                                uint32_t messageSize,
                                uint8_t *digest,
                                cy_en_crypto_sha_mode_t mode);

/** \endcond */


#endif /* #if (CPUSS_CRYPTO_SHA == 1) */

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_SHA_V2_H) */


/* [] END OF FILE */
