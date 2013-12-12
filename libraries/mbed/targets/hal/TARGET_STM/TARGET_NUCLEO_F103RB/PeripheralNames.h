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
    ADC_1 = (int)ADC1_BASE,
    ADC_2 = (int)ADC2_BASE
} ADCName;

typedef enum {
    UART_1 = (int)USART1_BASE,  
    UART_2 = (int)USART2_BASE
} UARTName;

#define STDIO_UART_TX  PA_2
#define STDIO_UART_RX  PA_3
#define STDIO_UART     UART_2

typedef enum {
    SPI_1 = (int)SPI1_BASE,
    SPI_2 = (int)SPI2_BASE
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE
} I2CName;

typedef enum {
    PWM_2 = (int)TIM2_BASE,
    PWM_3 = (int)TIM3_BASE
} PWMName;

typedef enum {
    CAN_1 = (int)CAN1_BASE
} CANName;

#ifdef __cplusplus
}
#endif

#endif
