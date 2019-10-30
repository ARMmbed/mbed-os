/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

/* This file will be included in PinNames.h (inside the PinName enum)
 *
 * It defines the names of the board-specific components, like
 *  - LEDs
 *  - Buttons
 *  - UARTS connected via e.g. USB
 */

// mbed standard names for UARTs
STDIO_UART_TX = P1_3,
STDIO_UART_RX = P1_2,
USBTX         = P1_3,
USBRX         = P1_2,

// mbed standard names for LEDs and Buttons
LED1          = P1_0,
LED2          = P2_0,
LED3          = P2_1,
LED4          = P2_2,
BUTTON1       = P1_1,
BUTTON2       = P1_4,

// Alternative names for LEDs and Buttons
LED_RED       = P1_0,
LED_RGB_RED   = P2_0,
LED_RGB_GREEN = P2_1,
LED_RGB_BLUE  = P2_2,
S1            = P1_1,
S2            = P1_4
