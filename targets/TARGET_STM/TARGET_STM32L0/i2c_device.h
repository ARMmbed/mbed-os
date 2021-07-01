/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2016-2021 STMicroelectronics.
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

#include "PeripheralNames.h"

#ifdef __cplusplus
extern "C" {
#endif

/*  Define I2C Device */
#if DEVICE_I2C

/*  Define IP version */
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

#define TIMING_VAL_32M_CLK_100KHZ 0x20602938   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_32M_CLK_400KHZ 0x00B0122A   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_32M_CLK_1MHZ   0x0030040E   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_32M              32000000     // 32 MHz

#define I2C_IT_ALL (I2C_IT_ERRI|I2C_IT_TCI|I2C_IT_STOPI|I2C_IT_NACKI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TXI)

/*  Family specifc settings for clock source */
#define I2CAPI_I2C1_CLKSRC RCC_I2C1CLKSOURCE_SYSCLK
#define I2CAPI_I2C3_CLKSRC RCC_I2C3CLKSOURCE_SYSCLK
#define I2CAPI_I2C4_CLKSRC RCC_I2C4CLKSOURCE_SYSCLK

uint32_t i2c_get_pclk(I2CName i2c);
uint32_t i2c_get_timing(I2CName i2c, uint32_t current_timing, int current_hz, int hz);

#if MBED_CONF_TARGET_I2C_TIMING_VALUE_ALGO
uint32_t i2c_compute_timing(uint32_t clock_src_freq, uint32_t i2c_freq);
void i2c_compute_presc_scldel_sdadel(uint32_t clock_src_freq, uint32_t I2C_speed);
uint32_t i2c_compute_scll_sclh(uint32_t clock_src_freq, uint32_t I2C_speed);
#endif // MBED_CONF_TARGET_I2C_TIMING_VALUE_ALGO

#endif // DEVICE_I2C

#ifdef __cplusplus
}
#endif

#endif
