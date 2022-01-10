/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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

#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SERIAL_0 = 0,
    SERIAL_1,
    SERIAL_2,
    SERIAL_3,
    INVALID_SERIAL = (int)NC
} UARTName;


typedef enum {
    PWM_0 = 0,
    PWM_1,
    PWM_2,
    PWM_3,
    PWM_4,
    INVALID_PWM = (int)NC
} PWMName;

typedef enum {
    ADC_A0 = 0,
    ADC_A1,
    ADC_A2,
    ADC_A3,
    ADC_A4,
    ADC_A5,
    ADC_A6,
    ADC_A7,
    ADC_A8,
    ADC_A9,
    ADC_A10,
    ADC_A11,
    ADC_A12,
    ADC_A13,
    ADC_A14,
    ADC_A15,
    ADC_A16,
    ADC_A17,
    ADC_A18,
    INVALID_ADC = (int)NC
} ADCName;

typedef enum {
    I2C_0 = 0,
    I2C_1,
    INVALID_I2C = (int)NC
} I2CName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
    INVALID_SPI = (int)NC
} SPIName;

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
    GPIO_IRQ_10,
    GPIO_IRQ_11,
    GPIO_IRQ_12,
    GPIO_IRQ_13,
    GPIO_IRQ_14,
    GPIO_IRQ_15,
    INVALID_GPIO_IRQ = (int)NC
} GPIO_IRQName;

// DAP UART
#if defined(MBED_CONF_TARGET_STDIO_UART_TX)
#define STDIO_UART_TX MBED_CONF_TARGET_STDIO_UART_TX
#else
#define STDIO_UART_TX CONSOLE_TX
#endif
#if defined(MBED_CONF_TARGET_STDIO_UART_RX)
#define STDIO_UART_RX MBED_CONF_TARGET_STDIO_UART_RX
#else
#define STDIO_UART_RX CONSOLE_RX
#endif

#define SERIAL_TX         CONSOLE_TX
#define SERIAL_RX         CONSOLE_RX

#define STDIO_UART        SERIAL_0

// TxD RxD
#define MBED_UART0        PC0, PC1
#define MBED_UART1        PC4, PC5
#define MBED_UART2        PU0, PU1
#define MBED_UART3        PF6, PF7
#define MBED_UARTUSB      CONSOLE_TX, CONSOLE_RX

// SDA SCK
#define MBED_I2C0         PD3, PD4

// MOSI, MISO, SCLK SS
#define MBED_SPI0         PA3, PA2, PA4, PA1
#define MBED_SPI1         PG5, PG4, PG6, PG1

#define MBED_ANALOGIN0    A0
#define MBED_ANALOGIN1    A1
#define MBED_ANALOGIN2    A2
#define MBED_ANALOGIN3    A3
#define MBED_ANALOGIN4    A4
#define MBED_ANALOGIN5    A5

#define MBED_PWMOUT0      PF4
#define MBED_PWMOUT2      PU2
#define MBED_PWMOUT3      PC2
#define MBED_PWMOUT4      PN1

#ifdef __cplusplus
}
#endif
#endif
