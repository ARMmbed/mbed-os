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

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "PinNamesTypes.h"
#include "cyhal_pin_package.h"

typedef cyhal_gpio_t PinName;

// Arduino connector namings
#define A0 P10_0
#define A1 P10_1
#define A2 P10_2
#define A3 P10_3
#define A4 P10_4
#define A5 P10_5

#define D0 P5_0
#define D1 P5_1
#define D2 P5_2
#define D3 P5_3
#define D4 P5_4
#define D5 P5_5
#define D6 P5_6
#define D7 P0_2
#define D8 P13_0
#define D9 P13_1
#define D10 P12_3
#define D11 P12_0
#define D12 P12_1
#define D13 P12_2
#define D14 P6_1
#define D15 P6_0

    // Generic signal names

#define I2C_SCL P6_0
#define I2C_SDA P6_1

#define SPI_MOSI P12_0
#define SPI_MISO P12_1
#define SPI_CLK P12_2
#define SPI_CS P12_4

#define UART_RX P5_0
#define UART_TX P5_1
#define UART_RTS P5_2
#define UART_CTS P5_3

// Reset pin unavailable

#define LED1 P13_7
#define LED2 P1_5
#define LED_RED LED1
#define LED_GREEN LED2

#define SWITCH2 P0_4
#define USER_BUTTON SWITCH2
#define BUTTON1 USER_BUTTON

#define QSPI_CLK P11_7
#define QSPI_IO_0 P11_6
#define QSPI_IO_1 P11_5
#define QSPI_IO_2 P11_4
#define QSPI_IO_3 P11_3
#define QSPI_SEL P11_2

#define QSPI_FLASH1_IO0 QSPI_IO_0
#define QSPI_FLASH1_IO1 QSPI_IO_1
#define QSPI_FLASH1_IO2 QSPI_IO_2
#define QSPI_FLASH1_IO3 QSPI_IO_3
#define QSPI_FLASH1_SCK QSPI_CLK
#define QSPI_FLASH1_CSN QSPI_SEL

// Standardized interfaces names
#define STDIO_UART_TX UART_TX
#define STDIO_UART_RX UART_RX
#define STDIO_UART_CTS UART_CTS
#define STDIO_UART_RTS UART_RTS

#define USBTX UART_TX
#define USBRX UART_RX

#endif