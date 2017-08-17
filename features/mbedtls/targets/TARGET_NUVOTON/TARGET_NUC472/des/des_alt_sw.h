/**
 * \file des.h
 *
 * \brief DES block cipher
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
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
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_DES_ALT_SW_H
#define MBEDTLS_DES_ALT_SW_H

#include "mbedtls/des.h"

#if defined(MBEDTLS_DES_C)
#if defined(MBEDTLS_DES_ALT)

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          DES context structure
 */
typedef struct {
    uint32_t sk[32];            /*!<  DES subkeys       */
}
mbedtls_des_sw_context;

/**
 * \brief          Triple-DES context structure
 */
typedef struct {
    uint32_t sk[96];            /*!<  3DES subkeys      */
}
mbedtls_des3_sw_context;

/**
 * \brief          Initialize DES context
 *
 * \param ctx      DES context to be initialized
 */
void mbedtls_des_sw_init( mbedtls_des_sw_context *ctx );

/**
 * \brief          Clear DES context
 *
 * \param ctx      DES context to be cleared
 */
void mbedtls_des_sw_free( mbedtls_des_sw_context *ctx );

/**
 * \brief          Initialize Triple-DES context
 *
 * \param ctx      DES3 context to be initialized
 */
void mbedtls_des3_sw_init( mbedtls_des3_sw_context *ctx );

/**
 * \brief          Clear Triple-DES context
 *
 * \param ctx      DES3 context to be cleared
 */
void mbedtls_des3_sw_free( mbedtls_des3_sw_context *ctx );

/**
 * \brief          Set key parity on the given key to odd.
 *
 *                 DES keys are 56 bits long, but each byte is padded with
 *                 a parity bit to allow verification.
 *
 * \param key      8-byte secret key
 */
void mbedtls_des_sw_key_set_parity( unsigned char key[MBEDTLS_DES_KEY_SIZE] );

/**
 * \brief          Check that key parity on the given key is odd.
 *
 *                 DES keys are 56 bits long, but each byte is padded with
 *                 a parity bit to allow verification.
 *
 * \param key      8-byte secret key
 *
 * \return         0 is parity was ok, 1 if parity was not correct.
 */
int mbedtls_des_sw_key_check_key_parity( const unsigned char key[MBEDTLS_DES_KEY_SIZE] );

/**
 * \brief          Check that key is not a weak or semi-weak DES key
 *
 * \param key      8-byte secret key
 *
 * \return         0 if no weak key was found, 1 if a weak key was identified.
 */
int mbedtls_des_sw_key_check_weak( const unsigned char key[MBEDTLS_DES_KEY_SIZE] );

/**
 * \brief          DES key schedule (56-bit, encryption)
 *
 * \param ctx      DES context to be initialized
 * \param key      8-byte secret key
 *
 * \return         0
 */
int mbedtls_des_sw_setkey_enc( mbedtls_des_sw_context *ctx, const unsigned char key[MBEDTLS_DES_KEY_SIZE] );

/**
 * \brief          DES key schedule (56-bit, decryption)
 *
 * \param ctx      DES context to be initialized
 * \param key      8-byte secret key
 *
 * \return         0
 */
int mbedtls_des_sw_setkey_dec( mbedtls_des_sw_context *ctx, const unsigned char key[MBEDTLS_DES_KEY_SIZE] );

/**
 * \brief          Triple-DES key schedule (112-bit, encryption)
 *
 * \param ctx      3DES context to be initialized
 * \param key      16-byte secret key
 *
 * \return         0
 */
int mbedtls_des3_sw_set2key_enc( mbedtls_des3_sw_context *ctx,
                                 const unsigned char key[MBEDTLS_DES_KEY_SIZE * 2] );

/**
 * \brief          Triple-DES key schedule (112-bit, decryption)
 *
 * \param ctx      3DES context to be initialized
 * \param key      16-byte secret key
 *
 * \return         0
 */
int mbedtls_des3_sw_set2key_dec( mbedtls_des3_sw_context *ctx,
                                 const unsigned char key[MBEDTLS_DES_KEY_SIZE * 2] );

/**
 * \brief          Triple-DES key schedule (168-bit, encryption)
 *
 * \param ctx      3DES context to be initialized
 * \param key      24-byte secret key
 *
 * \return         0
 */
int mbedtls_des3_sw_set3key_enc( mbedtls_des3_sw_context *ctx,
                                 const unsigned char key[MBEDTLS_DES_KEY_SIZE * 3] );

/**
 * \brief          Triple-DES key schedule (168-bit, decryption)
 *
 * \param ctx      3DES context to be initialized
 * \param key      24-byte secret key
 *
 * \return         0
 */
int mbedtls_des3_sw_set3key_dec( mbedtls_des3_sw_context *ctx,
                                 const unsigned char key[MBEDTLS_DES_KEY_SIZE * 3] );

/**
 * \brief          DES-ECB block encryption/decryption
 *
 * \param ctx      DES context
 * \param input    64-bit input block
 * \param output   64-bit output block
 *
 * \return         0 if successful
 */
int mbedtls_des_sw_crypt_ecb( mbedtls_des_sw_context *ctx,
                              const unsigned char input[8],
                              unsigned char output[8] );

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/**
 * \brief          DES-CBC buffer encryption/decryption
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      DES context
 * \param mode     MBEDTLS_DES_ENCRYPT or MBEDTLS_DES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 */
int mbedtls_des_sw_crypt_cbc( mbedtls_des_sw_context *ctx,
                              int mode,
                              size_t length,
                              unsigned char iv[8],
                              const unsigned char *input,
                              unsigned char *output );
#endif /* MBEDTLS_CIPHER_MODE_CBC */

/**
 * \brief          3DES-ECB block encryption/decryption
 *
 * \param ctx      3DES context
 * \param input    64-bit input block
 * \param output   64-bit output block
 *
 * \return         0 if successful
 */
int mbedtls_des3_sw_crypt_ecb( mbedtls_des3_sw_context *ctx,
                               const unsigned char input[8],
                               unsigned char output[8] );

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/**
 * \brief          3DES-CBC buffer encryption/decryption
 *
 * \note           Upon exit, the content of the IV is updated so that you can
 *                 call the function same function again on the following
 *                 block(s) of data and get the same result as if it was
 *                 encrypted in one call. This allows a "streaming" usage.
 *                 If on the other hand you need to retain the contents of the
 *                 IV, you should either save it manually or use the cipher
 *                 module instead.
 *
 * \param ctx      3DES context
 * \param mode     MBEDTLS_DES_ENCRYPT or MBEDTLS_DES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 *
 * \return         0 if successful, or MBEDTLS_ERR_DES_INVALID_INPUT_LENGTH
 */
int mbedtls_des3_sw_crypt_cbc( mbedtls_des3_sw_context *ctx,
                               int mode,
                               size_t length,
                               unsigned char iv[8],
                               const unsigned char *input,
                               unsigned char *output );
#endif /* MBEDTLS_CIPHER_MODE_CBC */

/**
 * \brief          Internal function for key expansion.
 *                 (Only exposed to allow overriding it,
 *                 see MBEDTLS_DES_SETKEY_ALT)
 *
 * \param SK       Round keys
 * \param key      Base key
 */
void mbedtls_des_sw_setkey( uint32_t SK[32],
                            const unsigned char key[MBEDTLS_DES_KEY_SIZE] );

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_DES_ALT */
#endif /* MBEDTLS_DES_C */

#endif /* des.h */
