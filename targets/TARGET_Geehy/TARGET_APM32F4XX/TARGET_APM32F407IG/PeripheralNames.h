/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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

#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
#define STDIO_UART_TX MBED_CONF_TARGET_STDIO_UART_TX
#else
#define STDIO_UART_TX PC_6
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
#define STDIO_UART_RX MBED_CONF_TARGET_STDIO_UART_RX
#else
#define STDIO_UART_RX PC_7
#endif
#define STDIO_UART    UART_6

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_1 = (int)ADC1_BASE,
    ADC_2 = (int)ADC2_BASE,
    ADC_3 = (int)ADC3_BASE
} ADCName;

typedef enum {
    DAC_1 = (int)DAC_BASE,
} DACName;

typedef enum {
    UART_1 = (int)USART1_BASE,
    UART_2 = (int)USART2_BASE,
    UART_3 = (int)USART3_BASE,
    UART_4 = (int)UART4_BASE,
    UART_5 = (int)UART5_BASE,
    UART_6 = (int)USART6_BASE
} UARTName;

typedef enum {
    SPI_1 = (int)SPI1_BASE,
    SPI_2 = (int)SPI2_BASE,
    SPI_3 = (int)SPI3_BASE,
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE,
    I2C_3 = (int)I2C3_BASE,
} I2CName;

typedef enum {
    PWM_1 = (int)TMR1_BASE,
    PWM_2 = (int)TMR2_BASE,
    PWM_3 = (int)TMR3_BASE,
    PWM_4 = (int)TMR4_BASE,
    PWM_5 = (int)TMR5_BASE,
    PWM_6 = (int)TMR6_BASE,
    PWM_7 = (int)TMR7_BASE,
    PWM_8 = (int)TMR8_BASE,
    PWM_9 = (int)TMR9_BASE,
    PWM_10 = (int)TMR10_BASE,
    PWM_11 = (int)TMR11_BASE,
    PWM_12 = (int)TMR12_BASE,
    PWM_13 = (int)TMR13_BASE,
    PWM_14 = (int)TMR14_BASE
} PWMName;

typedef enum {
    CAN_1 = (int)CAN1_BASE,
    CAN_2 = (int)CAN2_BASE
} CANName;

typedef enum {
    USB_FS = (int)USB_OTG_FS_BASE,
    USB_HS = (int)USB_OTG_HS_BASE
} USBName;

#ifdef __cplusplus
}
#endif

#endif
