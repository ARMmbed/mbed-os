/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OSC32KCLK = 0,
} RTCName;

typedef enum {
    UART_0 = 0,
    UART_1 = 1,
    UART_2 = 2,
    UART_3 = 3,
    UART_4 = 4,
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_0

typedef enum {
    I2C_0 = 0,
    I2C_1 = 1,
    I2C_2 = 2,
} I2CName;

#define TPM_SHIFT   8
typedef enum {
    PWM_1  = (0 << TPM_SHIFT) | (0),  // FTM0 CH0
    PWM_2  = (0 << TPM_SHIFT) | (1),  // FTM0 CH1
    PWM_3  = (0 << TPM_SHIFT) | (2),  // FTM0 CH2
    PWM_4  = (0 << TPM_SHIFT) | (3),  // FTM0 CH3
    PWM_5  = (0 << TPM_SHIFT) | (4),  // FTM0 CH4
    PWM_6  = (0 << TPM_SHIFT) | (5),  // FTM0 CH5
    PWM_7  = (0 << TPM_SHIFT) | (6),  // FTM0 CH6
    PWM_8  = (0 << TPM_SHIFT) | (7),  // FTM0 CH7
    PWM_9  = (1 << TPM_SHIFT) | (0),  // FTM1 CH0
    PWM_10 = (1 << TPM_SHIFT) | (1),  // FTM1 CH1
    PWM_11 = (1 << TPM_SHIFT) | (2),  // FTM1 CH2
    PWM_12 = (1 << TPM_SHIFT) | (3),  // FTM1 CH3
    PWM_13 = (1 << TPM_SHIFT) | (4),  // FTM1 CH4
    PWM_14 = (1 << TPM_SHIFT) | (5),  // FTM1 CH5
    PWM_15 = (1 << TPM_SHIFT) | (6),  // FTM1 CH6
    PWM_16 = (1 << TPM_SHIFT) | (7),  // FTM1 CH7
    PWM_17 = (2 << TPM_SHIFT) | (0),  // FTM2 CH0
    PWM_18 = (2 << TPM_SHIFT) | (1),  // FTM2 CH1
    PWM_19 = (2 << TPM_SHIFT) | (2),  // FTM2 CH2
    PWM_20 = (2 << TPM_SHIFT) | (3),  // FTM2 CH3
    PWM_21 = (2 << TPM_SHIFT) | (4),  // FTM2 CH4
    PWM_22 = (2 << TPM_SHIFT) | (5),  // FTM2 CH5
    PWM_23 = (2 << TPM_SHIFT) | (6),  // FTM2 CH6
    PWM_24 = (2 << TPM_SHIFT) | (7),  // FTM2 CH7
    // could be 4 or could be 3... not sure what register
    //  this is for... too much abstraction
    PWM_25 = (3 << TPM_SHIFT) | (0),  // FTM3 CH0
    PWM_26 = (3 << TPM_SHIFT) | (1),  // FTM3 CH1
    PWM_27 = (3 << TPM_SHIFT) | (2),  // FTM3 CH2
    PWM_28 = (3 << TPM_SHIFT) | (3),  // FTM3 CH3
    PWM_29 = (3 << TPM_SHIFT) | (4),  // FTM3 CH4
    PWM_30 = (3 << TPM_SHIFT) | (5),  // FTM3 CH5
    PWM_31 = (3 << TPM_SHIFT) | (6),  // FTM3 CH6
    PWM_32 = (3 << TPM_SHIFT) | (7),  // FTM3 CH7
} PWMName;

typedef enum {
    ADC0_VIN0 = 0,
    ADC0_VIN1 = 1,
    ADC0_VIN2 = 2,
    ADC0_VIN3 = 3,
    ADC0_VIN4 = 4,
    ADC0_VIN5 = 5,
    ADC0_VIN6 = 6,
    ADC0_VIN7 = 7
} ADCName;

typedef enum {
    DAC_0 = 0
} DACName;


typedef enum {
    SPI_0 = 0,
    SPI_1 = 1,
    SPI_2 = 2,
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
