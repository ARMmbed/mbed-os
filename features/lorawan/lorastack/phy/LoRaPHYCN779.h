/**
 *  @file LoRaPHYCN779.h
 *
 *  @brief Implements LoRaPHY for Chinese 779 MHz band
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

#ifndef MBED_OS_LORAPHY_CN779_H_
#define MBED_OS_LORAPHY_CN779_H_

#if !(DOXYGEN_ONLY)

#include "LoRaPHY.h"

#define CN779_MAX_NB_CHANNELS                       16

#define CN779_MAX_NB_BANDS                           1

#define CN779_CHANNEL_MASK_SIZE                     1


class LoRaPHYCN779 : public LoRaPHY {

public:

    LoRaPHYCN779();
    virtual ~LoRaPHYCN779();

private:
    /*!
     * LoRaMAC channels
     */
    channel_params_t channels[CN779_MAX_NB_CHANNELS];

    /*!
     * LoRaMac bands
     */
    band_t bands[CN779_MAX_NB_BANDS];

    /*!
     * LoRaMac channel mask
     */
    uint16_t channel_mask[CN779_CHANNEL_MASK_SIZE];

    /*!
     * LoRaMac default channel mask
     */
    uint16_t default_channel_mask[CN779_CHANNEL_MASK_SIZE];
};

#endif /* DOXYGEN_ONLY*/
#endif /* MBED_OS_LORAPHY_CN779_H_ */
