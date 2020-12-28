/*
 *  ecdsa_alt.c
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

#include "mbedtls/ecdsa.h"
#include <string.h>
#include "crys_ecpki_ecdsa.h"
#include "crys_ecpki_build.h"
#include "crys_common.h"
#include "crys_ecpki_kg.h"
#include "crys_ecpki_domain.h"
#include "crys_ec_edw_api.h"
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "cc_internal.h"

static CRYS_ECPKI_HASH_OpMode_t message_size_to_hash_mode( size_t blen )
{
    CRYS_ECPKI_HASH_OpMode_t hash_mode;
    switch( blen )
    {
    case CRYS_HASH_SHA1_DIGEST_SIZE_IN_WORDS*sizeof(uint32_t):
            hash_mode = CRYS_ECPKI_AFTER_HASH_SHA1_mode;
    break;
    case CRYS_HASH_SHA224_DIGEST_SIZE_IN_WORDS*sizeof(uint32_t):
            hash_mode = CRYS_ECPKI_AFTER_HASH_SHA224_mode;
    break;
    case CRYS_HASH_SHA256_DIGEST_SIZE_IN_WORDS*sizeof(uint32_t):
            hash_mode = CRYS_ECPKI_AFTER_HASH_SHA256_mode;
    break;
    case CRYS_HASH_SHA384_DIGEST_SIZE_IN_WORDS*sizeof(uint32_t):
            hash_mode = CRYS_ECPKI_AFTER_HASH_SHA384_mode;
    break;
    case CRYS_HASH_SHA512_DIGEST_SIZE_IN_WORDS*sizeof(uint32_t):
            hash_mode = CRYS_ECPKI_AFTER_HASH_SHA512_mode;
    break;
    default:
        hash_mode = CRYS_ECPKI_HASH_OpModeLast;
    }

    return hash_mode;
}

#if defined(MBEDTLS_ECDSA_SIGN_ALT)
int mbedtls_ecdsa_sign( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = 0;
    CRYSError_t CrysRet = CRYS_OK;
    void* pHeap = NULL;
    size_t heapSize = 0;
    uint8_t* pSignature = NULL;
    CRYS_ECPKI_HASH_OpMode_t hash_mode = message_size_to_hash_mode( blen );
    uint32_t signature_size =  ( ( grp->nbits + 7 ) / 8 ) *2;
    const uint32_t signature_size_for_heap = signature_size;
    mbedtls_rand_func_container cc_rand = { f_rng, p_rng };
    const CRYS_ECPKI_Domain_t*  pDomain =  CRYS_ECPKI_GetEcDomain ( convert_mbedtls_grp_id_to_crys_domain_id( grp->id ) );

#if SIZE_MAX > UINT_MAX
    if( blen > 0xFFFFFFFF )
    {
        return ( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
    }
#endif

    if ( pDomain != NULL )
    {
        uint8_t temp_buf[ MAX_KEY_SIZE_IN_BYTES ] = {0};
        cc_ecc_ws_sign_params_t* signParams = mbedtls_calloc( 1, sizeof(cc_ecc_ws_sign_params_t) );
        if ( signParams == NULL)
            return ( MBEDTLS_ERR_ECP_ALLOC_FAILED );
        pHeap = signParams;
        heapSize = sizeof(cc_ecc_ws_sign_params_t);

        pSignature = mbedtls_calloc( 1, signature_size_for_heap );
        if ( pSignature == NULL)
        {
            ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
            goto cleanup;
        }

        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, temp_buf, mbedtls_mpi_size( d ) ) );

        CrysRet = CRYS_ECPKI_BuildPrivKey( pDomain,
                                       temp_buf,
                                       mbedtls_mpi_size( d ),
                                       &signParams->privKey);
        if( CrysRet != CRYS_OK )
        {
            ret = convert_CrysError_to_mbedtls_err( CrysRet );
            mbedtls_platform_zeroize( temp_buf, sizeof(temp_buf) );
            goto cleanup;
        }

        CrysRet =  CRYS_ECDSA_Sign( &cc_rand,
                                convert_mbedtls_to_cc_rand,
                                &signParams->signContext,
                                &signParams->privKey,
                                hash_mode,
                                (uint8_t*)buf,
                                blen,
                                pSignature,
                                &signature_size );
        mbedtls_platform_zeroize( temp_buf, sizeof(temp_buf) );
        if( CrysRet != CRYS_OK )
        {
            ret = convert_CrysError_to_mbedtls_err( CrysRet );
            goto cleanup;
        }
    }
    else
    {
        ret =  MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
        goto cleanup;
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( r, pSignature, ( ( grp->nbits + 7 ) / 8 ) ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( s, pSignature + ( ( grp->nbits + 7 ) / 8 ), ( ( grp->nbits + 7 ) / 8 ) ) );


cleanup:

    if ( pHeap )
    {
        mbedtls_platform_zeroize( pHeap, heapSize );
        mbedtls_free( pHeap );
    }

    if( pSignature )
    {
        mbedtls_platform_zeroize( pSignature, signature_size_for_heap );
        mbedtls_free( pSignature );

    }

    return ( ret ) ;
}
#endif /* MBEDTLS_ECDSA_SIGN_ALT*/

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
//need to normalize the coordinates
int mbedtls_ecdsa_verify( mbedtls_ecp_group *grp,
                  const unsigned char *buf, size_t blen,
                  const mbedtls_ecp_point *Q, const mbedtls_mpi *r, const mbedtls_mpi *s)
{
    int ret = 0;
    CRYSError_t CrysRet = CRYS_OK;
    void* pHeap = NULL;
    size_t heapSize = 0;
    uint8_t * pSignature = NULL;
    CRYS_ECPKI_HASH_OpMode_t hash_mode = message_size_to_hash_mode( blen );
    size_t temp_size = 0;
    uint32_t signature_size = ( ( grp->nbits + 7 ) / 8 ) * 2;
    const CRYS_ECPKI_Domain_t*  pDomain =  CRYS_ECPKI_GetEcDomain ( convert_mbedtls_grp_id_to_crys_domain_id( grp->id ) );

#if SIZE_MAX > UINT_MAX
    if( blen > 0xFFFFFFFF )
    {
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
    }
#endif

    if ( pDomain )
    {
        uint8_t temp_buf[ 2*MAX_KEY_SIZE_IN_BYTES + 1 ] = {0};

        cc_ecc_ws_verify_params_t* verifyParams = mbedtls_calloc( 1, sizeof(cc_ecc_ws_verify_params_t) );
        if ( verifyParams == NULL)
            return ( MBEDTLS_ERR_ECP_ALLOC_FAILED );
        pHeap = verifyParams;
        heapSize = sizeof(cc_ecc_ws_verify_params_t);

        pSignature = mbedtls_calloc( 1, signature_size );
        if ( pSignature == NULL)
        {
            ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
            goto cleanup;
        }

        MBEDTLS_MPI_CHK( mbedtls_ecp_point_write_binary( grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED,
                                        &temp_size, temp_buf, sizeof(temp_buf) ) );

        CrysRet = CRYS_ECPKI_BuildPublKey(pDomain, temp_buf, temp_size, &verifyParams->pubKey);
        if( CrysRet != CRYS_OK )
        {
            ret = convert_CrysError_to_mbedtls_err( CrysRet );
            goto cleanup;
        }

        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( r, pSignature, ( ( grp->nbits + 7 ) / 8 )  ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( s, pSignature + ( ( grp->nbits + 7 ) / 8 ), ( ( grp->nbits + 7 ) / 8 ) ) );
        CrysRet =  CRYS_ECDSA_Verify ( &verifyParams->verifyContext,
                                       &verifyParams->pubKey,
                                       hash_mode,
                                       pSignature,
                                       signature_size,
                                       (uint8_t*)buf,
                                       blen );
        if( CrysRet != CRYS_OK )
        {
            ret = convert_CrysError_to_mbedtls_err( CrysRet );
            goto cleanup;
        }
    }
    else
        ret =  MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;

cleanup:

    if( pHeap )
    {
        mbedtls_platform_zeroize( pHeap, heapSize );
        mbedtls_free( pHeap );
    }

    if( pSignature )
    {
        mbedtls_platform_zeroize( pSignature, signature_size );
        mbedtls_free( pSignature );

    }

    return ret;
}
#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

#if defined(MBEDTLS_ECDSA_GENKEY_ALT)
int mbedtls_ecdsa_genkey( mbedtls_ecdsa_context *ctx, mbedtls_ecp_group_id gid,
                  int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = 0;
    CRYSError_t CrysRet = CRYS_OK;
    void* pHeap = NULL;
    size_t heapSize = 0;
    uint32_t key_size = 2*MAX_KEY_SIZE_IN_BYTES + 1;
    const CRYS_ECPKI_Domain_t*  pDomain =  CRYS_ECPKI_GetEcDomain ( convert_mbedtls_grp_id_to_crys_domain_id( gid ) );
    mbedtls_rand_func_container cc_rand = { f_rng, p_rng };


    if ( pDomain )
    {
        uint8_t temp_buf[ 2 * MAX_KEY_SIZE_IN_BYTES + 1 ] = {0};

        cc_ecc_ws_keygen_params_t* kgParams =  mbedtls_calloc( 1, sizeof(cc_ecc_ws_keygen_params_t) );
        if ( kgParams == NULL )
            return ( MBEDTLS_ERR_ECP_ALLOC_FAILED );

        pHeap = kgParams;
        heapSize = sizeof(cc_ecc_ws_keygen_params_t);

        CrysRet =  CRYS_ECPKI_GenKeyPair( &cc_rand, convert_mbedtls_to_cc_rand, pDomain,
                                          &kgParams->privKey, &kgParams->pubKey,
                                          &kgParams->kgTempData, NULL );
        if ( CrysRet != CRYS_OK )
        {
            ret = convert_CrysError_to_mbedtls_err( CrysRet );
            goto cleanup;
        }

        MBEDTLS_MPI_CHK( mbedtls_ecp_group_load( &ctx->grp, gid ) );

        CrysRet = CRYS_ECPKI_ExportPublKey( &kgParams->pubKey, CRYS_EC_PointUncompressed, temp_buf,  &key_size );
        if ( CrysRet != CRYS_OK )
        {
            ret = convert_CrysError_to_mbedtls_err( CrysRet );
            goto cleanup;
        }

        ret = mbedtls_ecp_point_read_binary( &ctx->grp, &ctx->Q, temp_buf, key_size );
        if ( ret != 0 )
            goto cleanup;

        memset ( temp_buf, 0 , sizeof(temp_buf) );

        CrysRet = CRYS_COMMON_ConvertLswMswWordsToMsbLsbBytes( temp_buf, (ctx->grp.nbits+7)/8,
                                                               kgParams->privKey.PrivKeyDbBuff,
                                                               4*((((ctx->grp.nbits+7)/8)+3)/4) );
        if ( CrysRet != CRYS_OK )
        {
            ret = convert_CrysError_to_mbedtls_err( CrysRet );
            mbedtls_platform_zeroize( temp_buf, sizeof(temp_buf) );
            goto cleanup;
        }

        ret = mbedtls_mpi_read_binary( &ctx->d, temp_buf, (ctx->grp.nbits+7)/8 );
        mbedtls_platform_zeroize( temp_buf, sizeof(temp_buf) );
        if ( ret != 0 )
        {
            goto cleanup;
        }
    }
    else
        ret =  MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;


cleanup:
    if ( pHeap )
    {
        mbedtls_platform_zeroize( pHeap, heapSize );
        mbedtls_free ( pHeap );
    }
    return ( ret );
}
#endif /* MBEDTLS_ECDSA_GENKEY_ALT */
