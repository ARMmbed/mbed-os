/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2016-2020 STMicroelectronics.
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

#if DEVICE_I2C

#define I2C_IP_VERSION_V2

#if defined I2C1_BASE
#define I2C1_EV_IRQn I2C1_IRQn
#define I2C1_ER_IRQn I2C1_IRQn
#endif
#if defined I2C2_BASE
#define I2C2_EV_IRQn I2C2_IRQn
#define I2C2_ER_IRQn I2C2_IRQn
#endif
#if defined I2C3_BASE
#define I2C3_EV_IRQn I2C3_IRQn
#define I2C3_ER_IRQn I2C3_IRQn
#endif

#define I2C_IT_ALL (I2C_IT_ERRI|I2C_IT_TCI|I2C_IT_STOPI|I2C_IT_NACKI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TXI)

/*  Family specifc settings for clock source */
#define I2CAPI_I2C1_CLKSRC RCC_I2C1CLKSOURCE_SYSCLK
#define I2CAPI_I2C3_CLKSRC RCC_I2C3CLKSOURCE_SYSCLK
#define I2CAPI_I2C4_CLKSRC RCC_I2C4CLKSOURCE_SYSCLK

/*  Provide the suitable timing depending on requested frequencie */
static inline uint32_t get_i2c_timing(int hz)
{
    uint32_t tim = 0;

    switch (hz) {
        case 100000:
            tim = 0x20602938; // Standard mode with Rise Time = 400ns and Fall Time = 100ns
            break;
        case 400000:
            tim = 0x00B0122A; // Fast mode with Rise Time = 250ns and Fall Time = 100ns
            break;
        case 1000000:
            tim = 0x0030040E; // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
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
