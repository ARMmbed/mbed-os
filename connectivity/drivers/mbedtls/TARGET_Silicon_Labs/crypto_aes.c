/*
 *  FIPS-197 compliant AES implementation
 *
 *  Copyright (C) 2017, Silicon Labs, http://www.silabs.com
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
 */

/*
 * This file includes alternative plugin implementations of various
 * functions in aes.c using the CRYPTO hardware accelerator incorporated
 * in MCU devices from Silicon Laboratories.
 */

/*
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#include "mbedtls/aes.h"
#include "em_device.h"

#if defined(CRYPTO_PRESENT)
#if defined(MBEDTLS_AES_C)
#if defined(MBEDTLS_AES_ALT)

#include "crypto_management.h"
#include "em_crypto.h"
#include "em_core.h"
#include <string.h>

__STATIC_INLINE void CRYPTO_DataReadUnaligned(volatile uint32_t * reg,
                                              uint8_t * const val)
{
  /* Check data is 32bit aligned, if not, read into temporary buffer and
     then move to user buffer. */
  if ((uint32_t)val & 0x3)
  {
    uint32_t temp[4];
    CRYPTO_DataRead(reg, temp);
    memcpy(val, temp, 16);
  }
  else
  {
    CRYPTO_DataRead(reg, (uint32_t* const)val);
  }
}

__STATIC_INLINE void CRYPTO_DataWriteUnaligned(volatile uint32_t * reg,
                                               uint8_t * const val)
{
  /* Check data is 32bit aligned, if not move to temporary buffer before
     writing.*/
  if ((uint32_t)val & 0x3)
  {
    uint32_t temp[4];
    memcpy(temp, val, 16);
    CRYPTO_DataWrite(reg, temp);
  }
  else
  {
    CRYPTO_DataWrite(reg, (uint32_t* const)val);
  }
}

/*
 * Initialize AES context
 */
void mbedtls_aes_init( mbedtls_aes_context *ctx )
{
    if( ctx == NULL ) {
        return;
    }

    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
}

/*
 * Clear AES context
 */
void mbedtls_aes_free( mbedtls_aes_context *ctx )
{
    if( ctx == NULL ) {
        return;
    }

    memset( ctx, 0, sizeof( mbedtls_aes_context ) );
}

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx,
                            const unsigned char *key,
                            unsigned int keybits )
{
    if( ctx == NULL || key == NULL ) {
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( ( 128UL != keybits ) && ( 256UL != keybits ) ) {
        /* Unsupported key size */
        return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    ctx->keybits = keybits;
    memcpy(ctx->key, key, keybits/8);

    return 0;
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx,
                            const unsigned char *key,
                            unsigned int keybits )
{
    CORE_DECLARE_IRQ_STATE;

    if( ctx == NULL || key == NULL ) {
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( ( 128UL != keybits ) && ( 256UL != keybits ) ) {
        /* Unsupported key size */
        return( MBEDTLS_ERR_AES_INVALID_KEY_LENGTH );
    }

    ctx->keybits = keybits;

    CRYPTO_TypeDef *device = crypto_management_acquire();
    device->WAC = 0;
    device->CTRL = 0;

    CORE_ENTER_CRITICAL();
    CRYPTO_KeyBufWrite(device, (uint32_t*)key, (keybits == 128) ? cryptoKey128Bits : cryptoKey256Bits);
    CORE_EXIT_CRITICAL();

    /* Busy-wait here to allow context-switching to occur */
    device->CMD = CRYPTO_CMD_INSTR_AESENC;
    while ((device->STATUS & CRYPTO_STATUS_INSTRRUNNING) != 0);

    CORE_ENTER_CRITICAL();
    CRYPTO_KeyRead(device, (uint32_t*)ctx->key, (keybits == 128) ? cryptoKey128Bits : cryptoKey256Bits);
    CORE_EXIT_CRITICAL();

    crypto_management_release(device);

    return 0;
}

/* TODO: underneath these, we should swap out the em_crypto-provided library
 * functions with in-place implemented functions, to get much shorter
 * critical sections */

/*
 * AES-ECB block encryption
 */
int mbedtls_internal_aes_encrypt( mbedtls_aes_context *ctx,
                                   const unsigned char input[16],
                                   unsigned char output[16] )
{
    return mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, input, output);
}

/*
 * AES-ECB block decryption
 */
int mbedtls_internal_aes_decrypt( mbedtls_aes_context *ctx,
                                   const unsigned char input[16],
                                   unsigned char output[16] )
{
    return mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_DECRYPT, input, output);
}

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb( mbedtls_aes_context *ctx,
                           int mode,
                           const unsigned char input[16],
                           unsigned char output[16] )
{
    int ret = 0;
    CORE_DECLARE_IRQ_STATE;

    if( ctx == NULL || input == NULL || output == NULL ) {
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( ctx->keybits != 128UL && ctx->keybits != 256UL) {
        return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    CRYPTO_TypeDef *device = crypto_management_acquire();
    device->WAC = 0;
    device->CTRL = 0;

    CORE_ENTER_CRITICAL();
    CRYPTO_KeyBufWrite(device, (uint32_t*)ctx->key, (ctx->keybits == 128UL) ? cryptoKey128Bits : cryptoKey256Bits);
    CRYPTO_DataWriteUnaligned(&device->DATA0, (uint8_t *)input);
    CORE_EXIT_CRITICAL();

    if ( mode == MBEDTLS_AES_ENCRYPT ) {
        device->CMD = CRYPTO_CMD_INSTR_AESENC;
    } else {
        device->CMD = CRYPTO_CMD_INSTR_AESDEC;
    }
    while ((device->STATUS & CRYPTO_STATUS_INSTRRUNNING) != 0);

    CORE_ENTER_CRITICAL();
    CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)output);
    CORE_EXIT_CRITICAL();

    crypto_management_release(device);

    return ret;
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)

/*
 * AES-CBC buffer encryption/decryption
 */
int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                           int mode,
                           size_t length,
                           unsigned char iv[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    int ret = 0;
    CORE_DECLARE_IRQ_STATE;
    size_t processed = 0;

    if( ctx == NULL || input == NULL || output == NULL || iv == NULL ) {
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    /* Input length must be a multiple of 16 bytes which is the AES block
       length. */
    if( length & 0xf ) {
        return( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( ctx->keybits != 128UL && ctx->keybits != 256UL) {
        return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    CRYPTO_TypeDef *device = crypto_management_acquire();
    device->WAC = 0;
    device->CTRL = 0;

    CORE_ENTER_CRITICAL();
    CRYPTO_KeyBufWrite(device, (uint32_t*)ctx->key, (ctx->keybits == 128UL) ? cryptoKey128Bits : cryptoKey256Bits);
    if ( mode == MBEDTLS_AES_ENCRYPT ) {
        CRYPTO_DataWriteUnaligned(&device->DATA0, (uint8_t *)iv);
    } else {
        CRYPTO_DataWriteUnaligned(&device->DATA2, (uint8_t *)iv);
    }
    CORE_EXIT_CRITICAL();

    while ( processed < length ) {
        if ( mode == MBEDTLS_AES_ENCRYPT ) {
            CORE_ENTER_CRITICAL();
            CRYPTO_DataWriteUnaligned(&device->DATA0XOR, (uint8_t *)(&input[processed]));
            device->CMD = CRYPTO_CMD_INSTR_AESENC;
            CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)(&output[processed]));
            CORE_EXIT_CRITICAL();
        } else {
            /* Decrypt input block, XOR IV to decrypted text, set ciphertext as next IV */
            CORE_ENTER_CRITICAL();
            CRYPTO_DataWriteUnaligned(&device->DATA0, (uint8_t *)(&input[processed]));
            CRYPTO_EXECUTE_4( device,
                              CRYPTO_CMD_INSTR_DATA0TODATA1,
                              CRYPTO_CMD_INSTR_AESDEC,
                              CRYPTO_CMD_INSTR_DATA2TODATA0XOR,
                              CRYPTO_CMD_INSTR_DATA1TODATA2);
            CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)(&output[processed]));
            CORE_EXIT_CRITICAL();
        }
        processed += 16;
    }

    if ( processed >= 16 ) {
        if ( mode == MBEDTLS_AES_ENCRYPT ) {
            memcpy(iv, &output[processed-16], 16);
        } else {
            CORE_ENTER_CRITICAL();
            CRYPTO_DataReadUnaligned(&device->DATA2, (uint8_t *)(iv));
            CORE_EXIT_CRITICAL();
        }
    }

    crypto_management_release(device);

    return ret;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb128( mbedtls_aes_context *ctx,
                              int mode,
                              size_t length,
                              size_t *iv_off,
                              unsigned char iv[16],
                              const unsigned char *input,
                              unsigned char *output )
{
    size_t n = iv_off ? *iv_off : 0;
    size_t processed = 0;
    int ret = 0;
    CORE_DECLARE_IRQ_STATE;

    if( ctx == NULL || input == NULL || output == NULL || iv == NULL ) {
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( ctx->keybits != 128UL && ctx->keybits != 256UL) {
        return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    while ( processed < length ) {
        if ( n > 0 ) {
            /* start by filling up the IV */
            if( mode == MBEDTLS_AES_ENCRYPT ) {
                iv[n] = output[processed] = (unsigned char)( iv[n] ^ input[processed] );
            } else {
                int c = input[processed];
                output[processed] = (unsigned char)( c ^ iv[n] );
                iv[n] = (unsigned char) c;
            }
            n = ( n + 1 ) & 0x0F;
            processed++;
            continue;
        } else {
            /* process one ore more blocks of data */
            CRYPTO_TypeDef *device = crypto_management_acquire();
            device->WAC = 0;
            device->CTRL = 0;

            CORE_ENTER_CRITICAL();
            CRYPTO_KeyBufWrite(device, (uint32_t*)ctx->key, (ctx->keybits == 128UL) ? cryptoKey128Bits : cryptoKey256Bits);
            CRYPTO_DataWriteUnaligned(&device->DATA0, (uint8_t *)iv);
            CORE_EXIT_CRITICAL();

            /* Encryption: encrypt IV, encIV xor input -> output and IV */
            /* Decryption: encrypt IV, encIV xor input -> output, input -> IV */
            size_t iterations = (length - processed) / 16;
            for (size_t i = 0; i < iterations; i++ ) {
                device->CMD = CRYPTO_CMD_INSTR_AESENC;
                while ((device->STATUS & CRYPTO_STATUS_INSTRRUNNING) != 0);

                CORE_ENTER_CRITICAL();
                if ( mode == MBEDTLS_AES_ENCRYPT ) {
                    CRYPTO_DataWriteUnaligned(&device->DATA0XOR, (uint8_t *)(&input[processed]));
                    CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)(&output[processed]));
                } else {
                    CRYPTO_DataWriteUnaligned(&device->DATA1, (uint8_t *)(&input[processed]));
                    device->CMD = CRYPTO_CMD_INSTR_DATA1TODATA0XOR;
                    CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)(&output[processed]));
                    device->CMD = CRYPTO_CMD_INSTR_DATA1TODATA0;
                }
                CORE_EXIT_CRITICAL();
                processed += 16;
            }

            CORE_ENTER_CRITICAL();
            CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)iv);
            CORE_EXIT_CRITICAL();

            while ( length - processed > 0 ) {
                if ( n == 0 ) {
                    device->CMD = CRYPTO_CMD_INSTR_AESENC;
                    while ((device->STATUS & CRYPTO_STATUS_INSTRRUNNING) != 0);
                    CORE_ENTER_CRITICAL();
                    CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)iv);
                    CORE_EXIT_CRITICAL();
                }
                /* Save remainder to iv */
                if( mode == MBEDTLS_AES_ENCRYPT ) {
                    iv[n] = output[processed] = (unsigned char)( iv[n] ^ input[processed] );
                } else {
                    int c = input[processed];
                    output[processed] = (unsigned char)( c ^ iv[n] );
                    iv[n] = (unsigned char) c;
                }
                n = ( n + 1 ) & 0x0F;
                processed++;
            }

            crypto_management_release(device);
        }
    }

    if ( iv_off ) {
        *iv_off = n;
    }

    return ret;
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                            int mode,
                            size_t length,
                            unsigned char iv[16],
                            const unsigned char *input,
                            unsigned char *output )
{
    unsigned char c;
    unsigned char ov[17];
    int ret = 0;

    if( ctx == NULL || input == NULL || output == NULL || iv == NULL ) {
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( ctx->keybits != 128UL && ctx->keybits != 256UL) {
        return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    while( length-- )
    {
        memcpy( ov, iv, 16 );
        if ( (ret = mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv ) ) != 0 ) {
            return ret;
        }

        if( mode == MBEDTLS_AES_DECRYPT )
            ov[16] = *input;

        c = *output++ = (unsigned char)( iv[0] ^ *input++ );

        if( mode == MBEDTLS_AES_ENCRYPT )
            ov[16] = c;

        memcpy( iv, ov + 1, 16 );
    }

    return ret;
}
#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                           size_t length,
                           size_t *nc_off,
                           unsigned char nonce_counter[16],
                           unsigned char stream_block[16],
                           const unsigned char *input,
                           unsigned char *output )
{
    size_t n = nc_off ? *nc_off : 0;
    size_t processed = 0;
    int ret = 0;
    CORE_DECLARE_IRQ_STATE;

    if( ctx == NULL || input == NULL || output == NULL || nonce_counter == NULL || stream_block == NULL ) {
        return ( MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH );
    }

    if ( ctx->keybits != 128UL && ctx->keybits != 256UL) {
        return MBEDTLS_ERR_AES_INVALID_KEY_LENGTH;
    }

    while ( processed < length ) {
        if ( n > 0 ) {
            /* start by filling up the IV */
            output[processed] = (unsigned char)( input[processed] ^ stream_block[n] );
            n = ( n + 1 ) & 0x0F;
            processed++;
            continue;
        } else {
            /* process one ore more blocks of data */
            CRYPTO_TypeDef *device = crypto_management_acquire();
            device->WAC = 0;
            device->CTRL = CRYPTO_CTRL_INCWIDTH_INCWIDTH4;

            CORE_ENTER_CRITICAL();
            CRYPTO_KeyBufWrite(device, (uint32_t*)ctx->key, (ctx->keybits == 128UL) ? cryptoKey128Bits : cryptoKey256Bits);
            CRYPTO_DataWriteUnaligned(&device->DATA1, (uint8_t *)nonce_counter);
            CORE_EXIT_CRITICAL();

            /* strategy: encrypt nonce, encNonce xor input -> output, inc(nonce) */
            size_t iterations = (length - processed) / 16;
            for (size_t i = 0; i < iterations; i++ ) {
                device->CMD = CRYPTO_CMD_INSTR_DATA1TODATA0;
                device->CMD = CRYPTO_CMD_INSTR_AESENC;
                while ((device->STATUS & CRYPTO_STATUS_INSTRRUNNING) != 0);
                device->CMD = CRYPTO_CMD_INSTR_DATA1INC;

                CORE_ENTER_CRITICAL();
                CRYPTO_DataWriteUnaligned(&device->DATA0XOR, (uint8_t *)(&input[processed]));
                CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)(&output[processed]));
                CORE_EXIT_CRITICAL();
                processed += 16;
            }

            while ( length - processed > 0 ) {
                if ( n == 0 ) {
                    device->CMD = CRYPTO_CMD_INSTR_DATA1TODATA0;
                    device->CMD = CRYPTO_CMD_INSTR_AESENC;
                    while ((device->STATUS & CRYPTO_STATUS_INSTRRUNNING) != 0);
                    device->CMD = CRYPTO_CMD_INSTR_DATA1INC;

                    CORE_ENTER_CRITICAL();
                    CRYPTO_DataReadUnaligned(&device->DATA0, (uint8_t *)stream_block);
                    CORE_EXIT_CRITICAL();
                }
                /* Save remainder to iv */
                output[processed] = (unsigned char)( input[processed] ^ stream_block[n] );
                n = ( n + 1 ) & 0x0F;
                processed++;
            }

            CORE_ENTER_CRITICAL();
            CRYPTO_DataReadUnaligned(&device->DATA1, (uint8_t *)nonce_counter);
            CORE_EXIT_CRITICAL();

            crypto_management_release(device);
        }
    }

    if ( nc_off ) {
        *nc_off = n;
    }

    return ret;
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_AES_ALT */
#endif /* MBEDTLS_AES_C */
#endif /* CRYPTO_PRESENT */
