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

class LoRaMacCommand
{
public:
    LoRaMacCommand(LoRaMac &lora_mac);
    ~LoRaMacCommand();

    /*!
     * \brief Adds a new MAC command to be sent.
     *
     * \Remark MAC layer internal function
     *
     * \param [in] cmd MAC command to be added
     *                 [MOTE_MAC_LINK_CHECK_REQ,
     *                  MOTE_MAC_LINK_ADR_ANS,
     *                  MOTE_MAC_DUTY_CYCLE_ANS,
     *                  MOTE_MAC_RX2_PARAM_SET_ANS,
     *                  MOTE_MAC_DEV_STATUS_ANS
     *                  MOTE_MAC_NEW_CHANNEL_ANS]
     * \param [in] p1  1st parameter ( optional depends on the command )
     * \param [in] p2  2nd parameter ( optional depends on the command )
     *
     * \retval status  Function status [0: OK, 1: Unknown command, 2: Buffer full]
     */
    lorawan_status_t AddMacCommand(uint8_t cmd, uint8_t p1, uint8_t p2);

    /*!
     * \brief Clear MAC command buffer.
     */
    void ClearCommandBuffer();

    /*!
     * \brief Get the length of MAC commands
     *
     * \retval status  Length of used MAC buffer (bytes)
     */
    uint8_t GetLength() const;

    /*!
     * \brief Get MAC command buffer
     *
     * \retval  Pointer to MAC command buffer
     */
    uint8_t *GetMacCommandsBuffer();

    /*!
     * \brief Parses the MAC commands which must be resent.
     */
    void ParseMacCommandsToRepeat();

    /*!
     * \brief Clear  MAC command repeat buffer.
     */
    void ClearRepeatBuffer();

    /*!
     * \brief Copy MAC commands from repeat buffer to actual MAC command buffer.
     */
    void CopyRepeatCommandsToBuffer();

    /*!
     * \brief Get the length of MAC commands in repeat buffer
     *
     * \retval status  Length of used MAC Repeat buffer (bytes)
     */
    uint8_t GetRepeatLength() const;

    /*!
     * \brief Clear MAC commands in next TX.
     */
    void ClearMacCommandsInNextTx();

    /*!
     * \brief Check if MAC command buffer has commands to be sent in next TX
     *
      * \retval status  True: buffer has MAC commands to be sent, false: no commands in buffer]
     */
    bool IsMacCommandsInNextTx() const;

    /*!
     * \brief Decodes MAC commands in the fOpts field and in the payload
     */
    void ProcessMacCommands(uint8_t *payload, uint8_t macIndex,
                            uint8_t commandsSize, uint8_t snr,
                            loramac_mlme_confirm_t& MlmeConfirm,
                            lora_mac_system_params_t& LoRaMacParams,
                            LoRaPHY& lora_phy);

    /*!
     * \brief Verifies if sticky MAC commands are pending.
     *
     * \retval [true: sticky MAC commands pending, false: No MAC commands pending]
     */
    bool IsStickyMacCommandPending();

private:
    LoRaMac& _lora_mac;

    /*!
     * Indicates if the MAC layer wants to send MAC commands
     */
    bool MacCommandsInNextTx;

    /*!
     * Contains the current MacCommandsBuffer index
     */
    uint8_t MacCommandsBufferIndex;

    /*!
     * Contains the current MacCommandsBuffer index for MAC commands to repeat
     */
    uint8_t MacCommandsBufferToRepeatIndex;

    /*!
     * Buffer containing the MAC layer commands
     */
    uint8_t MacCommandsBuffer[LORA_MAC_COMMAND_MAX_LENGTH];

    /*!
     * Buffer containing the MAC layer commands which must be repeated
     */
    uint8_t MacCommandsBufferToRepeat[LORA_MAC_COMMAND_MAX_LENGTH];
};

#endif //__LORAMACCOMMAND_H__
