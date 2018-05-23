/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************** Defines **/
#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_2

/********************************************************* Peripheral Names **/

/**
 * UART Names
 */
typedef enum {
    UART_0 = USIC0_CH0_BASE,
    UART_1 = USIC0_CH1_BASE,
    UART_2 = USIC1_CH0_BASE,
    UART_3 = USIC1_CH1_BASE,
    UART_4 = USIC2_CH0_BASE,
    UART_5 = USIC2_CH1_BASE
} UARTName;

/**
 * EXINT Names
 */
typedef enum {
    ERU0_0 = 0x00,
    ERU0_1 = 0x01,
    ERU0_2 = 0x02,
    ERU0_3 = 0x03,
    ERU1_0 = 0x10,
    ERU1_1 = 0x11,
    ERU1_2 = 0x12,
    ERU1_3 = 0x13
} EXTINTName;

/**
 * DAC Names
 */
typedef enum {
    DAC0 = 0x00,
    DAC1 = 0x01
} DACName;

/**
 * ADC Names
 */
typedef enum {
    ADC0_0 = 0x00,
    ADC0_1 = 0x01,
    ADC0_2 = 0x02,
    ADC0_3 = 0x03,
    ADC0_4 = 0x04,
    ADC0_5 = 0x05,
    ADC0_6 = 0x06,
    ADC0_7 = 0x07,
    ADC1_0 = 0x10,
    ADC1_1 = 0x11,
    ADC1_2 = 0x12,
    ADC1_3 = 0x13,
    ADC1_4 = 0x14,
    ADC1_5 = 0x15,
    ADC1_6 = 0x16,
    ADC1_7 = 0x17,
    ADC2_0 = 0x20,
    ADC2_1 = 0x21,
    ADC2_2 = 0x22,
    ADC2_3 = 0x23,
    ADC2_4 = 0x24,
    ADC2_5 = 0x25,
    ADC2_6 = 0x26,
    ADC2_7 = 0x27,
    ADC3_0 = 0x30,
    ADC3_1 = 0x31,
    ADC3_2 = 0x32,
    ADC3_3 = 0x33,
    ADC3_4 = 0x34,
    ADC3_5 = 0x35,
    ADC3_6 = 0x36,
    ADC3_7 = 0x37
} ADCName;

/**
 * PWM Names
 */
typedef enum {
    PWM_0  = 0x8000,
    PWM_1  = 0x8001,
    PWM_2  = 0x8002,
    PWM_3  = 0x8003,
    PWM_4  = 0x8100,
    PWM_5  = 0x8101,
    PWM_6  = 0x8102,
    PWM_7  = 0x8103,
    PWM_8  = 0x4000,
    PWM_9  = 0x4001,
    PWM_10 = 0x4002,
    PWM_11 = 0x4003,
    PWM_12 = 0x4100,
    PWM_13 = 0x4101,
    PWM_14 = 0x4102,
    PWM_15 = 0x4103,
    PWM_16 = 0x4200,
    PWM_17 = 0x4201,
    PWM_18 = 0x4202,
    PWM_19 = 0x4203,
    PWM_20 = 0x4300,
    PWM_21 = 0x4301,
    PWM_22 = 0x4302,
    PWM_23 = 0x4303,
} PWMName;

/**
 * I2C Names
 */
typedef enum {
    I2C_0 = USIC0_CH0_BASE,
    I2C_1 = USIC0_CH1_BASE,
    I2C_2 = USIC1_CH0_BASE,
    I2C_3 = USIC1_CH1_BASE,
    I2C_4 = USIC2_CH0_BASE,
    I2C_5 = USIC2_CH1_BASE
} I2CName;

/**
 * SPI Names
 */
typedef enum {
    SPI_0 = USIC0_CH0_BASE,
    SPI_1 = USIC0_CH1_BASE,
    SPI_2 = USIC1_CH0_BASE,
    SPI_3 = USIC1_CH1_BASE,
    SPI_4 = USIC2_CH0_BASE,
    SPI_5 = USIC2_CH1_BASE
} SPIName;

#ifdef __cplusplus
}
#endif

#endif

/*EOF*/
