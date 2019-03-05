/*
 * t_cose_crypto.h
 *
 * Copyright 2019, Laurence Lundblade
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.mdE.
 */


#ifndef __T_COSE_CRYPTO_H__
#define __T_COSE_CRYPTO_H__

#include "t_cose_common.h"
#include "useful_buf.h"
#include <stdint.h>
#include "t_cose_defines.h"


/**
 * \file t_cose_crypto.h
 *
 * \brief This is the adaptation layer for cryptographic functions used by
 * t_cose.
 *
 * This is  small wrapper around the cryptographic functions to:
 * - Map COSE algorithm IDs to TF-M algorithm IDs
 * - Map crypto errors to \ref t_cose_err_t errors
 * - Have inputs and outputs be \c struct \c useful_buf_c and
 *   \c struct \c useful_buf
 * - Handle key selection
 *
 * The idea is that implementations can be made of these functions
 * that adapt to various cryptographic libraries that are used on
 * various platforms and OSs.
 *
 * This runs entirely off of COSE-style algorithm identifiers.  They
 * are simple integers and thus work nice as function parameters. An
 * initial set is defined by [COSE (RFC 8152)]
 * (https://tools.ietf.org/html/rfc8152). New ones can be registered
 * in the [IANA COSE Registry]
 * (https://www.iana.org/assignments/cose/cose.xhtml). Local use new
 * ones can also be defined (\c \#define) if what is needed is not in
 * the IANA registry.
 *
 * Binary data is returned to the caller using a \c struct \c
 * useful_buf to pass the buffer to receive the data and its length in
 * and a \c useful_buf_c to return the pointer and length of the
 * returned data. The point of this is coding hygiene. The buffer
 * passed in is not const as it is to be modified.  The \c
 * useful_buf_c returned is const.
 *
 * The pointer in the \c useful_buf_c will always point to the buffer
 * passed in via the \c useful_buf so the lifetime of the data is
 * under control of the caller.
 *
 * This is not intended as any sort of general cryptographic API. It
 * is just the functions needed by t_cose in the form that is most
 * useful for t_cose.
 */


/**
 * Size of the signature output for the NIST P-256 Curve.
 */
#define T_COSE_EC_P256_SIG_SIZE 64

/**
 * Size of the largest signature of any of the algorithm types
 * supported.
 *
 * This will have to be adjusted if support for other algorithms
 * larger is added.
 *
 * This is a compile time constant so it can be used to define stack
 * variable sizes.
 */
#define T_COSE_MAX_EC_SIG_SIZE T_COSE_EC_P256_SIG_SIZE


/**
 * \brief Get the size in bytes of a particular signature type.
 *
 * \param[in] cose_sig_alg_id  The COSE algorithm ID.
 *
 * \return The size in bytes of the signature for a public-key signing
 * algorithm.
 */
static inline size_t t_cose_signature_size(int32_t cose_sig_alg_id);


/**
 * \brief Perform public key signing. Part of the t_cose crypto
 * adaptation layer.
 *
 * \param[in] cose_alg_id       The algorithm to sign with. The IDs are
 *                              defined in [COSE (RFC 8152)]
 *                              (https://tools.ietf.org/html/rfc8152) or
 *                              in the [IANA COSE Registry]
 *                          (https://www.iana.org/assignments/cose/cose.xhtml).
 *                              A proprietary ID can also be defined
 *                              locally (\c \#define) if the needed
 *                              one hasn't been registered.
 * \param[in] key_select        Indicates which key to use to sign.
 * \param[in] hash_to_sign      The bytes to sign. Typically, a hash of
 *                              a payload.
 * \param[in] signature_buffer  Pointer and length of buffer into which
 *                              the resulting signature is put.
 * \param[in] signature         Pointer and length of the signature
 *                              returned.
 *
 * \retval T_COSE_SUCCESS
 *         Successfully created the signature.
 * \retval T_COSE_ERR_SIG_BUFFER_SIZE
 *         The \c signature_buffer too small.
 * \retval T_COSE_ERR_UNSUPPORTED_SIGNING_ALG
 *         The requested signing algorithm, \c cose_alg_id, is not
 *         supported.
 * \retval T_COSE_ERR_UNKNOWN_KEY
 *         The key identified by \c key_select was not found.
 * \retval T_COSE_ERR_WRONG_TYPE_OF_KEY
 *         The key was found, but it was the wrong type.
 * \retval T_COSE_ERR_INVALID_ARGUMENT
 *         Some (unspecified) argument was not valid.
 * \retval T_COSE_ERR_INSUFFICIENT_MEMORY
 *         Insufficient heap memory.
 * \retval T_COSE_ERR_FAIL
 *         General unspecific failure.
 * \retval T_COSE_ERR_TAMPERING_DETECTED
 *         Equivalent to \c PSA_ERROR_TAMPERING_DETECTED.
 *
 * This is called to do public key signing. The implementation will
 * vary from one platform / OS to another but should conform to the
 * description here.
 *
 * The key selection depends on the platform / OS.
 *
 * See the note in the Detailed Description (the \\file comment block)
 * for details on how \c useful_buf and \c useful_buf_c are used to
 * return the signature.
 *
 * To find out the size of the signature buffer needed, call this with
 * \c signature_buffer->ptr \c NULL and \c signature_buffer->len a
 * very large number like \c UINT32_MAX. The size will be returned in
 * \c signature->len.
 */
enum t_cose_err_t
t_cose_crypto_pub_key_sign(int32_t cose_alg_id,
                           int32_t key_select,
                           struct useful_buf_c hash_to_sign,
                           struct useful_buf signature_buffer,
                           struct useful_buf_c *signature);


/**
 * \brief perform public key signature verification. Part of the
 * t_cose crypto adaptation layer.
 *
 * \param[in] cose_alg_id    The algorithm to use for verification.
 *                           The IDs are defined in [COSE (RFC 8152)]
 *                           (https://tools.ietf.org/html/rfc8152)
 *                           or in the [IANA COSE Registry]
 *                       (https://www.iana.org/assignments/cose/cose.xhtml).
 *                           A proprietary ID can also be defined
 *                           locally (\c \#define) if the needed one
 *                           hasn't been registered.
 * \param[in] key_select     Verification key selection.
 * \param[in] key_id         A key id or \c NULL_USEFUL_BUF_C.
 * \param[in] hash_to_verify The data or hash that is to be verified.
 * \param[in] signature      The signature.
 *
 * This verifies that the \c signature passed in was over the \c
 * hash_to_verify passed in.
 *
 * The public key used to verify the signature is selected by the \c
 * key_id if it is not \c NULL_USEFUL_BUF_C or the \c key_select if it
 * is.
 *
 * The key selected must be, or include, a public key of the correct
 * type for \c cose_alg_id.
 *
 * \retval T_COSE_SUCCESS
 *         The signature is valid
 * \retval T_COSE_ERR_SIG_VERIFY
 *         Signature verification failed. For example, the
 *         cryptographic operations completed successfully but hash
 *         wasn't as expected.
 * \retval T_COSE_ERR_UNKNOWN_KEY
 *         The key identified by \c key_select or a \c kid was
 *         not found.
 * \retval T_COSE_ERR_WRONG_TYPE_OF_KEY
 *         The key was found, but it was the wrong type
 *         for the operation.
 * \retval T_COSE_ERR_UNSUPPORTED_SIGNING_ALG
 *         The requested signing algorithm is not supported.
 * \retval T_COSE_ERR_INVALID_ARGUMENT
 *         Some (unspecified) argument was not valid.
 * \retval T_COSE_ERR_INSUFFICIENT_MEMORY
 *         Out of heap memory.
 * \retval T_COSE_ERR_FAIL
 *         General unspecific failure.
 * \retval T_COSE_ERR_TAMPERING_DETECTED
 *         Equivalent to \c PSA_ERROR_TAMPERING_DETECTED.
 */
enum t_cose_err_t
t_cose_crypto_pub_key_verify(int32_t cose_alg_id,
                             int32_t key_select,
                             struct useful_buf_c key_id,
                             struct useful_buf_c hash_to_verify,
                             struct useful_buf_c signature);


/**
 * The size of X and Y coordinate in 2 parameter style EC public
 * key. Format is as defined in [COSE (RFC 8152)]
 * (https://tools.ietf.org/html/rfc8152) and [SEC 1: Elliptic Curve
 * Cryptography](http://www.secg.org/sec1-v2.pdf).
 *
 * This size is well-known and documented in public standards.
 */
#define T_COSE_CRYPTO_EC_P256_COORD_SIZE 32


/**
 * \brief Get an elliptic curve public key. Part of the t_cose crypto
 * adaptation layer.
 *
 * \param[in] key_select     Used to look up the public
 *                           key to return when \c kid is
 *                           \c NULL_USEFUL_BUF_C.
 * \param[in] kid            A key ID to look up against. May be
 *                           \c NULL_USEFUL_BUF_C. This is typically
 *                           the kid from the COSE unprotected header.
 * \param[out] cose_curve_id The curve ID of the key returned as
 *                           defined by [COSE (RFC 8152)]
 *                           (https://tools.ietf.org/html/rfc8152)
 *                           or the IANA COSE registry.
 * \param[in] buf_to_hold_x_coord Pointer and length into which the
 *                                X coordinate is put.
 * \param[in] buf_to_hold_y_coord Pointer and length into which the
 *                                Y coordinate is put.
 * \param[out] x_coord       Pointer and length of the returned X
 *                           coordinate.
 * \param[out] y_coord       Pointer and length of the returned Y
 *                           coordinate.
 *
 * \retval T_COSE_SUCCESS
 *         The key was found and is returned.
 * \retval T_COSE_ERR_UNKNOWN_KEY
 *         The key identified by \c key_select or a \c kid was not
 *         found.
 * \retval T_COSE_ERR_WRONG_TYPE_OF_KEY
 *         The key was found, but it was the wrong type for the
 *         operation.
 * \retval T_COSE_ERR_FAIL
 *         General unspecific failure.
 * \retval T_COSE_ERR_KEY_BUFFER_SIZE
 *         Buffer to hold the output was too small.
 *
 * This finds and returns a public key. Where it looks for the key is
 * dependent on the OS / platform.
 *
 * \ref T_COSE_CRYPTO_EC_P256_COORD_SIZE is the size of the X or Y
 * coordinate for the NIST P-256 curve.
 *
 * See the note in the Detailed Description (the \\file comment block)
 * for details on how \c useful_buf and \c useful_buf_c are used to
 * return the X and Y coordinates.
 */
enum t_cose_err_t
t_cose_crypto_get_ec_pub_key(int32_t key_select,
                             struct useful_buf_c kid,
                             int32_t *cose_curve_id,
                             struct useful_buf buf_to_hold_x_coord,
                             struct useful_buf buf_to_hold_y_coord,
                             struct useful_buf_c  *x_coord,
                             struct useful_buf_c  *y_coord);


/*
 * No function to get private key because there is no need for it.
 * The private signing key only needs to exist behind
 * t_cose_crypto_pub_key_sign().
 */




/**
 * The context for use with the hash adaptation layer here.
 */
struct t_cose_crypto_hash {
    /* Can't put the actual size here without creating dependecy on
     * actual hash implementation, so this is a fairly large and
     * accommodating size.
     */
    uint8_t bytes[280];
};


/**
 * The size of the output of SHA-256 in bytes.
 *
 * (It is safe to define this independently here as its size is
 * well-known and fixed. There is no need to reference
 * platform-specific headers and incur messy dependence.)
 */
#define T_COSE_CRYPTO_SHA256_SIZE 32


/**
 * \brief Start cryptographic hash. Part of the t_cose crypto
 * adaptation layer.
 *
 * \param[in,out] hash_ctx      Pointer to the hash context that
 *                              will be initialized.
 * \param[in] cose_hash_alg_id  Algorithm ID that identifies the
 *                              hash to use. This is from the
 *                              [IANA COSE Registry]
 *                          (https://www.iana.org/assignments/cose/cose.xhtml).
 *                              As of the creation of this interface
 *                              no identifiers of only a hash
 *                              functions have been registered.
 *                              Signature algorithms that include
 *                              specification of the hash have been
 *                              registered, but they are not to be
 *                              used here. Until hash functions only
 *                              have been officially registered, some
 *                              IDs are defined in the proprietary
 *                              space in t_cose_common.h.
 *
 * \retval T_COSE_ERR_UNSUPPORTED_HASH
 *         The requested algorithm is unknown or unsupported.
 *
 * \retval T_COSE_ERR_HASH_GENERAL_FAIL
 *         Some general failure of the hash function
 *
 * This initializes the hash context for the particular algorithm. It
 * must be called first. A \c hash_ctx can be reused if it is
 * reinitialized.
 */
enum t_cose_err_t
t_cose_crypto_hash_start(struct t_cose_crypto_hash *hash_ctx,
                         int32_t cose_hash_alg_id);


/**
 * \brief Feed data into a cryptographic hash. Part of the t_cose
 * crypto adaptation layer.
 *
 * \param[in,out] hash_ctx  Pointer to the hash context in which
 *                          accumulate the hash.
 * \param[in]  data_to_hash Pointer and length of data to feed into
 *                          hash. The pointer may by \c NULL in which
 *                          case no hashing is performed.
 *
 * There is no return value. If an error occurs it is remembered in \c
 * hash_ctx and returned when t_cose_crypto_hash_finish() is called.
 * Once in the error state, this function may be called, but it will
 * not do anything.
 */
void t_cose_crypto_hash_update(struct t_cose_crypto_hash *hash_ctx,
                               struct useful_buf_c data_to_hash);


/**
 * \brief Finish a cryptographic hash. Part of the t_cose crypto
 * adaptation layer.
 *
 * \param[in,out] hash_ctx           Pointer to the hash context.
 * \param[in] buffer_to_hold_result  Pointer and length into which
 *                                   the resulting hash is put.
 * \param[out] hash_result           Pointer and length of the
 *                                   resulting hash.
 *
 * \retval T_COSE_ERR_HASH_GENERAL_FAIL
 *         Some general failure of the hash function.
 * \retval T_COSE_ERR_HASH_BUFFER_SIZE
 *         The size of the buffer to hold the hash result was
 *         too small.
 *
 * Call this to complete the hashing operation. If the everything
 * completed correctly, the resulting hash is returned. Note that any
 * errors that occurred during t_cose_crypto_hash_update() are
 * returned here.
 *
 * See the note in the Detailed Description (the \\file comment block)
 * for details on how \c useful_buf and \c useful_buf_c are used to
 * return the hash.
 */
enum t_cose_err_t
t_cose_crypto_hash_finish(struct t_cose_crypto_hash *hash_ctx,
                          struct useful_buf buffer_to_hold_result,
                          struct useful_buf_c *hash_result);



/*
 * Public inline function. See documentation above.
 */
static inline size_t t_cose_signature_size(int32_t cose_sig_alg_id)
{
    switch(cose_sig_alg_id) {
        case COSE_ALGORITHM_ES256:
            return T_COSE_EC_P256_SIG_SIZE;
        default:
            return T_COSE_EC_P256_SIG_SIZE;
    }
}


#endif /* __T_COSE_CRYPTO_H__ */
