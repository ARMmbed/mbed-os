/*
 *  t_cose_common.h
 *
 * Copyright 2019, Laurence Lundblade
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.mdE.
 */


#ifndef __T_COSE_COMMON_H__
#define __T_COSE_COMMON_H__


/*

 T H E  C O M M E N T S

 in this file are truthful, but not expansive,
 complete of formatted yet...

 */

/* Private value. Intentionally not documented for Doxygen.
 * This is the space allocated for the encoded protected headers.  It
 * needs to be big enough for make_protected_header() to succeed. It
 * currently sized for one header with an algorithm ID up to 32 bits
 * long -- one byte for the wrapping map, one byte for the label, 5
 * bytes for the ID. If this is made accidentially too small, QCBOR will
 * only return an error, and not overrun any buffers.
 */
#define T_COSE_SIGN1_MAX_PROT_HEADER (1+1+5)


/* TODO: document this */
enum t_cose_err_t {
    T_COSE_SUCCESS = 0,
    T_COSE_ERR_UNKNOWN_SIGNING_ALG,
    T_COSE_ERR_PROTECTED_HEADERS,
    T_COSE_ERR_UNSUPPORTED_HASH,
    T_COSE_ERR_HASH_GENERAL_FAIL,
    T_COSE_ERR_HASH_BUFFER_SIZE,
    T_COSE_ERR_SIG_BUFFER_SIZE,
    T_COSE_ERR_KEY_BUFFER_SIZE,
    T_COSE_ERR_FORMAT,
    T_COSE_ERR_CBOR_NOT_WELL_FORMED,
    T_COSE_ERR_NO_ALG_ID,
    T_COSE_ERR_NO_KID,
    T_COSE_ERR_SIG_VERIFY
};




/*
 CBOR Label for proprietary header indicating short-circuit
 signing was used. Just a random number in the proprietary
 label space */
#define T_COSE_SHORT_CIRCUIT_LABEL -8675309


#endif /* __T_COSE_COMMON_H__ */
