/*
 *  ecdh_alt.c
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

#include "mbedtls/ecdh.h"
#include <string.h>
#include "crys_ecpki_dh.h"
#include "crys_ecpki_build.h"
#include "crys_common.h"
#include "crys_ecpki_kg.h"
#include "crys_ecpki_domain.h"
#include "crys_ec_mont_api.h"
#include "mbedtls/platform.h"
#include "mbedtls/platform_util.h"
#include "cc_internal.h"


#if defined (MBEDTLS_ECDH_GEN_PUBLIC_ALT)
int mbedtls_ecdh_gen_public( mbedtls_ecp_group *grp, mbedtls_mpi *d, mbedtls_ecp_point *Q,
                     int ( *f_rng )( void *, unsigned char *, size_t ),
                     void *p_rng )
{
      int ret = 0;
      void* pHeap = NULL;
      size_t   heapSize = 0;

      uint32_t public_key_size = (2 * MAX_KEY_SIZE_IN_BYTES + 1);
      const CRYS_ECPKI_Domain_t*  pDomain =  CRYS_ECPKI_GetEcDomain ( convert_mbedtls_grp_id_to_crys_domain_id(  grp->id ) );
      mbedtls_rand_func_container cc_rand = { f_rng, p_rng };

      if ( pDomain )
      {
          uint8_t temp_buf[ 2 * MAX_KEY_SIZE_IN_BYTES + 1 ] = {0};
          cc_ecc_ws_keygen_params_t* kgParams =  mbedtls_calloc( 1, sizeof( cc_ecc_ws_keygen_params_t ) );

          if ( kgParams == NULL )
              return ( MBEDTLS_ERR_ECP_ALLOC_FAILED );
          pHeap = kgParams;
          heapSize = sizeof( cc_ecc_ws_keygen_params_t );

          ret = convert_CrysError_to_mbedtls_err( CRYS_ECPKI_GenKeyPair( &cc_rand, convert_mbedtls_to_cc_rand,
                                       pDomain, &kgParams->privKey,
                                       &kgParams->pubKey,
                                       &kgParams->kgTempData, NULL ) );
          if( ret != 0 )
          {
              goto cleanup;
          }

          ret = convert_CrysError_to_mbedtls_err( CRYS_ECPKI_ExportPublKey( &kgParams->pubKey,
                                         CRYS_EC_PointUncompressed,temp_buf,  &public_key_size ) );
          if( ret != 0 )
          {
              goto cleanup;
          }


          MBEDTLS_MPI_CHK( mbedtls_ecp_point_read_binary( grp, Q, temp_buf, public_key_size ) );
          memset ( temp_buf, 0 , sizeof(temp_buf) );

          ret = convert_CrysError_to_mbedtls_err( CRYS_COMMON_ConvertLswMswWordsToMsbLsbBytes( temp_buf, (grp->nbits+7)/8,
                                                            kgParams->privKey.PrivKeyDbBuff,
                                                            4*((((grp->nbits+7)/8)+3)/4) ) );
          if( ret != 0 )
          {
              mbedtls_platform_zeroize( temp_buf, sizeof( temp_buf ) );
              goto cleanup;
          }

          MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary( d, temp_buf, (grp->nbits+7)/8 ) );
          mbedtls_platform_zeroize( temp_buf, sizeof( temp_buf ) );
      }

      /* if CRYS_ECPKI_GetEcDomain returns NULL, then the given curve is either Montgomery 25519
       * or another curve which is not supported by CC310*/
      else if ( grp->id ==  MBEDTLS_ECP_DP_CURVE25519 )
      {
          size_t priv_key_size =  public_key_size = CURVE_25519_KEY_SIZE ;

          cc_ecc_25519_keygen_params_t* kgParams =  mbedtls_calloc( 1, sizeof(cc_ecc_25519_keygen_params_t) );

          if ( kgParams == NULL )
              return ( MBEDTLS_ERR_ECP_ALLOC_FAILED );
          pHeap = ( uint8_t* )kgParams;
          heapSize = sizeof(cc_ecc_25519_keygen_params_t);

          ret = convert_CrysError_to_mbedtls_err( CRYS_ECMONT_KeyPair( kgParams->pubKey, ( size_t* )&public_key_size, kgParams->privKey,
                                    &priv_key_size, &cc_rand, convert_mbedtls_to_cc_rand,
                                    &kgParams->kgTempData ) );
          if( ret != 0 )
          {
              goto cleanup;
          }

          MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( d, kgParams->privKey, priv_key_size ) );
          MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary(  &Q->X, kgParams->pubKey, public_key_size ) );
          MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &Q->Z, 1 ) );
      }
      else
          ret =  MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;

cleanup:

    if ( pHeap )
    {
        mbedtls_platform_zeroize( pHeap, heapSize );
        mbedtls_free( pHeap );
    }

    return ( ret );
}
#endif /* MBEDTLS_ECDH_GEN_PUBLIC_ALT */

/*
 * Compute shared secret (SEC1 3.3.1)
 */
#if defined (MBEDTLS_ECDH_COMPUTE_SHARED_ALT)
int mbedtls_ecdh_compute_shared( mbedtls_ecp_group *grp, mbedtls_mpi *z,
                         const mbedtls_ecp_point *Q, const mbedtls_mpi *d,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng )
{
    int ret;
    void* pHeap = NULL;
    size_t   heapSize = 0;

    size_t   public_key_size = (grp->nbits+7)/8 ;
    const CRYS_ECPKI_Domain_t*  pDomain =  CRYS_ECPKI_GetEcDomain ( convert_mbedtls_grp_id_to_crys_domain_id( grp->id ) );
    uint32_t secret_size =  ( ( grp->nbits + 7 ) / 8 ) ;
    const uint32_t secret_size_in_heap = secret_size;
    uint8_t* secret = mbedtls_calloc( 1, secret_size_in_heap );
    if ( secret == NULL )
        return ( MBEDTLS_ERR_ECP_ALLOC_FAILED );

    /*
     * Make sure Q is a valid pubkey before using it
     */
    MBEDTLS_MPI_CHK( mbedtls_ecp_check_pubkey( grp, Q ) );
    if ( pDomain )
    {
        uint8_t  temp_buf[ 2 * MAX_KEY_SIZE_IN_BYTES + 1 ] = {0};
        cc_ecc_ws_comp_shared_params_t* ecdhParams =  mbedtls_calloc( 1, sizeof(cc_ecc_ws_comp_shared_params_t) );

        if ( ecdhParams == NULL )
        {
            ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
            goto cleanup;
        }

        pHeap = ecdhParams;
        heapSize = sizeof(cc_ecc_ws_comp_shared_params_t);


        MBEDTLS_MPI_CHK( mbedtls_ecp_point_write_binary( grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED,
                                                &public_key_size, temp_buf, sizeof(temp_buf) ) );

        ret = convert_CrysError_to_mbedtls_err( CRYS_ECPKI_BuildPublKey( pDomain, temp_buf, public_key_size,
                                                                         &ecdhParams->pubKey ) );
        if ( ret != 0 )
        {
            goto cleanup;
        }

        memset ( temp_buf, 0, sizeof(temp_buf)  );

        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, temp_buf, mbedtls_mpi_size( d ) ) );

        ret = convert_CrysError_to_mbedtls_err( CRYS_ECPKI_BuildPrivKey( pDomain,
                                                                         temp_buf,
                                                                         mbedtls_mpi_size( d ),
                                                                         &ecdhParams->privKey ) );
        mbedtls_platform_zeroize( temp_buf, sizeof( temp_buf ) );
        if ( ret != 0 )
        {
            goto cleanup;
        }

        ret = convert_CrysError_to_mbedtls_err( CRYS_ECDH_SVDP_DH( &ecdhParams->pubKey, &ecdhParams->privKey,
                                                                   secret, &secret_size,
                                                                   &ecdhParams->ecdhTempData ) );
        if ( ret != 0 )
        {
            goto cleanup;
        }
    }
    else if ( grp->id ==  MBEDTLS_ECP_DP_CURVE25519 )
    {
        cc_ecc_25519_comp_shared_params_t* ecdhParams =  mbedtls_calloc( 1, sizeof(cc_ecc_25519_comp_shared_params_t) );
        if ( ecdhParams == NULL )
        {
            ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
            goto cleanup;
        }

        pHeap = ecdhParams;
        heapSize = sizeof(cc_ecc_25519_comp_shared_params_t);


        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, ecdhParams->privKey, mbedtls_mpi_size( d ) ) ) ;
        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &Q->X, ecdhParams->pubKey, public_key_size ) );

        ret = convert_CrysError_to_mbedtls_err( CRYS_ECMONT_Scalarmult( secret, ( size_t* )&secret_size,
                                                                        ecdhParams->privKey, CURVE_25519_KEY_SIZE ,
                                                                        ecdhParams->pubKey, CURVE_25519_KEY_SIZE ,
                                                                        &ecdhParams->kgTempData ) );
        if ( ret != 0 )
        {
            goto cleanup;
        }
    }
    else
    {
        ret =  MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED;
        goto cleanup;
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( z, secret, secret_size ) );

cleanup:

    if ( pHeap )
    {
        mbedtls_platform_zeroize( pHeap, heapSize );
        mbedtls_free ( pHeap );
    }

    if ( secret )
    {
        mbedtls_platform_zeroize( secret, secret_size_in_heap );
        mbedtls_free ( secret );
    }

    return ( ret );
}
#endif /* MBEDTLS_ECDH_COMPUTE_SHARED_ALT */
