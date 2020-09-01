/*
 *  ECDSA sign, verify and key generation functions
 *
 *  Copyright (C) 2019 Cypress Semiconductor Corporation
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
 */

/*
 * \file     ecdsa_alt.c
 * \version  1.0
 *
 * \brief This file provides an API for Elliptic Curves sign and verifications.
 *
 */

#if defined(MBEDTLS_CONFIG_FILE)
#include MBEDTLS_CONFIG_FILE
#else
#include "config.h"
#endif

#if defined(MBEDTLS_ECDSA_C)

#include "mbedtls/ecdsa.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/platform_util.h"

#if defined(MBEDTLS_ECDSA_SIGN_ALT)

#include "cy_crypto_core_ecc.h"
#include "cy_crypto_core_vu.h"
#include "crypto_common.h"

#include <string.h>
#include <stdlib.h>

/* Parameter validation macros based on platform_util.h */
#define ECDSA_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA )
#define ECDSA_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

/**
 * \brief           This function computes the ECDSA signature of a
 *                  previously-hashed message.
 *
 * \note            The deterministic version is usually preferred.
 *
 * \note            If the bitlength of the message hash is larger than the
 *                  bitlength of the group order, then the hash is truncated
 *                  as defined in <em>Standards for Efficient Cryptography Group
 *                  (SECG): SEC1 Elliptic Curve Cryptography</em>, section
 *                  4.1.3, step 5.
 *
 * \see             ecp.h
 *
 * \param grp       The ECP group.
 * \param r         The first output integer.
 * \param s         The second output integer.
 * \param d         The private signing key.
 * \param buf       The message hash.
 * \param blen      The length of \p buf.
 * \param f_rng     The RNG function.
 * \param p_rng     The RNG context.
 *
 * \return          \c 0 on success.
 * \return          An \c MBEDTLS_ERR_ECP_XXX
 *                  or \c MBEDTLS_MPI_XXX error code on failure.
 */
int mbedtls_ecdsa_sign( mbedtls_ecp_group *grp, mbedtls_mpi *r, mbedtls_mpi *s,
                const mbedtls_mpi *d, const unsigned char *buf, size_t blen,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret;
    size_t bytesize;
    uint8_t *sig = NULL;
    uint8_t *tmp_k = NULL;
    cy_hw_crypto_t crypto_obj;
    cy_stc_crypto_ecc_key key;
    cy_stc_crypto_ecc_dp_type *dp;
    cy_en_crypto_status_t ecdsa_status;

    ECDSA_VALIDATE_RET( grp   != NULL );
    ECDSA_VALIDATE_RET( r     != NULL );
    ECDSA_VALIDATE_RET( s     != NULL );
    ECDSA_VALIDATE_RET( d     != NULL );
    ECDSA_VALIDATE_RET( f_rng != NULL );
    ECDSA_VALIDATE_RET( buf   != NULL || blen == 0 );

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if( grp->N.p == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    key.curveID = cy_get_dp_idx(grp->id);
    ECDSA_VALIDATE_RET( key.curveID != CY_CRYPTO_ECC_ECP_NONE);

    /* Make sure d is in range 1..n-1 */
    if( mbedtls_mpi_cmp_int( d, 1 ) < 0 || mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

    /* Reserve the crypto hardware for the operation */
    cy_hw_crypto_reserve(&crypto_obj, CYHAL_CRYPTO_VU);

    dp = Cy_Crypto_Core_ECC_GetCurveParams(key.curveID);

    bytesize = CY_CRYPTO_BYTE_SIZE_OF_BITS(dp->size);

    key.k = malloc(bytesize);
    MBEDTLS_MPI_CHK((key.k == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( d, key.k, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(key.k, bytesize);

    sig = malloc(2 * bytesize);
    MBEDTLS_MPI_CHK((sig == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    tmp_k = malloc(bytesize);
    MBEDTLS_MPI_CHK((tmp_k == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    ecdsa_status = Cy_Crypto_Core_ECC_MakePrivateKey(crypto_obj.base, key.curveID, tmp_k, f_rng, p_rng);
    MBEDTLS_MPI_CHK((ecdsa_status == CY_CRYPTO_SUCCESS) ? 0 : MBEDTLS_ERR_ECP_HW_ACCEL_FAILED);

    ecdsa_status = Cy_Crypto_Core_ECC_SignHash(crypto_obj.base, buf, blen, sig, &key, tmp_k);
    MBEDTLS_MPI_CHK((ecdsa_status == CY_CRYPTO_SUCCESS) ? 0 : MBEDTLS_ERR_ECP_HW_ACCEL_FAILED);

    /* Prepare a signature to load into an mpi format */
    Cy_Crypto_Core_InvertEndianness(sig, bytesize);
    Cy_Crypto_Core_InvertEndianness(sig + bytesize, bytesize);

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( r, sig, bytesize ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( s, sig + bytesize, bytesize ) );

cleanup:
    /* Realease the crypto hardware */
    cy_hw_crypto_release(&crypto_obj);

    if (key.k != NULL)
    {
        mbedtls_platform_zeroize(key.k, bytesize);
        free(key.k);
    }
    if (sig != NULL)
    {
        mbedtls_platform_zeroize(sig, 2 * bytesize);
        free(sig);
    }
    if (tmp_k != NULL)
    {
        mbedtls_platform_zeroize(tmp_k, bytesize);
        free(tmp_k);
    }

    return( ret );
}
#endif /* MBEDTLS_ECDSA_SIGN_ALT */

#if defined(MBEDTLS_ECDSA_VERIFY_ALT)
/*
 * Verify ECDSA signature of hashed message (SEC1 4.1.4)
 * Obviously, compared to SEC1 4.1.3, we skip step 2 (hash message)
 */
int mbedtls_ecdsa_verify( mbedtls_ecp_group *grp,
                  const unsigned char *buf, size_t blen,
                  const mbedtls_ecp_point *Q, const mbedtls_mpi *r, const mbedtls_mpi *s)
{
    int ret;
    uint8_t stat;
    size_t bytesize;
    size_t olen;
    uint8_t *sig = NULL;
    uint8_t *point_arr = NULL;
    cy_hw_crypto_t crypto_obj;
    cy_stc_crypto_ecc_key key;
    cy_stc_crypto_ecc_dp_type *dp;
    cy_en_crypto_status_t ecdsa_ver_status;

    ECDSA_VALIDATE_RET( grp != NULL );
    ECDSA_VALIDATE_RET( Q   != NULL );
    ECDSA_VALIDATE_RET( r   != NULL );
    ECDSA_VALIDATE_RET( s   != NULL );
    ECDSA_VALIDATE_RET( buf != NULL || blen == 0 );

    /* Fail cleanly on curves such as Curve25519 that can't be used for ECDSA */
    if( grp->N.p == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    key.curveID = cy_get_dp_idx(grp->id);
    MBEDTLS_MPI_CHK( (key.curveID == CY_CRYPTO_ECC_ECP_NONE) ? MBEDTLS_ERR_ECP_BAD_INPUT_DATA : 0);

    /* Reserve the crypto hardware for the operation */
    cy_hw_crypto_reserve(&crypto_obj, CYHAL_CRYPTO_VU);

    dp = Cy_Crypto_Core_ECC_GetCurveParams(key.curveID);

    bytesize   = CY_CRYPTO_BYTE_SIZE_OF_BITS(dp->size);

    point_arr = malloc(2 * bytesize + 1u);
    MBEDTLS_MPI_CHK((point_arr == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);
    key.pubkey.x  = point_arr + 1u;
    key.pubkey.y  = point_arr + bytesize + 1u;

    sig = malloc(2 * bytesize);
    MBEDTLS_MPI_CHK((sig == NULL) ? MBEDTLS_ERR_ECP_ALLOC_FAILED : 0);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( r, sig, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(sig, bytesize);

    MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( s, sig + bytesize, bytesize ) );
    Cy_Crypto_Core_InvertEndianness(sig + bytesize, bytesize);

    /* Export a signature from an mpi format to verify */
    MBEDTLS_MPI_CHK( mbedtls_ecp_point_write_binary( grp, Q, MBEDTLS_ECP_PF_UNCOMPRESSED, &olen, point_arr, 2 * bytesize + 1) );
    Cy_Crypto_Core_InvertEndianness(key.pubkey.x, bytesize);
    Cy_Crypto_Core_InvertEndianness(key.pubkey.y, bytesize);

    ecdsa_ver_status = Cy_Crypto_Core_ECC_VerifyHash(crypto_obj.base, sig, buf, blen, &stat, &key);
    MBEDTLS_MPI_CHK((ecdsa_ver_status != CY_CRYPTO_SUCCESS) ? MBEDTLS_ERR_ECP_HW_ACCEL_FAILED : 0);

    MBEDTLS_MPI_CHK((stat == 1) ? 0 : MBEDTLS_ERR_ECP_VERIFY_FAILED);

cleanup:
    /* Realease the crypto hardware */
    cy_hw_crypto_release(&crypto_obj);

    if (point_arr != NULL)
    {
        mbedtls_platform_zeroize(point_arr, 2 * bytesize + 1u);
        free(point_arr);
    }
    if (sig != NULL)
    {
        mbedtls_platform_zeroize(sig, 2 * bytesize);
        free(sig);
    }

    return( ret );
}
#endif /* MBEDTLS_ECDSA_VERIFY_ALT */

#endif /* MBEDTLS_ECDSA_C */
