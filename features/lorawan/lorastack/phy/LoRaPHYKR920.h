/**
 *  @file LoRaPHYKR920.h
 *
 *  @brief Implements LoRaPHY for Korean 920 MHz band
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

#ifndef MBED_OS_LORAPHY_KR920_H_
#define MBED_OS_LORAPHY_KR920_H_

#include "LoRaPHY.h"

/*!
 * LoRaMac maximum number of channels
 */
#define KR920_MAX_NB_CHANNELS                       16

/*!
 * Maximum number of bands
 */
#define KR920_MAX_NB_BANDS                          1

#define KR920_CHANNEL_MASK_SIZE                    1


class LoRaPHYKR920 : public LoRaPHY {

public:

    LoRaPHYKR920(LoRaWANTimeHandler &lora_time);
    virtual ~LoRaPHYKR920();

    virtual bool verify_frequency_for_band(uint32_t freq, uint8_t band) const;

    virtual bool tx_config(tx_config_params_t* config, int8_t* tx_power,
                           lorawan_time_t* tx_toa);

    virtual lorawan_status_t set_next_channel(channel_selection_params_t* params, uint8_t* channel,
                                              lorawan_time_t* time,
                                              lorawan_time_t* aggregate_timeOff);

    virtual void set_tx_cont_mode(cw_mode_params_t* continuousWave,
                                  uint32_t frequency = 0);


private:

    int8_t get_max_eirp(uint32_t freq);

    /**
     * LoRaMAC channels
     */
    channel_params_t channels[KR920_MAX_NB_CHANNELS];

    /**
     * LoRaMac bands
     */
    band_t bands[KR920_MAX_NB_BANDS];

    /**
     * LoRaMac channel mask
     */
    uint16_t channel_mask[KR920_CHANNEL_MASK_SIZE];

    /**
     * LoRaMac default channel mask
     */
    uint16_t default_channel_mask[KR920_CHANNEL_MASK_SIZE];
};

#endif // MBED_OS_LORAPHY_KR920_H_

