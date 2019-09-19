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

// Generic signal names
#define UART_RX P5_0
#define UART_TX P5_1

#define I2C_SCL P6_4
#define I2C_SDA P6_5

#define LED1 P6_3
#define LED2 P7_1

#define SWITCH2 P0_4
#define USER_BUTTON SWITCH2
#define BUTTON1 USER_BUTTON

// Standardized interfaces names
#define STDIO_UART_TX UART_TX
#define STDIO_UART_RX UART_RX

#define USBTX UART_TX
#define USBRX UART_RX

#endif
