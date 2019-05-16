/*
 *  t_cose_util.h
 *
 * Copyright 2019, Laurence Lundblade
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.mdE.
 */


#ifndef __T_COSE_UTIL_H__
#define __T_COSE_UTIL_H__

#include <stdint.h>
#include "useful_buf.h"
#include "t_cose_common.h"

/**
 * \file t_cose_util.h
 *
 * \brief Utility functions used internally by the t_cose implementation.
 *
 */


/**
 * \brief Return hash algorithm ID from a signature algorithm ID
 *
 * \param[in] cose_sig_alg_id  A COSE signature algorithm identifier.
 *
 * \return \c INT32_MAX when the signature algorithm ID is not known.
 *
 * This works off of algorithm identifiers defined in the [IANA COSE
 * Registry] (https://www.iana.org/assignments/cose/cose.xhtml).
 * Corresponding local integer constants are defined in
 * t_cose_defines.h.
 *
 * COSE signing algorithms are the combination of public key
 * algorithm, curve, key size, hash algorithm and hash size. They are
 * simple integers making them convenient for direct use in code.
 *
 * This function returns an identifier for only the hash algorithm
 * from the combined identifier.
 *
 * If the needed algorithm identifiers are not in the IANA registry,
 * they can be added to it. This will take some time and work.  It is
 * also fine to use algorithms in the proprietary space.
 */
int32_t hash_alg_id_from_sig_alg_id(int32_t cose_sig_alg_id);


/**
 * \brief Create the hash of the to-be-signed (TBS) bytes for COSE.
 *
 * \param[in] cose_alg_id       The COSE signing algorithm ID. Used to
 *                              determine which hash function to use.
 * \param[in] buffer_for_hash   Pointer and length of buffer into which
 *                              the resulting hash is put.
 * \param[out] hash             Pointer and length of the
 *                              resulting hash.
 * \param[in] protected_headers The CBOR encoded protected headers.
 * \param[in] payload           The CBOR encoded payload
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * \retval T_COSE_ERR_SIG_STRUCT
 *         Most likely this is because the protected_headers passed in
 *         is larger than \ref T_COSE_SIGN1_MAX_PROT_HEADER.
 * \retval T_COSE_ERR_UNSUPPORTED_HASH
 *         If the hash algorithm is not known.
 * \retval T_COSE_ERR_HASH_GENERAL_FAIL
 *         In case of some general hash failure.
 *
 * The input to the public key signature algorithm in COSE is the hash
 * of a CBOR encoded structure containing the protected headers
 * algorithm ID and a few other things. This formats that structure
 * and computes the hash of it. These are known as the to-be-signed or
 * "TBS" bytes.
 */
enum t_cose_err_t create_tbs_hash(int32_t cose_alg_id,
                                  struct useful_buf buffer_for_hash,
                                  struct useful_buf_c *hash,
                                  struct useful_buf_c protected_headers,
                                  struct useful_buf_c payload);


/**
 * Size of the key returned by get_short_circuit_kid(). It is always
 * this size.
 */
#define T_COSE_SHORT_CIRCUIT_KID_SIZE 32


/**
 * \brief Get the special kid for short-circuit signing.
 *
 * \param[in] buffer_for_kid Pointer and length of buffer into which
 *                           the resulting hash is put. It should
 *                           always be at least \ref
 *                           T_COSE_SHORT_CIRCUIT_KID_SIZE.
 * \param[out] kid           Pointer and length of the returned kid.
 *
 * \retval T_COSE_SUCCESS
 *         The kid was returned.
 * \retval T_COSE_ERR_KEY_BUFFER_SIZE
 *         \c buffer_for_kid is too small
 *
 * This always returns the same key ID. It always indicates
 * short-circuit signing. It is OK to hard code this as the
 * probability of collision with this ID is extremely low and the same
 * as for collision between any two key IDs (kids) of any sort.
 *
 * This is the value of the kid.
 *
 *        0xef, 0x95, 0x4b, 0x4b, 0xd9, 0xbd, 0xf6, 0x70,
 *        0xd0, 0x33, 0x60, 0x82, 0xf5, 0xef, 0x15, 0x2a,
 *        0xf8, 0xf3, 0x5b, 0x6a, 0x6c, 0x00, 0xef, 0xa6,
 *        0xa9, 0xa7, 0x1f, 0x49, 0x51, 0x7e, 0x18, 0xc6
 *
 */
enum t_cose_err_t
get_short_circuit_kid(struct useful_buf buffer_for_kid,
                      struct useful_buf_c *kid);

#endif /* __T_COSE_UTIL_H__ */
