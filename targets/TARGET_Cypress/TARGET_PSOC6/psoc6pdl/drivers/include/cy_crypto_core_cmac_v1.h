/***************************************************************************//**
* \file cy_crypto_core_cmac_v1.h
* \version 2.40
*
* \brief
*  This file provides constants and function prototypes
*  for the API for the CMAC method in the Crypto block driver.
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


#if !defined(CY_CRYPTO_CORE_CMAC_V1_H)
#define CY_CRYPTO_CORE_CMAC_V1_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

#if (CPUSS_CRYPTO_AES == 1)

/** \cond INTERNAL */

/* The bit string used to generate sub-keys */
#define CY_CRYPTO_CMAC_RB  (0x87u)

/* The structure to store the AES-CMAC context */
typedef struct
{
    uint32_t *block;
    uint32_t *k;
    uint32_t *temp;
} cy_stc_crypto_v1_cmac_state_t;

/* The structure to define used memory buffers */
typedef struct
{
    uint32_t k[CY_CRYPTO_AES_BLOCK_SIZE_U32];
    uint32_t block0[CY_CRYPTO_AES_BLOCK_SIZE_U32];
    uint32_t block1[CY_CRYPTO_AES_BLOCK_SIZE_U32];
    cy_stc_crypto_v1_cmac_state_t cmacState;
} cy_stc_crypto_cmac_buffers_t;

/* The function prototypes */
void Cy_Crypto_Core_V1_Cmac_Init(cy_stc_crypto_v1_cmac_state_t *cmacState,
                              uint32_t *temp,
                              uint32_t *block,
                              uint32_t *k);

void Cy_Crypto_Core_V1_Cmac_Start(CRYPTO_Type *base,
                                cy_stc_crypto_aes_state_t  *aesState,
                                cy_stc_crypto_v1_cmac_state_t *cmacState);

void Cy_Crypto_Core_V1_Cmac_Update(CRYPTO_Type *base, cy_stc_crypto_aes_state_t  *aesState,
                                cy_stc_crypto_v1_cmac_state_t *cmacState,
                                uint8_t  const *message,
                                uint32_t messageSize);

void Cy_Crypto_Core_V1_Cmac_Finish(CRYPTO_Type *base,
                                cy_stc_crypto_aes_state_t  *aesState,
                                cy_stc_crypto_v1_cmac_state_t *cmacState,
                                uint8_t* cmac);

cy_en_crypto_status_t Cy_Crypto_Core_V1_Cmac(CRYPTO_Type *base,
                                          uint8_t  const *message,
                                          uint32_t messageSize,
                                          uint8_t  const *key,
                                          cy_en_crypto_aes_key_length_t keyLength,
                                          uint8_t *cmac,
                                          cy_stc_crypto_aes_state_t *aesState);

/** \endcond */


#endif /* (CPUSS_CRYPTO_AES == 1) */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_CMAC_V1_H) */


/* [] END OF FILE */
