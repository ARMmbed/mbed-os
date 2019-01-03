/*
 *  cc_internal.c
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

#include "cc_internal.h"
#include "crys_ecpki_error.h"
#include "crys_ec_mont_edw_error.h"
#include "mbedtls/platform.h"

CRYS_ECPKI_DomainID_t convert_mbedtls_grp_id_to_crys_domain_id( mbedtls_ecp_group_id grp_id )
{
    switch( grp_id )
    {
    case MBEDTLS_ECP_DP_SECP192K1:
        return ( CRYS_ECPKI_DomainID_secp192k1 );
    case MBEDTLS_ECP_DP_SECP192R1:
        return ( CRYS_ECPKI_DomainID_secp192r1 );
    case MBEDTLS_ECP_DP_SECP224K1:
        return ( CRYS_ECPKI_DomainID_secp224k1 );
    case MBEDTLS_ECP_DP_SECP224R1:
        return ( CRYS_ECPKI_DomainID_secp224r1 );
    case MBEDTLS_ECP_DP_SECP256K1:
        return ( CRYS_ECPKI_DomainID_secp256k1 );
    case MBEDTLS_ECP_DP_SECP256R1:
        return ( CRYS_ECPKI_DomainID_secp256r1 );
    case MBEDTLS_ECP_DP_SECP384R1:
        return ( CRYS_ECPKI_DomainID_secp384r1 );
    case MBEDTLS_ECP_DP_SECP521R1:
        return ( CRYS_ECPKI_DomainID_secp521r1 );
    default:
        return ( CRYS_ECPKI_DomainID_OffMode );
    }

}

uint32_t convert_mbedtls_to_cc_rand( void* mbedtls_rnd_ctx, uint16_t outSizeBytes, uint8_t* out_ptr )
{
    uint16_t i = 0;
    uint8_t temp = 0;
    mbedtls_rand_func_container* mbedtls_rand = (mbedtls_rand_func_container*)mbedtls_rnd_ctx;

    if( mbedtls_rand->f_rng( mbedtls_rand->ctx, out_ptr, outSizeBytes ) != 0 )
        return ( MBEDTLS_ERR_ECP_RANDOM_FAILED );

    /*
     * CC requires the random data as LE, so reversing the data
     * (although this is random, but test vectors are in specific Endianess)
     */
    while ( i < ( outSizeBytes / 2 ) )
    {
        temp = out_ptr[outSizeBytes - 1 - i];
        out_ptr[outSizeBytes - 1 - i] = out_ptr[i];
        out_ptr[i] = temp;
        ++i;
    }
    /*
     * CC increases the random data by one, to put the vector in the proper range (1 to  n),
     * The RFC tests supply a data buffer within range, and in order to generate the proper ephemeral key,
     * need to decrease one from this data, before CC increases the data, so the output will be as expected
     */
    i = 0;
    while( out_ptr[i] == 0 )
    {
        ++i;
    }
    while( i > 0 )
    {
        --out_ptr[i];
        --i;
    }
    --out_ptr[0];
    return ( 0 );
}

int convert_CrysError_to_mbedtls_err( CRYSError_t Crys_err )
{
    switch( Crys_err )
    {
    case CRYS_OK:
        return ( 0 );

    case CRYS_ECDH_SVDP_DH_INVALID_USER_PRIV_KEY_PTR_ERROR:
    case CRYS_ECDH_SVDP_DH_USER_PRIV_KEY_VALID_TAG_ERROR:
    case CRYS_ECDH_SVDP_DH_INVALID_PARTNER_PUBL_KEY_PTR_ERROR:
    case CRYS_ECDH_SVDP_DH_PARTNER_PUBL_KEY_VALID_TAG_ERROR:
    case CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_PTR_ERROR:
    case CRYS_ECDH_SVDP_DH_INVALID_TEMP_DATA_PTR_ERROR:
    case CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_SIZE_PTR_ERROR:
    case CRYS_ECDH_SVDP_DH_NOT_CONCENT_PUBL_AND_PRIV_DOMAIN_ID_ERROR:
    case CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_SIZE_ERROR:
    case CRYS_ECMONT_INVALID_INPUT_POINTER_ERROR:
    case CRYS_ECMONT_INVALID_INPUT_SIZE_ERROR:
    case CRYS_ECMONT_INVALID_DOMAIN_ID_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_USER_CONTEXT_PTR_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_USER_PRIV_KEY_PTR_ERROR:
    case CRYS_ECDSA_SIGN_ILLEGAL_HASH_OP_MODE_ERROR:
    case CRYS_ECDSA_SIGN_USER_PRIV_KEY_VALIDATION_TAG_ERROR:
    case CRYS_ECDSA_SIGN_USER_CONTEXT_VALIDATION_TAG_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_PTR_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_MESSAGE_DATA_IN_SIZE_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_PTR_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_PTR_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_IS_EPHEMER_KEY_INTERNAL_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_EPHEMERAL_KEY_PTR_ERROR:
    case CRYS_ECDSA_VERIFY_INVALID_SIGNER_PUBL_KEY_PTR_ERROR:
    case CRYS_ECDSA_VERIFY_SIGNER_PUBL_KEY_VALIDATION_TAG_ERROR:
    case CRYS_ECDSA_VERIFY_INVALID_USER_CONTEXT_PTR_ERROR:
    case CRYS_ECDSA_VERIFY_INVALID_SIGNATURE_IN_PTR_ERROR:
    case CRYS_ECDSA_VERIFY_INVALID_SIGNATURE_SIZE_ERROR:
    case CRYS_ECPKI_INVALID_RND_CTX_PTR_ERROR:
    case CRYS_ECPKI_INVALID_RND_FUNC_PTR_ERROR:
    case CRYS_ECDSA_SIGN_INVALID_SIGNATURE_OUT_SIZE_ERROR:
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    case CRYS_ECDSA_VERIFY_INCONSISTENT_VERIFY_ERROR:
        return ( MBEDTLS_ERR_ECP_VERIFY_FAILED );

    case CRYS_ECMONT_IS_NOT_SUPPORTED:
    case CRYS_ECEDW_IS_NOT_SUPPORTED:
        return ( MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED );

    case CRYS_ECEDW_RND_GEN_VECTOR_FUNC_ERROR:
        return ( MBEDTLS_ERR_ECP_RANDOM_FAILED );

    case CRYS_ECPKI_GEN_KEY_INVALID_PRIVATE_KEY_PTR_ERROR:
    case CRYS_ECPKI_EXPORT_PUBL_KEY_INVALID_PUBL_KEY_DATA_ERROR:
    case CRYS_ECPKI_BUILD_KEY_INVALID_PRIV_KEY_DATA_ERROR:
    case CRYS_ECPKI_BUILD_KEY_INVALID_PRIV_KEY_SIZE_ERROR:
    case CRYS_ECPKI_BUILD_KEY_INVALID_PUBL_KEY_SIZE_ERROR:
        return ( MBEDTLS_ERR_ECP_INVALID_KEY );

    default:
        return ( MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED );
    }


}
