/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *
 * either express or implied.
 *
 * See the License for the specific language governing permissions and limitations under the License.
 */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "BlueNRG1_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IO_0  = GPIO_Pin_0,
	IO_1  = GPIO_Pin_1,
	IO_2  = GPIO_Pin_2,
	IO_3  = GPIO_Pin_3,
	IO_4  = GPIO_Pin_4,
	IO_5  = GPIO_Pin_5,
	IO_6  = GPIO_Pin_6,
	IO_7  = GPIO_Pin_7,
	IO_8  = GPIO_Pin_8,
	IO_9  = GPIO_Pin_9,
	IO_10 = GPIO_Pin_10,
	IO_11 = GPIO_Pin_11,
	IO_12 = GPIO_Pin_12,
	IO_13 = GPIO_Pin_13,
	IO_14 = GPIO_Pin_14,
	IO_15 = GPIO_Pin_15,

	IO_16 = GPIO_Pin_16,
	IO_17 = GPIO_Pin_17,
	IO_18 = GPIO_Pin_18,
	IO_19 = GPIO_Pin_19,
	IO_20 = GPIO_Pin_20,
	IO_21 = GPIO_Pin_21,
	IO_22 = GPIO_Pin_22,
	IO_23 = GPIO_Pin_23,
	IO_24 = GPIO_Pin_24,
	IO_25 = GPIO_Pin_25,

    // ADC internal channels
	ADC_OPEN = 0x10,
	ADC2     = 0x11,
	ADC1     = 0x12,    /////////////cambiare!!!!!!!!!
	ADC_DIFF = 0x13,
    ADC_TEMP = 0x14,
    ADC_VBAT = 0x15,
	ADC_VREF = 0x16,


    // STEVAL_IDB008V2 signals namings
	// DIO
	DIO0 = IO_0,
	DIO1 = IO_1,
	DIO2 = IO_2,
	DIO3 = IO_3,
	DIO4 = IO_4,
	DIO5 = IO_5,
	DIO6 = IO_6,
	DIO7 = IO_7,
	DIO8 = IO_8,
	DIO11 = IO_11,
	DIO12 = IO_12,
	DIO13 = IO_13,
	DIO14 = IO_14,
	// LEDS
	DL1 = DIO6,
	DL2 = DIO7,
	DL3 = DIO14,
	// PUSH BUTTONS
	PUSH1 = DIO13,
	PUSH2 = DIO5,


	// Arduino connector namings
	A0          = DIO12,
	//A1          = TEST1,
	A2          = DIO13,
	A3          = DIO14,
	A4          = ADC1,
	A5          = ADC2,
	D0          = DIO11,
	D1          = DIO8,
	D2          = DIO11,
	D3          = DIO0,
	D4          = DIO2,
	D5          = DIO3,
	D6          = DIO6,
	//D7          = RESET,
	D8          = DIO8,
	D9          = DIO7,
	D10         = DIO1,
	D11         = DIO2,
	D12         = DIO3,
	D13         = DIO0,
	D14         = DIO5,
	D15         = DIO4,

    // STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
    STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
    STDIO_UART_TX = IO_8,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
    STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
    STDIO_UART_RX = IO_11,
#endif


    // Generic signals namings
    LED1           = DL1,
    LED2           = DL2,
    LED3           = DL3,
	USER_BUTTON    = PUSH1,
    USER_BUTTON_1  = PUSH1,
	USER_BUTTON_2  = PUSH2,

    // Standardized button names
	BUTTON      = USER_BUTTON_1,
    BUTTON1     = USER_BUTTON_1,
	BUTTON2     = USER_BUTTON_2,
    SERIAL_TX   = STDIO_UART_TX,
    SERIAL_RX   = STDIO_UART_RX,
    I2C_SCL     = DIO5,
    I2C_SDA     = DIO4,
    SPI_MOSI    = DIO2,
    SPI_MISO    = DIO3,
    SPI_SCK     = DIO0,
    SPI_CS      = DIO1,
	USBTX		= STDIO_UART_TX,
	USBRX		= STDIO_UART_RX,

    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;
/*
typedef enum {
	PullNone = 0,
	PullEnable = 1,
	PullDefault = PullNone,
    PullUp = PullEnable,
    PullDown = PullNone,
	NoPull = PullNone
} PinMode;
*/
typedef enum {
    PIN_INPUT = 0,
    PIN_OUTPUT = 6
} PinDirection;

typedef enum {
	PortA = 0
} PortName;

typedef enum {
    PullNone = 0,
	PullEnable = 1,
	PullDefault = PullNone
} PinMode;


#ifdef __cplusplus
}
#endif

#endif
