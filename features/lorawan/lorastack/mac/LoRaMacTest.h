/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: LoRa MAC layer test function implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef __LORAMACTEST_H__
#define __LORAMACTEST_H__

/**
 * \brief   Enabled or disables the reception windows
 *
 * \details This is a test function. It shall be used for testing purposes only.
 *          Changing this attribute may lead to a non-conformance LoRaMac operation.
 *
 * \param   [in] enable - Enabled or disables the reception windows
 */
void LoRaMacTestRxWindowsOn( bool enable );

/**
 * \brief   Enables the MIC field test
 *
 * \details This is a test function. It shall be used for testing purposes only.
 *          Changing this attribute may lead to a non-conformance LoRaMac operation.
 *
 * \param   [in] txPacketCounter - Fixed Tx packet counter value
 */
void LoRaMacTestSetMic( uint16_t txPacketCounter );

/**
 * \brief   Enabled or disables the duty cycle
 *
 * \details This is a test function. It shall be used for testing purposes only.
 *          Changing this attribute may lead to a non-conformance LoRaMac operation.
 *
 * \param   [in] enable - Enabled or disables the duty cycle
 */
void LoRaMacTestSetDutyCycleOn( bool enable );

/**
 * \brief   Sets the channel index
 *
 * \details This is a test function. It shall be used for testing purposes only.
 *          Changing this attribute may lead to a non-conformance LoRaMac operation.
 *
 * \param   [in] channel - Channel index
 */
void LoRaMacTestSetChannel( uint8_t channel );

#endif // __LORAMACTEST_H__
