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
    UART_1 = 1,
    UART_2 = 2,
    UART_3 = 3,
    UART_4 = 4,
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_1

#define DAISY_REG_SHIFT       (4)
#define DAISY_REG_VALUE_SHIFT (16)

typedef enum {
    I2C_1 = 1,
    I2C_2 = 2,
    I2C_3 = 3,
    I2C_4 = 4,
} I2CName;

#define PWM_MODULE_SHIFT   2
#define PWM_SHIFT          8

typedef enum {
    PWM_1  = (0 << PWM_SHIFT) | (0 << PWM_MODULE_SHIFT) | (0),  // PWM1 Submodule 0 PWMA
    PWM_2  = (0 << PWM_SHIFT) | (0 << PWM_MODULE_SHIFT) | (1),  // PWM1 Submodule 0 PWMB
    PWM_3  = (0 << PWM_SHIFT) | (1 << PWM_MODULE_SHIFT) | (0),  // PWM1 Submodule 1 PWMA
    PWM_4  = (0 << PWM_SHIFT) | (1 << PWM_MODULE_SHIFT) | (1),  // PWM1 Submodule 1 PWMB
    PWM_5  = (0 << PWM_SHIFT) | (2 << PWM_MODULE_SHIFT) | (0),  // PWM1 Submodule 2 PWMA
    PWM_6  = (0 << PWM_SHIFT) | (2 << PWM_MODULE_SHIFT) | (1),  // PWM1 Submodule 2 PWMB
    PWM_7  = (0 << PWM_SHIFT) | (3 << PWM_MODULE_SHIFT) | (0),  // PWM1 Submodule 3 PWMA
    PWM_8  = (0 << PWM_SHIFT) | (3 << PWM_MODULE_SHIFT) | (1),  // PWM1 Submodule 3 PWMB
} PWMName;

#define ADC_INSTANCE_SHIFT           8
typedef enum {
    ADC1_0 = (1 << ADC_INSTANCE_SHIFT | 0),
    ADC1_1 = (1 << ADC_INSTANCE_SHIFT | 1),
    ADC1_2 = (1 << ADC_INSTANCE_SHIFT | 2),
    ADC1_3 = (1 << ADC_INSTANCE_SHIFT | 3),
    ADC1_4 = (1 << ADC_INSTANCE_SHIFT | 4),
    ADC1_5 = (1 << ADC_INSTANCE_SHIFT | 5),
    ADC1_6 = (1 << ADC_INSTANCE_SHIFT | 6),
    ADC1_7 = (1 << ADC_INSTANCE_SHIFT | 7),
    ADC1_8 = (1 << ADC_INSTANCE_SHIFT | 8),
    ADC1_9 = (1 << ADC_INSTANCE_SHIFT | 9),
    ADC1_10 = (1 << ADC_INSTANCE_SHIFT | 10),
    ADC1_11 = (1 << ADC_INSTANCE_SHIFT | 11),
    ADC1_12 = (1 << ADC_INSTANCE_SHIFT | 12),
    ADC1_13 = (1 << ADC_INSTANCE_SHIFT | 13),
    ADC1_14 = (1 << ADC_INSTANCE_SHIFT | 14),
    ADC1_15 = (1 << ADC_INSTANCE_SHIFT | 15),
    ADC2_0 = (2 << ADC_INSTANCE_SHIFT | 0),
    ADC2_1 = (2 << ADC_INSTANCE_SHIFT | 1),
    ADC2_2 = (2 << ADC_INSTANCE_SHIFT | 2),
    ADC2_3 = (2 << ADC_INSTANCE_SHIFT | 3),
    ADC2_4 = (2 << ADC_INSTANCE_SHIFT | 4),
    ADC2_5 = (2 << ADC_INSTANCE_SHIFT | 5),
    ADC2_6 = (2 << ADC_INSTANCE_SHIFT | 6),
    ADC2_7 = (2 << ADC_INSTANCE_SHIFT | 7),
    ADC2_8 = (2 << ADC_INSTANCE_SHIFT | 8),
    ADC2_9 = (2 << ADC_INSTANCE_SHIFT | 9),
    ADC2_10 = (2 << ADC_INSTANCE_SHIFT | 10),
    ADC2_11 = (2 << ADC_INSTANCE_SHIFT | 11),
    ADC2_12 = (2 << ADC_INSTANCE_SHIFT | 12),
    ADC2_13 = (2 << ADC_INSTANCE_SHIFT | 13),
    ADC2_14 = (2 << ADC_INSTANCE_SHIFT | 14),
    ADC2_15 = (2 << ADC_INSTANCE_SHIFT | 15),
} ADCName;

typedef enum {
    DAC_0 = 0
} DACName;


typedef enum {
    SPI_1 = 1,
    SPI_2 = 2,
    SPI_3 = 3,
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
