/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

/**
 * \file arm_hal_aes.h
 * \brief AES Platform API
 */

#ifndef ARM_HAL_AES_H_
#define ARM_HAL_AES_H_

/* The network library requires a simple AES implementation for its
 * IEEE 802.15.4 security. The expectation is that 802.15.4 devices will have
 * some hardware acceleration, which should be made available through this API.
 *
 * If no hardware acceleration is available, the API can be easily hooked up to
 * an external software library such as mbed TLS.
 *
 * To simplify porting, the API is used single-threaded, so that no special care
 * is needed to maintain global context or state in the hardware accelerator.
 */

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Set the AES key
 *
 * This function sets the 128-bit AES key that will be used for future
 * calls to arm_aes_encrypt(). The key must be copied by the function.
 *
 * \param key pointer to 128-bit AES key
 */
void arm_aes_start(const uint8_t key[__static 16]);

/**
 * \brief This function performs dst=E[preset key,src] (Simple ECB block).
 *
 * This function performs a single-block AES encryption, using the preset key.
 * It is called between arm_aes_start() and arm_aes_finish().
 * Note that src and dst pointers may be equal.
 *
 * \param src pointer to 128-bit plaintext in
 * \param dst pointer for 128-bit ciphertext out
 */
extern void arm_aes_encrypt(
    const uint8_t src[__static 16],
    uint8_t dst[__static 16]);

/**
 * \brief Finish AES operations
 *
 * This function is called to terminate a series of AES operations.
 * It may be a no-op, or it may disable AES hardware. Use of the preset key is
 * no longer valid after this call.
 */
void arm_aes_finish(void);

#ifdef __cplusplus
}
#endif
#endif /* ARM_HAL_AES_H_ */
