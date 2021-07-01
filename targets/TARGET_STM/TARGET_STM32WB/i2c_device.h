/* mbed Microcontroller Library
 * Copyright (c) 2019-2021 STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_I2C_DEVICE_H
#define MBED_I2C_DEVICE_H

#include "PeripheralNames.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Define I2C Device */
#if DEVICE_I2C

/*  Define IP version */
#define I2C_IP_VERSION_V2

// Common settings: I2C clock = 64 MHz, Analog filter = ON, Digital filter coefficient = 0
#define TIMING_VAL_64M_CLK_100KHZ 0x10707DBC   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_64M_CLK_400KHZ 0x00602173   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_64M_CLK_1MHZ   0x00300B29   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_64M              64000000     // 64 MHz

// Common settings: I2C clock = 32 MHz, Analog filter = ON, Digital filter coefficient = 0
#define TIMING_VAL_32M_CLK_100KHZ 0x00707CBB   // Standard mode with Rise Time = 400ns and Fall Time = 100ns
#define TIMING_VAL_32M_CLK_400KHZ 0x00300F38   // Fast mode with Rise Time = 250ns and Fall Time = 100ns
#define TIMING_VAL_32M_CLK_1MHZ   0x00100413   // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
#define I2C_PCLK_32M              32000000     // 32 MHz

#define I2C_IT_ALL (I2C_IT_ERRI|I2C_IT_TCI|I2C_IT_STOPI|I2C_IT_NACKI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TXI)

/*  Family specific settings for clock source */
#define I2CAPI_I2C1_CLKSRC RCC_I2C1CLKSOURCE_SYSCLK
#define I2CAPI_I2C2_CLKSRC RCC_I2C2CLKSOURCE_SYSCLK
#define I2CAPI_I2C3_CLKSRC RCC_I2C3CLKSOURCE_SYSCLK

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
