/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file  psa_sst_asset_defs.h
 *
 * \brief Platform security architecture (PSA) asset definitions for
 *        secure storage partition.
 */

#ifndef __PSA_SST_ASSET_DEFS_H__
#define __PSA_SST_ASSET_DEFS_H__

#include <inttypes.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/* List of PSA SST asset category types */
/*!
 * \def PSA_SST_ASSET_CAT_TYPE_NONE
 *
 * \brief Asset category type NONE. This is value used in an asset when the
 *        asset category type is not set.
 */
#define PSA_SST_ASSET_CAT_TYPE_NONE            ((uint32_t)0x00000000)

/*!
 * \def PSA_SST_ASSET_CAT_RAW_DATA
 *
 * \brief This category type is used for those assets which contains raw data.
 */
#define PSA_SST_ASSET_CAT_RAW_DATA             ((uint32_t)0x02000000)

/*!
 * \def PSA_SST_ASSET_CAT_KEY_SYMMETRIC
 *
 * \brief This category type is used for those assets which contains a
 *        symmetric key.
 */
#define PSA_SST_ASSET_CAT_KEY_SYMMETRIC        ((uint32_t)0x04000000)

/*!
 * \def PSA_SST_ASSET_CAT_KEY_PUBLIC
 *
 * \brief This category type is used for those assets which contains an
 *        asymmetric public key.
 */
#define PSA_SST_ASSET_CAT_KEY_PUBLIC           ((uint32_t)0x06000000)

/*!
 * \def PSA_SST_ASSET_CAT_KEY_PRIVATE
 *
 * \brief This category type is used for those assets which contains an
 *        asymmetric private key.
 */
#define PSA_SST_ASSET_CAT_KEY_PRIVATE          ((uint32_t)0x07000000)

/*!
 * \def PSA_SST_ASSET_CAT_CERTIFICATE
 *
 * \brief This category type is used for those assets which contains a
 *        certificate.
 */
#define PSA_SST_ASSET_CAT_CERTIFICATE          ((uint32_t)0x08000000)

/*!
 * \def PSA_SST_ASSET_CAT_VENDOR_DEFINED
 *
 * \brief This category type is used when assets is vendor defined.
 */
#define PSA_SST_ASSET_CAT_VENDOR_DEFINED       ((uint32_t)0x80000000)

/* PSA SST asset category type masks */
/*!
 * \def PSA_SST_ASSET_CAT_TYPE_MASK
 *
 * \brief Asset category type mask.
 */
#define PSA_SST_ASSET_CAT_TYPE_MASK            ((uint32_t)0x7f000000)

/*!
 * \def PSA_SST_ASSET_CAT_KEY_MASK
 *
 * \brief Asset key category mask.
 */
#define PSA_SST_ASSET_CAT_KEY_MASK             ((uint32_t)0x04000000)

/*!
 * \def PSA_SST_ASSET_CAT_KEY_ASYMMETRIC_MASK
 *
 * \brief Asset asymmetric key mask.
 */
#define PSA_SST_ASSET_CAT_KEY_ASYMMETRIC_MASK  ((uint32_t)0x06000000)


/* Asset types for raw data category */
/*!
 * \def PSA_SST_ASSET_KEY_HMAC
 *
 * \brief HMAC key.
 */
#define PSA_SST_ASSET_KEY_HMAC    ((PSA_SST_ASSET_CAT_RAW_DATA)| 0x00000001)

/* Asset types for symmetric key category */
/*!
 * \def PSA_SST_ASSET_KEY_AES
 *
 * \brief AES key to be used in algorithms based on AES cipher
 *        (cipher, AEAD or MAC).
 */
#define PSA_SST_ASSET_KEY_AES     ((PSA_SST_ASSET_CAT_KEY_SYMMETRIC)|0x00000001)

/*!
 * \def PSA_SST_ASSET_KEY_DES
 *
 * \brief DES key to be used in algorithms based on DES or 3DES cipher
 *        (cipher or MAC).
 */
#define PSA_SST_ASSET_KEY_DES     ((PSA_SST_ASSET_CAT_KEY_SYMMETRIC)|0x00000002)

/*!
 * \def PSA_SST_ASSET_KEY_CAMELLIA
 *
 * \brief Camellia key to be used in algorithms based on Camellia cipher
 *        (cipher, AEAD or MAC).
 */
#define PSA_SST_ASSET_KEY_CAMELLIA \
                                  ((PSA_SST_ASSET_CAT_KEY_SYMMETRIC)|0x00000003)

/* Asset types for asymmetric key category */
/*!
 * \def PSA_SST_ASSET_KEY_RSA_PUBLIC
 *
 * \brief RSA public key.
 */
#define PSA_SST_ASSET_KEY_RSA_PUBLIC ((PSA_SST_ASSET_CAT_KEY_PUBLIC)|0x00010000)

/*!
 * \def PSA_SST_ASSET_KEY_RSA_PRIVATE
 *
 * \brief RSA private key.
 */
#define PSA_SST_ASSET_KEY_RSA_PRIVATE \
                                    ((PSA_SST_ASSET_CAT_KEY_PRIVATE)|0x00010000)

/*!
 * \def PSA_SST_ASSET_KEY_DSA_PUBLIC
 *
 * \brief DSA public key.
 */
#define PSA_SST_ASSET_KEY_DSA_PUBLIC ((PSA_SST_ASSET_CAT_KEY_PUBLIC)|0x00020000)

/*!
 * \def PSA_SST_ASSET_KEY_DSA_PRIVATE
 *
 * \brief DSA private key.
 */
#define PSA_SST_ASSET_KEY_DSA_PRIVATE \
                                    ((PSA_SST_ASSET_CAT_KEY_PRIVATE)|0x00020000)

/*!
 * \def PSA_SST_ASSET_KEY_ECC_PUBLIC
 *
 * \brief ECC public key.
 */
#define PSA_SST_ASSET_KEY_ECC_PUBLIC ((PSA_SST_ASSET_CAT_KEY_PUBLIC)|0x00030000)

/*!
 * \def PSA_SST_ASSET_KEY_ECC_PRIVATE
 *
 * \brief ECC private key.
 */
#define PSA_SST_ASSET_KEY_ECC_PRIVATE \
                                    ((PSA_SST_ASSET_CAT_KEY_PRIVATE)|0x00030000)

/*!
 * \def PSA_SST_ASSET_KEY_ECC_CURVE_MASK
 *
 * \brief ECC curve mask. This mask allows to encode the curve identifiers which
 *        are aligned with the TLS Supported Groups Registry (formerly known as
 *        the  TLS EC Named Curve Registry)
 *        https://www.iana.org/assignments/tls-parameters/tls-parameters.xhtml
 *        The values are defined by RFC 4492, RFC 7027 and RFC 7919.
 */
#define PSA_SST_ASSET_KEY_ECC_CURVE_MASK ((uint32_t)0x0000ffff)

/* Asset types for certificate category */
/*!
 * \def PSA_SST_ASSET_CERT_X_509
 *
 * \brief X509 certificate.
 */
#define PSA_SST_ASSET_CERT_X_509 ((PSA_SST_ASSET_CAT_CERTIFICATE)|0x00000001)

/* List of PSA SST asset */
/*!
 * \def PSA_SST_ASSET_ATTR_EXPIRED
 *
 * \brief Indicates if asset has expired.
 */
#define PSA_SST_ASSET_ATTR_EXPIRED              ((uint32_t)0x00000001)

/*!
 * \def PSA_SST_ASSET_ATTR_GENERATED_LOCALLY
 *
 * \brief Indicates if asset content has been generated locally.
 */
#define PSA_SST_ASSET_ATTR_GENERATED_LOCALLY    ((uint32_t)0x00000002)

/*!
 * \def PSA_SST_ASSET_ATTR_DERIVATION
 *
 * \brief Indicates if asset content can be used in a key derivation (i.e.
 *        if other keys can be derived from this one)
 *
 */
#define PSA_SST_ASSET_ATTR_DERIVATION           ((uint32_t)0x00000004)

/*!
 * \def PSA_SST_ASSET_ATTR_TRUSTED
 *
 * \brief Indicates if asset content can be trusted.
 */
#define PSA_SST_ASSET_ATTR_TRUSTED              ((uint32_t)0x00000008)

/*!
 * \def PSA_SST_ASSET_ATTR_WRITE_ONCE
 *
 * \brief Indicates if asset content can be modified once a content has been
 *        set.
 */
#define PSA_SST_ASSET_ATTR_WRITE_ONCE           ((uint32_t)0x00000010)

/* The next 3 bits are reserved */

/*!
 * \def PSA_SST_ASSET_ATTR_ENCRYPT
 *
 * \brief Indicates if asset content may be used to encrypt data.
 */
#define PSA_SST_ASSET_ATTR_ENCRYPT              ((uint32_t)0x00000100)

/*!
 * \def PSA_SST_ASSET_ATTR_DECRYPT
 *
 * \brief Indicates if asset content may be used to decrypt data.
 */
#define PSA_SST_ASSET_ATTR_DECRYPT              ((uint32_t)0x00000200)

/*!
 * \def PSA_SST_ASSET_ATTR_SIGN
 *
 * \brief Indicates if asset content may be used to sign data.
 */
#define PSA_SST_ASSET_ATTR_SIGN                 ((uint32_t)0x00000400)

/*!
 * \def PSA_SST_ASSET_ATTR_VERIFY
 *
 * \brief Indicates if asset content may be used to verify signed data.
 */
#define PSA_SST_ASSET_ATTR_VERIFY               ((uint32_t)0x00000800)

/*!
 * \def PSA_SST_ASSET_ATTR_VERIFY_RECOVER
 *
 * \brief Indicates if asset content may be used to verify data where
 *        data is recovered from the signature.
 */
#define PSA_SST_ASSET_ATTR_VERIFY_RECOVER       ((uint32_t)0x00001000)

/*!
 * \def PSA_SST_ASSET_ATTR_WRAP
 *
 * \brief Indicates if asset content may be used to wrap other data
 *        (i.e wrap other keys).
 */
#define PSA_SST_ASSET_ATTR_WRAP                 ((uint32_t)0x00002000)

/*!
 * \def PSA_SST_ASSET_ATTR_UNWRAP
 *
 * \brief Indicates if asset content may be used to unwrap other data
 *        (i.e unwrap other keys).
 */
#define PSA_SST_ASSET_ATTR_UNWRAP               ((uint32_t)0x00004000)

/*!
 * \def PSA_SST_ASSET_ATTR_SENSITIVE
 *
 * \brief Indicates if asset content is sensitive and cannot be revealed.
 */
#define PSA_SST_ASSET_ATTR_SENSITIVE            ((uint32_t)0x00008000)

/*!
 * \def PSA_SST_ASSET_ATTR_ALWAYS_SENSITIVE
 *
 * \brief Indicates if asset has always had the sensitive attribute. Attribute
 *        cannot be changed once set. It becomes a read only attribute.
 */
#define PSA_SST_ASSET_ATTR_ALWAYS_SENSITIVE     ((uint32_t)0x00010000)

/*!
 * \def PSA_SST_ASSET_ATTR_EXTRACTABLE
 *
 * \brief Indicates if asset content is extractable and can be wrapped.
 */
#define PSA_SST_ASSET_ATTR_EXTRACTABLE          ((uint32_t)0x00020000)

/*!
 * \def PSA_SST_ASSET_ATTR_NEVER_EXTRACTABLE
 *
 * \brief Indicates if asset has never had the extactable attribute. Attribute
 *        cannot be changed once set. It becomes a read only attribute.
 */
#define PSA_SST_ASSET_ATTR_NEVER_EXTRACTABLE    ((uint32_t)0x00040000)

/*!
 * \def PSA_SST_ASSET_ATTR_ALWAYS_AUTHENTICATE
 *
 * \brief Indicates if user has to be authenticate when the asset content has
 *        to be used.
 */
#define PSA_SST_ASSET_ATTR_ALWAYS_AUTHENTICATE  ((uint32_t)0x00080000)

/*!
 * \def PSA_SST_ASSET_ATTR_MASK
 *
 * \brief Asset attributes mask.
 */
#define PSA_SST_ASSET_ATTR_MASK                 ((uint32_t)0x7FFFFFFF)

/*!
 * \def PSA_SST_ASSET_ATTR_VENDOR_DEFINED
 *
 * \brief Indicates if the attribute is vendor defined.
 */
#define PSA_SST_ASSET_ATTR_VENDOR_DEFINED       ((uint32_t)0x80000000)


/* PSA SST asset structure definitions */
/*!
 * \struct psa_sst_asset_info_t
 *
 * \brief Asset information.
 */
struct psa_sst_asset_info_t {
    uint32_t type;          /*!< Asset type */
    uint32_t size_current;  /*!< The current size of the asset content */
    uint32_t size_max;      /*!< The maximum size of the asset content in
                             *   bytes
                             */
};

/*!
 * \struct psa_sst_asset_validity_t
 *
 * \brief Asset validity structure.
 *
 * \note  Start and end values must be set to 0 as they are reserved for future
 *        use.
 */
struct psa_sst_asset_validity_t {
    uint64_t  start; /*!< Start date/time on which the asset validity period
                      *   begins
                      */
    uint64_t  end;   /*!< End date on which the asset validity period ends */

};

/*!
 * \struct psa_sst_asset_attrs_t
 *
 * \brief Asset attributes structure.
 *
 */
struct psa_sst_asset_attrs_t {
    struct psa_sst_asset_validity_t validity; /*!< Asset validity period */
    uint32_t attrs;                           /*!< Asset attributes */
    uint8_t  reserved[4];                     /*!< Reserved for future
                                               *   expansion. Default value is
                                               *   0 in all bytes
                                               */
};

/*!
 * \def PSA_SST_ASSET_INFO_SIZE
 *
 * \brief Indicates the asset information structure size.
 */
#define PSA_SST_ASSET_INFO_SIZE sizeof(struct psa_sst_asset_info_t)

/*!
 * \def PSA_SST_ASSET_ATTR_SIZE
 *
 * \brief Indicates the asset attributes structure size.
 */
#define PSA_SST_ASSET_ATTR_SIZE sizeof(struct psa_sst_asset_attrs_t)

#ifdef __cplusplus
}
#endif

#endif /* __PSA_SST_ASSET_DEFS_H__ */
