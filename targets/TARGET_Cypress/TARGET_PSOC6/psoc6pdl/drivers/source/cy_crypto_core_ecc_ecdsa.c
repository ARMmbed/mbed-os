/***************************************************************************//**
* \file cy_crypto_core_ecc_ecdsa.c
* \version 2.20
*
* \brief
*  This file provides constant and parameters for the API for the ECC ECDSA
*  in the Crypto driver.
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


#include "cy_crypto_core_ecc.h"
#include "cy_crypto_core_ecc_nist_p.h"
#include "cy_crypto_core_mem.h"
#include "cy_crypto_core_vu.h"

#include <stdbool.h>


/*******************************************************************************
* Function Name: Cy_Crypto_Core_ECC_SignHash
****************************************************************************//**
*
* Sign a message digest.
*
* \param base
* The pointer to a Crypto instance.
*
* \param hash
* The message digest to sign.
*
* \param hashlen
* The length of the digest in bytes.
*
* \param sig
* [out] The destination for the signature, 'r' followed by 's'.
*
* \param key
* Key used for signature generation. See \ref cy_stc_crypto_ecc_key.
*
* \param messageKey
* Message key.
*
* \return status code. See \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_ECC_SignHash(CRYPTO_Type *base, const uint8_t *hash, uint32_t hashlen, uint8_t *sig,
        const cy_stc_crypto_ecc_key *key, const uint8_t *messageKey)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_BAD_PARAMS;

    cy_stc_crypto_ecc_key ephKey;
    uint8_t myKGX[CY_CRYPTO_ECC_MAX_BYTE_SIZE];
    uint8_t myKGY[CY_CRYPTO_ECC_MAX_BYTE_SIZE];

    const cy_stc_crypto_ecc_dp_type *eccDp;
    uint32_t mallocMask = 0U;
    uint32_t bitsize;

    /* NULL parameters checking */
    if ((hash != NULL) && (sig != NULL) && (key != NULL) && (messageKey != NULL))
    {
        tmpResult = CY_CRYPTO_NOT_SUPPORTED;

        eccDp = Cy_Crypto_Core_ECC_GetCurveParams(key->curveID);

        if (eccDp != NULL)
        {
            bitsize = eccDp->size;

            /* make ephemeral key pair */
            ephKey.pubkey.x = myKGX;
            ephKey.pubkey.y = myKGY;

            tmpResult = Cy_Crypto_Core_ECC_MakePublicKey(base, key->curveID, messageKey, &ephKey);

            if (CY_CRYPTO_SUCCESS == tmpResult)
            {
                tmpResult = CY_CRYPTO_BAD_PARAMS;

                uint32_t dividend =  0U;   /* for whatever reason Crypto_EC_DivMod only works if dividend is in register 0 */
                uint32_t p_temp   =  8U;
                uint32_t p_r      =  9U;
                uint32_t p_s      = 10U;
                uint32_t p_d      = 11U;

                /* load values needed for reduction modulo order of the base point */
                CY_CRYPTO_VU_ALLOC_MEM (base, VR_P, bitsize);
                Cy_Crypto_Core_Vu_SetMemValue (base, VR_P, (uint8_t *)eccDp->order, bitsize);

                CY_CRYPTO_VU_ALLOC_MEM (base, VR_BARRETT, bitsize + 1U);
                Cy_Crypto_Core_Vu_SetMemValue (base, VR_BARRETT, (uint8_t *)eccDp->barrett_o, bitsize);

                CY_CRYPTO_VU_ALLOC_MEM (base, p_r, bitsize);
                Cy_Crypto_Core_Vu_SetMemValue (base, p_r, (uint8_t *)ephKey.pubkey.x, bitsize);

                mallocMask = CY_CRYPTO_VU_REG_BIT(VR_P) | CY_CRYPTO_VU_REG_BIT(VR_BARRETT) | CY_CRYPTO_VU_REG_BIT(p_r);

                if (!Cy_Crypto_Core_Vu_IsRegZero(base, p_r))
                {
                    tmpResult = CY_CRYPTO_SUCCESS;

                    CY_CRYPTO_VU_ALLOC_MEM (base, p_d, bitsize);
                    CY_CRYPTO_VU_ALLOC_MEM (base, p_s, bitsize);

                    mallocMask |= CY_CRYPTO_VU_REG_BIT(p_d)  | CY_CRYPTO_VU_REG_BIT(p_s);

                    /* check that x1 is smaller than the order of the base point */
                    if (!Cy_Crypto_Core_Vu_IsRegLess(base, p_r, VR_P))
                    {
                        /* x1 >= order, needs reduction */
                        CY_CRYPTO_VU_ALLOC_MEM (base, p_temp, bitsize);
                        CY_CRYPTO_VU_MOV(base, p_temp, p_r);

                        /* z = x % mod */
                        Cy_Crypto_Core_EC_Bar_MulRed(base, p_r, p_temp, bitsize);

                        CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(p_temp));

                        /* r = x1 mod n */
                        Cy_Crypto_Core_Vu_GetMemValue (base, sig, p_r, bitsize);

                        if (Cy_Crypto_Core_Vu_IsRegZero(base, p_r))
                        {
                            /* R is zero!!! */
                            tmpResult = CY_CRYPTO_HW_ERROR;
                        }
                    }
                    else
                    {
                        /* carry is clear, i. e. x1 < order */
                        /* r = x1 */
                        Cy_Crypto_Core_MemCpy(base, sig, ephKey.pubkey.x, (uint16_t)CY_CRYPTO_BYTE_SIZE_OF_BITS(bitsize));
                    }

                    if (CY_CRYPTO_SUCCESS == tmpResult)
                    {
                        /* find s = (e + d*r)/k */

                        /* load signing private key */
                        Cy_Crypto_Core_Vu_SetMemValue (base, p_d, (uint8_t *)key->k, bitsize);

                        /* use Barrett reduction algorithm for operations modulo n (order of the base point) */
                        Cy_Crypto_Core_EC_NistP_SetRedAlg(CY_CRYPTO_NIST_P_BARRETT_RED_ALG);

                        /* d*r mod n */
                        Cy_Crypto_Core_EC_MulMod(base, p_s, p_d, p_r, bitsize);    /* z = a * b % mod */

                        /* load message hash, truncate it if needed */
                        CY_CRYPTO_VU_SET_TO_ZERO(base, p_d);

                        if ((hashlen * 8u) > bitsize)
                        {
                            Cy_Crypto_Core_Vu_SetMemValue (base, p_d, (uint8_t *)(&hash[hashlen - CY_CRYPTO_BYTE_SIZE_OF_BITS(bitsize)]), bitsize);
                        }
                        else
                        {
                            Cy_Crypto_Core_Vu_SetMemValue (base, p_d, (uint8_t *)hash, hashlen * 8U);
                        }

                        Cy_Crypto_Core_Vu_SetMemValue (base, p_r, messageKey, bitsize);

                        /* e + d*r mod n */
                        Cy_Crypto_Core_EC_AddMod (base, p_s, p_d, p_s);      /* z = a + b % mod */

                        /* (e + d*r)/k mod n */
                        CY_CRYPTO_VU_ALLOC_MEM (base, dividend, bitsize);
                        CY_CRYPTO_VU_MOV(base, dividend, p_s);

                        /* z = a / b % mod */
                        Cy_Crypto_Core_EC_DivMod(base, p_s, dividend, p_r, bitsize);

                        CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(dividend));

                        if (!Cy_Crypto_Core_Vu_IsRegZero(base, p_s))
                        {
                            /* S is not zero!!! */
                            Cy_Crypto_Core_Vu_GetMemValue (base, &sig[CY_CRYPTO_BYTE_SIZE_OF_BITS(bitsize)], p_s, bitsize);
                        }
                        else
                        {
                            tmpResult = CY_CRYPTO_HW_ERROR;
                        }
                    }

                }

                CY_CRYPTO_VU_FREE_MEM(base, mallocMask);
            }
        }
    }

    return (tmpResult);
}


/*******************************************************************************
* Function Name: Cy_Crypto_Core_ECC_VerifyHash
****************************************************************************//**
*
* Verify an ECC signature.
*
* \param base
* The pointer to a Crypto instance.
*
* \param sig
* The signature to verify, 'R' followed by 'S'.
*
* \param hash
* The hash (message digest) that was signed.
*
* \param hashlen
* The length of the hash (octets).
*
* \param stat
* Result of signature, 1==valid, 0==invalid.
*
* \param key
* The corresponding public ECC key. See \ref cy_stc_crypto_ecc_key.
*
* \return status code. See \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_ECC_VerifyHash(CRYPTO_Type *base,
                            const uint8_t *sig, const uint8_t *hash, uint32_t hashlen,
                            uint8_t *stat, const cy_stc_crypto_ecc_key *key)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_BAD_PARAMS;

    const cy_stc_crypto_ecc_dp_type *eccDp;

    uint32_t bitsize;
    uint32_t mallocMask = 0U;

    /* NULL parameters checking */
    if ((sig != NULL) && (hash != NULL) && (stat != NULL) && (key != NULL))
    {
        tmpResult = CY_CRYPTO_NOT_SUPPORTED;

        eccDp = Cy_Crypto_Core_ECC_GetCurveParams(key->curveID);

        if (eccDp != NULL)
        {
            bitsize = eccDp->size;

            tmpResult = CY_CRYPTO_SUCCESS;

            uint32_t dividend = 0u;   /* for whatever reason Crypto_EC_DivMod only works if dividend is in register 0 */
            uint32_t p_r  = 4U;
            uint32_t p_s  = 5U;
            uint32_t p_u1 = 6U;
            uint32_t p_u2 = 7U;
            uint32_t p_o  = 8U;
            uint32_t p_gx = 9U;
            uint32_t p_gy = 10U;
            uint32_t p_qx = 11U;
            uint32_t p_qy = 12U;

            /* use Barrett reduction algorithm for operations modulo n (order of the base point) */
            Cy_Crypto_Core_EC_NistP_SetRedAlg(CY_CRYPTO_NIST_P_BARRETT_RED_ALG);
            Cy_Crypto_Core_EC_NistP_SetMode(bitsize);

            /*******************************************************************************/
            /* load values needed for reduction modulo order of the base point             */
            CY_CRYPTO_VU_ALLOC_MEM (base, VR_P, bitsize);
            Cy_Crypto_Core_Vu_SetMemValue (base, VR_P, (uint8_t *)eccDp->order, bitsize);

            CY_CRYPTO_VU_ALLOC_MEM (base, VR_BARRETT, bitsize + 1U);
            Cy_Crypto_Core_Vu_SetMemValue (base, VR_BARRETT, (uint8_t *)eccDp->barrett_o, bitsize + 1U);

            /*******************************************************************************/
            /* check that R and S are within the valid range, i.e. 0 < R < n and 0 < S < n */
            CY_CRYPTO_VU_ALLOC_MEM (base, p_r, bitsize);
            CY_CRYPTO_VU_ALLOC_MEM (base, p_s, bitsize);
            Cy_Crypto_Core_Vu_SetMemValue (base, p_r, (uint8_t *)sig, bitsize);
            Cy_Crypto_Core_Vu_SetMemValue (base, p_s, (uint8_t *)&sig[CY_CRYPTO_BYTE_SIZE_OF_BITS(bitsize)], bitsize);

            mallocMask = CY_CRYPTO_VU_REG_BIT(VR_P) | CY_CRYPTO_VU_REG_BIT(VR_BARRETT) |
                         CY_CRYPTO_VU_REG_BIT(p_r)  | CY_CRYPTO_VU_REG_BIT(p_s);

            /* Check R and S range */
            if (Cy_Crypto_Core_Vu_IsRegZero(base, p_r))
            {
                /* R is zero!!! */
                tmpResult = CY_CRYPTO_BAD_PARAMS;
            }
            if (!Cy_Crypto_Core_Vu_IsRegLess(base, p_r, VR_P))
            {
                /* R is not less than n!!! */
                tmpResult = CY_CRYPTO_BAD_PARAMS;
            }
            if (Cy_Crypto_Core_Vu_IsRegZero(base, p_s))
            {
                /* S is zero!!! */
                tmpResult = CY_CRYPTO_BAD_PARAMS;
            }
            if (!Cy_Crypto_Core_Vu_IsRegLess(base, p_s, VR_P))
            {
                /* S is not less than n!!! */
                tmpResult = CY_CRYPTO_BAD_PARAMS;
            }

            if (CY_CRYPTO_SUCCESS == tmpResult)
            {
                CY_CRYPTO_VU_ALLOC_MEM (base, dividend, bitsize);
                CY_CRYPTO_VU_ALLOC_MEM (base, p_o, bitsize);
                CY_CRYPTO_VU_ALLOC_MEM (base, p_u1, bitsize);
                CY_CRYPTO_VU_ALLOC_MEM (base, p_u2, bitsize);

                CY_CRYPTO_VU_ALLOC_MEM (base, p_gx, bitsize);
                CY_CRYPTO_VU_ALLOC_MEM (base, p_gy, bitsize);
                CY_CRYPTO_VU_ALLOC_MEM (base, p_qx, bitsize);
                CY_CRYPTO_VU_ALLOC_MEM (base, p_qy, bitsize);

                mallocMask |= CY_CRYPTO_VU_REG_BIT(dividend) |
                              CY_CRYPTO_VU_REG_BIT(p_o)  |
                              CY_CRYPTO_VU_REG_BIT(p_u1) | CY_CRYPTO_VU_REG_BIT(p_u2) |
                              CY_CRYPTO_VU_REG_BIT(p_gx) | CY_CRYPTO_VU_REG_BIT(p_gy) |
                              CY_CRYPTO_VU_REG_BIT(p_qx) | CY_CRYPTO_VU_REG_BIT(p_qy);

                /* load message hash, truncate it if needed */
                CY_CRYPTO_VU_SET_TO_ZERO(base, p_u1);
                if ((hashlen * 8u) > bitsize)
                {
                    Cy_Crypto_Core_Vu_SetMemValue (base, p_u1, (uint8_t *)(&hash[hashlen - CY_CRYPTO_BYTE_SIZE_OF_BITS(bitsize)]), bitsize);
                }
                else
                {
                    Cy_Crypto_Core_Vu_SetMemValue (base, p_u1, (uint8_t *)hash, hashlen * 8U);
                }

                /* w = s^-1 mod n */
                CY_CRYPTO_VU_SET_TO_ONE(base, dividend);
                Cy_Crypto_Core_EC_DivMod(base, p_s, dividend, p_s, bitsize);

                /* u1 = e*w mod n */
                Cy_Crypto_Core_EC_MulMod(base, p_u1, p_u1, p_s, bitsize);
                /* u2 = r*w mod n */
                Cy_Crypto_Core_EC_MulMod(base, p_u2, p_r, p_s, bitsize);

                /* Initialize point multiplication */
                Cy_Crypto_Core_EC_NistP_SetRedAlg(eccDp->algo);

                /* load prime, order and Barrett coefficient */
                Cy_Crypto_Core_Vu_SetMemValue (base, VR_P, (uint8_t *)eccDp->prime, bitsize);
                Cy_Crypto_Core_Vu_SetMemValue (base, p_o, (uint8_t *)eccDp->order, bitsize);
                Cy_Crypto_Core_Vu_SetMemValue (base, VR_BARRETT, (uint8_t *)eccDp->barrett_p, bitsize + 1U);

                /* load base Point G */
                Cy_Crypto_Core_Vu_SetMemValue (base, p_gx, (uint8_t *)eccDp->Gx, bitsize);
                Cy_Crypto_Core_Vu_SetMemValue (base, p_gy, (uint8_t *)eccDp->Gy, bitsize);

                /* load public key Qa */
                Cy_Crypto_Core_Vu_SetMemValue (base, p_qx, (uint8_t *)key->pubkey.x, bitsize);
                Cy_Crypto_Core_Vu_SetMemValue (base, p_qy, (uint8_t *)key->pubkey.y, bitsize);

                /* u1 * G */
                Cy_Crypto_Core_EC_NistP_PointMul(base, p_gx, p_gy, p_u1, p_o, bitsize);

                /* reload order since p_o is modified by Crypto_EC_JacobianEcScalarMul_coZ */
                Cy_Crypto_Core_Vu_SetMemValue (base, p_o, (uint8_t *)eccDp->order, bitsize);

                /* u2 * Qa */
                Cy_Crypto_Core_EC_NistP_PointMul(base, p_qx, p_qy, p_u2, p_o, bitsize);

                /* P = u1 * G + u2 * Qa. Only Px is needed */
                Cy_Crypto_Core_EC_SubMod(base, dividend, p_qy, p_gy);           /* (y2-y1) */
                Cy_Crypto_Core_EC_SubMod(base, p_s, p_qx, p_gx);                /* (x2-x1) */
                Cy_Crypto_Core_EC_DivMod(base, p_s, dividend, p_s, bitsize);    /* s = (y2-y1)/(x2-x1) */

                Cy_Crypto_Core_EC_SquareMod (base, p_s, p_s, bitsize);         /* s^2 */
                Cy_Crypto_Core_EC_SubMod    (base, p_s, p_s, p_gx);            /* s^2 - x1 */
                Cy_Crypto_Core_EC_SubMod    (base, p_s, p_s, p_qx);            /* s^2 - x1 - x2 which is Px mod n */

                if (Cy_Crypto_Core_Vu_IsRegEqual(base, p_s, p_r))
                {
                    *stat = 1u;
                }
                else
                {
                    *stat = 0u;
                }

            }

            CY_CRYPTO_VU_FREE_MEM(base, mallocMask);
        }
    }

    return (tmpResult);
}


/* [] END OF FILE */
