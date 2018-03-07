/* Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __SECURE_TIME_CLIENT_SPE_H__
#define __SECURE_TIME_CLIENT_SPE_H__

#include <stdint.h>
#include <stdlib.h>

#include "secure_time_client.h"

/** @addtogroup Secure-Time-API-SPE
 * SPE only C interface for setting and getting secure time.
 * All functions are blocking.
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**< Maximal allowed blob size in bytes. */
#define SECURE_TIME_MAX_BLOB_SIZE_BYTES                 (10 * 1024UL)

/**< Timestamp size in bytes. */
#define SECURE_TIME_TIMESTAMP_SIZE_BYTES                (8UL)

/**< Nonce size in bytes. */
#define SECURE_TIME_NONCE_SIZE_BYTES                    (8UL)

/**< The size of delegation length in bytes. */
#define SECURE_TIME_DELEGATION_LENGTH_SIZE_BYTES        (2UL)

/**< The size of public key length field in bytes. */
#define SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES            (2UL)

/**< The size of signature length field in bytes. */
#define SECURE_TIME_SIGNATURE_LENGTH_SIZE_BYTES         (2UL)

/**< The size of public key length field in bytes. */
#define SECURE_TIME_PUBKEY_LENGTH_SIZE_BYTES            (2UL)

/**< The size of the constant length blob header. */
#define SECURE_TIME_BLOB_HEADER_SIZE_BYTES              \
    (                                                   \
    SECURE_TIME_TIMESTAMP_SIZE_BYTES +                  \
    SECURE_TIME_NONCE_SIZE_BYTES +                      \
    SECURE_TIME_DELEGATION_LENGTH_SIZE_BYTES            \
    )

/**< The location of the delegation length field in the blob. */
#define SECURE_TIME_DELEGATION_LENGTH_OFFSET            \
    (                                                   \
    SECURE_TIME_TIMESTAMP_SIZE_BYTES +                  \
    SECURE_TIME_NONCE_SIZE_BYTES                        \
    )

/**
 * Enumeration for the possible blob signature algorithms
 */
typedef enum signature_alg {
    SIGNATURE_ALG_INVALID = 0,      /**< Invalid algorithm type */
    SIGNATURE_ALG_SHA256_ECDSA = 1, /**< ECDSA on a SHA256 hash */
    SIGNATURE_ALG_MAX = SIGNATURE_ALG_SHA256_ECDSA
} SignatureAlg;

/**
 * Factory-setup provisioning of public key to be used by secure_time_set_trusted().
 * Defined as a weak function which by default tries to write the public key to NVStore.
 * If the user wants to provision the public key differently than this function needs
 * to be implemented by the user according to the provisioning method, as well as
 * secure_time_get_stored_public_key().
 *
 * @param[in] pubkey    Public key for blob verification.
 * @param[in] key_size  Size in bytes of public key.
 * @return 0 or negative error code if failed.
 */
int32_t secure_time_set_stored_public_key(const void* pubkey, size_t key_size);

/**
 * Return the previously-provisioned public key.
 * Defined as a weak function which by default tries to read the public key from NVStore.
 * If the user provisioned the public key differently (By implementing secure_time_set_stored_public_key())
 * than this function also needs to be implemented.
 *
 * @param[out] pubkey       Buffer to fill with the public key.
 * @param[in]  size         Size in bytes of the buffer.
 * @param[out] actual_size  Actual size in bytes of the returned public key.
 * @return 0 or negative error code if failed.
 */
int32_t secure_time_get_stored_public_key(uint8_t *pubkey, size_t size, size_t *actual_size);

/**
 * Return the size in bytes of the previously-provisioned public key.
 * Defined as a weak function which by default tries to read the public key from NVStore.
 * If the user provisioned the public key differently (By implementing secure_time_set_stored_public_key())
 * than this function also needs to be implemented.
 *
 * @param[out] actual_size  Actual size in bytes of the returned public key.
 * @return 0 or negative error code if failed.
 */
int32_t secure_time_get_stored_public_key_size(size_t *actual_size);

#ifdef __cplusplus
}
#endif

/** @}*/ // end of Secure-Time-API group

#endif  // __SECURE_TIME_CLIENT_SPE_H__
