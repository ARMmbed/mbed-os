/**
 *  @file LoRaPHYAU915.h
 *
 *  @brief Implements LoRaPHY for Australian 915 MHz band
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

#ifndef MBED_OS_LORAPHY_AU915_H_

#define MBED_OS_LORAPHY_AU915_H_

#include "LoRaPHY.h"

// Definitions
/*!
 * LoRaMac maximum number of channels
 */
#define AU915_MAX_NB_CHANNELS                       72

/*!
 * LoRaMac maximum number of bands
 */
#define AU915_MAX_NB_BANDS                          1

#define AU915_CHANNEL_MASK_SIZE                    5


class LoRaPHYAU915 : public LoRaPHY{

public:

    LoRaPHYAU915(LoRaWANTimeHandler &lora_time);
    virtual ~LoRaPHYAU915();

    virtual bool rx_config(rx_config_params_t* config, int8_t* datarate);

    virtual bool tx_config(tx_config_params_t* config, int8_t* txPower,
                           lorawan_time_t* txTimeOnAir);

    virtual uint8_t link_ADR_request(adr_req_params_t* params,
                                     int8_t* drOut, int8_t* txPowOut,
                                     uint8_t* nbRepOut,
                                     uint8_t* nbBytesParsed);

    virtual uint8_t accept_rx_param_setup_req(rx_param_setup_req_t* params);

    virtual int8_t get_alternate_DR(uint8_t nb_trials);

    virtual bool set_next_channel(channel_selection_params_t* next_chan_params,
                                  uint8_t* channel, lorawan_time_t* time,
                                  lorawan_time_t* aggregate_timeoff);

    virtual uint8_t apply_DR_offset(int8_t dr, int8_t dr_offset);

private:

    /*!
     * LoRaMAC channels
     */
    channel_params_t channels[AU915_MAX_NB_CHANNELS];

    /*!
     * LoRaMac bands
     */
    band_t bands[AU915_MAX_NB_BANDS];

    /*!
     * LoRaMac channel mask
     */
    uint16_t channel_mask[AU915_CHANNEL_MASK_SIZE];

    /*!
     * Previously used channel mask
     */
    uint16_t current_channel_mask[AU915_CHANNEL_MASK_SIZE];

    /*!
     * LoRaMac channels default mask
     */
    uint16_t default_channel_mask[AU915_CHANNEL_MASK_SIZE];
};

#endif /* MBED_OS_LORAPHY_AU915_H_ */
