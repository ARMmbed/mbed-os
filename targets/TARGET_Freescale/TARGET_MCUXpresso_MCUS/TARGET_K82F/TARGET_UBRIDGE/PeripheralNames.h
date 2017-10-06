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

typedef enum {
    LPUART_0 = 0,
    LPUART_1 = 1,
    LPUART_2 = 2,
    LPUART_3 = 3,
    LPUART_4 = 4,
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        LPUART_0

/* LPTMR interrupt is defined differently in K82F */
#define LPTMR0_IRQn LPTMR0_LPTMR1_IRQn

typedef enum {
    I2C_0 = 0,
    I2C_1 = 1,
    I2C_2 = 2,
    I2C_3 = 3,
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
    PWM_11 = (2 << TPM_SHIFT) | (0),  // FTM2 CH0
    PWM_12 = (2 << TPM_SHIFT) | (1),  // FTM2 CH1
    PWM_13 = (3 << TPM_SHIFT) | (0),  // FTM3 CH0
    PWM_14 = (3 << TPM_SHIFT) | (1),  // FTM3 CH1
    PWM_15 = (3 << TPM_SHIFT) | (2),  // FTM3 CH2
    PWM_16 = (3 << TPM_SHIFT) | (3),  // FTM3 CH3
    PWM_17 = (3 << TPM_SHIFT) | (4),  // FTM3 CH4
    PWM_18 = (3 << TPM_SHIFT) | (5),  // FTM3 CH5
    PWM_19 = (3 << TPM_SHIFT) | (6),  // FTM3 CH6
    PWM_20 = (3 << TPM_SHIFT) | (7),  // FTM3 CH7
} PWMName;

#define ADC_INSTANCE_SHIFT           8
#define ADC_B_CHANNEL_SHIFT          5
typedef enum {
    ADC0_SE4a = (0 << ADC_INSTANCE_SHIFT) | 4,
    ADC0_SE4b = (0 << ADC_INSTANCE_SHIFT) | (1 << ADC_B_CHANNEL_SHIFT) | 4,
    ADC0_SE5a = (0 << ADC_INSTANCE_SHIFT) | 5,
    ADC0_SE5b = (0 << ADC_INSTANCE_SHIFT) | (1 << ADC_B_CHANNEL_SHIFT) | 5,
    ADC0_SE6a = (0 << ADC_INSTANCE_SHIFT) | 6,
    ADC0_SE6b = (0 << ADC_INSTANCE_SHIFT) | (1 << ADC_B_CHANNEL_SHIFT) | 6,
    ADC0_SE7a = (0 << ADC_INSTANCE_SHIFT) | 7,
    ADC0_SE7b = (0 << ADC_INSTANCE_SHIFT) | (1 << ADC_B_CHANNEL_SHIFT) | 7,
    ADC0_SE8  = (0 << ADC_INSTANCE_SHIFT) | 8,
    ADC0_SE9  = (0 << ADC_INSTANCE_SHIFT) | 9,
    ADC0_SE10 = (0 << ADC_INSTANCE_SHIFT) | 10,
    ADC0_SE11 = (0 << ADC_INSTANCE_SHIFT) | 11,
    ADC0_SE12 = (0 << ADC_INSTANCE_SHIFT) | 12,
    ADC0_SE13 = (0 << ADC_INSTANCE_SHIFT) | 13,
    ADC0_SE14 = (0 << ADC_INSTANCE_SHIFT) | 14,
    ADC0_SE15 = (0 << ADC_INSTANCE_SHIFT) | 15,
    ADC0_SE22 = (0 << ADC_INSTANCE_SHIFT) | 22,
    ADC0_SE23 = (0 << ADC_INSTANCE_SHIFT) | 23,
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
