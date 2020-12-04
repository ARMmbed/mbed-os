/***************************************************************************//**
 * @file PinNames.h
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
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

    /* Serial Console usart */
    SERIAL_TX = PIO_PA4_IDX,
    SERIAL_RX = PIO_PA5_IDX,

    /* It's not USB, but it is used in mbed_pinmap_default.cpp */
    USBTX = SERIAL_TX,
    USBRX = SERIAL_RX,

    /* Board STDIO */
    STDIO_UART_TX = SERIAL_TX,
    STDIO_UART_RX = SERIAL_RX,

    /* Xplain Pro Serial */
    XPLAINPRO_SERIAL_TX = PIO_PA0_IDX,
    XPLAINPRO_SERIAL_RX = PIO_PA1_IDX,

    /* Mikro Bus 1 Serial */
    MIKROBUS1_SERIAL_TX = PIO_PD2_IDX,
    MIKROBUS1_SERIAL_RX = PIO_PD1_IDX,

    /* Mikro Bus 2 Serial */
    MIKROBUS2_SERIAL_TX = PIO_PB4_IDX,
    MIKROBUS2_SERIAL_RX = PIO_PB3_IDX,

    /* Not connected */
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
