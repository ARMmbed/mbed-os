/*
 * t_cose_sign1_sign.c
 *
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#include "t_cose_sign1_sign.h"
#include "qcbor.h"
#include "t_cose_defines.h"
#include "t_cose_crypto.h"
#include "t_cose_util.h"


/**
 * \file t_cose_sign1_sign.c
 *
 * \brief This implements t_cose signing
 */


/**
 * \brief Create a short-circuit signature
 *
 * \param[in] cose_alg_id       Algorithm ID. This is used only to make
 *                              the short-circuit signature the same size
 *                              as the real signature would be for the
 *                              particular algorithm.
 * \param[in] hash_to_sign      The bytes to sign. Typically, a hash of
 *                              a payload.
 * \param[in] signature_buffer  Pointer and length of buffer into which
 *                              the resulting signature is put.
 * \param[in] signature         Pointer and length of the signature
 *                              returned.
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * This creates the short-circuit signature that is a concatenation of
 * hashes up to the expected size of the signature. This is a test
 * mode only has it has no security value. This is retained in
 * commercial production code as a useful test or demo that can run
 * even if key material is not set up or accessible.
 */
static inline enum t_cose_err_t
short_circuit_sign(int32_t cose_alg_id,
                   struct useful_buf_c hash_to_sign,
                   struct useful_buf signature_buffer,
                   struct useful_buf_c *signature)
{
    /* approximate stack use on 32-bit machine: local use: 16
     */
    enum t_cose_err_t return_value;
    size_t array_indx;
    size_t amount_to_copy;
    size_t sig_size;

    sig_size = t_cose_signature_size(cose_alg_id);

    if(sig_size > signature_buffer.len) {
        /* Buffer too small for this signature type */
        return_value = T_COSE_ERR_SIG_BUFFER_SIZE;
        goto Done;
    }

    /* Loop concatening copies of the hash to fill out to signature size */
    for(array_indx = 0; array_indx < sig_size; array_indx += hash_to_sign.len) {
        amount_to_copy = sig_size - array_indx;
        if(amount_to_copy > hash_to_sign.len) {
            amount_to_copy = hash_to_sign.len;
        }
        memcpy((uint8_t *)signature_buffer.ptr + array_indx,
               hash_to_sign.ptr,
               amount_to_copy);
    }
    signature->ptr = signature_buffer.ptr;
    signature->len = sig_size;
    return_value   = T_COSE_SUCCESS;

Done:
    return return_value;
}



/**
 * The maximum size of a CBOR Encoded \c COSE_Key that this
 * implementation can handle. \c COSE_Key is the serialization format
 * for public and other types of keys defined by [COSE (RFC 8152)]
 * (https://tools.ietf.org/html/rfc8152).
 *
 *  This can be increased to handle larger keys, but stack usage will
 *  go up with this increase.
 */
#define MAX_ENCODED_COSE_KEY_SIZE \
    1 + /* 1 byte to encode map */ \
    2 + /* 2 bytes to encode key type */ \
    2 + /* 2 bytes to encode curve */ \
    2 * /* the X and Y coordinates at 32 bytes each */ \
        (T_COSE_CRYPTO_EC_P256_COORD_SIZE + 1 + 2)


/**
 * \brief CBOR encode a public key as a \c COSE_Key
 *
 * \param[in] key_select  Identifies the public key to encode.
 *
 * \param[in] buffer_for_cose_key  Pointer and length of buffer into which
 *                              the encoded \c COSE_Key is put.
 * \param[in] cose_key         Pointer and length of the encoded \c COSE_Key.
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * \c COSE_Key is the COSE-defined format for serializing a key for
 * transmission in a protocol. This function encodes an EC public key
 * expressed as an X and Y coordinate.
 */
static enum t_cose_err_t
t_cose_encode_cose_key(int32_t key_select,
                       struct useful_buf buffer_for_cose_key,
                       struct useful_buf_c *cose_key)
{
    /* approximate stack use on 32-bit machine:
     * local use: 328
     * with calls: 370
     */
    enum t_cose_err_t         return_value;
    QCBORError                qcbor_result;
    QCBOREncodeContext        cbor_encode_ctx;
    USEFUL_BUF_MAKE_STACK_UB( buffer_for_x_coord,
                                  T_COSE_CRYPTO_EC_P256_COORD_SIZE);
    USEFUL_BUF_MAKE_STACK_UB( buffer_for_y_coord,
                                  T_COSE_CRYPTO_EC_P256_COORD_SIZE);
    struct useful_buf_c       x_coord;
    struct useful_buf_c       y_coord;
    int32_t                   cose_curve_id;
    struct useful_buf_c       encoded_key_id;

    /* Get the public key x and y */
    return_value = t_cose_crypto_get_ec_pub_key(key_select,
                                                NULL_USEFUL_BUF_C,
                                                &cose_curve_id,
                                                buffer_for_x_coord,
                                                buffer_for_y_coord,
                                                &x_coord,
                                                &y_coord);
    if(return_value != T_COSE_SUCCESS) {
        goto Done;
    }

    /* Encode it into a COSE_Key structure */
    QCBOREncode_Init(&cbor_encode_ctx, buffer_for_cose_key);
    QCBOREncode_OpenMap(&cbor_encode_ctx);
    QCBOREncode_AddInt64ToMapN(&cbor_encode_ctx,
                               COSE_KEY_COMMON_KTY,
                               COSE_KEY_TYPE_EC2);
    QCBOREncode_AddInt64ToMapN(&cbor_encode_ctx,
                               COSE_KEY_PARAM_CRV,
                               cose_curve_id);
    QCBOREncode_AddBytesToMapN(&cbor_encode_ctx,
                               COSE_KEY_PARAM_X_COORDINATE,
                               x_coord);
    QCBOREncode_AddBytesToMapN(&cbor_encode_ctx,
                               COSE_KEY_PARAM_Y_COORDINATE,
                               y_coord);
    QCBOREncode_CloseMap(&cbor_encode_ctx);

    qcbor_result = QCBOREncode_Finish(&cbor_encode_ctx, &encoded_key_id);
    if(qcbor_result != QCBOR_SUCCESS) {
        /* Mainly means that the COSE_Key was too big for buffer_for_cose_key */
        return_value = T_COSE_ERR_PROTECTED_HEADERS;
        goto Done;
    }

    /* Finish up and return */
    *cose_key = encoded_key_id;
    return_value = T_COSE_SUCCESS;

Done:
    return return_value;
}


/**
 * \brief SHA-256 hash a buffer in one quick function
 *
 * \param[in] bytes_to_hash The bytes to be hashed.
 *
 * \param[in] buffer_for_hash  Pointer and length into which
 *                                   the resulting hash is put.
 * \param[out] hash           Pointer and length of the
 *                                   resulting hash.
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * Simple wrapper for start, update and finish interface to a hash.
 *
 * Having this as a separate function helps keep stack usage down and
 * is convenient.
 */
static enum t_cose_err_t quick_sha256(struct useful_buf_c bytes_to_hash,
                                      struct useful_buf buffer_for_hash,
                                      struct useful_buf_c *hash)
{
    /* approximate stack use on 32-bit machine:
     local use: 132
     */
    enum t_cose_err_t           return_value = 0;
    struct t_cose_crypto_hash   hash_ctx;

    return_value = t_cose_crypto_hash_start(&hash_ctx,
                                            COSE_ALG_SHA256_PROPRIETARY);
    if(return_value) {
        goto Done;
    }
    t_cose_crypto_hash_update(&hash_ctx, bytes_to_hash);
    return_value = t_cose_crypto_hash_finish(&hash_ctx,
                                             buffer_for_hash,
                                             hash);

Done:
    return return_value;
}


/**
 * \brief Make a key ID based on the public key to go in the kid
 * unprotected header.
 *
 * \param[in] key_select         Identifies the public key.
 * \param[in] buffer_for_key_id  Pointer and length into which
 *                               the resulting key ID is put.
 * \param[out] key_id            Pointer and length of the
 *                               resulting key ID.
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 *
 * See t_cose_sign1_init() for documentation of the key ID format
 * created here.
 */
static inline enum t_cose_err_t get_keyid(int32_t key_select,
                                          struct useful_buf buffer_for_key_id,
                                          struct useful_buf_c *key_id)
{
    /* approximate stack use on 32-bit machine:
     * local use: 100
     * with calls inlined: 560
     * with calls not inlined: 428
     */
    enum t_cose_err_t           return_value;
    USEFUL_BUF_MAKE_STACK_UB(   buffer_for_cose_key,
                                    MAX_ENCODED_COSE_KEY_SIZE);
    struct useful_buf_c         cose_key;

    /* Doing the COSE encoding and the hashing in separate functions
     * called from here reduces the stack usage in this function by a
     * lot
     */

    /* Get the key and encode it as a COSE_Key  */
    return_value = t_cose_encode_cose_key(key_select,
                                          buffer_for_cose_key,
                                          &cose_key);
    if(return_value != T_COSE_SUCCESS) {
        goto Done;
    }

    /* SHA256 hash of it is all we care about in the end */
    return_value = quick_sha256(cose_key, buffer_for_key_id, key_id);

Done:
    return return_value;
}


/**
 * \brief  Makes the protected headers for COSE.
 *
 * \param[in] cose_alg_id  The COSE algorithm ID to put in the headers.
 *
 * \param[in] buffer_for_header  Pointer and length into which
 *                               the resulting encoded protected
 *                               headers is put.
 *
 * \return The pointer and length of the protected headers is
 * returned, or \c NULL_USEFUL_BUF_C if this fails.
 *
 * The protected headers are returned in fully encoded CBOR format as
 * they are added to the \c COSE_Sign1 as a binary string. This is
 * different from the unprotected headers which are not handled this
 * way.
 *
 * This returns \c NULL_USEFUL_BUF_C if buffer_for_header was too
 * small. See also definition of \ref T_COSE_SIGN1_MAX_PROT_HEADER
 */
static inline struct useful_buf_c
make_protected_header(int32_t cose_alg_id,
                      struct useful_buf buffer_for_header)
{
    /* approximate stack use on 32-bit machine:
     * local use: 170
     * with calls: 210
     */
    struct useful_buf_c protected_headers;
    QCBORError          qcbor_result;
    QCBOREncodeContext  cbor_encode_ctx;
    struct useful_buf_c return_value;

    QCBOREncode_Init(&cbor_encode_ctx, buffer_for_header);
    QCBOREncode_OpenMap(&cbor_encode_ctx);
    QCBOREncode_AddInt64ToMapN(&cbor_encode_ctx,
                               COSE_HEADER_PARAM_ALG,
                               cose_alg_id);
    QCBOREncode_CloseMap(&cbor_encode_ctx);
    qcbor_result = QCBOREncode_Finish(&cbor_encode_ctx, &protected_headers);

    if(qcbor_result == QCBOR_SUCCESS) {
        return_value = protected_headers;
    } else {
        return_value = NULL_USEFUL_BUF_C;
    }

    return return_value;
}


/**
 * \brief Add the unprotected headers to a CBOR encoding context
 *
 * \param[in] cbor_encode_ctx  CBOR encoding context to output to
 * \param[in] kid              The key ID to go into the kid header.
 *
 * No error is returned. If an error occurred it will be returned when
 * \c QCBOR_Finish() is called on \c cbor_encode_ctx.
 *
 * The unprotected headers added by this are just the key ID
 */
static inline void add_unprotected_headers(QCBOREncodeContext *cbor_encode_ctx,
                                           struct useful_buf_c kid)
{
    QCBOREncode_OpenMap(cbor_encode_ctx);
    QCBOREncode_AddBytesToMapN(cbor_encode_ctx, COSE_HEADER_PARAM_KID, kid);
    QCBOREncode_CloseMap(cbor_encode_ctx);
}


/*
 * Public function. See t_cose_sign1_sign.h
 */
enum t_cose_err_t t_cose_sign1_init(struct t_cose_sign1_ctx *me,
                                    bool short_circuit_sign,
                                    int32_t cose_alg_id,
                                    int32_t key_select,
                                    QCBOREncodeContext *cbor_encode_ctx)
{
    /* approximate stack use on 32-bit machine:
     * local use: 66
     * with calls inlined: 900
     * with calls not inlined: 500
     */

    int32_t                     hash_alg;
    enum t_cose_err_t           return_value;
    USEFUL_BUF_MAKE_STACK_UB(   buffer_for_kid, T_COSE_CRYPTO_SHA256_SIZE);
    struct useful_buf_c         kid;
    struct useful_buf           buffer_for_protected_header;

    /* Check the cose_alg_id now by getting the hash alg as an early
     error check even though it is not used until later. */
    hash_alg = hash_alg_id_from_sig_alg_id(cose_alg_id);
    if(hash_alg == INT32_MAX) {
        return T_COSE_ERR_UNSUPPORTED_SIGNING_ALG;
    }

    /* Remember all the parameters in the context */
    me->cose_algorithm_id   = cose_alg_id;
    me->key_select          = key_select;
    me->short_circuit_sign  = short_circuit_sign;
    me->cbor_encode_ctx     = cbor_encode_ctx;

    /* Get the key id because it goes into the headers that are about
     to be made. */
    if(short_circuit_sign) {
        return_value = get_short_circuit_kid(buffer_for_kid, &kid);
    } else {
        return_value = get_keyid(key_select, buffer_for_kid, &kid);
    }
    if(return_value) {
        goto Done;
    }

    /* Get started with the tagged array that holds the four parts of
     a cose single signed message */
    QCBOREncode_AddTag(cbor_encode_ctx, CBOR_TAG_COSE_SIGN1);
    QCBOREncode_OpenArray(cbor_encode_ctx);

    /* The protected headers, which are added as a wrapped bstr  */
    buffer_for_protected_header =
        USEFUL_BUF_FROM_BYTE_ARRAY(me->buffer_for_protected_headers);
    me->protected_headers = make_protected_header(cose_alg_id,
                                                  buffer_for_protected_header);
    if(useful_buf_c_is_null(me->protected_headers)) {
        /* The sizing of storage for protected headers is
          off (should never happen in tested, released code) */
        return_value = T_COSE_SUCCESS;
        goto Done;
    }
    QCBOREncode_AddBytes(cbor_encode_ctx, me->protected_headers);

    /* The Unprotected headers */
    add_unprotected_headers(cbor_encode_ctx, kid);

    /* Any failures in CBOR encoding will be caught in finish
     when the CBOR encoding is closed off. No need to track
     here as the CBOR encoder tracks it internally. */

    return_value = T_COSE_SUCCESS;

Done:
    return return_value;
}


/*
 * Public function. See t_cose_sign1_sign.h
 */
enum t_cose_err_t t_cose_sign1_finish(struct t_cose_sign1_ctx *me,
                                      struct useful_buf_c signed_payload)
{
    /* approximate stack use on 32-bit machine:
     *   local use: 116
     * with calls inline: 500
     * with calls not inlined; 450
     */
    enum t_cose_err_t          return_value;
    /* pointer and length of the completed tbs hash */
    struct useful_buf_c        tbs_hash;
    /* Pointer and length of the completed signature */
    struct useful_buf_c        signature;
    /* Buffer for the actual signature */
    USEFUL_BUF_MAKE_STACK_UB(  buffer_for_signature,
                                   T_COSE_MAX_EC_SIG_SIZE);
    /* Buffer for the tbs hash. Only big enough for SHA256 */
    USEFUL_BUF_MAKE_STACK_UB(  buffer_for_tbs_hash,
                                   T_COSE_CRYPTO_SHA256_SIZE);

    /* Create the hash of the to-be-signed bytes. Inputs to the hash
     * are the protected headers, the payload that getting signed, the
     * cose signature alg from which the hash alg is determined. The
     * cose_algorithm_id was checked in t_cose_sign1_init() so it
     * doesn't need to be checked here.
     */
    return_value = create_tbs_hash(me->cose_algorithm_id,
                                   buffer_for_tbs_hash,
                                   &tbs_hash,
                                   me->protected_headers,
                                   signed_payload);
    if(return_value) {
        goto Done;
    }

    /* Compute the signature using public key crypto. The key selector
     * and algorithm ID are passed in to know how and what to sign
     * with. The hash of the TBS bytes are what is signed. A buffer in
     * which to place the signature is passed in and the signature is
     * returned.
     *
     * Short-circuit signing is invoked if requested. It does no
     * public key operation and requires no key. It is just a test
     * mode that always works.
     */
    if(me->short_circuit_sign) {
        return_value = short_circuit_sign(me->cose_algorithm_id,
                                          tbs_hash,
                                          buffer_for_signature,
                                          &signature);
    } else {
        return_value = t_cose_crypto_pub_key_sign(me->cose_algorithm_id,
                                                  me->key_select,
                                                  tbs_hash,
                                                  buffer_for_signature,
                                                  &signature);
    }
    if(return_value) {
        goto Done;
    }

    /* Add signature to CBOR and close out the array */
    QCBOREncode_AddBytes(me->cbor_encode_ctx, signature);
    QCBOREncode_CloseArray(me->cbor_encode_ctx);

    /* CBOR encoding errors are tracked in the CBOR encoding context
     and handled in the layer above this */

Done:
    return return_value;
}
