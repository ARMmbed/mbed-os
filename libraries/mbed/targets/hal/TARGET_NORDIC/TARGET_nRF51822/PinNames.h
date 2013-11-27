/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  12

typedef enum {
	LED_START = 18,
	LED_STOP = 19,
    LED1 = LED_START,
    LED2 = LED_STOP,
    LED3 = LED_START,
    LED4 = LED_STOP,

	BUTTON_START = 16,
	BUTTON_STOP = 17,
	BUTTON0 = BUTTON_START,
	BUTTON1 = BUTTON_STOP,
    // USB Pins
	RX_PIN_NUMBER = 11,
	TX_PIN_NUMBER = 9,
	CTS_PIN_NUMBER = 10,
	RTS_PIN_NUMBER = 8,
	
	USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER,
    // Not connected
    NC = (int)0xFFFFFFFF
} PinName;

typedef enum {
    PullNone = 0,
	PullDown = 1,
    PullUp = 3,
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
