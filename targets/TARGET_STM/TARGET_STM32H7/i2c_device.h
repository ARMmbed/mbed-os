/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
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

#define I2C_IT_ALL (I2C_IT_ERRI|I2C_IT_TCI|I2C_IT_STOPI|I2C_IT_NACKI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TXI)

/*  Family specifc settings for clock source */
#define I2CAPI_I2C1_CLKSRC RCC_I2C1CLKSOURCE_D2PCLK1
#define I2CAPI_I2C2_CLKSRC RCC_I2C2CLKSOURCE_D2PCLK1
#define I2CAPI_I2C3_CLKSRC RCC_I2C3CLKSOURCE_D2PCLK1
#define I2CAPI_I2C4_CLKSRC RCC_I2C4CLKSOURCE_D3PCLK1

/*  Provide the suitable timing depending on requested frequencie */
static inline uint32_t get_i2c_timing(int hz)
{
    uint32_t tim = 0;
    /*
       Values calculated with I2C_Timing_Configuration tool (excel file)
       * Standard mode (up to 100 kHz)
       * Fast Mode (up to 400 kHz)
       * Fast Mode Plus (up to 1 MHz)
       Below values obtained with:
       - I2Cx clock source = APB1CLK = 54 MHz
       - Analog filter delay = ON
       - Digital filter coefficient = 0
    */
    switch (hz) {
        case 100000:
            tim = 0x10916998; // Standard mode with Rise time = 120ns, Fall time = 120ns
            break;
        case 400000:
            tim = 0x00B11B54; // Fast Mode with Rise time = 120ns, Fall time = 120ns
            break;
        case 1000000:
            tim = 0x0090091B; // Fast Mode Plus with Rise time = 120ns, Fall time = 10ns
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
