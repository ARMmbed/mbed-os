/***************************************************************************//**
* \file cy_crypto_core_cmac.h
* \version 2.20
*
* \brief
*  This file provides constants and function prototypes
*  for the API for the CMAC method in the Crypto block driver.
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


#if !defined(CY_CRYPTO_CORE_CMAC_H)
#define CY_CRYPTO_CORE_CMAC_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_AES == 1)

#include "cy_crypto_core_cmac_v1.h"
#include "cy_crypto_core_cmac_v2.h"

/** \cond INTERNAL */

typedef cy_en_crypto_status_t (*cy_crypto_cmac_func_t)(CRYPTO_Type *base,
                                          uint8_t  const *src,
                                          uint32_t srcSize,
                                          uint8_t  const *key,
                                          cy_en_crypto_aes_key_length_t keyLength,
                                          uint8_t *dst,
                                          cy_stc_crypto_aes_state_t *aesState);
/** \endcond */

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Cmac
****************************************************************************//**
*
* Performs CMAC(Cipher-based Message Authentication Code) operation
* on a message to produce message authentication code using AES.
*
* \param base
* The pointer to the CRYPTO instance address.
*
* \param message
* The pointer to a source plain text. Must be 4-byte aligned.
*
* \param messageSize
* The size of a source plain text.
*
* \param key
* The pointer to the encryption key. Must be 4-byte aligned.
*
* \param keyLength
* \ref cy_en_crypto_aes_key_length_t
*
* \param cmac
* The pointer to the calculated CMAC.
*
* \param aesState
* The pointer to the aesState structure which stores the AES context.
*
* \return
* A Crypto status \ref en_crypto_status_t.
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Cmac(CRYPTO_Type *base,
                                          uint8_t  const *message,
                                          uint32_t messageSize,
                                          uint8_t  const *key,
                                          cy_en_crypto_aes_key_length_t keyLength,
                                          uint8_t *cmac,
                                          cy_stc_crypto_aes_state_t *aesState)
{
    cy_en_crypto_status_t myResult;

    if (cy_device->cryptoVersion == 1u)
    {
        myResult = Cy_Crypto_Core_V1_Cmac(base, message, messageSize, key, keyLength, cmac, aesState);
    }
    else
    {
        myResult = Cy_Crypto_Core_V2_Cmac(base, message, messageSize, key, keyLength, cmac, aesState);
    }

    return myResult;
}

#endif /* (CPUSS_CRYPTO_AES == 1) */

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_CMAC_H) */


/* [] END OF FILE */
