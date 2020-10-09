/*
* Copyright (c) 2018-2019 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/** @addtogroup PSA-Attestation
 * @{
 */

#ifndef __PSA_INJECT_KEY_H__
#define __PSA_INJECT_KEY_H__

#include "psa/crypto.h"
#include <stdint.h>
#include <string.h>

#ifdef   __cplusplus
extern "C"
{
#endif

/**
 * \brief Generate or import a given key pair and export the public part in a binary format.
 *        Initial attestation key: Private key for ECDSA-P256 to sign initial attestation token.
 *                                 Attestation private key is a persistent key that saved to
 *                                 persistent storage with persistent storage id = 17.
 *
 * \param[in] key_data      Buffer containing the private key data if given.
 *                          It must conain the format described in the documentation
 *                          of psa_export_public_key() for
 *                          the chosen type.
 *                          In case of generate the private key - NULL will pass.
 * \param key_data_length   Size of the \p data buffer in bytes - must be 256 bits. in case key_data isn't NULL.
 *                          In case of private key generation - 0 will pass.
 * \param type              Key type -  must be a ECC key type
 *                          (a \c PSA_KEY_TYPE_ECC_KEYPAIR(PSA_ECC_CURVE_XXX) value).
 * \param[out] data         Buffer where the key data is to be written.
 * \param data_size         Size of the \p data buffer in bytes -
 *                          needs to be bigger then the max size of the public part.
 * \param[out] data_length  On success, the number of bytes
 *                          that make up the key data.
 *
 * \retval #PSA_SUCCESS
 *         Success.
 * \retval #PSA_ERROR_INVALID_HANDLE
 * \retval #PSA_ERROR_OCCUPIED_SLOT
 *         There is already a key in the specified slot.
 * \retval #PSA_ERROR_NOT_SUPPORTED
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 * \retval #PSA_ERROR_INSUFFICIENT_MEMORY
 * \retval #PSA_ERROR_INSUFFICIENT_ENTROPY
 * \retval #PSA_ERROR_COMMUNICATION_FAILURE
 * \retval #PSA_ERROR_HARDWARE_FAILURE
 * \retval #PSA_ERROR_TAMPERING_DETECTED
 * \retval #PSA_ERROR_BAD_STATE
 *         The library has not been previously initialized by psa_crypto_init().
 *         It is implementation-dependent whether a failure to initialize
 *         results in this error code.
 */
psa_status_t
psa_attestation_inject_key(const uint8_t *key_data,
                           size_t key_data_length,
                           psa_key_type_t type,
                           uint8_t *public_key_data,
                           size_t public_key_data_size,
                           size_t *public_key_data_length);

#ifdef   __cplusplus
}
#endif

/** @}*/ // PSA-Attestation

#endif /* __PSA_INJECT_KEY_H__ */
