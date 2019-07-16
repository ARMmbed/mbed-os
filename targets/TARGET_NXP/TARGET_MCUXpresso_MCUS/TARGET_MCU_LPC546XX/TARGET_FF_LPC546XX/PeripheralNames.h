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
#include "PortNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OSC32KCLK = 0,
} RTCName;

typedef enum {
    UART_0 = Flexcomm0,
    UART_2 = Flexcomm2,
    UART_7 = Flexcomm7
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_0

typedef enum {
    I2C_7 = Flexcomm7,
    I2C_2 = Flexcomm2
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
    PWM_25 = (3 << TPM_SHIFT) | (0),  // FTM3 CH0
    PWM_26 = (3 << TPM_SHIFT) | (1),  // FTM3 CH1
    PWM_27 = (3 << TPM_SHIFT) | (2),  // FTM3 CH2
    PWM_28 = (3 << TPM_SHIFT) | (3),  // FTM3 CH3
    PWM_29 = (3 << TPM_SHIFT) | (4),  // FTM3 CH4
    PWM_30 = (3 << TPM_SHIFT) | (5),  // FTM3 CH5
    PWM_31 = (3 << TPM_SHIFT) | (6),  // FTM3 CH6
    PWM_32 = (3 << TPM_SHIFT) | (7),  // FTM3 CH7
} PWMName;

#define ADC_INSTANCE_SHIFT         8
#define ADC_B_CHANNEL_SHIFT        5

typedef enum {
    ADC0_SE0  = 0,
    ADC0_SE1  = 1,
    ADC0_SE2  = 2,
    ADC0_SE3  = 3,
    ADC0_SE4  = 4,
    ADC0_SE5  = 5,
    ADC0_SE6  = 6,
    ADC0_SE7  = 7,
    ADC0_SE8  = 8,
    ADC0_SE9  = 9,
    ADC0_SE10 = 10,
    ADC0_SE11 = 11,
} ADCName;

typedef enum {
    CAN_1 = 1
} CANName;

#define SSELNUM_SHIFT    16
typedef enum {
    SPI_0 = Flexcomm0,
    SPI_2 = Flexcomm2,
    SPI_3 = Flexcomm3
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
