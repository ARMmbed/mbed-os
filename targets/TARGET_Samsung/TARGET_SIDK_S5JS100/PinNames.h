/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  5

typedef enum {
	P00 = 0,
	P01,
	P02,
	P03,
	P04,
	P05,
	P06,
	P07,
	P08,
	P09,
	P10,
	P11,
	P12,
	P13,
	P14,
	P15,
	P16,
	P17,
	P18,
	P19,
	P20,
	P21,
	P22,
	P23,
	P24,
	P25,
	P26,
	P27,
	P28,
	P29,
	P30,
	P31,
	P32,
	P33,
	P34,
	P35,
	P36,
	P37,
	P38,
	P39,
	P40,
	P41,
	P42,
	P43,
	P44,
	P45,
	P46,
	P47,
	P48,
	P49,
	P50,
	P51,
	P52,
	P53,
	P54,
	P55,
	P56,
	P57,
	P58,
	P59,
	P60,
	P61,
	P62,
	P63,
	P64,
	P65,
	P66,
	P67,
	P68,
	P69,
	P70,
	P71,
	P72,

    //Uart
    UART_RX0 = P08,
    UART_TX0 = P09,
    UART_RX1 = P12,
    UART_TX1 = P13,
	UART_RX2 = P27,
	UART_TX2 = P28,
	UART_RX3 = P29,
	UART_TX3 = P30,

    LED1 = P00,
    LED2 = P01,
    LED3 = P02,
    LED4 = P03,

/* I2C Pin Names */
	I2C_SDA = (36 << 16 | 2 << 8),
	I2C_SCL = (35 << 16 | 2 << 8),

/* SPI Pin Names */
	SPI0_CLK = ((0 << 0) | (0x0 << 11) | (0x2 << 8) | (33 << 16)),
	SPI0_CSN = ((0 << 0) | (0x0 << 11) | (0x2 << 8) | (34 << 16)),
	SPI0_MISO = ((0 << 0) | (0x0 << 11) | (0x2 << 8) | (31 << 16)),
	SPI0_MOSI = ((0 << 0) | (0x0 << 11) | (0x2 << 8) | (32 << 16)),

	SPI1_CLK = ((0x1 << 0) | (0x0 << 11) | (0x0 << 8) | (12 << 16)),
	SPI1_CSN = ((0x1 << 0) | (0x0 << 11) | (0x0 << 8) | (15 << 16)),
	SPI1_MISO = ((0x1 << 0) | (0x0 << 11) | (0x0 << 8) | (14 << 16)),
	SPI1_MOSI = ((0x1 << 0) | (0x0 << 11) | (0x0 << 8) | (13 << 16)),

    // Not connected
    NC = (int)0xFFFFFFFF,
} PinName;

typedef enum {
    PullUp = 2,
    PullDown = 1,
    PullNone = 0,
    Repeater = 3,
    OpenDrain = 4,
    PullDefault = PullDown
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
