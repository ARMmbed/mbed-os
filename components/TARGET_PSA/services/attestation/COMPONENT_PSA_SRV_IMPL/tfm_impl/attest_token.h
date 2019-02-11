/*
 * attest_token.h
 *
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#ifndef __ATTEST_TOKEN_h__
#define __ATTEST_TOKEN_h__

#include <stdint.h>
#include "qcbor.h"
#include "t_cose_sign1.h"


/**
 * \file Attestation Token Creation
 *
 * The context and functions here are the way to create
 * an attestation token. The steps are roughly:
 *
 * 1) Creation and initalize an attest_token_ctx indicating
 * the options, key and such.

 * 2) Use various add methods to fill in the payload
 * with claims

 * 3) Call finish to create the signature and finish
 * formatting the COSE signed output.
 */


enum attest_token_err_t {
    ATTEST_TOKEN_ERR_SUCCESS = 0,
    /** The buffer passed in to receive the token is too small */
    ATTEST_TOKEN_ERR_TOO_SMALL,
    /** Something went wrong formatting the CBOR, most likely the payload
     has maps or arrays that are not closed */
    ATTEST_TOKEN_ERR_CBOR_FORMATTING,
    /** A general, unspecific error when creating the token */
    ATTEST_TOKEN_ERR_GENERAL,
    /** A hash function that is needed to make the token is not available */
    ATTEST_TOKEN_ERR_HASH_UNAVAILABLE
};


/** The default key to sign tokens with for this device
 */
#define TOKEN_OPT_DEFAULT_KEY  0x00


/** Sign with the globally known debug key. This key is
 * always available. It is always the same on every device.
 * It is very useful for test and debug. Tokens signed
 * by it have no security value because they can be forged
 * because this debug key is considered to be widely known.
 */
#define TOKEN_OPT_DEBUG_KEY    0x07

/** Request that the claims internally generated not be added to the token.
 * This is a test mode that results in a static token that never changes.
 */
#define TOKEN_OPT_OMIT_CLAIMS        0x40000000


/** A special test mode where a proper signature is not produced. In its place
 * there is a concatenation of hashes of the payload to be the same size as the
 * signature. This works and can be used to verify all of the SW stack except
 * the public signature part. The token has no security value in this mode
 * because anyone can replicate it. */
#define TOKEN_OPT_SHORT_CIRCUIT_SIGN 0x80000000


/**
 * The context for creating an attestation token.
 * The caller of attest_token must create one of these
 * and pass it to the functions here. It is small
 * enough that it can go on the stack. It is most
 * of the memory needed to create a token except
 * the output buffer and any memory requirements
 * for the cryptographic operations.
 *
 * The structure is opaque for the caller.
 *
 * This is roughly 148 + 8 + 32 = 188 bytes
 */
struct attest_token_ctx {
    /* Private data structure */
    QCBOREncodeContext      cbor_enc_ctx;
    uint32_t                opt_flags;
    int32_t                 key_select;
    struct t_cose_sign1_ctx signer_ctx;
};


/**
 * \brief Initalize a token creation context.
 *
 * \param[in] me          The token creation context to be initialized
 * \param[in] opt_flags   Flags to select different custom options
 * \param[in] key_select  Selects which attestation key to sign with
 * \param[in] alg_select  Ciphersuite-like designator signing and hash
 *                        algorithn to use
 * \param[out] out_buffer The output buffer to write the encoded token into
 *
 * \return (TODO: error codes)
 *
 * The size of the buffer in pOut (pOut->len) determines the size of the
 * token that can be created. It must be able to hold the final encoded
 * and signed token. The data encoding overhead is just that of
 * CBOR. The signing overhead depends on the signing key size. It is
 * about 150 bytes for ATTEST_TOKEN_CIPHERSUITE_256.
 *
 */
enum attest_token_err_t
attest_token_start(struct attest_token_ctx *me,
                   uint32_t opt_flags,
                   int32_t key_select,
                   int32_t alg_select,
                   struct useful_buf out_buffer);



/**
 * \brief Get a copy of the CBOR encoding context
 *
 * \param[in] me     Token Creation Context.
 *
 * \return The CBOR encoding context
 *
 * Allows the caller to encode CBOR right into
 * the output buffer using any of the QCBOREncode_AddXXXX()
 * methods. Anything added here will be part of the
 * payload that gets hashed. This can be used
 * to make complex CBOR structures. All open
 * arrays and maps must be close before calling
 * any other attest_token methods.
 * QCBOREncode_Finish() should not be closed on
 * this context.
 */
QCBOREncodeContext *attest_token_borrow_cbor_cntxt(struct attest_token_ctx *me);

/**
 * \brief Add a 64-bit signed integer claim
 *
 * \param[in] me     Token Creation Context.
 * \param[in] label  Integer label for claim.
 * \param[in] value  The integer claim data.
 */
void attest_token_add_integer(struct attest_token_ctx *me,
                              int32_t label,
                              int64_t value);

/**
 * \brief Add a binary string claim
 *
 * \param[in] me     Token Creation Context.
 * \param[in] label  Integer label for claim.
 * \param[in] value  The binary claim data.
 */
void attest_token_add_bstr(struct attest_token_ctx *me,
                           int32_t label,
                           struct useful_buf_c value);

/**
 * \brief Add a text string claim
 *
 * \param[in] me     Token Creation Context.
 * \param[in] label  Integer label for claim.
 * \param[in] value  The text claim data.
 */
void attest_token_add_tstr(struct attest_token_ctx *me,
                           int32_t label,
                           struct useful_buf_c value);

/**
 * \brief Add some already-encoded CBOR to payload
 *
 * \param[in] me       Token Creation Context.
 * \param[in] label    Integer label for claim.
 * \param[in] encoded  The already-encoded CBOR.
 *
 * Encoded CBOR must be a full map or full array
 * or a non-aggregate type. It cannot be a partial
 * map or array. It can be nested maps and
 * arrays, but they must all be complete.
 */
void attest_token_add_encoded(struct attest_token_ctx *me,
                              int32_t label,
                              struct useful_buf_c encoded);


/**
 * \brief Finish the token, complete the signing and get the result
 *
 * \param[in] me                Token Creation Context.
 * \param[out] completed_token  Pointer and length to completed token.
 *
 * \return One of the ATTEST_TOKEN_ERR_XXXX codes (TODO: fill all this in)
 *
 * This completes the token after the payload has been added. When this
 * is called the signing algorithm is run and the final formatting of
 * the token is completed.
 */
enum attest_token_err_t
attest_token_finish(struct attest_token_ctx *me,
                    struct useful_buf_c *completed_token);

#endif /* __ATTEST_TOKEN_h__ */
