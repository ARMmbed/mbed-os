/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = (int)SCB0_BASE,
    UART_1 = (int)SCB1_BASE,
    UART_2 = (int)SCB2_BASE,
    UART_3 = (int)SCB3_BASE,
    UART_4 = (int)SCB4_BASE,
    UART_5 = (int)SCB5_BASE,
    UART_6 = (int)SCB6_BASE,
    UART_7 = (int)SCB7_BASE,
} UARTName;


typedef enum {
    SPI_0 = (int)SCB0_BASE,
    SPI_1 = (int)SCB1_BASE,
    SPI_2 = (int)SCB2_BASE,
    SPI_3 = (int)SCB3_BASE,
    SPI_4 = (int)SCB4_BASE,
    SPI_5 = (int)SCB5_BASE,
    SPI_6 = (int)SCB6_BASE,
    SPI_7 = (int)SCB7_BASE,
} SPIName;

typedef enum {
    I2C_0 = (int)SCB0_BASE,
    I2C_1 = (int)SCB1_BASE,
    I2C_2 = (int)SCB2_BASE,
    I2C_3 = (int)SCB3_BASE,
    I2C_4 = (int)SCB4_BASE,
    I2C_5 = (int)SCB5_BASE,
    I2C_6 = (int)SCB6_BASE,
    I2C_7 = (int)SCB7_BASE,
} I2CName;

typedef enum {
    PWM_32b_0 = TCPWM0_BASE,
    PWM_32b_1,
    PWM_32b_2,
    PWM_32b_3,
    PWM_32b_4,
    PWM_32b_5,
    PWM_32b_6,
    PWM_32b_7,
    PWM_16b_0 = TCPWM1_BASE,
    PWM_16b_1,
    PWM_16b_2,
    PWM_16b_3,
    PWM_16b_4,
    PWM_16b_5,
    PWM_16b_6,
    PWM_16b_7,
    PWM_16b_8,
    PWM_16b_9,
    PWM_16b_10,
    PWM_16b_11,
    PWM_16b_12,
    PWM_16b_13,
    PWM_16b_14,
    PWM_16b_15,
    PWM_16b_16,
    PWM_16b_17,
    PWM_16b_18,
    PWM_16b_19,
    PWM_16b_20,
    PWM_16b_21,
    PWM_16b_22,
    PWM_16b_23,
} PWMName;

typedef enum {
    ADC_0 = (int)SAR_BASE,
} ADCName;

typedef enum {
    DAC_0 = (int)CTDAC0_BASE,
} DACName;

#ifdef __cplusplus
}
#endif

#endif
