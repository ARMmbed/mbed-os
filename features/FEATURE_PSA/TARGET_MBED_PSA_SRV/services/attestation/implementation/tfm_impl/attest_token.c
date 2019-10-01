/*
 * attest_token.c
 *
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#include "attest_token.h"
#include "qcbor.h"
#include "t_cose_sign1_sign.h"


/**
 * \file attest_token.c
 *
 * \brief Attestation token creation implementation
 *
 * Outline of token creation. Much of this occurs inside
 * t_cose_sign1_init() and t_cose_sign1_finish().
 *
 * - Create encoder context
 * - Open the CBOR array that hold the \c COSE_Sign1
 * - Write COSE Headers
 *   - Protected Header
 *      - Algorithm ID
 *   - Unprotected Headers
 *     - Key ID
 * - Open payload bstr
 *   - Write payload data… lots of it…
 *   - Get bstr that is the encoded payload
 * - Compute signature
 *   - Create a separate encoder context for \c Sig_structure
 *     - Encode CBOR context identifier
 *     - Encode protected headers
 *     - Encode two empty bstr
 *     - Add one more empty bstr that is a "fake payload"
 *     - Close off \c Sig_structure
 *   - Hash all but "fake payload" of \c Sig_structure
 *   - Get payload bstr ptr and length
 *   - Continue hash of the real encoded payload
 *   - Run ECDSA
 * - Write signature into the CBOR output
 * - Close CBOR array holding the \c COSE_Sign1
 */

/*
 * \brief Map t_cose error to attestation token error.
 *
 * \param[in] err   The t_cose error to map.
 *
 * \return the attestation token error.
 *
 */
static enum attest_token_err_t t_cose_err_to_attest_err(enum t_cose_err_t err)
{
    switch(err) {

    case T_COSE_SUCCESS:
        return ATTEST_TOKEN_ERR_SUCCESS;

    case T_COSE_ERR_UNSUPPORTED_HASH:
        return ATTEST_TOKEN_ERR_HASH_UNAVAILABLE;

    default:
        /* A lot of the errors are not mapped because they are
         * primarily internal errors that should never happen. They
         * end up here.
         */
        return ATTEST_TOKEN_ERR_GENERAL;
    }
}


/*
 Public function. See attest_token.h
 */
enum attest_token_err_t attest_token_start(struct attest_token_ctx *me,
                                           uint32_t opt_flags,
                                           int32_t key_select,
                                           int32_t cose_alg_id,
                                           const struct useful_buf *out_buf)
{
    /* approximate stack usage on 32-bit machine: 4 bytes */
    enum t_cose_err_t cose_return_value;
    enum attest_token_err_t return_value;

    /* Remember some of the configuration values */
    me->opt_flags  = opt_flags;
    me->key_select = key_select;

    /* Spin up the CBOR encoder */
    QCBOREncode_Init(&(me->cbor_enc_ctx), *out_buf);


    /* Initialize COSE signer. This will cause the cose headers to be
     * encoded and written into out_buf using me->cbor_enc_ctx
     */
    cose_return_value = t_cose_sign1_init(&(me->signer_ctx),
                                          opt_flags &
                                            TOKEN_OPT_SHORT_CIRCUIT_SIGN,
                                          cose_alg_id,
                                          key_select,
                                          &(me->cbor_enc_ctx));
    if(cose_return_value) {
        return_value = t_cose_err_to_attest_err(cose_return_value);
        goto Done;
    }

    /* Open the payload-wrapping bstr */
    QCBOREncode_BstrWrap(&(me->cbor_enc_ctx));

    QCBOREncode_OpenMap(&(me->cbor_enc_ctx));

    return_value = ATTEST_TOKEN_ERR_SUCCESS;

Done:
    return return_value;
}


/*
 Public function. See attest_token.h
 */
QCBOREncodeContext *attest_token_borrow_cbor_cntxt(struct attest_token_ctx *me)
{
    return &(me->cbor_enc_ctx);
}


/*
 Public function. See attest_token.h
 */
void attest_token_add_integer(struct attest_token_ctx *me,
                              int32_t label,
                              int64_t Value)
{
    QCBOREncode_AddInt64ToMapN(&(me->cbor_enc_ctx), label, Value);
}


/*
 Public function. See attest_token.h
 */
void attest_token_add_bstr(struct attest_token_ctx *me,
                           int32_t label,
                           const struct useful_buf_c *bstr)
{
    QCBOREncode_AddBytesToMapN(&(me->cbor_enc_ctx),
                               label,
                               *bstr);
}


/*
 Public function. See attest_token.h
 */
void attest_token_add_tstr(struct attest_token_ctx *me,
                           int32_t label,
                           const struct useful_buf_c *tstr)
{
    QCBOREncode_AddTextToMapN(&(me->cbor_enc_ctx), label, *tstr);
}


/*
 See attest_token.h
 */
void attest_token_add_encoded(struct attest_token_ctx *me,
                              int32_t label,
                              const struct useful_buf_c *encoded)
{
    QCBOREncode_AddEncodedToMapN(&(me->cbor_enc_ctx), label, *encoded);
}


/*
 Public function. See attest_token.h
 */
enum attest_token_err_t
attest_token_finish(struct attest_token_ctx *me,
                    struct useful_buf_c *completed_token)
{
    /* approximate stack usage on 32-bit machine: 4 + 4 + 8 + 8 = 24 */
    enum attest_token_err_t return_value = ATTEST_TOKEN_ERR_SUCCESS;
    /* The payload with all the claims that is signed */
    struct useful_buf_c     token_payload_ub;
    /* The completed and signed encoded cose_sign1 */
    struct useful_buf_c     completed_token_ub;
    QCBORError              qcbor_result;
    enum t_cose_err_t       cose_return_value;

    QCBOREncode_CloseMap(&(me->cbor_enc_ctx));

    /* Close off the payload-wrapping bstr. This gives us back the
     * pointer and length of the payload that needs to be hashed as
     * part of the signature
     */
    QCBOREncode_CloseBstrWrap(&(me->cbor_enc_ctx), &token_payload_ub);

    /* Finish off the cose signature. This does all the interesting work of
     hashing and signing */
    cose_return_value =
        t_cose_sign1_finish(&(me->signer_ctx), token_payload_ub);
    if(cose_return_value) {
        /* Main errors are invoking the hash or signature */
        return_value = t_cose_err_to_attest_err(cose_return_value);
        goto Done;
    }

    /* Close off the CBOR encoding and return the completed token */
    qcbor_result = QCBOREncode_Finish(&(me->cbor_enc_ctx),
                                      &completed_token_ub);
    if(qcbor_result == QCBOR_ERR_BUFFER_TOO_SMALL) {
        return_value = ATTEST_TOKEN_ERR_TOO_SMALL;
    } else if (qcbor_result != QCBOR_SUCCESS) {
        /* likely from array not closed, too many closes, ... */
        return_value = ATTEST_TOKEN_ERR_CBOR_FORMATTING;
    } else {
        *completed_token = completed_token_ub;
    }

Done:
    return return_value;
}

