/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#ifndef MBED_CRYPTO_MISC_H
#define MBED_CRYPTO_MISC_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Init/Uninit crypto module */
void crypto_init(void);
void crypto_uninit(void);

void crypto_zeroize(void *v, size_t n);

/* Acquire/release ownership of AES H/W */
/* NOTE: If "acquire" succeeds, "release" must be done to pair it. */
bool crypto_aes_acquire(void);
void crypto_aes_release(void);

/* Acquire/release ownership of DES H/W */
/* NOTE: If "acquire" succeeds, "release" must be done to pair it. */
bool crypto_des_acquire(void);
void crypto_des_release(void);

/* Acquire/release ownership of SHA H/W */
/* NOTE: If "acquire" succeeds, "release" must be done to pair it. */
bool crypto_sha_acquire(void);
void crypto_sha_release(void);

/* Check if buffer can be used for crypto DMA. It requires to be:
 *
 * 1) Word-aligned
 * 2) Located in 0x20000000-0x2FFFFFFF region
 */
bool crypto_dma_buff_compat(const void *buff, size_t buff_size);

#ifdef __cplusplus
}
#endif

#endif
