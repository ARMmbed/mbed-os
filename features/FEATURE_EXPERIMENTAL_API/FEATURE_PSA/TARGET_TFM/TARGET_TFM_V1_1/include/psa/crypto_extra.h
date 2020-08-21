/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/**
 * \file psa/crypto_extra.h
 *
 * \brief PSA cryptography module: vendor extensions
 *
 * \note This file may not be included directly. Applications must
 * include psa/crypto.h.
 *
 * This file is reserved for vendor-specific definitions.
 */

#ifndef PSA_CRYPTO_EXTRA_H
#define PSA_CRYPTO_EXTRA_H

#include "psa/crypto_compat.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup crypto_types
 * @{
 */

/** DSA public key.
 *
 * The import and export format is the
 * representation of the public key `y = g^x mod p` as a big-endian byte
 * string. The length of the byte string is the length of the base prime `p`
 * in bytes.
 */
#define PSA_KEY_TYPE_DSA_PUBLIC_KEY             ((psa_key_type_t)0x60020000)

/** DSA key pair (private and public key).
 *
 * The import and export format is the
 * representation of the private key `x` as a big-endian byte string. The
 * length of the byte string is the private key size in bytes (leading zeroes
 * are not stripped).
 *
 * Determinstic DSA key derivation with psa_generate_derived_key follows
 * FIPS 186-4 &sect;B.1.2: interpret the byte string as integer
 * in big-endian order. Discard it if it is not in the range
 * [0, *N* - 2] where *N* is the boundary of the private key domain
 * (the prime *p* for Diffie-Hellman, the subprime *q* for DSA,
 * or the order of the curve's base point for ECC).
 * Add 1 to the resulting integer and use this as the private key *x*.
 *
 */
#define PSA_KEY_TYPE_DSA_KEY_PAIR                ((psa_key_type_t)0x70020000)

/**@}*/

/** \brief Declare the enrollment algorithm for a key.
 *
 * An operation on a key may indifferently use the algorithm set with
 * psa_set_key_algorithm() or with this function.
 *
 * \param[out] attributes       The attribute structure to write to.
 * \param alg2                  A second algorithm that the key may be used
 *                              for, in addition to the algorithm set with
 *                              psa_set_key_algorithm().
 *
 * \warning Setting an enrollment algorithm is not recommended, because
 *          using the same key with different algorithms can allow some
 *          attacks based on arithmetic relations between different
 *          computations made with the same key, or can escalate harmless
 *          side channels into exploitable ones. Use this function only
 *          if it is necessary to support a protocol for which it has been
 *          verified that the usage of the key with multiple algorithms
 *          is safe.
 */
static inline void psa_set_key_enrollment_algorithm(
    psa_key_attributes_t *attributes,
    psa_algorithm_t alg2)
{
    attributes->alg2 = alg2;
}

/** Retrieve the enrollment algorithm policy from key attributes.
 *
 * \param[in] attributes        The key attribute structure to query.
 *
 * \return The enrollment algorithm stored in the attribute structure.
 */
static inline psa_algorithm_t psa_get_key_enrollment_algorithm(
    const psa_key_attributes_t *attributes)
{
    return attributes->alg2;
}

#ifdef __cplusplus
}
#endif

#endif /* PSA_CRYPTO_EXTRA_H */
