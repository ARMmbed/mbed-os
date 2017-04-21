/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
    OSC32KCLK = 0,
} RTCName;

/* LPUART */
typedef enum {
    LPUART_0 = 0,
    LPUART_1 = 1,
    LPUART_2 = 2,
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
    PWM_5  = (0 << TPM_SHIFT) | (4),  // TPM0 CH4
    PWM_6  = (0 << TPM_SHIFT) | (5),  // TPM0 CH5
    PWM_7  = (1 << TPM_SHIFT) | (0),  // TPM1 CH0
    PWM_8  = (1 << TPM_SHIFT) | (1),  // TPM1 CH1
    PWM_9  = (2 << TPM_SHIFT) | (0),  // TPM2 CH0
    PWM_10 = (2 << TPM_SHIFT) | (1),  // TPM2 CH1
} PWMName;

#define ADC_INSTANCE_SHIFT           8
#define ADC_B_CHANNEL_SHIFT          5
typedef enum {
    ADC0_SE0  = (0 << ADC_INSTANCE_SHIFT) | 0,
    ADC0_SE1  = (0 << ADC_INSTANCE_SHIFT) | 1,
    ADC0_SE2  = (0 << ADC_INSTANCE_SHIFT) | 2,
    ADC0_SE3  = (0 << ADC_INSTANCE_SHIFT) | 3,
    ADC0_SE4a = (0 << ADC_INSTANCE_SHIFT) | 4,
    ADC0_SE5a = (0 << ADC_INSTANCE_SHIFT) | 5,
    ADC0_SE6a = (0 << ADC_INSTANCE_SHIFT) | 6,
    ADC0_SE7a = (0 << ADC_INSTANCE_SHIFT) | 7,
    ADC0_SE4b = (0 << ADC_INSTANCE_SHIFT) | (1 << ADC_B_CHANNEL_SHIFT) | 4,
    ADC0_SE5b = (0 << ADC_INSTANCE_SHIFT) | (1 << ADC_B_CHANNEL_SHIFT) | 5,
    ADC0_SE6b = (0 << ADC_INSTANCE_SHIFT) | (1 << ADC_B_CHANNEL_SHIFT) | 6,
    ADC0_SE7b = (0 << ADC_INSTANCE_SHIFT) | (1 << ADC_B_CHANNEL_SHIFT) | 7,
    ADC0_SE8  = (0 << ADC_INSTANCE_SHIFT) | 8,
    ADC0_SE9  = (0 << ADC_INSTANCE_SHIFT) | 9,
    ADC0_SE11  = (0 << ADC_INSTANCE_SHIFT) | 11,
    ADC0_SE12 = (0 << ADC_INSTANCE_SHIFT) | 12,
    ADC0_SE13 = (0 << ADC_INSTANCE_SHIFT) | 13,
    ADC0_SE14 = (0 << ADC_INSTANCE_SHIFT) | 14,
    ADC0_SE15 = (0 << ADC_INSTANCE_SHIFT) | 15,
    ADC0_SE16 = (0 << ADC_INSTANCE_SHIFT) | 16,
    ADC0_SE17 = (0 << ADC_INSTANCE_SHIFT) | 17,
    ADC0_SE18 = (0 << ADC_INSTANCE_SHIFT) | 18,
    ADC0_SE21 = (0 << ADC_INSTANCE_SHIFT) | 21,
    ADC0_SE22 = (0 << ADC_INSTANCE_SHIFT) | 22,
    ADC0_SE23 = (0 << ADC_INSTANCE_SHIFT) | 23,
} ADCName;

typedef enum {
    SPI_0 = 0,
    SPI_1 = 1,
} SPIName;

typedef enum {
    DAC_0 = 0
} DACName;

#ifdef __cplusplus
}
#endif

#endif
