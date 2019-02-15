/***************************************************************************//**
* \file cy_crypto_core_ecc.h
* \version 2.20
*
* \brief
*  This file provides constant and parameters for the API for the ECC
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


#if !defined(CY_CRYPTO_CORE_ECC_H)
#define CY_CRYPTO_CORE_ECC_H

#include "cy_crypto_common.h"
#include "cy_syslib.h"

#if defined(CY_IP_MXCRYPTO)

#if (CPUSS_CRYPTO_VU == 1)

/**
* \addtogroup group_crypto_lld_asymmetric_enums
* \{
*/

/** List of supported elliptic curve IDs */
typedef enum {
    CY_CRYPTO_ECC_ECP_NONE = 0,
    CY_CRYPTO_ECC_ECP_SECP192R1,
    CY_CRYPTO_ECC_ECP_SECP224R1,
    CY_CRYPTO_ECC_ECP_SECP256R1,
    CY_CRYPTO_ECC_ECP_SECP384R1,
    CY_CRYPTO_ECC_ECP_SECP521R1,
    /* Count of supported curves */
    CY_CRYPTO_ECC_ECP_CURVES_CNT
} cy_en_crypto_ecc_curve_id_t;

/** \} group_crypto_lld_asymmetric_enums */

typedef enum cy_en_red_mul_algs {
    CY_CRYPTO_NIST_P_CURVE_SPECIFIC_RED_ALG = 0,
    CY_CRYPTO_NIST_P_SHIFT_MUL_RED_ALG,
    CY_CRYPTO_NIST_P_BARRETT_RED_ALG
} cy_en_crypto_ecc_red_mul_algs_t;

/** Structure defines a NIST GF(p) curve */
typedef struct {
    /**  The curve ID */
    cy_en_crypto_ecc_curve_id_t id;

    /** The size of the curve in bits */
    uint32_t size;

    /** name of curve */
    const char_t *name;

    /** ECC calculation default algorithm */
    cy_en_crypto_ecc_red_mul_algs_t algo;

    /** The prime that defines the field the curve is in (encoded in hex) */
    const uint8_t *prime;

    /** Barrett coefficient for reduction modulo ECC prime (hex) */
    const uint8_t *barrett_p;

    /** The order of the curve (hex) */
    const uint8_t *order;

    /** Barrett coefficient for reduction modulo ECC order (hex) */
    const uint8_t *barrett_o;

    /** The x co-ordinate of the base point on the curve (hex) */
    const uint8_t *Gx;

    /** The y co-ordinate of the base point on the curve (hex) */
    const uint8_t *Gy;
} cy_stc_crypto_ecc_dp_type;


/** A point on a ECC curve */
typedef struct {
    /** The x co-ordinate */
    void *x;
    /** The y co-ordinate */
    void *y;
} cy_stc_crypto_ecc_point;

cy_stc_crypto_ecc_dp_type *Cy_Crypto_Core_ECC_GetCurveParams(cy_en_crypto_ecc_curve_id_t curveId);


typedef enum cy_en_crypto_ecc_key_type {
   PK_PUBLIC     = 0u,
   PK_PRIVATE    = 1u
} cy_en_crypto_ecc_key_type_t;

/** An ECC key */
typedef struct {
    /** Type of key, PK_PRIVATE or PK_PUBLIC */
    cy_en_crypto_ecc_key_type_t type;

    /** pointer to domain parameters */

    cy_en_crypto_ecc_curve_id_t curveID;

    /** The public key */
    cy_stc_crypto_ecc_point pubkey;

    /** The private key */
    void *k;
} cy_stc_crypto_ecc_key;


/**
* \addtogroup group_crypto_lld_asymmetric_functions
* \{
*/

/** Pointer to a random number supplier function */
typedef int (*cy_func_get_random_data_t)(void *rndInfo, uint8_t *rndData, size_t rndSize);

cy_en_crypto_status_t Cy_Crypto_Core_ECC_MakeKeyPair(CRYPTO_Type *base,
                                   cy_en_crypto_ecc_curve_id_t curveID,
                                   cy_stc_crypto_ecc_key *key,
                                   cy_func_get_random_data_t GetRandomDataFunc, void *randomDataInfo);
cy_en_crypto_status_t Cy_Crypto_Core_ECC_SignHash(CRYPTO_Type *base,
                                    const uint8_t *hash,
                                    uint32_t hashlen,
                                    uint8_t *sig,
                                    const cy_stc_crypto_ecc_key *key,
                                    const uint8_t *messageKey);
cy_en_crypto_status_t Cy_Crypto_Core_ECC_VerifyHash(CRYPTO_Type *base,
                                    const uint8_t *sig,
                                    const uint8_t *hash,
                                    uint32_t hashlen,
                                    uint8_t *stat,
                                    const cy_stc_crypto_ecc_key *key);
cy_en_crypto_status_t Cy_Crypto_Core_ECC_MakePrivateKey(CRYPTO_Type *base,
        cy_en_crypto_ecc_curve_id_t curveID, uint8_t *key,
        cy_func_get_random_data_t GetRandomDataFunc, void *randomDataInfo);
cy_en_crypto_status_t Cy_Crypto_Core_ECC_MakePublicKey(CRYPTO_Type *base,
        cy_en_crypto_ecc_curve_id_t curveID,
        const uint8_t *privateKey,
        cy_stc_crypto_ecc_key *publicKey);

/** \} group_crypto_lld_asymmetric_functions */

/** Calculates the actual size in bytes of the bits value */
#define CY_CRYPTO_BYTE_SIZE_OF_BITS(x)     (uint32_t)(((x) + 7u) >> 3u)

/* Sizes for NIST P-curves */
#define CY_CRYPTO_ECC_P192_SIZE            (192u)      /* 2^192 - 2^64 - 1 */
#define CY_CRYPTO_ECC_P192_BYTE_SIZE       CY_CRYPTO_BYTE_SIZE_OF_BITS(CY_CRYPTO_ECC_P192_SIZE)

#define CY_CRYPTO_ECC_P224_SIZE            (224u)      /* 2^224 - 2^96  + 1 */
#define CY_CRYPTO_ECC_P224_BYTE_SIZE       CY_CRYPTO_BYTE_SIZE_OF_BITS(CY_CRYPTO_ECC_P224_SIZE)

#define CY_CRYPTO_ECC_P256_SIZE            (256u)      /* 2^256 - 2^224 + 2^192 + 2^96 - 1 */
#define CY_CRYPTO_ECC_P256_BYTE_SIZE       CY_CRYPTO_BYTE_SIZE_OF_BITS(CY_CRYPTO_ECC_P256_SIZE)

#define CY_CRYPTO_ECC_P384_SIZE            (384u)      /* 2^384 - 2^128 - 2^96 + 2^32 - 1 */
#define CY_CRYPTO_ECC_P384_BYTE_SIZE       CY_CRYPTO_BYTE_SIZE_OF_BITS(CY_CRYPTO_ECC_P384_SIZE)

#define CY_CRYPTO_ECC_P521_SIZE            (521u)      /* 2^521 - 1 */
#define CY_CRYPTO_ECC_P521_BYTE_SIZE       CY_CRYPTO_BYTE_SIZE_OF_BITS(CY_CRYPTO_ECC_P521_SIZE)

#define CY_CRYPTO_ECC_MAX_SIZE             (CY_CRYPTO_ECC_P521_SIZE)
#define CY_CRYPTO_ECC_MAX_BYTE_SIZE        (CY_CRYPTO_ECC_P521_BYTE_SIZE)

/* "Global" vector unit registers. */
#define VR_D                               10u
#define VR_S_X                             11u
#define VR_S_Y                             12u
#define VR_BARRETT                         13u
#define VR_P                               14u         /* polynomial */


#endif /* #if (CPUSS_CRYPTO_VU == 1) */

#endif /* #if defined(CY_IP_MXCRYPTO) */

#endif /* CY_CRYPTO_CORE_ECC_H  */


/* [] END OF FILE */
