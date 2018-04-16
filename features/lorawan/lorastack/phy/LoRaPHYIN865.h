/**
 *  @file LoRaPHYIN865.h
 *
 *  @brief Implements LoRaPHY for Indian 865 MHz band
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

#ifndef MBED_OS_LORAPHY_IN865_H_
#define MBED_OS_LORAPHY_IN865_H_

#include "LoRaPHY.h"


/*!
 * LoRaMac maximum number of channels
 */
#define IN865_MAX_NB_CHANNELS                       16

/*!
 * Maximum number of bands
 */
#define IN865_MAX_NB_BANDS                          1

#define IN865_CHANNEL_MASK_SIZE                    1


class LoRaPHYIN865 : public LoRaPHY {

public:

    LoRaPHYIN865(LoRaWANTimeHandler &lora_time);
    virtual ~LoRaPHYIN865();

    virtual uint8_t apply_DR_offset(int8_t dr, int8_t dr_offset );

private:
    /*!
     * LoRaMAC channels
     */
    channel_params_t channels[IN865_MAX_NB_CHANNELS];

    /*!
     * LoRaMac bands
     */
    band_t bands[IN865_MAX_NB_BANDS];

    /*!
     * LoRaMac channel mask
     */
    uint16_t channel_mask[IN865_CHANNEL_MASK_SIZE];

    /*!
     * LoRaMac default channel mask
     */
    uint16_t default_channel_mask[IN865_CHANNEL_MASK_SIZE];
};

#endif /* MBED_OS_LORAPHY_IN865_H_ */
