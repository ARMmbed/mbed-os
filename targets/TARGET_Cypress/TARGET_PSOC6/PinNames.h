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
#include "cybsp_types.h"

// I2C
#ifdef CYBSP_I2C_SCL
#define I2C_SCL CYBSP_I2C_SCL
#endif
#ifdef CYBSP_I2C_SDA
#define I2C_SDA CYBSP_I2C_SDA
#endif

// SPI
#ifdef CYBSP_SPI_MOSI
#define SPI_MOSI CYBSP_SPI_MOSI
#endif
#ifdef CYBSP_SPI_MISO
#define SPI_MISO CYBSP_SPI_MISO
#endif
#ifdef CYBSP_SPI_CLK
#define SPI_CLK CYBSP_SPI_CLK
#endif
#ifdef CYBSP_SPI_CS
#define SPI_CS CYBSP_SPI_CS
#endif

// UART
#ifdef CYBSP_DEBUG_UART_RX
#define UART_RX CYBSP_DEBUG_UART_RX
#define STDIO_UART_RX CYBSP_DEBUG_UART_RX
#define USBRX CYBSP_DEBUG_UART_RX
#endif
#ifdef CYBSP_DEBUG_UART_TX
#define UART_TX CYBSP_DEBUG_UART_TX
#define STDIO_UART_TX CYBSP_DEBUG_UART_TX
#define USBTX CYBSP_DEBUG_UART_TX
#endif
#ifdef CYBSP_DEBUG_UART_RTS
#define UART_RTS CYBSP_DEBUG_UART_RTS
#define STDIO_UART_RTS CYBSP_DEBUG_UART_RTS
#define USBRTS CYBSP_DEBUG_UART_RTS
#endif
#ifdef CYBSP_DEBUG_UART_CTS
#define UART_CTS CYBSP_DEBUG_UART_CTS
#define STDIO_UART_CTS CYBSP_DEBUG_UART_CTS
#define USBCTS CYBSP_DEBUG_UART_CTS
#endif

// LEDs
#ifdef CYBSP_USER_LED1
#define LED1 CYBSP_USER_LED1
#else
#define LED1 NC
#endif
#ifdef CYBSP_USER_LED2
#define LED2 CYBSP_USER_LED2
#else
#define LED2 NC
#endif
#ifdef CYBSP_USER_LED3
#define LED3 CYBSP_USER_LED3
#else
#define LED3 NC
#endif
#ifdef CYBSP_USER_LED4
#define LED4 CYBSP_USER_LED4
#else
#define LED4 NC
#endif
#ifdef CYBSP_USER_LED5
#define LED5 CYBSP_USER_LED5
#else
#define LED5 NC
#endif

#ifdef CYBSP_LED_RGB_RED
#define LED_RED CYBSP_LED_RGB_RED
#endif
#ifdef CYBSP_LED_RGB_BLUE
#define LED_BLUE CYBSP_LED_RGB_BLUE
#endif
#ifdef CYBSP_LED_RGB_GREEN
#define LED_GREEN CYBSP_LED_RGB_GREEN
#endif

// User button
#ifdef CYBSP_USER_BTN
#define USER_BUTTON CYBSP_USER_BTN
#define BUTTON1 CYBSP_USER_BTN
#endif

// QSPI
#ifdef CYBSP_QSPI_D0
#define QSPI_IO_0 CYBSP_QSPI_D0
#define QSPI_FLASH1_IO0 CYBSP_QSPI_D0
#endif
#ifdef CYBSP_QSPI_D1
#define QSPI_IO_1 CYBSP_QSPI_D1
#define QSPI_FLASH1_IO1 CYBSP_QSPI_D1
#endif
#ifdef CYBSP_QSPI_D2
#define QSPI_IO_2 CYBSP_QSPI_D2
#define QSPI_FLASH1_IO2 CYBSP_QSPI_D2
#endif
#ifdef CYBSP_QSPI_D3
#define QSPI_IO_3 CYBSP_QSPI_D3
#define QSPI_FLASH1_IO3 CYBSP_QSPI_D3
#endif
#ifdef CYBSP_QSPI_SCK
#define QSPI_CLK CYBSP_QSPI_SCK
#define QSPI_FLASH1_SCK CYBSP_QSPI_SCK
#endif
#ifdef CYBSP_QSPI_SS
#define QSPI_SEL CYBSP_QSPI_SS
#define QSPI_FLASH1_CSN CYBSP_QSPI_SS
#endif

// Arduino Header
#ifdef CYBSP_A0
#define A0 CYBSP_A0
#endif
#ifdef CYBSP_A1
#define A1 CYBSP_A1
#endif
#ifdef CYBSP_A2
#define A2 CYBSP_A2
#endif
#ifdef CYBSP_A3
#define A3 CYBSP_A3
#endif
#ifdef CYBSP_A4
#define A4 CYBSP_A4
#endif
#ifdef CYBSP_A5
#define A5 CYBSP_A5
#endif
#ifdef CYBSP_D0
#define D0 CYBSP_D0
#endif
#ifdef CYBSP_D1
#define D1 CYBSP_D1
#endif
#ifdef CYBSP_D2
#define D2 CYBSP_D2
#endif
#ifdef CYBSP_D3
#define D3 CYBSP_D3
#endif
#ifdef CYBSP_D4
#define D4 CYBSP_D4
#endif
#ifdef CYBSP_D5
#define D5 CYBSP_D5
#endif
#ifdef CYBSP_D6
#define D6 CYBSP_D6
#endif
#ifdef CYBSP_D7
#define D7 CYBSP_D7
#endif
#ifdef CYBSP_D8
#define D8 CYBSP_D8
#endif
#ifdef CYBSP_D9
#define D9 CYBSP_D9
#endif
#ifdef CYBSP_D10
#define D10 CYBSP_D10
#endif
#ifdef CYBSP_D11
#define D11 CYBSP_D11
#endif
#ifdef CYBSP_D12
#define D12 CYBSP_D12
#endif
#ifdef CYBSP_D13
#define D13 CYBSP_D13
#endif
#ifdef CYBSP_D14
#define D14 CYBSP_D14
#endif
#ifdef CYBSP_D15
#define D15 CYBSP_D15
#endif

#endif // MBED_PINNAMES_H
