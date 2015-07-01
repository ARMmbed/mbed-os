/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#define PORT_SHIFT  5

typedef enum {
    // MPS2 EXP Pin Names
		EXP0 = 0,
		EXP1 = 1,
		EXP2 = 2,
		EXP3 = 3,
		EXP4 = 4,
		EXP5 = 5,
		EXP6 = 6,
		EXP7 = 7,
		EXP8 = 8,
		EXP9 = 9,
		EXP10 = 10,
		EXP11 = 11,
		EXP12 = 12,
		EXP13 = 13,
		EXP14 = 14,
		EXP15 = 15,
		EXP16 = 16,
		EXP17 = 17,
		EXP18 = 18,
		EXP19 = 19,
		EXP20 = 20,
		EXP21 = 21,
		EXP22 = 22,
		EXP23 = 23,
		EXP24 = 24,
		EXP25 = 25,
		
		EXP26 = 26,
		EXP27 = 27,
		EXP28 = 28,
		EXP29 = 29,
		EXP30 = 30,
		EXP31 = 31,
		EXP32 = 32, 
		EXP33 = 33, 
		EXP34 = 34, 
		EXP35 = 35, 
		EXP36 = 36, 
		EXP37 = 37, 
		EXP38 = 38, 
		EXP39 = 39, 
		EXP40 = 40, 
		EXP41 = 41, 
		EXP42 = 42, 
		EXP43 = 43, 
		EXP44 = 44, 
		EXP45 = 45, 
		EXP46 = 46, 
		EXP47 = 47, 
		EXP48 = 48, 
		EXP49 = 49, 
		EXP50 = 50, 
		EXP51 = 51, 
		
// Other mbed Pin Names

		//LEDs on mps2
		//user leds
		USERLED1 = 100,
		USERLED2 = 101,
		//user switches
		USERSW1  = 110,
		USERSW2  = 111,
		
		//mcc leds
		LED1 = 200,
		LED2 = 201,
		LED3 = 202,
		LED4 = 203,
		LED5 = 204,
		LED6 = 205,
		LED7 = 206,
		LED8 = 207,
		
		//MCC Switches
		SW1 = 210,
		SW2 = 211,
		SW3 = 212,
		SW4 = 213,
		SW5 = 214,
		SW6 = 215,
		SW7 = 216,
		SW8 = 217,
		
		//MPS2 SPI header pins j21
		MOSI_SPI = 300,
		MISO_SPI = 301,
		SCLK_SPI = 302,
		SSEL_SPI = 303,
		
		//MPS2 CLCD SPI
		CLCD_MOSI = 304,
		CLCD_MISO = 305,
		CLCD_SCLK = 306,
		CLCD_SSEL = 307,
    	CLCD_RESET = 308,
    	CLCD_RS = 309,
    	CLCD_RD = 310,
    	CLCD_BL_CTRL = 311,
		
		
		//MPS2 Uart
		USBTX  = 400,
		USBRX  = 401,
		UART_TX1 = 402,
		UART_RX1 = 403,
		UART_TX2 = 404,
		UART_RX2 = 405,
		
		//MPS2 I2C touchscreen and audio
		TSC_SDA = 500,
		TSC_SCL = 501,
		AUD_SDA = 502,
		AUD_SCL = 503,
		
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

