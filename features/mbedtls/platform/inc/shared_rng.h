/*
 *  shared_rng.h
 *
 *  Copyright (C) 2019-2020, Arm Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef SHARED_RNG_H
#define SHARED_RNG_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SSL_CONF_RNG)

#define MBED_SHARED_RNG_NOT_INITIALIZED -1  /**< init_global_rng not called before global_rng */

#ifdef __cplusplus
extern "C" {
#endif

#include "mbedtls/hmac_drbg.h"
#include "mbedtls/entropy.h"

/**
 * \brief       Initializes hmac ready for rng
 *
 * \return      0 if successful, or
 *              MBEDTLS_ERR_MD_BAD_INPUT_DATA, or
 *              MBEDTLS_ERR_MD_ALLOC_FAILED, or
 *              MBEDTLS_ERR_HMAC_DRBG_ENTROPY_SOURCE_FAILED.
 */
int init_global_rng();

/**
 * \brief       Global HMAC_DRBG generate random
 *
 * \note        Automatically reseeds if reseed_counter is reached or PR is enabled.
 * \note        init_global_rng function must be called
 *              before calling this function!
 *
 * \param ctx   DRBG context
 * \param dst   Buffer to fill
 * \param len   Length of the buffer
 *
 * \return      0 if successful, or
 *              MBEDTLS_ERR_HMAC_DRBG_ENTROPY_SOURCE_FAILED, or
 *              MBEDTLS_ERR_HMAC_DRBG_REQUEST_TOO_BIG or
 *              MBED_SHARED_RNG_NOT_INITIALIZED
 */
int global_rng( void *ctx, unsigned char *dst, size_t len );

/**
 * \brief       Free allocated resources
 */
void free_global_rng();

/**
 * \brief       Getter function for global hmac context
 *
 * \return      global hmac context
 */
mbedtls_hmac_drbg_context *get_global_hmac_drbg();

/**
 * \brief       Getter function for global entropy context
 *
 * \return      global entropy context
 */
mbedtls_entropy_context *get_global_entropy();

#ifdef __cplusplus
}
#endif

#endif // MBEDTLS_SSL_CONF_RNG
#endif // SHARED_RNG_H
