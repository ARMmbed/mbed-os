/**
 * \file mbedtls_ecc_group_to_psa.h
 *
 * Excerpted from Mbed TLS for internal use by Mbed TLS's PK module to
 * interface with generic PSA Crypto implementations.
 *
 */
/*
 *  Copyright The Mbed TLS Contributors
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

#ifndef MBEDTLS_ECC_GROUP_TO_PSA_H
#define MBEDTLS_ECC_GROUP_TO_PSA_H

//#include "mbedtls/platform_util.h"

//#include "crypto_compat.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup psa_tls_helpers TLS helper functions
 * @{
 */

#if defined(MBEDTLS_ECP_C)
#include <mbedtls/ecp.h>

/** Convert an ECC curve identifier from the Mbed TLS encoding to PSA.
 *
 * \note This function is provided solely for the convenience of
 *       Mbed TLS and may be removed at any time without notice.
 *
 * \param grpid         An Mbed TLS elliptic curve identifier
 *                      (`MBEDTLS_ECP_DP_xxx`).
 * \param[out] bits     On success, the bit size of the curve.
 *
 * \return              The corresponding PSA elliptic curve identifier
 *                      (`PSA_ECC_FAMILY_xxx`).
 * \return              \c 0 on failure (\p grpid is not recognized).
 */
static inline psa_ecc_family_t mbedtls_ecc_group_to_psa( mbedtls_ecp_group_id grpid,
                                                        size_t *bits )
{
    switch( grpid )
    {
        case MBEDTLS_ECP_DP_SECP192R1:
            *bits = 192;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_SECP224R1:
            *bits = 224;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_SECP256R1:
            *bits = 256;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_SECP384R1:
            *bits = 384;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_SECP521R1:
            *bits = 521;
            return( PSA_ECC_FAMILY_SECP_R1 );
        case MBEDTLS_ECP_DP_BP256R1:
            *bits = 256;
            return( PSA_ECC_FAMILY_BRAINPOOL_P_R1 );
        case MBEDTLS_ECP_DP_BP384R1:
            *bits = 384;
            return( PSA_ECC_FAMILY_BRAINPOOL_P_R1 );
        case MBEDTLS_ECP_DP_BP512R1:
            *bits = 512;
            return( PSA_ECC_FAMILY_BRAINPOOL_P_R1 );
        case MBEDTLS_ECP_DP_CURVE25519:
            *bits = 255;
            return( PSA_ECC_FAMILY_MONTGOMERY );
        case MBEDTLS_ECP_DP_SECP192K1:
            *bits = 192;
            return( PSA_ECC_FAMILY_SECP_K1 );
        case MBEDTLS_ECP_DP_SECP224K1:
            *bits = 224;
            return( PSA_ECC_FAMILY_SECP_K1 );
        case MBEDTLS_ECP_DP_SECP256K1:
            *bits = 256;
            return( PSA_ECC_FAMILY_SECP_K1 );
        case MBEDTLS_ECP_DP_CURVE448:
            *bits = 448;
            return( PSA_ECC_FAMILY_MONTGOMERY );
        default:
            *bits = 0;
            return( 0 );
    }
}

#endif /* MBEDTLS_ECP_C */

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_ECC_GROUP_TO_PSA_H */
