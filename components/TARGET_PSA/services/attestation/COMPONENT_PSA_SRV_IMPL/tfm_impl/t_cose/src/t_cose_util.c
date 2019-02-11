//
//  t_cose_util.c
/*
 *  t_cose_common.h
 *
 * Copyright 2019, Laurence Lundblade
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.mdE.
 */

#include "t_cose_util.h"
#include "qcbor.h"
#include "t_cose_defines.h"
#include "t_cose_common.h"
#include "t_cose_crypto.h"


/*

 T H E  C O M M E N T S

 in this file are truthful, but not expansive,
 complete of formatted yet...

 */



/*
 Returns COSE ID of hash algorithm for a particular signature algorithm
 for COSE-defined algorithm IDs.

 returns INT32_MAX when sig alg is not known
 */
int32_t hash_alg_id_from_sig_alg_id(int32_t cose_sig_alg_id)
{
    /* if other hashes, particularly those that output bigger hashes
     are added here, various other parts of this code have
     to be changed to have larger buffers.
     */
    switch(cose_sig_alg_id) {

        case COSE_ALG_ES256:
            return COSE_ALG_SHA256_PROPRIETARY;

        default:
            return INT32_MAX;
    }
}


/*
 Format of TBS bytes

 Sig_structure = [
    context : "Signature" / "Signature1" / "CounterSignature",
    body_protected : empty_or_serialized_map,
    ? sign_protected : empty_or_serialized_map,
    external_aad : bstr,
    payload : bstr
 ]
 */


/*
 This is the size of the first part of the CBOR encoded
 TBS bytes. It is around 20 bytes. See create_tbs_hash()
 */
#define T_COSE_SIZE_OF_TBS \
    1 + /* For opening the array */ \
    sizeof(COSE_SIG_CONTEXT_STRING_SIGNATURE1) + /* "Signature1" */ \
    2 + /* Overhead for encoding string */ \
    T_COSE_SIGN1_MAX_PROT_HEADER + /* entire protected headers */ \
    3 * ( /* 3 NULL bstrs for fields not used */ \
        1 /* size of a NULL bstr */  \
    )

/*
 Creates the hash of the to-be-signed (TBS) bytes for COSE

 Possible errors:
 - Something gone wrong with hashing
 - Protected headers too big
 */

enum t_cose_err_t create_tbs_hash(int32_t cose_alg_id,
                                  struct useful_buf buffer_for_hash,
                                  struct useful_buf_c *hash,
                                  struct useful_buf_c protected_headers,
                                  struct useful_buf_c payload)
{
    /* approximate stack use on 32-bit machine:
     local use: 320
     with calls: 360

     */

    enum t_cose_err_t           return_value;
    QCBOREncodeContext          cbor_encode_ctx;
    UsefulBuf_MAKE_STACK_UB(    buffer_for_TBS_first_part, T_COSE_SIZE_OF_TBS);
    struct useful_buf_c         tbs_first_part;
    QCBORError                  qcbor_result;
    struct t_cose_crypto_hash   hash_ctx;
    int32_t                     hash_alg_id;

    /* This builds the CBOR-format to-be-signed bytes */
    QCBOREncode_Init(&cbor_encode_ctx, buffer_for_TBS_first_part);
    QCBOREncode_OpenArray(&cbor_encode_ctx);
    /* context */
    QCBOREncode_AddSZString(&cbor_encode_ctx,
                            COSE_SIG_CONTEXT_STRING_SIGNATURE1);
    /* body_protected */
    QCBOREncode_AddBytes(&cbor_encode_ctx,
                         protected_headers);
    /* sign_protected */
    QCBOREncode_AddBytes(&cbor_encode_ctx, NULL_USEFUL_BUF_C);
    /* external_aad */
    QCBOREncode_AddBytes(&cbor_encode_ctx, NULL_USEFUL_BUF_C);
    /* fake payload */
    QCBOREncode_AddBytes(&cbor_encode_ctx, NULL_USEFUL_BUF_C);
    QCBOREncode_CloseArray(&cbor_encode_ctx);

    /* get the result and convert it to struct useful_buf_c representation */
    qcbor_result = QCBOREncode_Finish(&cbor_encode_ctx, &tbs_first_part);
    if(qcbor_result) {
        /* Mainly means that the protected_headers were too big
         (which should never happen) */
        return_value = T_COSE_ERR_PROTECTED_HEADERS;
        goto Done;
    }

    /* Start the hashing */
    hash_alg_id = hash_alg_id_from_sig_alg_id(cose_alg_id);
    return_value = t_cose_crypto_hash_start(&hash_ctx, hash_alg_id);
    if(return_value) {
        goto Done;
    }

    /* Hash the first part of the TBS. Take all but the last two
     bytes. The last two bytes are the fake payload from above. It is
     replaced by the real payload which is hashed next. The fake
     payload is needed so the array count is right. This is one of the
     main things that make it possible to implement with one buffer
     for the whole cose sign1.
     */
    t_cose_crypto_hash_update(&hash_ctx,
                              useful_buf_head(tbs_first_part,
                                              tbs_first_part.len - 2));

    /* Hash the payload */
    t_cose_crypto_hash_update(&hash_ctx, payload);

    /* Finish the hash and set up to return it */
    return_value = t_cose_crypto_hash_finish(&hash_ctx,
                                             buffer_for_hash,
                                             hash);

Done:
    return return_value;
}
