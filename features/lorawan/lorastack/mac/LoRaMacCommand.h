/**
 * \file      LoRaMacCommand.h
 *
 * \brief     LoRa MAC layer implementation
 *
 * \copyright Revised BSD License, see LICENSE.TXT file include in the project
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 *
 * \defgroup  LORAMAC LoRa MAC layer implementation
 *            This module specifies the API implementation of the LoRaMAC layer.
 *            This is a placeholder for a detailed description of the LoRaMac
 *            layer and the supported features.
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __LORAMACCOMMAND_H__
#define __LORAMACCOMMAND_H__

#include <stdint.h>
#include "lorawan/system/lorawan_data_structures.h"
#include "lorawan/lorastack/phy/LoRaPHY.h"

/*!
 * Maximum MAC commands buffer size
 */
#define LORA_MAC_COMMAND_MAX_LENGTH                 128

class LoRaMac;

class LoRaMacCommand {

public:
    LoRaMacCommand(LoRaMac &lora_mac);
    ~LoRaMacCommand();

    /**
     * @brief Adds a new MAC command to be sent.
     *
     * @remark MAC layer internal function
     *
     * @param [in] cmd    MAC command to be added
     *                    [MOTE_MAC_LINK_CHECK_REQ,
     *                     MOTE_MAC_LINK_ADR_ANS,
     *                     MOTE_MAC_DUTY_CYCLE_ANS,
     *                     MOTE_MAC_RX2_PARAM_SET_ANS,
     *                     MOTE_MAC_DEV_STATUS_ANS
     *                     MOTE_MAC_NEW_CHANNEL_ANS]
     * @param [in] p1  1st parameter (optional depends on the command)
     * @param [in] p2  2nd parameter (optional depends on the command)
     *
     * @return status  Function status [0: OK, 1: Unknown command, 2: Buffer full]
     */
    lorawan_status_t add_mac_command(uint8_t cmd, uint8_t p1, uint8_t p2);

    /**
     * @brief Clear MAC command buffer.
     */
    void clear_command_buffer(void);

    /**
     * @brief Get the length of MAC commands
     *
     * @return status    Length of used MAC buffer (bytes)
     */
    uint8_t get_mac_cmd_length() const;

    /**
     * @brief Get MAC command buffer
     *
     * @return    Pointer to MAC command buffer
     */
    uint8_t *get_mac_commands_buffer();

    /**
     * @brief Parses the MAC commands which must be resent.
     */
    void parse_mac_commands_to_repeat();

    /**
     * @brief Clear  MAC command repeat buffer.
     */
    void clear_repeat_buffer();

    /**
     * @brief Copy MAC commands from repeat buffer to actual MAC command buffer.
     */
    void copy_repeat_commands_to_buffer();

    /**
     * @brief Get the length of MAC commands in repeat buffer
     *
     * @return status  Length of used MAC Repeat buffer (bytes)
     */
    uint8_t get_repeat_commands_length() const;

    /**
     * @brief Clear MAC commands in next TX.
     */
    void clear_mac_commands_in_next_tx();

    /**
     * @brief Check if MAC command buffer has commands to be sent in next TX
     *
     * @return status  True: buffer has MAC commands to be sent, false: no commands in buffer]
     */
    bool is_mac_command_in_next_tx() const;

    /**
     * @brief Decodes MAC commands in the fOpts field and in the payload
     *
     * @return status  Function status. LORAWAN_STATUS_OK if command successful.
     */
    lorawan_status_t process_mac_commands(uint8_t *payload, uint8_t mac_index,
                                          uint8_t commands_size, uint8_t snr,
                                          loramac_mlme_confirm_t& mlme_conf,
                                          lora_mac_system_params_t& mac_params,
                                          LoRaPHY& lora_phy);

    /**
     * @brief Verifies if sticky MAC commands are pending.
     *
     * @return [true: sticky MAC commands pending, false: No MAC commands pending]
     */
    bool is_sticky_mac_command_pending();

private:
    LoRaMac& _lora_mac;

    /**
     * Indicates if the MAC layer wants to send MAC commands
     */
    bool mac_cmd_in_next_tx;

    /**
     * Contains the current Mac command buffer index in 'mac_cmd_buffer'
     */
    uint8_t mac_cmd_buf_idx;

    /**
     * Contains the current Mac command buffer index for MAC commands to repeat in
     * 'mac_cmd_buffer_to_repeat'
     */
    uint8_t mac_cmd_buf_idx_to_repeat;

    /**
     * Buffer containing the MAC layer commands
     */
    uint8_t mac_cmd_buffer[LORA_MAC_COMMAND_MAX_LENGTH];

    /**
     * Buffer containing the MAC layer commands which must be repeated
     */
    uint8_t mac_cmd_buffer_to_repeat[LORA_MAC_COMMAND_MAX_LENGTH];
};

#endif //__LORAMACCOMMAND_H__
