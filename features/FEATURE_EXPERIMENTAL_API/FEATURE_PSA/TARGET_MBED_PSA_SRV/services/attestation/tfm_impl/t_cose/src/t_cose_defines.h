/*
 * t_cose_defines.h
 *
 * Copyright (c) 2018-2019, Laurence Lundblade. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in README.md
 */

#ifndef __T_COSE_DEFINES_H__
#define __T_COSE_DEFINES_H__

/**
 * \file t_cose_defines.h
 *
 * \brief Constants from COSE standard and IANA registry.
 *
 * This file contains constants identifiers defined in [COSE (RFC
 * 8152)] (https://tools.ietf.org/html/rfc8152) and [IANA COSE
 * Registry] (https://www.iana.org/assignments/cose/cose.xhtml). They
 * include algorithm IDs and other constants.
 *
 * Many constants in the IANA registry are not included here yet as
 * they are not needed by t_cose. They can be added if they become
 * needed.
 */




/* --------------- COSE Header parameters -----------
 * https://www.iana.org/assignments/cose/cose.xhtml#header-parameters
 */

/**
 * \def COSE_HEADER_PARAM_ALG
 *
 * \brief Label of COSE header that indicates an algorithm.
 */
#define COSE_HEADER_PARAM_ALG 1

/**
 * \def COSE_HEADER_PARAM_KID
 *
 * \brief Label of COSE header that contains a key ID.
 */
#define COSE_HEADER_PARAM_KID 4




/* ------------ COSE Header Algorithm Parameters --------------
 * https://www.iana.org/assignments/cose/cose.xhtml#header-algorithm-parameters
 *
 * None of these are defined here yet, as they are not needed by t_cose yet.
 */




/* ------------- COSE Algorithms ----------------------------
 * https://www.iana.org/assignments/cose/cose.xhtml#algorithms
 */

/**
 * \def COSE_ALGORITHM_ES256
 *
 * \brief Indicates ECDSA with SHA-256.
 *
 * Value for \ref COSE_HEADER_PARAM_ALG to indicate ECDSA.  w/SHA-256
 */
#define COSE_ALGORITHM_ES256 -7

/**
 * \def COSE_ALGORITHM_ES384
 *
 * \brief Indicates ECDSA with SHA-384.
 *
 * Value for \ref COSE_HEADER_PARAM_ALG to indicate ECDSA.  w/SHA-384
 */
#define COSE_ALGORITHM_ES384 -35

/**
 * \def COSE_ALGORITHM_ES512
 *
 * \brief Indicates ECDSA with SHA-384.
 *
 * Value for \ref COSE_HEADER_PARAM_ALG to indicate ECDSA.  w/SHA-512
 */
#define COSE_ALGORITHM_ES512 -36


/**
 * \def COSE_ALG_SHA256_PROPRIETARY
 *
 * \brief COSE-style algorithm ID for SHA256. The official COSE
 * algorithm registry doesn't yet define an ID for a pure hash
 * function. One is needed for internal use, so this is defined.
 *
 * This is only used internally in the implementation and doesn't
 * appear in any protocol messages so there are no interoperability
 * issues. When this gets defined in the IANA registry, that value can
 * be substituted here and all will work fine.
 */
#define COSE_ALG_SHA256_PROPRIETARY -72000




/* ---------- COSE Key Common Parameters --------------
 * https://www.iana.org/assignments/cose/cose.xhtml#key-common-parameters
 */

/**
 * \def COSE_KEY_COMMON_KTY
 *
 * \brief Label for data item containing the key type.
 *
 * In a \c COSE_Key, label that indicates the data item containing the
 * key type.
 */
#define COSE_KEY_COMMON_KTY  1

/**
 * \def COSE_KEY_COMMON_KID
 *
 * \brief Label for data item containing the key's kid.
 *
 * In a \c COSE_Key, label that indicates the data item containing the
 * kid of this key.
 */
#define COSE_KEY_COMMON_KID  2




/* ---------- COSE Key Type Parameters --------------------
 * https://www.iana.org/assignments/cose/cose.xhtml#key-type-parameters
 */

/**
 * \def COSE_KEY_PARAM_CRV
 *
 * \brief Label for data item indicating EC curve.
 *
 * In a \c COSE_Key that holds an EC key of either type \ref
 * COSE_KEY_TYPE_EC2 or \ref COSE_KEY_TYPE_OKP this labels the data
 * item with the EC curve for the key.
 */
#define COSE_KEY_PARAM_CRV           -1

/**
 * \def COSE_KEY_PARAM_X_COORDINATE
 *
 * \brief Label for data item that is an X coordinate of an EC key.
 *
 * In a \c COSE_Key that holds an EC key, this is label that indicates
 * the data item containing the X coordinate.
 *
 * This is used for both key types \ref COSE_KEY_TYPE_EC2 and \ref
 * COSE_KEY_TYPE_OKP.
 */
#define COSE_KEY_PARAM_X_COORDINATE  -2

/**
 * \def COSE_KEY_PARAM_Y_COORDINATE
 *
 * \brief Label for data item that is a y coordinate of an EC key.
 *
 * In a COSE_Key that holds an EC key, this is label that indicates
 * the data item containing the Y coordinate.
 *
 * This is used only for key type \ref COSE_KEY_TYPE_EC2.
 */
#define COSE_KEY_PARAM_Y_COORDINATE  -3

/**
 * \def COSE_KEY_PARAM_PRIVATE_D
 *
 * \brief Label for data item that is d, the private part of EC key.
 *
 * In a \c COSE_Key that holds an EC key, this is label that indicates
 * the data item containing the Y coordinate.
 *
 * This is used for both key types \ref COSE_KEY_TYPE_EC2 and \ref
 * COSE_KEY_TYPE_OKP.
 */
#define COSE_KEY_PARAM_PRIVATE_D  -4




/* ---------- COSE Key Types --------------------------------
 * https://www.iana.org/assignments/cose/cose.xhtml#key-type
 */

/**
 * \def COSE_KEY_TYPE_OKP
 *
 * \brief Key type is Octet Key Pair
 *
 * In a \c COSE_Key, this is a value of the data item labeled \ref
 * COSE_KEY_COMMON_KTY that indicates the \c COSE_Key is some sort of
 * key pair represented by some octets. It may or may not be an EC
 * key.
 */
#define COSE_KEY_TYPE_OKP       1

/**
 * \def COSE_KEY_TYPE_EC2
 *
 * \brief Key is a 2-parameter EC key.
 *
 * In a \c COSE_Key, this is a value of the data item labeled \ref
 * COSE_KEY_COMMON_KTY that indicates the \c COSE_Key is an EC key
 * specified with two coordinates, X and Y.
 */
#define COSE_KEY_TYPE_EC2       2

/**
 * \def COSE_KEY_TYPE_SYMMETRIC
 *
 * \brief Key is a symmetric key.
 *
 * In a \c COSE_Key, this is a value of the data item labeled \ref
 * COSE_KEY_COMMON_KTY that indicates the \c COSE_Key is a symmetric
 * key.
 */
#define COSE_KEY_TYPE_SYMMETRIC  4




/* ----------- COSE Elliptic Curves ---------------------
 * https://www.iana.org/assignments/cose/cose.xhtml#elliptic-curves
 */

/**
 * \def COSE_ELLIPTIC_CURVE_P_256
 *
 * \brief Key type for NIST P-256 key
 *
 * In a \c COSE_Key, this is a value of the data item labeled \ref
 * COSE_KEY_PARAM_CRV to indicate the NIST P-256 curve, also known as
 * secp256r1.
 *
 * This key type is always \ref COSE_KEY_TYPE_EC2.
 */
#define COSE_ELLIPTIC_CURVE_P_256 1

/**
 * \def COSE_ELLIPTIC_CURVE_P_384
 *
 * \brief Key type for NIST P-384 key
 *
 * In a \c COSE_Key, this is a value of the data item labeled \ref
 * COSE_KEY_PARAM_CRV to indicate the NIST P-384 curve, also known as
 * secp384r1.
 *
 * This key type is always \ref COSE_KEY_TYPE_EC2.
 */
#define COSE_ELLIPTIC_CURVE_P_384 2

/**
 * \def COSE_ELLIPTIC_CURVE_P_521
 *
 * \brief Key type for NIST P-521 key
 *
 * In a \c COSE_Key, this is a value of the data item labeled \ref
 * COSE_KEY_PARAM_CRV to indicate the NIST P-521 curve, also known as
 * secp521r1.
 */
#define COSE_ELLIPTIC_CURVE_P_521 3




/* ------- Constants from RFC 8152 ---------
 */

/**
 * \def COSE_SIG_CONTEXT_STRING_SIGNATURE1
 *
 * \brief This is a string constant used by COSE to label \c COSE_Sign1
 * structures. See RFC 8152, section 4.4.
 */
#define COSE_SIG_CONTEXT_STRING_SIGNATURE1 "Signature1"


#endif /* __T_COSE_DEFINES_H__ */
