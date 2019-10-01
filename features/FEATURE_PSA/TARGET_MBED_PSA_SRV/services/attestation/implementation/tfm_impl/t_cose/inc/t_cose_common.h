/*
 * t_cose_common.h
 *
 * Copyright 2019, Laurence Lundblade
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.mdE.
 */


#ifndef __T_COSE_COMMON_H__
#define __T_COSE_COMMON_H__


/**
 * \file t_cose_common.h
 *
 * \brief Defines common to all public t_cose interfaces.
 *
 */


/* Private value. Intentionally not documented for Doxygen.
 * This is the size allocated for the encoded protected headers.  It
 * needs to be big enough for make_protected_header() to succeed. It
 * currently sized for one header with an algorithm ID up to 32 bits
 * long -- one byte for the wrapping map, one byte for the label, 5
 * bytes for the ID. If this is made accidentially too small, QCBOR will
 * only return an error, and not overrun any buffers.
 *
 * 9 extra bytes are added, rounding it up to 16 total, in case some
 * other protected header is to be added.
 */
#define T_COSE_SIGN1_MAX_PROT_HEADER (1+1+5+9)


/**
 * Error codes return by t_cose.
 *
 * Do not reorder these. It is OK to add
 * new ones at the end.
 */
enum t_cose_err_t {
    /**
     * Operation completed successfully
     */
    T_COSE_SUCCESS = 0,
    /**
     * The requested signing algorithm is not supported.
     */
    T_COSE_ERR_UNSUPPORTED_SIGNING_ALG,
    /**
     * Error constructing the protected headers.
     */
    T_COSE_ERR_PROTECTED_HEADERS,
    /**
     * The hash algorithm needed is not supported. Note that the
     * signing algorithm identifier usually identifies the hash
     * algorithm.
     */
    T_COSE_ERR_UNSUPPORTED_HASH,
    /**
     * Some system failure when running the hash algorithm.
     */
    T_COSE_ERR_HASH_GENERAL_FAIL,
    /**
     * The buffer to receive a hash result is too small.
     */
    T_COSE_ERR_HASH_BUFFER_SIZE,
    /**
     * The buffer to receive result of a signing operation is too
     * small.
     */
    T_COSE_ERR_SIG_BUFFER_SIZE,
    /**
     * The buffer to receive to receive a key is too small.
     */
    T_COSE_ERR_KEY_BUFFER_SIZE,
    /**
     * When verifying a \c COSE_Sign1, something is wrong with the
     * format of the CBOR. For example, it is missing something like
     * the payload.
     */
    T_COSE_ERR_SIGN1_FORMAT,
    /**
     * When decoding some CBOR like a \c COSE_Sign1, the CBOR was not
     * well-formed. Most likely what was supposed to be CBOR was is
     * either not or it has been corrupted.
     */
    T_COSE_ERR_CBOR_NOT_WELL_FORMED,
    /**
     * No algorithm ID was found when one is needed. For example, when
     * verifying a \c COSE_Sign1.
     */
    T_COSE_ERR_NO_ALG_ID,
    /**
     * No key ID was found when one is needed. For example, when
     * verifying a \c COSE_Sign1.
     */
    T_COSE_ERR_NO_KID,
    /**
     * Signature verification failed. For example, the cryptographic
     * operations completed successfully but hash wasn't as expected.
     */
    T_COSE_ERR_SIG_VERIFY,
    /**
     * Verification of a short-circuit signature failed.
     */
    T_COSE_ERR_BAD_SHORT_CIRCUIT_KID,
    /**
     * Some (unspecified) argument was not valid.
     */
    T_COSE_ERR_INVALID_ARGUMENT,
    /**
     * Out of heap memory.
     */
    T_COSE_ERR_INSUFFICIENT_MEMORY,
    /**
     * General unspecific failure.
     */
    T_COSE_ERR_FAIL,
    /**
     * Equivalent to \c PSA_ERROR_TAMPERING_DETECTED.
     */
    T_COSE_ERR_TAMPERING_DETECTED,
    /**
     * The key identified by a key slot of a key ID was not found.
     */
    T_COSE_ERR_UNKNOWN_KEY,
    /**
     * The key was found, but it was the wrong type for the operation.
     */
    T_COSE_ERR_WRONG_TYPE_OF_KEY,
    /**
     * Error constructing the \c Sig_structure when signing or verify.
     */
    T_COSE_ERR_SIG_STRUCT,
    /**
      * Signature was short-circuit. THe option to allow verification
      * of short-circuit signatures was not set
     */
    T_COSE_ERR_SHORT_CIRCUIT_SIG
};



#endif /* __T_COSE_COMMON_H__ */
