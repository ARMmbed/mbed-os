/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OSC32KCLK = 0
} RTCName;

typedef enum {
    LPUART_0 = 0
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        LPUART_0

typedef enum {
    I2C_0 = 0,
    I2C_1 = 1,
} I2CName;

#define TPM_SHIFT   8
typedef enum {
    PWM_1  = (0 << TPM_SHIFT) | (0),  // TPM0 CH0
    PWM_2  = (0 << TPM_SHIFT) | (1),  // TPM0 CH1
    PWM_3  = (0 << TPM_SHIFT) | (2),  // TPM0 CH2
    PWM_4  = (0 << TPM_SHIFT) | (3),  // TPM0 CH3
    PWM_5  = (1 << TPM_SHIFT) | (0),  // TPM1 CH0
    PWM_6  = (1 << TPM_SHIFT) | (1)   // TPM1 CH1
} PWMName;

#define ADC_INSTANCE_SHIFT           8
#define ADC_B_CHANNEL_SHIFT          5
typedef enum {
    ADC0_SE1 = (0 << ADC_INSTANCE_SHIFT) | 1,
    ADC0_SE2 = (0 << ADC_INSTANCE_SHIFT) | 2,
    ADC0_SE3 = (0 << ADC_INSTANCE_SHIFT) | 3,
    ADC0_SE4 = (0 << ADC_INSTANCE_SHIFT) | 4,
    ADC0_SE5 = (0 << ADC_INSTANCE_SHIFT) | 5,
} ADCName;

typedef enum {
    DAC_0 = 0
} DACName;

#define DEVICE_SPI_COUNT 2
typedef enum {
    SPI_0 = 0,
    SPI_1 = 1,
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
