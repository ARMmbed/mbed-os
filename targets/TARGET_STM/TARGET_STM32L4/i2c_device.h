/* mbed Microcontroller Library
 * Copyright (c) 2016-2020 STMicroelectronics
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

#include "cmsis.h"
#include "mbed_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_I2C

#define I2C_IP_VERSION_V2

#define I2C_IT_ALL (I2C_IT_ERRI|I2C_IT_TCI|I2C_IT_STOPI|I2C_IT_NACKI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TXI)

/*  Family specifc settings for clock source */
#define I2CAPI_I2C1_CLKSRC RCC_I2C1CLKSOURCE_SYSCLK
#define I2CAPI_I2C2_CLKSRC RCC_I2C2CLKSOURCE_SYSCLK
#define I2CAPI_I2C3_CLKSRC RCC_I2C3CLKSOURCE_SYSCLK
#define I2CAPI_I2C4_CLKSRC RCC_I2C4CLKSOURCE_SYSCLK

/*  Provide the suitable timing depending on requested frequencie */
static inline uint32_t get_i2c_timing(int hz)
{
    uint32_t tim = 0;
    if (SystemCoreClock == 80000000) {
        // Common settings: I2C clock = 80 MHz, Analog filter = ON, Digital filter coefficient = 0
        switch (hz) {
            case 100000:
                tim = 0x30C14E6B; // Standard mode with Rise Time = 400ns and Fall Time = 100ns
                break;
            case 400000:
                tim = 0x10D1143A; // Fast mode with Rise Time = 250ns and Fall Time = 100ns
                break;
            case 1000000:
                tim = 0x00810E27; // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
                break;
            default:
                break;
        }
    } else if (SystemCoreClock == 48000000) {
        // Common settings: I2C clock = 48 MHz, Analog filter = ON, Digital filter coefficient = 0
        switch (hz) {
            case 100000:
                tim = 0x20A03E55; // Standard mode with Rise Time = 400ns and Fall Time = 100ns
                break;
            case 400000:
                tim = 0x10800C21; // Fast mode with Rise Time = 250ns and Fall Time = 100ns
                break;
            case 1000000:
                tim = 0x00500816; // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
                break;
            default:
                break;
        }
    } else if (SystemCoreClock == 120000000) {
        // Common settings: I2C clock = 120 MHz, Analog filter = ON, Digital filter coefficient = 0
        switch (hz) {
            case 100000:
                tim = 0x107075B0; // Standard mode with Rise Time = 400ns and Fall Time = 100ns
                break;
            case 400000:
                tim = 0x00501E6C; // Fast mode with Rise Time = 250ns and Fall Time = 100ns
                break;
            case 1000000:
                tim = 0x00200A26; // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
                break;
            default:
                break;
        }
    } else {
        error("get_i2c_timing error\n");
    }
    return tim;
}

#ifdef __cplusplus
}
#endif

#endif // DEVICE_I2C

#endif
