/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2019 Cypress Semiconductor Corporation
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
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
#if defined(SCB0_BASE) && (SCB0_UART == 1)
    UART_0 = (int)SCB0_BASE,
#endif
#if defined(SCB1_BASE) && (SCB1_UART == 1)
    UART_1 = (int)SCB1_BASE,
#endif
#if defined(SCB2_BASE) && (SCB2_UART == 1)
    UART_2 = (int)SCB2_BASE,
#endif
#if defined(SCB3_BASE) && (SCB3_UART == 1)
    UART_3 = (int)SCB3_BASE,
#endif
#if defined(SCB4_BASE) && (SCB4_UART == 1)
    UART_4 = (int)SCB4_BASE,
#endif
#if defined(SCB5_BASE) && (SCB5_UART == 1)
    UART_5 = (int)SCB5_BASE,
#endif
#if defined(SCB6_BASE) && (SCB6_UART == 1)
    UART_6 = (int)SCB6_BASE,
#endif
#if defined(SCB7_BASE) && (SCB7_UART == 1)
    UART_7 = (int)SCB7_BASE,
#endif
#if defined(SCB8_BASE) && (SCB8_UART == 1)
    UART_8 = (int)SCB8_BASE,
#endif
#if defined(SCB9_BASE) && (SCB9_UART == 1)
    UART_9 = (int)SCB9_BASE,
#endif
#if defined(SCB10_BASE) && (SCB10_UART == 1)
    UART_10 = (int)SCB10_BASE,
#endif
#if defined(SCB11_BASE) && (SCB11_UART == 1)
    UART_11 = (int)SCB11_BASE,
#endif
#if defined(SCB12_BASE) && (SCB12_UART == 1)
    UART_12 = (int)SCB12_BASE,
#endif
} UARTName;

#define DEVICE_SPI_COUNT CY_IP_MXSCB_INSTANCES

typedef enum {
#if defined(SCB0_BASE) && (SCB0_SPI == 1)
    SPI_0 = (int)SCB0_BASE,
#endif
#if defined(SCB1_BASE) && (SCB1_SPI == 1)
    SPI_1 = (int)SCB1_BASE,
#endif
#if defined(SCB2_BASE) && (SCB2_SPI == 1)
    SPI_2 = (int)SCB2_BASE,
#endif
#if defined(SCB3_BASE) && (SCB3_SPI == 1)
    SPI_3 = (int)SCB3_BASE,
#endif
#if defined(SCB4_BASE) && (SCB4_SPI == 1)
    SPI_4 = (int)SCB4_BASE,
#endif
#if defined(SCB5_BASE) && (SCB5_SPI == 1)
    SPI_5 = (int)SCB5_BASE,
#endif
#if defined(SCB6_BASE) && (SCB6_SPI == 1)
    SPI_6 = (int)SCB6_BASE,
#endif
#if defined(SCB7_BASE) && (SCB7_SPI == 1)
    SPI_7 = (int)SCB7_BASE,
#endif
#if defined(SCB8_BASE) && (SCB8_SPI == 1)
    SPI_8 = (int)SCB8_BASE,
#endif
#if defined(SCB9_BASE) && (SCB9_SPI == 1)
    SPI_9 = (int)SCB9_BASE,
#endif
#if defined(SCB10_BASE) && (SCB10_SPI == 1)
    SPI_10 = (int)SCB10_BASE,
#endif
#if defined(SCB11_BASE) && (SCB11_SPI == 1)
    SPI_11 = (int)SCB11_BASE,
#endif
#if defined(SCB12_BASE) && (SCB12_SPI == 1)
    SPI_12 = (int)SCB12_BASE,
#endif
} SPIName;

typedef enum {
#if defined(SCB0_BASE) && (SCB0_I2C == 1)
    I2C_0 = (int)SCB0_BASE,
#endif
#if defined(SCB1_BASE) && (SCB1_I2C == 1)
    I2C_1 = (int)SCB1_BASE,
#endif
#if defined(SCB2_BASE) && (SCB2_I2C == 1)
    I2C_2 = (int)SCB2_BASE,
#endif
#if defined(SCB3_BASE) && (SCB3_I2C == 1)
    I2C_3 = (int)SCB3_BASE,
#endif
#if defined(SCB4_BASE) && (SCB4_I2C == 1)
    I2C_4 = (int)SCB4_BASE,
#endif
#if defined(SCB5_BASE) && (SCB5_I2C == 1)
    I2C_5 = (int)SCB5_BASE,
#endif
#if defined(SCB6_BASE) && (SCB6_I2C == 1)
    I2C_6 = (int)SCB6_BASE,
#endif
#if defined(SCB7_BASE) && (SCB7_I2C == 1)
    I2C_7 = (int)SCB7_BASE,
#endif
#if defined(SCB8_BASE) && (SCB8_I2C == 1)
    I2C_8 = (int)SCB8_BASE,
#endif
#if defined(SCB9_BASE) && (SCB9_I2C == 1)
    I2C_9 = (int)SCB9_BASE,
#endif
#if defined(SCB10_BASE) && (SCB10_I2C == 1)
    I2C_10 = (int)SCB10_BASE,
#endif
#if defined(SCB11_BASE) && (SCB11_I2C == 1)
    I2C_11 = (int)SCB11_BASE,
#endif
#if defined(SCB12_BASE) && (SCB12_I2C == 1)
    I2C_12 = (int)SCB12_BASE,
#endif
} I2CName;

typedef enum {
#ifdef TCPWM0_BASE
#ifdef TCPWM0_CNT0
    PWM_32b_0 = TCPWM0_BASE,
#endif
#ifdef TCPWM0_CNT1
    PWM_32b_1,
#endif
#ifdef TCPWM0_CNT2
    PWM_32b_2,
#endif
#ifdef TCPWM0_CNT3
    PWM_32b_3,
#endif
#ifdef TCPWM0_CNT4
    PWM_32b_4,
#endif
#ifdef TCPWM0_CNT5
    PWM_32b_5,
#endif
#ifdef TCPWM0_CNT6
    PWM_32b_6,
#endif
#ifdef TCPWM0_CNT7
    PWM_32b_7,
#endif
#endif
#ifdef TCPWM1_BASE
#ifdef TCPWM1_CNT0
    PWM_16b_0 = TCPWM1_BASE,
#endif
#ifdef TCPWM1_CNT1
    PWM_16b_1,
#endif
#ifdef TCPWM1_CNT2
    PWM_16b_2,
#endif
#ifdef TCPWM1_CNT3
    PWM_16b_3,
#endif
#ifdef TCPWM1_CNT4
    PWM_16b_4,
#endif
#ifdef TCPWM1_CNT5
    PWM_16b_5,
#endif
#ifdef TCPWM1_CNT6
    PWM_16b_6,
#endif
#ifdef TCPWM1_CNT7
    PWM_16b_7,
#endif
#ifdef TCPWM1_CNT8
    PWM_16b_8,
#endif
#ifdef TCPWM1_CNT9
    PWM_16b_9,
#endif
#ifdef TCPWM1_CNT10
    PWM_16b_10,
#endif
#ifdef TCPWM1_CNT11
    PWM_16b_11,
#endif
#ifdef TCPWM1_CNT12
    PWM_16b_12,
#endif
#ifdef TCPWM1_CNT13
    PWM_16b_13,
#endif
#ifdef TCPWM1_CNT14
    PWM_16b_14,
#endif
#ifdef TCPWM1_CNT15
    PWM_16b_15,
#endif
#ifdef TCPWM1_CNT16
    PWM_16b_16,
#endif
#ifdef TCPWM1_CNT17
    PWM_16b_17,
#endif
#ifdef TCPWM1_CNT18
    PWM_16b_18,
#endif
#ifdef TCPWM1_CNT19
    PWM_16b_19,
#endif
#ifdef TCPWM1_CNT20
    PWM_16b_20,
#endif
#ifdef TCPWM1_CNT21
    PWM_16b_21,
#endif
#ifdef TCPWM1_CNT22
    PWM_16b_22,
#endif
#ifdef TCPWM1_CNT23
    PWM_16b_23,
#endif
#endif
} PWMName;

#ifdef SAR_BASE
typedef enum {
    ADC_0 = (int)SAR_BASE,
} ADCName;
#endif

#ifdef CTDAC0_BASE
typedef enum {
    DAC_0 = (int)CTDAC0_BASE,
} DACName;
#endif

#ifdef SMIF0_BASE
typedef enum {
    QSPI_0 = (int)SMIF0_BASE,
} SMIFName;
#endif

#ifdef __cplusplus
}
#endif

#endif
