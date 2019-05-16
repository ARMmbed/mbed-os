/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_0 = (int)ADC0,
    ADC_1 = (int)ADC1
} ADCName;

typedef enum {
    DAC_0 = (int)DAC,
} DACName;

typedef enum {
    UART_0 = (int)USART0,
    UART_1 = (int)USART1,
    UART_2 = (int)USART2,
    UART_3 = (int)UART3,
    UART_4 = (int)UART4
} UARTName;

#define STDIO_UART_TX  PORTA_2
#define STDIO_UART_RX  PORTA_3
#define STDIO_UART     UART_1

typedef enum {
    SPI_0 = (int)SPI0,
    SPI_1 = (int)SPI1,
    SPI_2 = (int)SPI2
} SPIName;

typedef enum {
    I2C_0 = (int)I2C0,
    I2C_1 = (int)I2C1
} I2CName;

typedef enum {
    PWM_0 = (int)TIMER0,
    PWM_1 = (int)TIMER1,
    PWM_2 = (int)TIMER2,
    PWM_3 = (int)TIMER3,
    PWM_4 = (int)TIMER4,
    PWM_5 = (int)TIMER7,
    PWM_6 = (int)TIMER8,
    PWM_7 = (int)TIMER9,
    PWM_8 = (int)TIMER10,
    PWM_9 = (int)TIMER11,
    PWM_10 = (int)TIMER12,
    PWM_11 = (int)TIMER13
} PWMName;

typedef enum {
    CAN_0 = (int)CAN0,
    CAN_1 = (int)CAN1
} CANName;

#ifdef __cplusplus
}
#endif

#endif
