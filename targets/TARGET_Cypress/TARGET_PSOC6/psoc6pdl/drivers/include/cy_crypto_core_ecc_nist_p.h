/***************************************************************************//**
* \file cy_crypto_core_ecc_nist_p.h
* \version 2.20
*
* \brief
*  This file provides constant and parameters for the API for the ECC
*  in the Crypto driver.
*
********************************************************************************
* Copyright 2016-2019 Cypress Semiconductor Corporation
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


#if !defined(CY_CRYPTO_CORE_NIST_P_H)
#define CY_CRYPTO_CORE_NIST_P_H

#include "cy_crypto_core_ecc.h"
#include "cy_syslib.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_VU == 1)

void Cy_Crypto_Core_EC_NistP_SetMode(int bitsize);
void Cy_Crypto_Core_EC_NistP_SetRedAlg(cy_en_crypto_ecc_red_mul_algs_t alg);
void Cy_Crypto_Core_EC_NistP_PointMul(CRYPTO_Type *base, int p_x, int p_y, int p_d, int p_order, int bitsize);
cy_en_crypto_status_t Cy_Crypto_Core_EC_NistP_PointMultiplication(CRYPTO_Type *base,
    cy_en_crypto_ecc_curve_id_t curveID,
    uint8_t* ecpGX,
    uint8_t* ecpGY,
    uint8_t* ecpD,
    uint8_t* ecpQX,
    uint8_t* ecpQY);
void Cy_Crypto_Core_EC_MulMod( CRYPTO_Type *base,
    int z,
    int a,
    int b,
    int size);
void Cy_Crypto_Core_EC_DivMod( CRYPTO_Type *base,
    int z,
    int a,
    int b,
    int size);
void Cy_Crypto_Core_EC_SquareMod( CRYPTO_Type *base,
    int z,
    int a,
    int size);
void Cy_Crypto_Core_EC_Bar_MulRed(CRYPTO_Type *base,
    int z,
    int x,
    int size
);
void Cy_Crypto_Core_EC_AddMod( CRYPTO_Type *base, int z, int a, int b);
void Cy_Crypto_Core_EC_SubMod( CRYPTO_Type *base, int z, int a, int b);
void Cy_Crypto_Core_EC_HalfMod( CRYPTO_Type *base, int z, int a);

#endif /* #if (CPUSS_CRYPTO_VU == 1) */

#endif /* #if defined(CY_IP_MXCRYPTO) */

#endif /* #if !defined(CY_CRYPTO_CORE_NIST_P_H) */


/* [] END OF FILE */
