/***************************************************************************//**
* \file cy_crypto_core_des.h
* \version 2.40
*
* \brief
*  This file provides constant and parameters for the API for the DES method
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


#if !defined(CY_CRYPTO_CORE_DES_H)
#define CY_CRYPTO_CORE_DES_H

#include "cy_crypto_common.h"

#if defined(CY_IP_MXCRYPTO)

#if defined(__cplusplus)
extern "C" {
#endif

#if (CPUSS_CRYPTO_DES == 1)

#include "cy_crypto_core_des_v1.h"
#include "cy_crypto_core_des_v2.h"

typedef cy_en_crypto_status_t (*cy_crypto_des_func_t)(CRYPTO_Type *base,
                                        cy_en_crypto_dir_mode_t dirMode,
                                        uint8_t const *key,
                                        uint8_t *dst,
                                        uint8_t const *src);

/**
* \addtogroup group_crypto_lld_symmetric_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Des
****************************************************************************//**
*
* Performs the DES operation on a single block. All addresses must be 4-byte aligned.
* Ciphertext (dst) may overlap with plaintext (src).
* This function is independent from the previous Crypto state.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param dirMode
* Can be \ref CY_CRYPTO_ENCRYPT or \ref CY_CRYPTO_DECRYPT
* (\ref cy_en_crypto_dir_mode_t)
*
* \param key
* The pointer to the encryption/decryption key.
*
* \param dst
* The pointer to the destination cipher block.
*
* \param src
* The pointer to the source block.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Des(CRYPTO_Type *base,
                                        cy_en_crypto_dir_mode_t dirMode,
                                        uint8_t const *key,
                                        uint8_t *dst,
                                        uint8_t const *src)
{
    cy_en_crypto_status_t tmpResult;

    if (CY_CRYPTO_V1)
    {
        tmpResult = Cy_Crypto_Core_V1_Des(base, dirMode, key, dst, src);
    }
    else
    {
        tmpResult = Cy_Crypto_Core_V2_Des(base, dirMode, key, dst, src);
    }

    return tmpResult;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Tdes
****************************************************************************//**
*
* Performs  the TDES operation on a single block. All addresses must be 4-byte aligned.
* Ciphertext (dst) may overlap with plaintext (src).
* This function is independent from the previous Crypto state.
*
* \param base
* The pointer to the CRYPTO instance.
*
* \param dirMode
* Can be \ref CY_CRYPTO_ENCRYPT or \ref CY_CRYPTO_DECRYPT
* (\ref cy_en_crypto_dir_mode_t)
*
* \param key
* The pointer to the encryption/decryption keys.
*
* \param dst
* The pointer to the destination cipher block.
*
* \param src
* The pointer to the source data block.
*
* \return
* \ref cy_en_crypto_status_t
*
*******************************************************************************/
__STATIC_INLINE cy_en_crypto_status_t Cy_Crypto_Core_Tdes(CRYPTO_Type *base,
                                        cy_en_crypto_dir_mode_t dirMode,
                                        uint8_t const *key,
                                        uint8_t *dst,
                                        uint8_t const *src)
{
    cy_en_crypto_status_t tmpResult;

    if (CY_CRYPTO_V1)
    {
        tmpResult = Cy_Crypto_Core_V1_Tdes(base, dirMode, key, dst, src);
    }
    else
    {
        tmpResult = Cy_Crypto_Core_V2_Tdes(base, dirMode, key, dst, src);
    }

    return tmpResult;
}

/** \} group_crypto_lld_symmetric_functions */

#endif /* #if (CPUSS_CRYPTO_DES == 1) */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXCRYPTO */

#endif /* #if !defined(CY_CRYPTO_CORE_DES_H) */


/* [] END OF FILE */
