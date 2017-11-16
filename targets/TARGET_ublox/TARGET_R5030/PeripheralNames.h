/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STDIO_UART_TX     UART1_TX
#define STDIO_UART_RX     UART1_RX


typedef enum {
    PIN_FUNCTION_UNCLAIMED = 0,
    PIN_FUNCTION_GPIO = 1,  		
    PIN_FUNCTION_MVGPIO = 2,
    PIN_FUNCTION_PWRGPIO = 3, 		
    PIN_FUNCTION_KEY = 4, 
    PIN_FUNCTION_HOSTSELECT = 5, 		
    PIN_FUNCTION_UART = 6, 			
    PIN_FUNCTION_PWM = 7, 			
    PIN_FUNCTION_SPI = 8, 			
    PIN_FUNCTION_I2C = 9, 			
    PIN_FUNCTION_I2S = 10,
    PIN_FUNCTION_DISPLAY = 11,
    PIN_FUNCTION_SDCARD = 12,
    PIN_FUNCTION_TRC = 13,
    PIN_FUNCTION_OSPI = 14,
    PIN_FUNCTION_USIM = 15,
    PIN_FUNCTION_AUDIO = 16,
    PIN_FUNCTION_GNSS = 17,
    PIN_FUNCTION_RF = 18,
    PIN_FUNCTION_MAX_NUMBER
} PinFunction;

typedef enum {
    UART_1 = (int)UART1_CONTROL_BASE,
    UART_2 = (int)UART2_CONTROL_BASE
} UARTName;

typedef enum {
    SPI_1 = (int)SPI1_CONTROL_BASE,
    SPI_2 = (int)SPI2_CONTROL_BASE
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_CONTROL_BASE,
    I2C_2 = (int)I2C2_CONTROL_BASE,
    I2C_3 = (int)I2C3_CONTROL_BASE
} I2CName;

typedef enum {
    PWM_1  = 1,
    PWM_2  = 2,
    PWM_3  = 3
} PWMName;

#ifdef __cplusplus
}
#endif

#endif
