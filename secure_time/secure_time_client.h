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
#ifndef __SECURE_TIME_CLIENT_H__
#define __SECURE_TIME_CLIENT_H__

#include <stdint.h>
#include <stdlib.h>

/** @addtogroup Secure-Time-API
 *  The C interface for setting and getting secure time.
 *  All functions are blocking.
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Error codes */
#define SECURE_TIME_SUCCESS                             (0UL)
#define SECURE_TIME_BAD_PARAMS                          (-1L)
#define SECURE_TIME_INVALID_BLOB_SIZE                   (-2L)
#define SECURE_TIME_SIGNATURE_VERIFICATION_FAILED       (-3L)
#define SECURE_TIME_NONCE_MISSING                       (-4L)
#define SECURE_TIME_NONCE_NOT_MATCH                     (-5L)
#define SECURE_TIME_NONCE_TIMEOUT                       (-6L)
#define SECURE_TIME_NOT_ALLOWED                         (-7L)

/**
 * Initialize secure time setting from a trusted time source.
 * 
 * The function generates 64-bit nonce that will be used in the next invocation
 * of ::secure_time_set_trusted_commit() to verify the trusted time blob freshness.
 *
 * @param[out] nonce       The generated nonce.
 * @return 0 or negative error code if failed.
 */
int32_t secure_time_set_trusted_init(uint64_t *nonce);

/**
 * Set the secure time from a trusted time source.
 * 
 * The time is encapsulated inside blob which is signed with the trusted time
 * sources' private key.
 * The blob will be verified with the trusted time sources' public key.
 * The signatuire to verify is located within the blob's data.
 *
 * @param[in] blob       Buffer which holds the blob.
 * @param[in] blob_size  Size in bytes of blob.
 * @return 0 or negative error code if failed.
 */
int32_t secure_time_set_trusted_commit(const void *blob, size_t blob_size);

/**
 * Set the secure time from an arbitrary time source.
 *
 * @param[in] new_time  Time value in seconds since EPOCH.
 * @return 0 or negative error code if failed.
 */
int32_t secure_time_set(uint64_t new_time);

/**
 * Return the current secure-time value.
 * 
 * This function will return 0 in case secure time was never set.
 *
 * @return 64-bit value which can be:\n
 *         @a time in seconds since EPOCH.\n
 *         @a 0 in case of en error
 */
uint64_t secure_time_get(void);

#ifdef __cplusplus
}
#endif

/** @}*/ // end of Secure-Time-API group

#endif // __SECURE_TIME_CLIENT_H__
