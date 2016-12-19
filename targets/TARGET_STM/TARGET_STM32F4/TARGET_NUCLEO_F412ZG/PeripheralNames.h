/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
    ADC_1 = (int)ADC1_BASE
} ADCName;

typedef enum {
    UART_1 = (int)USART1_BASE,
    UART_2 = (int)USART2_BASE,
    UART_3 = (int)USART3_BASE,
    UART_6 = (int)USART6_BASE
} UARTName;

#define STDIO_UART_TX  PD_8
#define STDIO_UART_RX  PD_9
#define STDIO_UART     UART_3

typedef enum {
    SPI_1 = (int)SPI1_BASE,
    SPI_2 = (int)SPI2_BASE,
    SPI_3 = (int)SPI3_BASE,
    SPI_4 = (int)SPI4_BASE,
    SPI_5 = (int)SPI5_BASE
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE,
    I2C_3 = (int)I2C3_BASE,
    FMPI2C_1 = (int)FMPI2C1_BASE
} I2CName;

typedef enum {
    PWM_1  = (int)TIM1_BASE,
    PWM_2  = (int)TIM2_BASE,
    PWM_3  = (int)TIM3_BASE,
    PWM_4  = (int)TIM4_BASE,
    PWM_5  = (int)TIM5_BASE,
    PWM_8  = (int)TIM8_BASE,
    PWM_9  = (int)TIM9_BASE,
    PWM_10 = (int)TIM10_BASE,
    PWM_11 = (int)TIM11_BASE,
    PWM_12 = (int)TIM12_BASE,
    PWM_13 = (int)TIM13_BASE,
    PWM_14 = (int)TIM14_BASE
} PWMName;

typedef enum {
    CAN_1 = (int)CAN1_BASE,
    CAN_2 = (int)CAN2_BASE
} CANName;

#ifdef __cplusplus
}
#endif

#endif
