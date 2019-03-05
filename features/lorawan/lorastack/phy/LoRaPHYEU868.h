/**
 *  @file LoRaPHYEU868.h
 *
 *  @brief Implements LoRaPHY for European 868 MHz band
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

#ifndef MBED_OS_LORAPHY_EU868_H_
#define MBED_OS_LORAPHY_EU868_H_

#if !(DOXYGEN_ONLY)

#include "LoRaPHY.h"

/*!
 * LoRaMac maximum number of channels
 */
#define EU868_MAX_NB_CHANNELS                       16

/*!
 * Maximum number of bands
 *
 * We have broken down EU-868 MHz BAND 2 into two parts. That's why
 * total number of sub-bands is 6.
 * from 863 MHz to 865 MHz region is part of BAND 2, however
 * we call it Band-5 here. Duty cycle limit is 0.1 % in this sub band.
 */
#define EU868_MAX_NB_BANDS                          6

#define EU868_CHANNEL_MASK_SIZE                    1

class LoRaPHYEU868 : public LoRaPHY {

public:
    LoRaPHYEU868();
    virtual ~LoRaPHYEU868();

private:
    /*!
     * LoRaMAC channels
     */
    channel_params_t channels[EU868_MAX_NB_CHANNELS];

    /*!
     * LoRaMac bands
     */
    band_t bands[EU868_MAX_NB_BANDS];

    /*!
     * LoRaMac channels mask
     */
    uint16_t channel_mask[EU868_CHANNEL_MASK_SIZE];

    /*!
     * LoRaMac default channel mask
     */
    uint16_t default_channel_mask[EU868_CHANNEL_MASK_SIZE];
};

#endif /* DOXYGEN_ONLY*/
#endif /* MBED_OS_LORAPHY_EU868_H_ */
