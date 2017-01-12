/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
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

/*  Family specifc settings for clock source */
#define I2CAPI_I2C1_CLKSRC RCC_I2C1CLKSOURCE_SYSCLK
#define I2CAPI_I2C2_CLKSRC RCC_I2C2CLKSOURCE_SYSCLK
#define I2CAPI_I2C3_CLKSRC RCC_I2C3CLKSOURCE_SYSCLK

/*  Provide the suitable timing depending on requested frequencie */
static inline uint32_t get_i2c_timing(int hz)
{
    uint32_t tim = 0;
    /*
       Values calculated with I2C_Timing_Configuration_V1.0.1.xls file (see AN4235)
       * Standard mode (up to 100 kHz)
       * Fast Mode (up to 400 kHz)
       * Fast Mode Plus (up to 1 MHz)
       Below values obtained with:
       - I2C clock source = 64 MHz (System Clock w/ HSI) or 72 (System Clock w/ HSE)
       - Analog filter delay = ON
       - Digital filter coefficient = 0
    */
    if (SystemCoreClock == 64000000) {
        switch (hz) {
            case 100000:
                tim = 0x10B17DB4; // Standard mode with Rise time = 120ns, Fall time = 120ns
                break;
            case 400000:
                tim = 0x00E22163; // Fast Mode with Rise time = 120ns, Fall time = 120ns
                break;
            case 1000000:
                tim = 0x00A00D1E; // Fast Mode Plus with Rise time = 120ns, Fall time = 10ns
                break;
            default:
                break;
        }
    } else if (SystemCoreClock == 72000000) {
        switch (hz) {
            case 100000:
                tim = 0x10D28DCB; // Standard mode with Rise time = 120ns, Fall time = 120ns
                break;
            case 400000:
                tim = 0x00F32571; // Fast Mode with Rise time = 120ns, Fall time = 120ns
                break;
            case 1000000:
                tim = 0x00C00D24; // Fast Mode Plus with Rise time = 120ns, Fall time = 10ns
                break;
            default:
                break;
        }
    }
    return tim;
}

#ifdef __cplusplus
}
#endif

#endif // DEVICE_I2C

#endif
