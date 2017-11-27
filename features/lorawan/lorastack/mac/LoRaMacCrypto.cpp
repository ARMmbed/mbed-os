/**
 *  / _____)             _              | |
 * ( (____  _____ ____ _| |_ _____  ____| |__
 *  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *  _____) ) ____| | | || |_| ____( (___| | | |
 * (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *    (C)2013 Semtech
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions===============
 *
 * Description: LoRa MAC crypto implementation
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jäckle ( STACKFORCE )
 *
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdlib.h>
#include <stdint.h>
#include "lorastack/mac/LoRaMacCrypto.h"
#include "lorawan/system/lorawan_data_structures.h"

#include "mbedtls/aes.h"
#include "mbedtls/cmac.h"

#if defined(MBEDTLS_CMAC_C) && defined(MBEDTLS_AES_C) && defined(MBEDTLS_CIPHER_C)

/**
 * CMAC/AES Message Integrity Code (MIC) Block B0 size
 */
#define LORAMAC_MIC_BLOCK_B0_SIZE                   16

/**
 * MIC field computation initial data
 */
static uint8_t MicBlockB0[] = { 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                              };

/**
 * Contains the computed MIC field.
 *
 * \remark Only the 4 first bytes are used
 */
static uint8_t Mic[16];

/**
 * Encryption aBlock and sBlock
 */
static uint8_t aBlock[] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                          };
static uint8_t sBlock[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                          };

/**
 * AES computation context variable
 */
static mbedtls_aes_context AesContext;

/**
 * CMAC computation context variable
 */
static mbedtls_cipher_context_t AesCmacCtx[1];

#define AES_CMAC_KEY_LENGTH     16

/**
 * \brief Computes the LoRaMAC frame MIC field
 *
 * \param [in]  buffer          Data buffer
 * \param [in]  size            Data buffer size
 * \param [in]  key             AES key to be used
 * \param [in]  address         Frame address
 * \param [in]  dir             Frame direction [0: uplink, 1: downlink]
 * \param [in]  sequenceCounter Frame sequence counter
 * \param [out] mic Computed MIC field
 */
int LoRaMacComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t address, uint8_t dir, uint32_t sequenceCounter, uint32_t *mic )
{

    int ret = 0;

    MicBlockB0[5] = dir;

    MicBlockB0[6] = ( address ) & 0xFF;
    MicBlockB0[7] = ( address >> 8 ) & 0xFF;
    MicBlockB0[8] = ( address >> 16 ) & 0xFF;
    MicBlockB0[9] = ( address >> 24 ) & 0xFF;

    MicBlockB0[10] = ( sequenceCounter ) & 0xFF;
    MicBlockB0[11] = ( sequenceCounter >> 8 ) & 0xFF;
    MicBlockB0[12] = ( sequenceCounter >> 16 ) & 0xFF;
    MicBlockB0[13] = ( sequenceCounter >> 24 ) & 0xFF;

    MicBlockB0[15] = size & 0xFF;

    mbedtls_cipher_init(AesCmacCtx);
    const mbedtls_cipher_info_t* cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    if (NULL != cipher_info) {
        ret = mbedtls_cipher_setup(AesCmacCtx, cipher_info);
        if (0 != ret)
            goto exit;

        ret = mbedtls_cipher_cmac_starts(AesCmacCtx, key, AES_CMAC_KEY_LENGTH*8);
        if (0 != ret)
            goto exit;

        ret = mbedtls_cipher_cmac_update(AesCmacCtx, MicBlockB0, LORAMAC_MIC_BLOCK_B0_SIZE);
        if (0 != ret)
            goto exit;

        ret = mbedtls_cipher_cmac_update(AesCmacCtx, buffer, size & 0xFF);
        if (0 != ret)
            goto exit;

        ret = mbedtls_cipher_cmac_finish(AesCmacCtx, Mic);
        if (0 != ret)
            goto exit;

        *mic = ( uint32_t )( ( uint32_t )Mic[3] << 24 | ( uint32_t )Mic[2] << 16 | ( uint32_t )Mic[1] << 8 | ( uint32_t )Mic[0] );
    } else {
        ret = MBEDTLS_ERR_CIPHER_ALLOC_FAILED;
    }

exit:
    mbedtls_cipher_free( AesCmacCtx );
    return ret;
}

int LoRaMacPayloadEncrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t address, uint8_t dir, uint32_t sequenceCounter, uint8_t *encBuffer )
{
    uint16_t i;
    uint8_t bufferIndex = 0;
    uint16_t ctr = 1;
    int ret = 0;

    mbedtls_aes_init(&AesContext);
    ret = mbedtls_aes_setkey_enc(&AesContext, key, 16*8);
    if (0 != ret)
        goto exit;

    aBlock[5] = dir;

    aBlock[6] = ( address ) & 0xFF;
    aBlock[7] = ( address >> 8 ) & 0xFF;
    aBlock[8] = ( address >> 16 ) & 0xFF;
    aBlock[9] = ( address >> 24 ) & 0xFF;

    aBlock[10] = ( sequenceCounter ) & 0xFF;
    aBlock[11] = ( sequenceCounter >> 8 ) & 0xFF;
    aBlock[12] = ( sequenceCounter >> 16 ) & 0xFF;
    aBlock[13] = ( sequenceCounter >> 24 ) & 0xFF;

    while( size >= 16 )
    {
        aBlock[15] = ( ( ctr ) & 0xFF );
        ctr++;
        ret = mbedtls_aes_crypt_ecb(&AesContext, MBEDTLS_AES_ENCRYPT, aBlock, sBlock);
        if (0 != ret)
            goto exit;

        for( i = 0; i < 16; i++ )
        {
            encBuffer[bufferIndex + i] = buffer[bufferIndex + i] ^ sBlock[i];
        }
        size -= 16;
        bufferIndex += 16;
    }

    if( size > 0 )
    {
        aBlock[15] = ( ( ctr ) & 0xFF );
        ret = mbedtls_aes_crypt_ecb(&AesContext, MBEDTLS_AES_ENCRYPT, aBlock, sBlock);
        if (0 != ret)
            goto exit;

        for( i = 0; i < size; i++ )
        {
            encBuffer[bufferIndex + i] = buffer[bufferIndex + i] ^ sBlock[i];
        }
    }

exit:
    mbedtls_aes_free(&AesContext);
    return ret;
}

int LoRaMacPayloadDecrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t address, uint8_t dir, uint32_t sequenceCounter, uint8_t *decBuffer )
{
    return LoRaMacPayloadEncrypt( buffer, size, key, address, dir, sequenceCounter, decBuffer );
}

int LoRaMacJoinComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t *mic )
{
    int ret = 0;

    mbedtls_cipher_init(AesCmacCtx);
    const mbedtls_cipher_info_t* cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    if (NULL != cipher_info) {
        ret = mbedtls_cipher_setup(AesCmacCtx, cipher_info);
        if (0 != ret)
            goto exit;

        ret = mbedtls_cipher_cmac_starts(AesCmacCtx, key, AES_CMAC_KEY_LENGTH*8);
        if (0 != ret)
            goto exit;

        ret = mbedtls_cipher_cmac_update(AesCmacCtx, buffer, size & 0xFF);
        if (0 != ret)
            goto exit;

        ret = mbedtls_cipher_cmac_finish(AesCmacCtx, Mic);
        if (0 != ret)
            goto exit;

        *mic = ( uint32_t )( ( uint32_t )Mic[3] << 24 | ( uint32_t )Mic[2] << 16 | ( uint32_t )Mic[1] << 8 | ( uint32_t )Mic[0] );
    } else {
        ret = MBEDTLS_ERR_CIPHER_ALLOC_FAILED;
    }

exit:
    mbedtls_cipher_free(AesCmacCtx);
    return ret;
}

int LoRaMacJoinDecrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t *decBuffer )
{
    int ret = 0;

    mbedtls_aes_init(&AesContext);

    ret = mbedtls_aes_setkey_enc(&AesContext, key, 16*8);
    if (0 != ret)
        goto exit;

    ret = mbedtls_aes_crypt_ecb(&AesContext, MBEDTLS_AES_ENCRYPT, buffer, decBuffer);
    if (0 != ret)
        goto exit;

    // Check if optional CFList is included
    if( size >= 16 )
    {
        ret = mbedtls_aes_crypt_ecb(&AesContext, MBEDTLS_AES_ENCRYPT, buffer + 16, decBuffer + 16);
    }

exit:
    mbedtls_aes_free(&AesContext);
    return ret;
}

int LoRaMacJoinComputeSKeys( const uint8_t *key, const uint8_t *appNonce, uint16_t devNonce, uint8_t *nwkSKey, uint8_t *appSKey )
{
    uint8_t nonce[16];
    uint8_t *pDevNonce = ( uint8_t * )&devNonce;
    int ret = 0;

    mbedtls_aes_init(&AesContext);

    ret = mbedtls_aes_setkey_enc(&AesContext, key, 16*8);
    if (0 != ret)
        goto exit;

    memset( nonce, 0, sizeof( nonce ) );
    nonce[0] = 0x01;
    memcpy( nonce + 1, appNonce, 6 );
    memcpy( nonce + 7, pDevNonce, 2 );
    ret = mbedtls_aes_crypt_ecb(&AesContext, MBEDTLS_AES_ENCRYPT, nonce, nwkSKey);
    if (0 != ret)
        goto exit;

    memset( nonce, 0, sizeof( nonce ) );
    nonce[0] = 0x02;
    memcpy( nonce + 1, appNonce, 6 );
    memcpy( nonce + 7, pDevNonce, 2 );
    ret = mbedtls_aes_crypt_ecb(&AesContext, MBEDTLS_AES_ENCRYPT, nonce, appSKey);

exit:
    mbedtls_aes_free(&AesContext);
    return ret;
}
#else

// If mbedTLS is not configured properly, these dummies will ensure that
// user knows what is wrong and in addition to that these ensure that
// Mbed-OS compiles properly under normal conditions where LoRaWAN in conjunction
// with mbedTLS is not being used.
int LoRaMacComputeMic( const uint8_t *, uint16_t , const uint8_t *, uint32_t,
                       uint8_t dir, uint32_t, uint32_t * )
{
    MBED_ASSERT("[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORA_MAC_STATUS_CRYPTO_FAIL;
}

int LoRaMacPayloadEncrypt( const uint8_t *, uint16_t , const uint8_t *, uint32_t,
                           uint8_t , uint32_t , uint8_t * )
{
    MBED_ASSERT("[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORA_MAC_STATUS_CRYPTO_FAIL;
}

int LoRaMacPayloadDecrypt( const uint8_t *, uint16_t , const uint8_t *, uint32_t,
                           uint8_t , uint32_t , uint8_t * )
{
    MBED_ASSERT("[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORA_MAC_STATUS_CRYPTO_FAIL;
}
int LoRaMacJoinComputeMic( const uint8_t *, uint16_t , const uint8_t *, uint32_t * )
{
    MBED_ASSERT("[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORA_MAC_STATUS_CRYPTO_FAIL;
}

int LoRaMacJoinDecrypt( const uint8_t *, uint16_t , const uint8_t *, uint8_t * )
{
    MBED_ASSERT("[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORA_MAC_STATUS_CRYPTO_FAIL;
}

int LoRaMacJoinComputeSKeys( const uint8_t *, const uint8_t *, uint16_t , uint8_t *, uint8_t * )
{
    MBED_ASSERT("[LoRaCrypto] Must enable AES, CMAC & CIPHER from mbedTLS");

    // Never actually reaches here
    return LORA_MAC_STATUS_CRYPTO_FAIL;
}

#endif
