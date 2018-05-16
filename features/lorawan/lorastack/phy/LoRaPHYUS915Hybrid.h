/**
 *  @file LoRaPHYUS915Hybrid.h
 *
 *  @brief Implements LoRaPHY for US 915 MHz Hybrid band
 *
 *  \code
 *   ______                              _
 *  / _____)             _              | |
 * ( (____  _____ ____ _| |_ _____  ____| |__
 *  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *  _____) ) ____| | | || |_| ____( (___| | | |
 * (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *   (C)2013 Semtech
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions===============
 *
 * \endcode
 *
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBED_OS_LORAPHY_US915_HYBRID_H_
#define MBED_OS_LORAPHY_US915_HYBRID_H_

#include "LoRaPHY.h"

/*!
 * LoRaMac maximum number of channels
 */
#define US915_HYBRID_MAX_NB_CHANNELS                72

/*!
 * LoRaMac maximum number of bands
 */
#define US915_HYBRID_MAX_NB_BANDS                   1

#define US915_HYBRID_CHANNEL_MASK_SIZE             5


class LoRaPHYUS915Hybrid : public LoRaPHY {

public:

    LoRaPHYUS915Hybrid(LoRaWANTimeHandler &lora_time);
    virtual ~LoRaPHYUS915Hybrid();

    virtual void restore_default_channels();

    virtual bool get_next_ADR(bool restore_channel_mask, int8_t& dr_out,
                              int8_t& tx_power_out, uint32_t& adr_ack_cnt);

    virtual bool rx_config(rx_config_params_t* rxConfig);

    virtual bool tx_config(tx_config_params_t* tx_config, int8_t* tx_power,
                                lorawan_time_t* tx_toa);

    virtual uint8_t link_ADR_request(adr_req_params_t* params,
                                     int8_t* dr_out, int8_t* tx_power_out,
                                     uint8_t* nb_rep_out,
                                     uint8_t* nb_bytes_parsed);

    virtual uint8_t accept_rx_param_setup_req(rx_param_setup_req_t* params);

    virtual int8_t get_alternate_DR(uint8_t nb_trials);

    virtual lorawan_status_t set_next_channel(channel_selection_params_t* params,
                                              uint8_t* channel, lorawan_time_t* time,
                                              lorawan_time_t* aggregate_timeoff);

    virtual void set_tx_cont_mode(cw_mode_params_t* continuousWave,
                                  uint32_t frequency = 0);

    virtual uint8_t apply_DR_offset(int8_t dr, int8_t dr_offset);

private:
    int8_t limit_tx_power(int8_t tx_power, int8_t max_band_tx_power, int8_t datarate);
    bool validate_channel_mask(uint16_t* channel_mask);
    void reenable_500khz_channels(uint16_t mask, uint16_t* channel_mask);

    /*!
     * LoRaMAC channels
     */
    channel_params_t channels[US915_HYBRID_MAX_NB_CHANNELS];

    /*!
     * LoRaMac bands
     */
    band_t bands[US915_HYBRID_MAX_NB_BANDS];

    /*!
     * LoRaMac channels mask
     */
    uint16_t channel_mask[US915_HYBRID_CHANNEL_MASK_SIZE];

    /*!
     * Previously used channel mask
     */
    uint16_t current_channel_mask[US915_HYBRID_CHANNEL_MASK_SIZE];

    /*!
     * LoRaMac default channel mask
     */
    uint16_t default_channel_mask[US915_HYBRID_CHANNEL_MASK_SIZE];
};

#endif /* MBED_OS_LORAPHY_US915HYBRID_H_ */
