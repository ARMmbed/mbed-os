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

#include "mbedtls/aes.h"
#include "mbedtls/cmac.h"
#include "system/lorawan_data_structures.h"


class LoRaMacCrypto {
public:
    /**
     * Constructor
     */
    LoRaMacCrypto();

    /**
     * @brief set_keys Set keys to be used for crypto operations
     * @param [in] nwk_key Network key
     * @param [in] app_key Application key
     *
     * @return LORAWAN_STATUS_OK if accepted, LORAWAN_STATUS_CRYPTO_FAIL otherwise
     */
    lorawan_status_t set_keys(uint8_t *nwk_key, uint8_t *app_key, uint8_t *nwk_skey = NULL,
                              uint8_t *app_skey = NULL, uint8_t *snwk_sintkey = NULL, uint8_t *nwk_senckey = NULL);

    /**
     * Computes the LoRaMAC frame MIC field
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  args            - LW1.1 bytes 1-4 (ConfFCnt, TxDr, TxCh)
     * @param [in]  address         - Frame address
     * @param [in]  dir             - Frame direction [0: uplink, 1: downlink]
     * @param [in]  seq_counter     - Frame sequence counter
     * @param [out] mic             - Computed MIC field
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int compute_mic(const uint8_t *buffer, uint16_t size,
                    uint32_t args, uint32_t address,
                    uint8_t dir, uint32_t seq_counter,
                    uint32_t *mic);

    /**
     * Performs payload encryption
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  address         - Frame address
     * @param [in]  dir             - Frame direction [0: uplink, 1: downlink]
     * @param [in]  seq_counter     - Frame sequence counter
     * @param [in]  s1_block_start  - 0 for FOpts field, 1 for FMRPayload
     * @param [out] enc_buffer      - Encrypted buffer
     * @param [in]  is_fopts        - Specifies whether to use FOpts key or not
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int encrypt_payload(const uint8_t *buffer, uint16_t size,
                        uint32_t address, uint8_t dir, uint32_t seq_counter,
                        seq_counter_type_t seq_cnt_type,
                        payload_type_t pld_type,
                        uint8_t *enc_buffer,
                        server_type_t serv_type, bool is_fopts = false);

    /**
     * Performs payload decryption
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  address         - Frame address
     * @param [in]  dir             - Frame direction [0: uplink, 1: downlink]
     * @param [in]  seq_counter     - Frame sequence counter
     * @param [in]  s1_block_start  - 0 for FOpts field, 1 for FMRPayload
     * @param [out] dec_buffer      - Decrypted buffer
     * @param [in]  is_fopts        - Specifies whether to use FOpts key or not
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int decrypt_payload(const uint8_t *buffer, uint16_t size,
                        uint32_t address, uint8_t dir, uint32_t seq_counter,
                        seq_counter_type_t seq_cnt_type,
                        payload_type_t pld_type,
                        uint8_t *dec_buffer,
                        server_type_t serv_type, bool is_fopts = false);

    /**
     * Computes the LoRaMAC Join Request frame MIC field
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [in]  type            - Type of message to handle
     * @param [out] mic             - Computed MIC field
     *
     * @return                        0 if successful, or a cipher specific error code
     *
     */
    int compute_join_frame_mic(const uint8_t *buffer, uint16_t size,
                               join_frame_type_t type, uint32_t *mic);

    /**
     * Computes the LoRaMAC join frame decryption
     *
     * @param [in]  buffer          - Data buffer
     * @param [in]  size            - Data buffer size
     * @param [out] dec_buffer      - Decrypted buffer
     * @param [in]  is_join_req     - True for join request, false for rejoin requests
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int decrypt_join_frame(const uint8_t *buffer, uint16_t size,
                           uint8_t *dec_buffer, bool is_join_req);

    /**
     * @brief unset_js_keys In case we support LoRaWAN 1.1 but server does not, we need to
     *        unset Join server keys to work with LoRaWAN 1.0.2
     */
    void unset_js_keys();

    /**
     * Computes the LoRaMAC join frame decryption
     *
     * @param [in]  args             - Combined string of args needed for key derivation
     * @param [in]  args_size        - Args size
     * @param [in]  stype            - Server type (LW1_0_2 or LW1_1)
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int compute_skeys_for_join_frame(const uint8_t *args, uint8_t args_size,
                                     server_type_t stype);

    /**
     * Computes the LoRaMAC join server keys
     * In case of LoRaWAN 1.0.x key will be copied to outputs,
     * so we can continue using same code for all versions
     *
     * @param [in]  eui              - DevEUI
     *
     * @return                        0 if successful, or a cipher specific error code
     */
    int compute_join_server_keys(const uint8_t *eui);

private:
    /**
     * AES computation context variable
     */
    mbedtls_aes_context aes_ctx;

    /**
     * CMAC computation context variable
     */
    mbedtls_cipher_context_t aes_cmac_ctx[1];

    loramac_keys _keys;

    /*!
     * Mote Address
     */
    uint32_t _dev_addr;
};

#endif // MBED_LORAWAN_MAC_LORAMAC_CRYPTO_H__
