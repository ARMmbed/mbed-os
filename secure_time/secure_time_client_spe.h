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

// secure_time needs TRNG and NVSTORE enabled (NVSTORE depends on FLASH)
#if !defined(DEVICE_TRNG) || !defined(DEVICE_FLASH)
#undef SECURE_TIME_ENABLED
#define SECURE_TIME_ENABLED 0
#endif

#if (SECURE_TIME_ENABLED) || defined(DOXYGEN_ONLY)

#include "secure_time_client.h"

/** @addtogroup Secure-Time-API-SPE
 * SPE only C interface for setting and getting secure time.
 * All functions are blocking.
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

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
int32_t secure_time_set_stored_public_key(const void *pubkey, size_t key_size);

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
int32_t secure_time_get_stored_public_key(void *pubkey, size_t size, size_t *actual_size);

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

#endif // (SECURE_TIME_ENABLED) || defined(DOXYGEN_ONLY)

#endif  // __SECURE_TIME_CLIENT_SPE_H__
