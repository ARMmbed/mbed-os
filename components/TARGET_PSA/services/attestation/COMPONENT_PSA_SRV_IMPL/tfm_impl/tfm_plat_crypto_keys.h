/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLAT_CRYPTO_KEYS_H__
#define __TFM_PLAT_CRYPTO_KEYS_H__
/**
 * \note The interfaces defined in this file must be implemented for each
 *       SoC.
 */

#include <stdint.h>
#include "tfm_plat_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Elliptic curve key type identifiers according to RFC8152 (COSE encoding)
 * https://www.iana.org/assignments/cose/cose.xhtml#elliptic-curves
 */
enum ecc_curve_t {
    P_256        = 1,  /* NIST P-256 also known as secp256r1 */
    P_384        = 2,  /* NIST P-384 also known as secp384r1 */
    P_521        = 3,  /* NIST P-521 also known as secp521r1 */
    X25519       = 4,  /* X25519 for use with ECDH only      */
    X448         = 5,  /* X448 for use with ECDH only        */
    ED25519      = 6,  /* Ed25519 for use with EdDSA only    */
    ED448        = 7,  /* Ed448 for use with EdDSA only      */
};

/**
 * Structure definition to carry pointer and size information about an Elliptic
 * curve key which is stored in a buffer(key_buf) in raw format (without
 * encoding):
 *   - priv_key       Base address of the private key in key_buf. It must be
 *                    present on the device.
 *   - priv_key_size  Size of the private key in bytes.
 *   - pubx_key       Base address of x-coordinate of the public key in key_buf.
 *                    It can be empty, because it can be recomputed based on
 *                    private key.
 *   - pubx_key_size  Length of x-coordinate of the public key in key_buf.
 *                    It can be empty, because it can be recomputed based on
 *                    private key.
 *   - puby_key       Base address of y-coordinate of the public key in key_buf.
 *                    It can be empty, because either it can be recomputed based
 *                    on private key or some curve type works without it.
 *   - puby_key_size  Length of y-coordinate of the public key in key_buf.
 */
struct ecc_key_t {
    uint8_t  *priv_key;
    uint32_t  priv_key_size;
    uint8_t  *pubx_key;
    uint32_t  pubx_key_size;
    uint8_t  *puby_key;
    uint32_t  puby_key_size;
};

#define ECC_P_256_KEY_SIZE  (96u)  /* 3 x 32 = 96 bytes priv + pub-x + pub-y */

/**
 * \brief Gets hardware unique key for encryption
 *
 * \param[out] key   Buf to store the key in
 * \param[in]  size  Size of the buffer
 *
 * \return Returns error code specified in \ref tfm_plat_err_t
 */
enum tfm_plat_err_t tfm_plat_get_crypto_huk(uint8_t *key, uint32_t size);

/**
 * \brief Get the initial attestation key
 *
 * The device MUST contain an initial attestation key, which is used to sign the
 * token. Initial attestation service supports elliptic curve signing
 * algorithms. Device maker can decide whether store only the private key on the
 * device or store both (public and private) key. Public key can be recomputed
 * based on private key. Keys must be provided in raw format, just binary data
 * without any encoding (DER, COSE). Caller provides a buffer to copy all the
 * available key components to there. Key components must be copied after
 * each other to the buffer. The base address and the length of each key
 * component must be indicating in the corresponding field of ecc_key
 * (\ref struct ecc_key_t).
 * Curve_type indicates to which curve belongs the key.
 *
 *
 * Keys must be provided in
 *
 * \param[in/out]  key_buf     Buffer to store the initial attestation key.
 * \param[in]      size        Size of the buffer.
 * \param[out]     ecc_key     A structure to carry pointer and size information
 *                             about the initial attestation key, which is
 *                             stored in key_buf.
 * \param[out]     curve_type  The type of the EC curve, which the key belongs
 *                             to according to \ref ecc_curve_t
 *
 * \return Returns error code specified in \ref tfm_plat_err_t
 */
enum tfm_plat_err_t
tfm_plat_get_initial_attest_key(uint8_t          *key_buf,
                                uint32_t          size,
                                struct ecc_key_t *ecc_key,
                                enum ecc_curve_t *curve_type);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLAT_CRYPTO_KEYS_H__ */
