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

/*
 These are defined in the COSE standard, RFC 8152, or in the IANA
 registry for COSE at https://www.iana.org/assignments/cose/cose.xhtml
 */



/* COSE Header parameters:
   https://www.iana.org/assignments/cose/cose.xhtml#header-parameters
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


/* COSE Algorithms:
 * https://www.iana.org/assignments/cose/cose.xhtml#algorithms
 */

/**
 * \def COSE_ALG_ES256
 *
 * \brief Value for COSE_HEADER_PARAM_ALG to indicate ECDSA
 *  w/SHA-256
 */
#define COSE_ALG_ES256 -7



/**
 * \def COSE_SIG_CONTEXT_STRING_SIGNATURE1
 *
 * \brief This is a strong constant used by COSE to
 * label COSE_Sign1 structures. SeeRFC 8152, section 4.4.
 */
#define COSE_SIG_CONTEXT_STRING_SIGNATURE1 "Signature1"

/**
 * \def COSE_ALG_SHA256_PROPRIETARY
 *
 * \brief COSE-style algorithm ID for SHA256. The offical COSE
 * algorithm registry doesn't yet define an ID for a pure hash
 * function. One is needed for internal use, so this is defined.
 */
#define COSE_ALG_SHA256_PROPRIETARY -72000


/* From CBOR IANA registry,
 * https://www.iana.org/assignments/cbor-tags/cbor-tags.xhtml
 */

/**
 * \def COSE_KEY_TYPE_EC2
 *
 * \brief In a COSE_Key, this is a value of the
 * data item labeled COSE_KEY_KTY that indicates
 * the COSE_Key is an EC key specified with
 * two coordinates, X and Y.
 */
#define COSE_KEY_TYPE_EC2       2

/**
 * \def COSE_ELLIPTIC_CURVE_P_256
 *
 * \brief In a COSE_Key, this is a value of the
 * data item labeled COSE_KEY_CRV to indicate the
 * NIST P-256 curve.
 */
#define COSE_ELLIPTIC_CURVE_P_256 1


/**
 * \def COSE_KEY_KTY
 *
 * \brief In a COSE_Key, label that indicates
 * the data item containing the key type
 */
#define COSE_KEY_KTY            1

/**
 * \def COSE_KEY_CRV
 *
 * \brief In a COSE_Key that holds an EC key,
 * this is  label that indicates
 * the data item containing the specific curve.
 */
#define COSE_KEY_CRV           -1

/**
 * \def COSE_KEY_X_COORDINATE
 *
 * \brief In a COSE_Key that holds an EC key,
 * this is  label that indicates
 * the data item containing the X coordinate.
 */
#define COSE_KEY_X_COORDINATE  -2

/**
 * \def COSE_KEY_Y_COORDINATE
 *
 * \brief In a COSE_Key that holds an EC key,
 * this is  label that indicates
 * the data item containing the Y coordinate.
 */
#define COSE_KEY_Y_COORDINATE  -3


/**
 * \def  T_COSE_MAX_SIG_SIZE
 *
 * \brief Compile time constant for largest signature that can be handled.
 * Set at 64 bytes for ECDSA 256
 */
#define T_COSE_MAX_SIG_SIZE 64


/**
  * \brief Get the size in bytes of a particular signature type
  *
  * \param[in] cose_signature_algorithm  The COSE algorithm ID.
  *
  * \return The size in bytes of the signature
  */
static inline size_t t_cose_signature_size(int32_t cose_signature_algorithm)
{
    switch(cose_signature_algorithm) {
    case COSE_ALG_ES256:
        return 64; /* Size of an ECDSA 256 signature */
    default:
        return T_COSE_MAX_SIG_SIZE;
    }
}


#endif /* __T_COSE_DEFINES_H__ */
