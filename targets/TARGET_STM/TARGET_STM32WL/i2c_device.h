/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_I2C_DEVICE_H
#define MBED_I2C_DEVICE_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEVICE_I2C

#define I2C_IP_VERSION_V2

#define I2C_IT_ALL (I2C_IT_ERRI|I2C_IT_TCI|I2C_IT_STOPI|I2C_IT_NACKI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TXI)

/*  Family specific settings for clock source */
#define I2CAPI_I2C1_CLKSRC RCC_I2C1CLKSOURCE_SYSCLK
#define I2CAPI_I2C2_CLKSRC RCC_I2C2CLKSOURCE_SYSCLK
#define I2CAPI_I2C3_CLKSRC RCC_I2C3CLKSOURCE_SYSCLK

/*  Provide the suitable timing depending on requested frequency */
static inline uint32_t get_i2c_timing(int hz)
{
    uint32_t tim = 0;

    // Common settings: I2C clock = 48 MHz, Analog filter = ON, Digital filter coefficient = 0
    switch (hz) {
        case 100000:
            tim = 0x20E03F53; // Standard mode with Rise Time = 640ns and Fall Time = 20ns
            break;
        case 400000:
            tim = 0x20500817; // Fast mode with Rise Time = 250ns and Fall Time = 100ns
            break;
        case 1000000:
            tim = 0x00500A18; // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
            break;
        default:
            break;
    }
    return tim;
}

#ifdef __cplusplus
}
#endif

#endif // DEVICE_I2C

#endif
