/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: LoRa MAC Crypto implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef MBED_LORAWAN_MAC_LORAMAC_CRYPTO_H__
#define MBED_LORAWAN_MAC_LORAMAC_CRYPTO_H__

/**
 * Computes the LoRaMAC frame MIC field
 *
 * \param [in]  buffer          - Data buffer
 * \param [in]  size            - Data buffer size
 * \param [in]  key             - AES key to be used
 * \param [in]  address         - Frame address
 * \param [in]  dir             - Frame direction [0: uplink, 1: downlink]
 * \param [in]  sequenceCounter - Frame sequence counter
 * \param [out] mic             - Computed MIC field
 *
 * \return                        0 if successful, or a cipher specific error code
 */
int LoRaMacComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t address, uint8_t dir, uint32_t sequenceCounter, uint32_t *mic );

/**
 * Computes the LoRaMAC payload encryption
 *
 * \param [in]  buffer          - Data buffer
 * \param [in]  size            - Data buffer size
 * \param [in]  key             - AES key to be used
 * \param [in]  address         - Frame address
 * \param [in]  dir             - Frame direction [0: uplink, 1: downlink]
 * \param [in]  sequenceCounter - Frame sequence counter
 * \param [out] encBuffer       - Encrypted buffer
 *
 * \return                        0 if successful, or a cipher specific error code
 */
int LoRaMacPayloadEncrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t address, uint8_t dir, uint32_t sequenceCounter, uint8_t *encBuffer );

/**
 * Computes the LoRaMAC payload decryption
 *
 * \param [in]  buffer          - Data buffer
 * \param [in]  size            - Data buffer size
 * \param [in]  key             - AES key to be used
 * \param [in]  address         - Frame address
 * \param [in]  dir             - Frame direction [0: uplink, 1: downlink]
 * \param [in]  sequenceCounter - Frame sequence counter
 * \param [out] decBuffer       - Decrypted buffer
 *
 * \return                        0 if successful, or a cipher specific error code
 */
int LoRaMacPayloadDecrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t address, uint8_t dir, uint32_t sequenceCounter, uint8_t *decBuffer );

/**
 * Computes the LoRaMAC Join Request frame MIC field
 *
 * \param [in]  buffer          - Data buffer
 * \param [in]  size            - Data buffer size
 * \param [in]  key             - AES key to be used
 * \param [out] mic             - Computed MIC field
 *
 * \return                        0 if successful, or a cipher specific error code
 *
 */
int LoRaMacJoinComputeMic( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint32_t *mic );

/**
 * Computes the LoRaMAC join frame decryption
 *
 * \param [in]  buffer          - Data buffer
 * \param [in]  size            - Data buffer size
 * \param [in]  key             - AES key to be used
 * \param [out] decBuffer       - Decrypted buffer
 *
 * \return                        0 if successful, or a cipher specific error code
 */
int LoRaMacJoinDecrypt( const uint8_t *buffer, uint16_t size, const uint8_t *key, uint8_t *decBuffer );

/**
 * Computes the LoRaMAC join frame decryption
 *
 * \param [in]  key             - AES key to be used
 * \param [in]  appNonce        - Application nonce
 * \param [in]  devNonce        - Device nonce
 * \param [out] nwkSKey         - Network session key
 * \param [out] appSKey         - Application session key
 *
 * \return                        0 if successful, or a cipher specific error code
 */
int LoRaMacJoinComputeSKeys( const uint8_t *key, const uint8_t *appNonce, uint16_t devNonce, uint8_t *nwkSKey, uint8_t *appSKey );

#endif // MBED_LORAWAN_MAC_LORAMAC_CRYPTO_H__
