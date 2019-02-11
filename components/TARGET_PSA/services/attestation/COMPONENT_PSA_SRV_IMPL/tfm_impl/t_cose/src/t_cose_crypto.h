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


/*

 T H E  C O M M E N T S

 in this file are truthful, but not expansive,
 complete of formatted yet...

 */

/*
 A small wrapper around the pub key functions to a) map cose alg IDs to
 TF-M alg IDs, b) map crypto errors to t_cose errors, c) have inputs
 and outputs be struct useful_buf_c and struct useful_buf,
 d) handle key selection.
 */

enum t_cose_err_t
t_cose_crypto_pub_key_sign(int32_t cose_alg_id,
                           int32_t key_select,
                           struct useful_buf_c hash_to_sign,
                           struct useful_buf signature_buffer,
                           struct useful_buf_c *signature);



enum t_cose_err_t
t_cose_crypto_pub_key_verify(int32_t cose_alg_id,
                             int32_t key_select,
                             struct useful_buf_c hash_to_verify,
                             struct useful_buf_c signature);




/*
 Size of X and Y coord in 2 param style EC public key. Format
 is as defined in COSE RFC and http://www.secg.org/sec1-v2.pdf.

 No function to get private key because there is no need for it.
 The private signing key only needs to exist behind
 t_cose_crypto_pub_key_sign()
 */

/* This size well-known by public standards. Implementation should
 have a compile time cross check to be sure it matches
 */
#define T_COSE_CRYPTO_P256_COORD_SIZE 32

enum t_cose_err_t
t_cose_crypto_get_ec_pub_key(int32_t key_select,
                             int32_t *cose_curve_id,
                             struct useful_buf buf_to_hold_x_coord,
                             struct useful_buf buf_to_hold_y_coord,
                             struct useful_buf_c  *x_coord,
                             struct useful_buf_c  *y_coord);




/*
 A small wrapper around the hash function to a) map cose alg IDs to
 TF-M alg IDs, b) map crypto errors to t_cose errors, c) have inputs
 and outputs be struct useful_buf_c and struct useful_buf.
 */


struct t_cose_crypto_hash {
    /* Can't put actual size here without creating dependecy on actual
     hash implementation, so pick a fairly large and accommodating
     size. There are checks that it is big enough in the implementation
     so no risk of buffer overflow */
    uint8_t bytes[128];
};


/*
 These sizes are well-known and fixed. Do not want to include
 platform-dependent headers here and make this file platform
 dependent. There are compile-time checks in the implementation
 to make sure it matches the platform's size. It would be
 very weird if it didn't.
 */
#define T_COSE_CRYPTO_SHA256_SIZE 32


enum t_cose_err_t t_cose_crypto_hash_start(struct t_cose_crypto_hash *hash_ctx,
                                           int32_t cose_hash_alg_id);

void t_cose_crypto_hash_update(struct t_cose_crypto_hash *hash_ctx,
                               struct useful_buf_c data_to_hash);

enum t_cose_err_t
t_cose_crypto_hash_finish(struct t_cose_crypto_hash *hash_ctx,
                          struct useful_buf buffer_to_hold_result,
                          struct useful_buf_c *hash_result);

#endif /* __T_COSE_CRYPTO_H__ */
