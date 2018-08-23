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
#include "system/lorawan_data_structures.h"
#include "lorastack/phy/LoRaPHY.h"

/*!
 * Maximum MAC commands buffer size
 */
#define LORA_MAC_COMMAND_MAX_LENGTH                 128

class LoRaMac;

class LoRaMacCommand {

public:
    LoRaMacCommand();

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
     * @brief Parses the MAC commands which must be re-sent.
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
     * @brief Clear sticky MAC commands.
     */
    void clear_sticky_mac_cmd();

    /**
     * @brief Check if MAC command buffer contains sticky commands
     *
     * @return status  True: buffer has sticky MAC commands in it, false: no sticky commands in buffer
     */
    bool has_sticky_mac_cmd() const;

    /**
     * @brief Decodes MAC commands in the fOpts field and in the payload
     *
     * @return status  Function status. LORAWAN_STATUS_OK if command successful.
     */
    lorawan_status_t process_mac_commands(const uint8_t *payload, uint8_t mac_index,
                                          uint8_t commands_size, uint8_t snr,
                                          lora_mac_system_params_t& mac_params,
                                          LoRaPHY& lora_phy,
                                          mbed::Callback<void(loramac_mlme_confirm_t&)> confirm_handler);

    /**
     * @brief Adds a new LinkCheckReq MAC command to be sent.
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_link_check_req();

    /**
     * @brief add_reset_ind Adds a ResetInd MAC command to be sent.
     *
     * @param version LoRaWAN version of the stack
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_reset_ind(uint8_t version);

    /**
     * @brief add_rekey_ind Adds a RekeyInd MAC command to be sent.
     *
     * @param version LoRaWAN version of the stack
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_rekey_ind(uint8_t version);

    /**
     * @brief add_device_mode_indication Adds a DeviceModeInd MAC command to be sent.
     * @param classType Class type that is used from now on
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_device_mode_indication(uint8_t classType);

    /**
     * @brief add_device_time_req Adds DeviceTimeReq MAC command to be sent.
     *        Requests server's current time
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_device_time_req();

    /**
     * @brief Set battery level query callback method
     *        If callback is not set, BAT_LEVEL_NO_MEASURE is returned.
     */
    void set_batterylevel_callback(mbed::Callback<uint8_t(void)> battery_level);

private:
    /**
     * @brief Get the remaining size of the MAC command buffer
     *
     * @return      Remaining free space in buffer (bytes).
     */
    int32_t cmd_buffer_remaining() const;

    /**
     * @brief Adds a new LinkAdrAns MAC command to be sent.
     *
     * @param [in] status Status bits
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_link_adr_ans(uint8_t status);

    /**
     * @brief Adds a new DutyCycleAns MAC command to be sent.
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_duty_cycle_ans();

    /**
     * @brief Adds a new RXParamSetupAns MAC command to be sent.
     *
     * @param [in] status Status bits
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_rx_param_setup_ans(uint8_t status);

    /**
     * @brief Adds a new DevStatusAns MAC command to be sent.
     *
     * @param [in] battery  Battery level
     * @param [in] margin   Demodulation signal-to-noise ratio (dB)
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_dev_status_ans(uint8_t battery, uint8_t margin);

    /**
     * @brief Adds a new NewChannelAns MAC command to be sent.
     *
     * @param [in] status Status bits
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_new_channel_ans(uint8_t status);

    /**
     * @brief Adds a new RXTimingSetupAns MAC command to be sent.
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_rx_timing_setup_ans();

    /**
     * @brief Adds a new TXParamSetupAns MAC command to be sent.
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_tx_param_setup_ans();

    /**
     * @brief Adds a new DlChannelAns MAC command to be sent.
     *
     * @param [in] status Status bits
     *
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_dl_channel_ans(uint8_t status);

    /**
     * @brief Adds a new AdrParamSetupAns MAC command to be sent.
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_adr_param_setup_ans();

    /**
     * @brief Adds a new RejoinParamSetupAns MAC command to be sent.
     * @param status 1 if time limitation was accepted, 0 otherwise.
     * @return status  Function status: LORAWAN_STATUS_OK: OK,
     *                                  LORAWAN_STATUS_LENGTH_ERROR: Buffer full
     */
    lorawan_status_t add_rejoin_param_setup_ans(uint8_t status);

private:
    /**
      * Indicates if there are any pending sticky MAC commands
      */
    bool sticky_mac_cmd;

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

    mbed::Callback<uint8_t(void)> _battery_level_cb;
};

#endif //__LORAMACCOMMAND_H__
