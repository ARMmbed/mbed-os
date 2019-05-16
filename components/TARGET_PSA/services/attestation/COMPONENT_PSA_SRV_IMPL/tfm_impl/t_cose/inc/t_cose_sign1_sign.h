/*
 * t_cose_sign1_sign.h
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
#include <stdbool.h>
#include "qcbor.h"
#include "t_cose_common.h"


/**
 * \file t_cose_sign1_sign.h
 *
 * \brief Create a \c COSE_Sign1, usually for EAT or CWT Token.
 *
 * This creates a \c COSE_Sign1 in compliance with [COSE (RFC 8152)]
 * (https://tools.ietf.org/html/rfc8152). A \c COSE_Sign1 is a CBOR
 * encoded binary blob that contains headers, a payload and a
 * signature. Usually the signature is made with an EC signing
 * algorithm like ECDSA.
 *
 * This implementation is intended to be small and portable to
 * different OS's and platforms. Its dependencies are:
 * - QCBOR
 * - <stdint.h>, <string.h>, <stddef.h>
 * - Hash functions like SHA-256
 * - Signing functions like ECDSA
 *
 * There is a cryptographic adaptation layer defined in
 * t_cose_crypto.h.  An implementation can be made of the functions in
 * it for different platforms or OS's. This means that different
 * platforms and OS's may support only signing with a particular set
 * of algorithms.
 *
 * This \c COSE_Sign1 implementations is optimized for creating EAT
 * tokens.
 *
 * It should work for CWT and others use cases too. The main point of
 * the optimization is that only one output buffer is needed. There is
 * no need for one buffer to hold the payload and another to hold the
 * end result \c COSE_Sign1. The payload is encoded right into its final
 * place in the end result \c COSE_Sign1.
 */


/**
 * This is the context for creating a \c COSE_Sign1 structure. The caller
 * should allocate it and pass it to the functions here.  This is
 * about 32 bytes so it fits easily on the stack.
 */
struct t_cose_sign1_ctx {
    /* Private data structure */
    uint8_t             buffer_for_protected_headers[
                        T_COSE_SIGN1_MAX_PROT_HEADER];
    struct useful_buf_c protected_headers;
    int32_t             cose_algorithm_id;
    int32_t             key_select;
    bool                short_circuit_sign;
    QCBOREncodeContext *cbor_encode_ctx;
};


/**
 * \brief  Initialize to start creating a \c COSE_Sign1.
 *
 * \param[in] me                 The t_cose signing context.
 * \param[in] short_circuit_sign \c true to select special test mode.
 * \param[in] cose_algorithm_id  The algorithm to sign with. The IDs are
 *                               defined in [COSE (RFC 8152)]
 *                               (https://tools.ietf.org/html/rfc8152) or
 *                               in the [IANA COSE Registry]
 *                           (https://www.iana.org/assignments/cose/cose.xhtml).
 * \param[in] key_select         Which signing key to use.
 * \param[in] cbor_encode_ctx    The CBOR encoder context to output to.
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * It is possible to use this to compute the exact size of the
 * resulting token so the exact sized buffer can be allocated. To do
 * this initialize the \c cbor_encode_ctx with \c UsefulBufC that has
 * a \c NULL pointer and large length like \c UINT32_MAX. Then run the
 * normal token creation.  The result will have a NULL pointer and the
 * length of the token that would have been created. When this is run
 * like this, the cryptographic functions will not actually run, but
 * the size of their output will be taken into account.
 *
 * The key selection depends on the platform / OS.
 *
 * Which signing algorithms are supported depends on the platform/OS.
 * The header file t_cose_defines.h contains defined constants for
 * some of them. A typical example is \ref COSE_ALGORITHM_ES256 which
 * indicates ECDSA with the NIST P-256 curve and SHA-256.
 *
 * To use this, create a \c QCBOREncodeContext and initialize it with
 * an output buffer big enough to hold the payload and the COSE Sign 1
 * overhead. This overhead is about 30 bytes plus the size of the
 * signature and the size of the key ID.
 *
 * After the \c QCBOREncodeContext is initialized, call
 * t_cose_sign1_init() on it.
 *
 * Next call \c QCBOREncode_BstrWrap() to indicate the start of the
 * payload.
 *
 * Next call various \c QCBOREncode_Addxxxx() methods to create the
 * payload.
 *
 * Next call \c QCBOREncode_CloseBstrWrap() to indicate the end of the
 * payload. This will also return a pointer and length of the payload
 * that gets hashed.
 *
 * Next call t_cose_sign1_finish() with the pointer and length of the
 * payload.  This will do all the cryptography and complete the COSE
 * Sign1.
 *
 * Finally, call \c QCBOREncode_Finish() to get the pointer and length
 * of the complete token.
 *
 * This implements a special signing test mode called _short_
 * _circuit_ _signing_. This mode is useful when there is no signing
 * key available, perhaps because it has not been provisioned or
 * configured for the particular device. It may also be because the
 * public key cryptographic functions have not been connected up in
 * the cryptographic adaptation layer.
 *
 * It has no value for security at all. Data signed this way should
 * not be trusted as anyone can sign like this.
 *
 * In this mode the signature is the hash of that would normally be
 * signed by the public key algorithm. To make the signature the
 * correct size for the particular algorithm instances of the hash are
 * concatenated to pad it out.
 *
 * This mode is very useful for testing because all the code except
 * the actual signing algorithm is run exactly as it would if a proper
 * signing algorithm was run.
 *
 * The kid (Key ID) put in the unprotected headers is created as
 * follows. The EC public key is CBOR encoded as a \c COSE_Key as
 * defined in the COSE standard. That encoded CBOR is then
 * hashed with SHA-256. This is similar to key IDs defined in IETF
 * PKIX, but is based on COSE and CBOR rather than ASN.1.
 */
enum t_cose_err_t t_cose_sign1_init(struct t_cose_sign1_ctx *me,
                                    bool short_circuit_sign,
                                    int32_t cose_algorithm_id,
                                    int32_t key_select,
                                    QCBOREncodeContext *cbor_encode_ctx);


/**
 * \brief Finish creation of the \c COSE_Sign1.
 *
 * \param[in] me       The t_cose signing context.
 * \param[in] payload  The pointer and length of the payload.
 *
 * \return This returns one of the error codes defined by \ref t_cose_err_t.
 *
 * Call this to complete creation of a signed token started with
 * t_cose_sign1_init().
 *
 * This is when the signature algorithm is run.
 *
 * The payload parameter is used only to compute the hash for
 * signing. The completed \c COSE_Sign1 is retrieved from the \c
 * cbor_encode_ctx by calling \c QCBOREncode_Finish()
 */
enum t_cose_err_t t_cose_sign1_finish(struct t_cose_sign1_ctx *me,
                                      struct useful_buf_c payload);


#endif /* __T_COSE_SIGN1_H__ */
