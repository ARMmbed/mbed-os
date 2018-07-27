/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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

#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SERIAL_0 = 0,
    SERIAL_1,
    SERIAL_2,
    INVALID_SERIAL = (int)NC
} UARTName;

typedef enum {
    ADC_AINA0 = 0,
    ADC_AINA1,
    ADC_AINA2,
    ADC_AINA3,
    ADC_AINA4,
    ADC_AINA5,
    ADC_AINA6,
    ADC_AINA7,
    ADC_AINA8,
    ADC_AINA9,
    ADC_AINA10,
    ADC_AINA11,
    ADC_AINA12,
    ADC_AINA13,
    ADC_AINA14,
    ADC_AINA15,
    INVALID_ADC = (int)NC
} ADCName;

typedef enum {
    DAC_A0 = 0,
    DAC_A1,
    INVALID_DAC = (int)NC
} DACName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
    INVALID_SPI = (int)NC
} SPIName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    INVALID_I2C = (int)NC
} I2CName;

typedef enum {
    PWM_0 = 0,
    PWM_1,
    PWM_2,
    PWM_3,
    PWM_4,
    INVALID_PWM = (int)NC
} PWMName;

typedef enum {
    GPIO_IRQ_0 = 0,
    GPIO_IRQ_1,
    GPIO_IRQ_2,
    GPIO_IRQ_3,
    GPIO_IRQ_4,
    GPIO_IRQ_5,
    GPIO_IRQ_6,
    GPIO_IRQ_7,
    GPIO_IRQ_8,
    GPIO_IRQ_9,
    GPIO_IRQ_A,
    GPIO_IRQ_B,
    GPIO_IRQ_C,
    GPIO_IRQ_D,
    GPIO_IRQ_E,
    GPIO_IRQ_F,
    INVALID_GPIO_IRQ = (int)NC
} GPIO_IRQName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        SERIAL_1

#ifdef __cplusplus
}
#endif

#endif
