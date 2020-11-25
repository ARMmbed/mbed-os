/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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

#include "pic32cx.h"
#include "CommonPinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_SHIFT  5

typedef enum {
	
	/* Board leds */
    LED_GREEN = PIO_PD16_IDX,
	LED_BLUE  = PIO_PD19_IDX,

    /* mbed original LED naming */
    LED1 = LED_GREEN,
    LED2 = LED_BLUE,

    /* Push buttons */
    SW2 = PIO_PA7_IDX,
    /* Standardized button names */
    BUTTON1 = SW2,

    /* Serial */
    SERIAL_TX = PIO_PA4_IDX,
    SERIAL_RX = PIO_PA5_IDX,

    /* It's not USB, but it is used in mbed_pinmap_default.cpp */
    USBTX = SERIAL_TX,
    USBRX = SERIAL_RX,

    /* Board Controller */
    STDIO_UART_TX = SERIAL_TX,
    STDIO_UART_RX = SERIAL_RX,

    /* Not connected */
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
