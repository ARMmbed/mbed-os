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

#include "cmsis.h"
#include "PinNamesTypes.h"
#include "PortNames.h"
#include "cyhal_pin_package.h"
#include "cyhal_utils.h"

typedef cyhal_gpio_t PinName;

// Generic signal names

#define I2C_SCL P6_0
#define I2C_SDA P6_1

#define SPI_MOSI P12_0
#define SPI_MISO P12_1
#define SPI_CLK P12_2
#define SPI_CS P12_4

#define UART_RX P13_0
#define UART_TX P13_1
#define UART_RTS P13_2
#define UART_CTS P13_3

#define BT_UART_RX P3_0
#define BT_UART_TX P3_1
#define BT_UART_CTS P3_3
#define BT_UART_RTS P3_2

#define BT_PIN_POWER P3_4
#define BT_PIN_HOST_WAKE P4_0
#define BT_PIN_DEVICE_WAKE P3_5
#define BT_PIN_DEVICE_RESET P4_1

#define SWITCH2 P0_4
#define LED1 P0_3
#define LED2 P1_1
#define LED3 P10_6

#define LED_RED LED1
#define LED_BLUE LED3
#define LED_GREEN LED2

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

#define CY_STDIO_UART_RX STDIO_UART_RX
#define CY_STDIO_UART_TX STDIO_UART_TX
#define CY_STDIO_UART_CTS STDIO_UART_CTS
#define CY_STDIO_UART_RTS STDIO_UART_RTS

#define CY_BT_UART_RX BT_UART_RX
#define CY_BT_UART_TX BT_UART_TX
#define CY_BT_UART_CTS BT_UART_CTS
#define CY_BT_UART_RTS BT_UART_RTS

#define CY_BT_PIN_POWER BT_PIN_POWER
#define CY_BT_PIN_HOST_WAKE BT_PIN_HOST_WAKE
#define CY_BT_PIN_DEVICE_WAKE BT_PIN_DEVICE_WAKE

#define USBTX UART_TX
#define USBRX UART_RX

#define CY_WIFI_HOST_WAKE P2_7

#endif
