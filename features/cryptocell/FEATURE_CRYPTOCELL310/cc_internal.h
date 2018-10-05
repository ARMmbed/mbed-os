/*
 *  cc_internal.h
 *
 *  Internal utility functions and definitions,
 *  used for converting mbedtls types to CC types, and vice versa
 *
 *  Copyright (C) 2018, Arm Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef __CC_INTERNAL_H__
#define __CC_INTERNAL_H__
#include "crys_ecpki_types.h"
#include "crys_ec_mont_api.h"
#include "mbedtls/ecp.h"
#include <stddef.h>
#include <stdint.h>

#define CURVE_25519_KEY_SIZE    32

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_KEY_SIZE_IN_BYTES ( ( CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS ) * SASI_32BIT_WORD_SIZE)

/* ECC utility functions and structures*/
typedef struct cc_ecc_ws_keygen_params{
    CRYS_ECPKI_UserPublKey_t pubKey;
    CRYS_ECPKI_UserPrivKey_t privKey;
    CRYS_ECPKI_KG_TempData_t kgTempData;
} cc_ecc_ws_keygen_params_t;

typedef struct cc_ecc_ws_comp_shared_params{
    CRYS_ECPKI_UserPublKey_t pubKey;
    CRYS_ECPKI_UserPrivKey_t privKey;
    CRYS_ECDH_TempData_t     ecdhTempData;
} cc_ecc_ws_comp_shared_params_t;

typedef struct cc_ecc_ws_verify_params{
    CRYS_ECPKI_UserPublKey_t pubKey;
    CRYS_ECDSA_VerifyUserContext_t verifyContext;
} cc_ecc_ws_verify_params_t;

typedef struct cc_ecc_ws_sign_params{
    CRYS_ECPKI_UserPrivKey_t privKey;
    CRYS_ECDSA_SignUserContext_t signContext;
} cc_ecc_ws_sign_params_t;

typedef struct cc_ecc_25519_keygen_params{
    uint8_t pubKey[CURVE_25519_KEY_SIZE];
    uint8_t privKey[CURVE_25519_KEY_SIZE];
    CRYS_ECMONT_TempBuff_t kgTempData;
} cc_ecc_25519_keygen_params_t;

typedef cc_ecc_25519_keygen_params_t cc_ecc_25519_comp_shared_params_t;

/**
 * \brief      This function converts mbedtls type mbedtls_ecp_group_id
 *             to Cryptocell type CRYS_ECPKI_DomainID_t
 *
 * \param grp_id           The mbedtls mbedtls_ecp_group_id to convert
 *
 * \return     \c The corresponding CRYS_ECPKI_DomainID_t.
 *                CRYS_ECPKI_DomainID_OffMode if not recognized.
 */
CRYS_ECPKI_DomainID_t convert_mbedtls_grp_id_to_crys_domain_id( mbedtls_ecp_group_id grp_id );

/* f_rng conversion from mbedtls type to cc type*/
typedef struct
{
    int  (*f_rng)( void* ctx, unsigned char* output, size_t outSizeBytes );
    void* ctx;

}mbedtls_rand_func_container;

/**
 * \brief      This function converts mbedtls f_rng type to
 *             Cryptocell f_rng type(SaSiRndGenerateVectWorkFunc_t)
 *
 *             Note: The Mbed TLS type f_rng signature is:
 *             int  (*f_rng)( void* ctx, unsigned char* output, size_t outSizeBytes );
 *             while CC f_rng signature is:
 *             uint32_t (*SaSiRndGenerateVectWorkFunc_t)(
 *                        void  *rndState_ptr,
 *                        uint16_t outSizeBytes,
 *                        uint8_t          *out_ptr)
 *
 *             so the Mbed TLS f_rng can't  be sent as is to the CC API.
 *
 *             In addition, this function manipulates the different random data,
 *             to adjust between the way Cryptocell reads the random data. This is done for
 *             different standard tests to pass.
 *
 *
 * \param mbedtls_rand      The mbedtls rnd context pointer
 * \param outSizeBytes      The size of the output buffer
 * \param out_ptr           Pointer to the output buffer
 *
 * \return     \c The corresponding CRYS_ECPKI_DomainID_t.
 *                CRYS_ECPKI_DomainID_OffMode if not recognized.
 */

uint32_t convert_mbedtls_to_cc_rand( void* mbedtls_rand, uint16_t outSizeBytes, uint8_t* out_ptr );

/**
 * \brief      This function convertsCryptocell error
 *             Mbed TLS related error.
 *
 *
 * \return     \c The corresponding Mbed TLS error,
 *                MBEDTLS_ERR_ECP_HW_ACCEL_FAILED as default, if none found
 */
int convert_CrysError_to_mbedtls_err( CRYSError_t Crys_err );

#ifdef __cplusplus
}
#endif

#endif /* __CC_INTERNAL_H__ */
