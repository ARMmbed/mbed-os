/***************************************************************************//**
* \file cy_crypto_core_ecc_key_gen.c
* \version 2.20
*
* \brief
*  This file provides constant and parameters for the API for the ECC key
*  generations in the Crypto driver.
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
#include "cy_crypto_core_vu.h"
#include "cy_crypto_core_trng.h"

#define CY_ECC_CONFIG_TR_GARO_CTL      0x6C740B8DuL
#define CY_ECC_CONFIG_TR_FIRO_CTL      0x52D246E1uL


/*******************************************************************************
* Function Name: Cy_Crypto_Core_ECC_MakeKeyPair
****************************************************************************//**
*
* Make a new ECC key pair.
*
* \param base
* The pointer to a Crypto instance.
*
* \param curveID
* See \ref cy_en_crypto_ecc_curve_id_t.
*
* \param key
* [out] Destination of the newly created key. See \ref cy_stc_crypto_ecc_key.
*
* \param GetRandomDataFunc
* See \ref cy_func_get_random_data_t.
*
* \param randomDataInfo
*
* \return status code. See \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_ECC_MakeKeyPair(CRYPTO_Type *base,
        cy_en_crypto_ecc_curve_id_t curveID,
        cy_stc_crypto_ecc_key *key,
        cy_func_get_random_data_t GetRandomDataFunc,
        void *randomDataInfo)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_NOT_SUPPORTED;

    cy_stc_crypto_ecc_dp_type *eccDp = Cy_Crypto_Core_ECC_GetCurveParams(curveID);

    if ((eccDp != NULL) && (key != NULL))
    {
        tmpResult = CY_CRYPTO_SUCCESS;

        uint32_t bitsize = eccDp->size;

        /* used VU registers. Same values as in crypto_NIST_P.c */
        uint32_t p_temp = 8u;     /* temporal values */
        uint32_t p_order = 9u;    /* order of the curve */
        uint32_t p_d = 10u;       /* private key */
        uint32_t p_x = 11u;       /* x coordinate */
        uint32_t p_y = 12u;       /* y coordinate */

        CY_CRYPTO_VU_ALLOC_MEM(base, VR_P, bitsize);
        CY_CRYPTO_VU_ALLOC_MEM(base, p_order, bitsize);
        CY_CRYPTO_VU_ALLOC_MEM(base, VR_BARRETT, bitsize + 1U);
        CY_CRYPTO_VU_ALLOC_MEM(base, p_x, bitsize);
        CY_CRYPTO_VU_ALLOC_MEM(base, p_y, bitsize);

        /***************************************************************
        *               Apply domain parameters
        ***************************************************************/
        /* load prime and order defining the curve as well as the Barrett coefficient. */

        /* P and BARRETT_U are "globally" defined in cy_crypto_core_ecc.h  */
        Cy_Crypto_Core_Vu_SetMemValue (base, VR_P, eccDp->prime, bitsize);
        Cy_Crypto_Core_Vu_SetMemValue (base, p_order, eccDp->order, bitsize);
        Cy_Crypto_Core_Vu_SetMemValue (base, VR_BARRETT, eccDp->barrett_p, bitsize + 1U);

        /* Base Point, G = (p_x, p_y) */
        Cy_Crypto_Core_Vu_SetMemValue (base, p_x, eccDp->Gx, bitsize);
        Cy_Crypto_Core_Vu_SetMemValue (base, p_y, eccDp->Gy, bitsize);

        Cy_Crypto_Core_EC_NistP_SetMode(bitsize);
        Cy_Crypto_Core_EC_NistP_SetRedAlg(eccDp->algo);

        /***************************************************************
        *               generate random string
        ***************************************************************/
        if (GetRandomDataFunc != NULL)
        {
            (void)GetRandomDataFunc( randomDataInfo, (uint8_t*)key->k, ((bitsize + 7U) >> 3U) );
        }
        else
        {
            uint32_t i = 0U;
            int32_t randomsize = (int32_t)bitsize;
            cy_en_crypto_status_t status = CY_CRYPTO_SUCCESS;

            while ((randomsize > 0) && (CY_CRYPTO_SUCCESS == status))
            {
                uint32_t randombits = (uint32_t)CY_CRYPTO_MIN(randomsize, 32);

                status = Cy_Crypto_Core_Trng(base, CY_ECC_CONFIG_TR_GARO_CTL, CY_ECC_CONFIG_TR_FIRO_CTL,
                                                  randombits, &((uint32_t *)key->k)[i]);
                randomsize -= 32;
                i++;

                if (CY_CRYPTO_SUCCESS != status)
                {
                    tmpResult = CY_CRYPTO_HW_ERROR;
                }
            }
        }

        if (CY_CRYPTO_SUCCESS == tmpResult)
        {
            /***************************************************************
            *               Load random data into VU
            ***************************************************************/
            CY_CRYPTO_VU_ALLOC_MEM(base, p_d, bitsize);
            Cy_Crypto_Core_Vu_SetMemValue(base, p_d, (uint8_t *)key->k, bitsize);

            /* check that the key is smaller than the order of base point */
            CY_CRYPTO_VU_CMP_SUB (base, p_d, p_order);                    /*  C = (a >= b) */
            uint32_t status = Cy_Crypto_Core_Vu_StatusRead(base);

            if (0u != (status &  CY_CRYPTO_VU_STATUS_CARRY_BIT))
            {
                /*  random data >= order, needs reduction */

                CY_CRYPTO_VU_PUSH_REG(base);

                /*  load values needed for reduction modulo curve order */
                CY_CRYPTO_VU_ALLOC_MEM(base, VR_P, bitsize);
                Cy_Crypto_Core_Vu_SetMemValue (base, VR_P, eccDp->order, bitsize);

                CY_CRYPTO_VU_ALLOC_MEM(base, VR_BARRETT, bitsize + 1u);
                Cy_Crypto_Core_Vu_SetMemValue (base, VR_BARRETT, eccDp->barrett_o, bitsize + 1u);

                CY_CRYPTO_VU_ALLOC_MEM(base, p_temp, bitsize);
                CY_CRYPTO_VU_MOV(base, p_temp, p_d);

                /*  z = x % mod */
                Cy_Crypto_Core_EC_Bar_MulRed(base, p_d, p_temp, bitsize);

                CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(p_temp) | CY_CRYPTO_VU_REG_BIT(VR_P) | CY_CRYPTO_VU_REG_BIT(VR_BARRETT));

                Cy_Crypto_Core_Vu_GetMemValue(base, (uint8_t *)key->k, p_d, bitsize);

                /*  restore previous prime and Barrett values */
                CY_CRYPTO_VU_POP_REG(base);
            }

            /*
             * Make the public key
             * EC scalar multiplication - X,Y-only co-Z arithmetic
             */
            Cy_Crypto_Core_EC_NistP_PointMul(base, p_x, p_y, p_d, p_order, bitsize);

            Cy_Crypto_Core_Vu_GetMemValue(base, (uint8_t *)key->pubkey.x, p_x, bitsize);
            Cy_Crypto_Core_Vu_GetMemValue(base, (uint8_t *)key->pubkey.y, p_y, bitsize);

            key->type = PK_PRIVATE;
            key->curveID = curveID;

            tmpResult = CY_CRYPTO_SUCCESS;
        }

        CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(VR_P) | CY_CRYPTO_VU_REG_BIT(VR_BARRETT) |
                                    CY_CRYPTO_VU_REG_BIT(p_x)  | CY_CRYPTO_VU_REG_BIT(p_y) |
                                    CY_CRYPTO_VU_REG_BIT(p_order) | CY_CRYPTO_VU_REG_BIT(p_d));
    }

    return (tmpResult);
}


/*******************************************************************************
* Function Name: Cy_Crypto_Core_ECC_MakePrivateKey
****************************************************************************//**
*
* Make a new ECC private key
*
* \param base
* The pointer to a Crypto instance.
*
* \param curveID
* See \ref cy_en_crypto_ecc_curve_id_t.
*
* \param key
* [out] Destination of the newly created key.
*
* \param GetRandomDataFunc
* See \ref cy_func_get_random_data_t.
*
* \param randomDataInfo
*
* \return status code. See \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_ECC_MakePrivateKey(CRYPTO_Type *base,
        cy_en_crypto_ecc_curve_id_t curveID,
        uint8_t *key,
        cy_func_get_random_data_t GetRandomDataFunc, void *randomDataInfo)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_BAD_PARAMS;

    const cy_stc_crypto_ecc_dp_type *eccDp = Cy_Crypto_Core_ECC_GetCurveParams(curveID);

    if ((eccDp != NULL) && (key != NULL))
    {
        tmpResult = CY_CRYPTO_SUCCESS;

        uint32_t bitsize = eccDp->size;

        uint32_t p_temp = 8u;     /* temporal values */
        uint32_t p_d = 10u;       /* private key */

        /* generate random string */
        if (GetRandomDataFunc != NULL)
        {
            (void)GetRandomDataFunc( randomDataInfo, key, ((bitsize + 7U) >> 3U) );
        }
        else
        {
            uint32_t i = 0U;
            int32_t randomsize = (int32_t)bitsize;
            cy_en_crypto_status_t status = CY_CRYPTO_SUCCESS;

            while ((randomsize > 0) && (CY_CRYPTO_SUCCESS == status))
            {
                uint32_t randombits = (uint32_t)CY_CRYPTO_MIN(randomsize, 32);

                status = Cy_Crypto_Core_Trng(base, CY_ECC_CONFIG_TR_GARO_CTL, CY_ECC_CONFIG_TR_FIRO_CTL,
                                                  randombits, &((uint32_t *)key)[i]);
                randomsize -= 32;
                i++;

                if (CY_CRYPTO_SUCCESS != status)
                {
                    tmpResult = CY_CRYPTO_HW_ERROR;
                }
            }
        }

        if (CY_CRYPTO_SUCCESS == tmpResult)
        {
            /* load prime and order defining the curve as well as the barrett coefficient. */
            /* P and BARRETT_U are "globally" defined in cy_crypto_core_ecc.h */
            CY_CRYPTO_VU_ALLOC_MEM(base, VR_P, bitsize);
            CY_CRYPTO_VU_ALLOC_MEM(base, VR_BARRETT, bitsize + 1u);
            Cy_Crypto_Core_Vu_SetMemValue (base, VR_P, eccDp->order, bitsize);
            Cy_Crypto_Core_Vu_SetMemValue (base, VR_BARRETT, eccDp->barrett_o, bitsize + 1u);

            /* Load random data into VU */
            CY_CRYPTO_VU_ALLOC_MEM(base, p_d, bitsize);
            Cy_Crypto_Core_Vu_SetMemValue(base, p_d, (uint8_t *)key, bitsize);

            CY_CRYPTO_VU_ALLOC_MEM(base, p_temp, bitsize);
            CY_CRYPTO_VU_MOV(base, p_temp, p_d);

            /* check that the key is smaller than the order of base point */
            CY_CRYPTO_VU_CMP_SUB (base, p_d, VR_P);                    /* C = (a >= b) */
            uint32_t status = Cy_Crypto_Core_Vu_StatusRead(base);

            if (0u != (status & CY_CRYPTO_VU_STATUS_CARRY_BIT))
            {
                /* private key (random data) >= order, needs reduction */

                /* use Barrett reduction algorithm for operations modulo n (order of the base point) */
                Cy_Crypto_Core_EC_NistP_SetRedAlg(eccDp->algo);
                Cy_Crypto_Core_EC_NistP_SetMode(bitsize);

                /* z = x % mod */
                Cy_Crypto_Core_EC_Bar_MulRed(base, p_d, p_temp, bitsize);

                Cy_Crypto_Core_Vu_GetMemValue(base, (uint8_t *)key, p_d, bitsize);
            }

            CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(VR_P) | CY_CRYPTO_VU_REG_BIT(VR_BARRETT) |
                                        CY_CRYPTO_VU_REG_BIT(p_d)  | CY_CRYPTO_VU_REG_BIT(p_temp));

            tmpResult = CY_CRYPTO_SUCCESS;
        }
    }

    return (tmpResult);
}


/*******************************************************************************
* Function Name: Cy_Crypto_Core_ECC_MakePublicKey
****************************************************************************//**
*
* Make a new ECC public key
*
* \param base
* The pointer to a Crypto instance.
*
* \param curveID
* See \ref cy_en_crypto_ecc_curve_id_t.
*
* \param privateKey
* [out] Destination of the newly created key.
*
* \param publicKey
* See \ref cy_stc_crypto_ecc_key.
*
* \return status code. See \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_ECC_MakePublicKey(CRYPTO_Type *base,
        cy_en_crypto_ecc_curve_id_t curveID,
        const uint8_t *privateKey,
        cy_stc_crypto_ecc_key *publicKey)
{
    cy_en_crypto_status_t tmpResult = CY_CRYPTO_BAD_PARAMS;

    cy_stc_crypto_ecc_dp_type *eccDp = Cy_Crypto_Core_ECC_GetCurveParams(curveID);

    if ((eccDp != NULL) && (privateKey != NULL) && (publicKey != NULL))
    {
        uint32_t bitsize = eccDp->size;

        uint32_t p_order = 9u;    /* order of the curve */
        uint32_t p_d = 10u;       /* private key */
        uint32_t p_x = 11u;       /* x coordinate */
        uint32_t p_y = 12u;       /* y coordinate */

        /* make the public key
        * EC scalar multiplication - X,Y-only co-Z arithmetic
        */
        CY_CRYPTO_VU_ALLOC_MEM(base, VR_P, bitsize);
        CY_CRYPTO_VU_ALLOC_MEM(base, p_order, bitsize);
        CY_CRYPTO_VU_ALLOC_MEM(base, VR_BARRETT, bitsize + 1u);
        CY_CRYPTO_VU_ALLOC_MEM(base, p_x, bitsize);
        CY_CRYPTO_VU_ALLOC_MEM(base, p_y, bitsize);

        /* Apply domain parameters */

        /* load prime and order defining the curve as well as the barrett coefficient. */

        /*  P and BARRETT_U are "globally" defined in cy_crypto_core_ecc.h */
        Cy_Crypto_Core_Vu_SetMemValue (base, VR_P, eccDp->prime, bitsize);
        Cy_Crypto_Core_Vu_SetMemValue (base, p_order, eccDp->order, bitsize);
        Cy_Crypto_Core_Vu_SetMemValue (base, VR_BARRETT, eccDp->barrett_p, bitsize + 1u);

        /*Base Point, G = (p_x, p_y) */
        Cy_Crypto_Core_Vu_SetMemValue (base, p_x, eccDp->Gx, bitsize);
        Cy_Crypto_Core_Vu_SetMemValue (base, p_y, eccDp->Gy, bitsize);

        Cy_Crypto_Core_EC_NistP_SetMode(bitsize);
        Cy_Crypto_Core_EC_NistP_SetRedAlg(eccDp->algo);

        /* Load random data into VU */
        CY_CRYPTO_VU_ALLOC_MEM(base, p_d, bitsize);
        Cy_Crypto_Core_Vu_SetMemValue(base, p_d, (uint8_t *)privateKey, bitsize);

        Cy_Crypto_Core_EC_NistP_PointMul(base, p_x, p_y, p_d, p_order, bitsize);

        Cy_Crypto_Core_Vu_GetMemValue(base, (uint8_t *)publicKey->pubkey.x, p_x, bitsize);
        Cy_Crypto_Core_Vu_GetMemValue(base, (uint8_t *)publicKey->pubkey.y, p_y, bitsize);

        publicKey->type = PK_PUBLIC;
        publicKey->curveID   = curveID;

        CY_CRYPTO_VU_FREE_MEM(base, CY_CRYPTO_VU_REG_BIT(VR_P) | CY_CRYPTO_VU_REG_BIT(p_order) | CY_CRYPTO_VU_REG_BIT(VR_BARRETT) |
                                    CY_CRYPTO_VU_REG_BIT(p_x) | CY_CRYPTO_VU_REG_BIT(p_y) | CY_CRYPTO_VU_REG_BIT(p_d));

        tmpResult = CY_CRYPTO_SUCCESS;
    }

    return (tmpResult);
}


/* [] END OF FILE */
