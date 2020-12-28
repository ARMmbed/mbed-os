/**
 *  @file LoRaPHYAS923.h
 *
 *  @brief Implements LoRaPHY for Asia-Pacific 923 MHz band
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

#ifndef MBED_OS_LORAPHY_AS923_H_
#define MBED_OS_LORAPHY_AS923_H_

#if !(DOXYGEN_ONLY)

#include "LoRaPHY.h"

/*!
 * LoRaMac maximum number of channels
 */
#define AS923_MAX_NB_CHANNELS                       16

/*!
 * Maximum number of bands
 */
#define AS923_MAX_NB_BANDS                          1

#define AS923_CHANNEL_MASK_SIZE                    1

class LoRaPHYAS923 : public LoRaPHY {

public:
    LoRaPHYAS923();
    virtual ~LoRaPHYAS923();

    virtual int8_t get_alternate_DR(uint8_t nb_trials);

    virtual lorawan_status_t set_next_channel(channel_selection_params_t *nextChanParams,
                                              uint8_t *channel, lorawan_time_t *time,
                                              lorawan_time_t *aggregatedTimeOff);

    virtual uint8_t apply_DR_offset(int8_t dr, int8_t drOffset);

private:
    channel_params_t channels[AS923_MAX_NB_CHANNELS];
    band_t bands[AS923_MAX_NB_BANDS];
    uint16_t channel_mask[AS923_CHANNEL_MASK_SIZE];
    uint16_t default_channel_mask[AS923_CHANNEL_MASK_SIZE];
};

#endif /* DOXYGEN_ONLY*/
#endif /* MBED_OS_LORAPHY_AS923_H_ */
