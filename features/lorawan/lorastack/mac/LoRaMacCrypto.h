/**
\code

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

\endcode

Description: LoRa MAC Crypto implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef MBED_LORAWAN_MAC_LORAMAC_CRYPTO_H__
#define MBED_LORAWAN_MAC_LORAMAC_CRYPTO_H__

#include "mbedtls/config.h"
#include "mbedtls/aes.h"
#include "mbedtls/cmac.h"


class LoRaMacCrypto {
public:
    /**
     * Constructor
     */
    LoRaMacCrypto();

    /**
     * Destructor
     */
    ~LoRaMacCrypto();

    /**
     * Computes the LoRaMAC frame MIC field
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  key             - AES key to be used
     * @param [in]  key_length      - Length of the key (bits)
     * @param [in]  address         - Frame address
     * @param [in]  dir             - Frame direction [0: uplink, 1: downlink]
     * @param [in]  seq_counter     - Frame sequence counter
     * @param [out] mic             - Computed MIC field
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int compute_mic(const uint8_t *buffer, uint16_t size,
                    const uint8_t *key, uint32_t key_length,
                    uint32_t address, uint8_t dir, uint32_t seq_counter,
                    uint32_t *mic);

    /**
     * Performs payload encryption
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  key             - AES key to be used
     * @param [in]  key_length      - Length of the key (bits)
     * @param [in]  address         - Frame address
     * @param [in]  dir             - Frame direction [0: uplink, 1: downlink]
     * @param [in]  seq_counter     - Frame sequence counter
     * @param [out] enc_buffer      - Encrypted buffer
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int encrypt_payload(const uint8_t *buffer, uint16_t size,
                        const uint8_t *key, uint32_t key_length,
                        uint32_t address, uint8_t dir, uint32_t seq_counter,
                        uint8_t *enc_buffer);

    /**
     * Performs payload decryption
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  key             - AES key to be used
     * @param [in]  key_length      - Length of the key (bits)
     * @param [in]  address         - Frame address
     * @param [in]  dir             - Frame direction [0: uplink, 1: downlink]
     * @param [in]  seq_counter     - Frame sequence counter
     * @param [out] dec_buffer      - Decrypted buffer
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int decrypt_payload(const uint8_t *buffer, uint16_t size,
                        const uint8_t *key, uint32_t key_length,
                        uint32_t address, uint8_t dir, uint32_t seq_counter,
                        uint8_t *dec_buffer);

    /**
     * Computes the LoRaMAC Join Request frame MIC field
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  key             - AES key to be used
     * @param [in]  key_length      - Length of the key (bits)
     * @param [out] mic             - Computed MIC field
     *
     * @return                        0 if successful, or a cipher specific error code
     *
     */
    int compute_join_frame_mic(const uint8_t *buffer, uint16_t size,
                               const uint8_t *key, uint32_t key_length,
                               uint32_t *mic);

    /**
     * Computes the LoRaMAC join frame decryption
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  key             - AES key to be used
     * @param [in]  key_length      - Length of the key (bits)
     * @param [out] dec_buffer      - Decrypted buffer
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int decrypt_join_frame(const uint8_t *buffer, uint16_t size,
                           const uint8_t *key, uint32_t key_length,
                           uint8_t *dec_buffer);

    /**
     * Computes the LoRaMAC join frame decryption
     *
     * @param [in]  key              - AES key to be used
     * @param [in]  key_length       - Length of the key (bits)
     * @param [in]  app_nonce        - Application nonce
     * @param [in]  dev_nonce        - Device nonce
     * @param [out] nwk_skey         - Network session key
     * @param [out] app_skey         - Application session key
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int compute_skeys_for_join_frame(const uint8_t *key, uint32_t key_length,
                                     const uint8_t *app_nonce, uint16_t dev_nonce,
                                     uint8_t *nwk_skey, uint8_t *app_skey);

private:
    /**
     * AES computation context variable
     */
    mbedtls_aes_context aes_ctx;

    /**
     * CMAC computation context variable
     */
    mbedtls_cipher_context_t aes_cmac_ctx[1];
};

#endif // MBED_LORAWAN_MAC_LORAMAC_CRYPTO_H__
