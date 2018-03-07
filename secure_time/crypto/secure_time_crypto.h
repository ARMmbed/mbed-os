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
#ifndef __SECURE_TIME_CRYPTO_H__
#define __SECURE_TIME_CRYPTO_H__

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Verify the data buffer signature.
 *
 * @param[in] data         Data buffer.
 * @param[in] data_size    Size in bytes of buffer.
 * @param[in] sign         Data buffer's signature
 * @param[in] sign_size    Signature size
 * @param[in] pubkey       Buffer which holds the public key.
 * @param[in] pubkey_size  Size in bytes of the public key.
 * @return SECURE_TIME_SUCCESS or negative error code if failed.
 */
int32_t secure_time_verify_signature(
    const void *data,
    size_t data_size,
    const void *sign,
    size_t sign_size,
    const void *pubkey,
    size_t pubkey_size
    );

/*
 * Generate a sequence of random bytes.
 *
 * @param[in]  size         Size in bytes of the random buffer
 * @param[out] random_buf   Buffer to fill with the generated random bytes.
 */
void secure_time_generate_random_bytes(size_t size, void *random_buf);

#ifdef __cplusplus
}
#endif

#endif // __SECURE_TIME_CRYPTO_H__
