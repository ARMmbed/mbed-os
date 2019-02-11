/*
 * t_cose_sign1.h
 *
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#ifndef __T_COSE_SIGN1_H__
#define __T_COSE_SIGN1_H__

#include <stdint.h>
#include "qcbor.h"
#include "t_cose_common.h"



/**
 * This is the context for creating a COSE Sign1
 * structure. The caller should allocate it and
 * pass it to the functions here.
 * This is about 32 bytes.
 */
struct t_cose_sign1_ctx {
    /* Private data structure */
    uint8_t       buffer_for_protected_headers[T_COSE_SIGN1_MAX_PROT_HEADER];
    struct useful_buf_c protected_headers;
    int32_t             cose_algorithm_id;
    int32_t             key_select;
    bool                short_circuit_sign;
    QCBOREncodeContext *cbor_encode_ctx;
};



/**
 * \brief  Initialize to start creating a COSE_Sign1.
 *
 * \param[in] me                   The cose signing context.
 * \param[in] short_circuit_sign   Select special test mode.
 * \param[in] cose_algorithm_id    The signing alg ID from COSE IANA registry.
 * \param[in] key_select           Which signing key to use.
 * \param[in] cbor_encode_ctx      The CBOR encoder context to output to.
 *
 * \return Error code (TODO: error codes)
 *
 * This COSE_Sign1 implementations is optimized for creating EAT tokens. It
 * should work for CWT and others use cases too. The main point of the
 * optimization is that only one output buffer is needed. There is no need
 * for one buffer to hold the payload and another to hold the end result
 * COSE Sign 1. The payload can be encoded right into its final place in
 * the end result COSE Sign 1.
 *
 * To use this, create a QCBOREncodeContext and initialize it with an
 * output buffer big enough to hold the payload and the COSE Sign 1
 * overhead. This overhead is about 30 bytes plus the size of the
 * signatue and the size of the key ID.
 *
 * After the QCBOREncodeContext is initialized, call t_cose_sign1_init()
 * on it.
 *
 * Next call QCBOREncode_BstrWrap() to indicate the start of the paylod.
 *
 * Next call various QCBOREncode_Addxxxx methods to create the payload.
 *
 * Next call QCBOREncode_CloseBstrWrap() to indicate the end of the
 * payload. This will also return a pointer and length of the payload
 * that gets hashed.
 *
 * Next call t_cose_sign1_finish() with the pointer and length of the
 * payload.  This will do all the cryptography and complete the COSE
 * Sign1.
 *
 * Finally, call QCBOREncode_Finish() to get the pointer and length of
 * the complete token.
 */
enum t_cose_err_t t_cose_sign1_init(struct t_cose_sign1_ctx *me,
                                    bool short_circuit_sign,
                                    int32_t cose_algorithm_id,
                                    int32_t key_select,
                                    QCBOREncodeContext *cbor_encode_ctx);


/**
 * \brief Finish creation of the COSE_Sign1.
 *
 * \param[in] me       The cose signing context.
 * \param[in] payload  The pointer and length of the payload.
 *
 * \return One of the  T_COSE_ERR_XXX errors (TODO: detail the errors)
 *
 * This is when the signature algorithm is run.  The payload is
 * used only to compute the hash for signing. The completed COSE_Sign1
 * is retrieved from the cbor_encode_ctx by calling QCBOREncode_Finish()
 */
enum t_cose_err_t t_cose_sign1_finish(struct t_cose_sign1_ctx *me,
                                      struct useful_buf_c payload);


#endif /* __T_COSE_SIGN1_H__ */
