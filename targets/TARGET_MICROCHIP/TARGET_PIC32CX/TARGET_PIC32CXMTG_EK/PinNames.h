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

    // Arduino Headers
    A0  = PIO_PA0_IDX,
    A1  = PIO_PA1_IDX,
    A4  = PIO_PA4_IDX,
    A5  = PIO_PA5_IDX,
    A6  = PIO_PA6_IDX,
    A7  = PIO_PA7_IDX,
    A8  = PIO_PA8_IDX,
    A9  = PIO_PA9_IDX,
    A12 = PIO_PA12_IDX,
    A13 = PIO_PA13_IDX,
    A14 = PIO_PA14_IDX,
    A15 = PIO_PA15_IDX,
    A21 = PIO_PA21_IDX,
    A22 = PIO_PA22_IDX,
    A23 = PIO_PA23_IDX,
    A24 = PIO_PA24_IDX,
    A25 = PIO_PA25_IDX,
    A26 = PIO_PA26_IDX,
    A27 = PIO_PA27_IDX,
    A28 = PIO_PA28_IDX,
    A29 = PIO_PA29_IDX,
    A30 = PIO_PA30_IDX,
    A31 = PIO_PA31_IDX,
	B0  = PIO_PB0_IDX,
	B1  = PIO_PB1_IDX,
	B2  = PIO_PB2_IDX,
	B3  = PIO_PB3_IDX,
	B4  = PIO_PB4_IDX,
	B5  = PIO_PB5_IDX,
	B6  = PIO_PB6_IDX,
	B7  = PIO_PB7_IDX,
	B8  = PIO_PB8_IDX,
	B9  = PIO_PB9_IDX,
	B10 = PIO_PB10_IDX,
	B11 = PIO_PB11_IDX,
	B12 = PIO_PB12_IDX,
	B18 = PIO_PB18_IDX,
	B19 = PIO_PB19_IDX,
	B20 = PIO_PB20_IDX,
	B21 = PIO_PB21_IDX,
	B22 = PIO_PB22_IDX,
	B23 = PIO_PB23_IDX,
	B24 = PIO_PB24_IDX,
	B25 = PIO_PB25_IDX,
	B26 = PIO_PB26_IDX,
	C0  = PIO_PC0_IDX,
	C2  = PIO_PC2_IDX,
	C3  = PIO_PC3_IDX,
	C4  = PIO_PC4_IDX,
	C5  = PIO_PC5_IDX,
	C6  = PIO_PC6_IDX,
	C7  = PIO_PC7_IDX,
	C10 = PIO_PC10_IDX,
	C11 = PIO_PC11_IDX,
	C12 = PIO_PC12_IDX,
	C13 = PIO_PC13_IDX,
	C14 = PIO_PC14_IDX,
	C15 = PIO_PC15_IDX,
	C18 = PIO_PC18_IDX,
	C19 = PIO_PC19_IDX,
	C20 = PIO_PC20_IDX,
	C21 = PIO_PC21_IDX,
	C22 = PIO_PC22_IDX,
	D0  = PIO_PD0_IDX,
	D1  = PIO_PD1_IDX,
	D2  = PIO_PD2_IDX,
	D4  = PIO_PD4_IDX,
	D5  = PIO_PD5_IDX,
	D6  = PIO_PD6_IDX,
	D7  = PIO_PD7_IDX,
	D8  = PIO_PD8_IDX,
	D9  = PIO_PD9_IDX,
	D10 = PIO_PD10_IDX,
	D11 = PIO_PD11_IDX,
	D14 = PIO_PD14_IDX,
	D15 = PIO_PD15_IDX,
	D16 = PIO_PD16_IDX,
	D17 = PIO_PD17_IDX,
	D18 = PIO_PD18_IDX,
	D19 = PIO_PD19_IDX,

	/* Board leds */
    LED_GREEN = D16,
    LED_BLUE  = D19,

    /* mbed original LED naming */
    LED1 = LED_GREEN,
    LED2 = LED_BLUE,

    /* Push buttons */
    SW2 = A7,
    /* Standardized button names */
    BUTTON1 = SW2,

    /* Serial Console usart */
    SERIAL_TX = A4,
    SERIAL_RX = A5,

    /* It's not USB, but it is used in mbed_pinmap_default.cpp */
    USBTX = SERIAL_TX,
    USBRX = SERIAL_RX,

    /* Board STDIO */
    STDIO_UART_TX = SERIAL_TX,
    STDIO_UART_RX = SERIAL_RX,

    /* Xplain Pro Serial */
    XPLAINPRO_SERIAL_TX = A0,
    XPLAINPRO_SERIAL_RX = A1,

    /* Mikro Bus 1 Serial */
    MIKROBUS1_SERIAL_TX = D2,
    MIKROBUS1_SERIAL_RX = D1,

    /* Mikro Bus 2 Serial */
    MIKROBUS2_SERIAL_TX = B4,
    MIKROBUS2_SERIAL_RX = B3,

    /* Xplain Pro TWI */
    XPLAINPRO_TWI_SDA = A0,
    XPLAINPRO_TWI_SCL = A1,

    /* Not connected */
    NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
