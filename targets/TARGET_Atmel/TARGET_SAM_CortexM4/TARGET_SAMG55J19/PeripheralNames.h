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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = (int)0x4000C200U,
    UART_1 = (int)0x40020200U,
    UART_2 = (int)0x40024200U,
    UART_3 = (int)0x40018200U,
    UART_4 = (int)0x4001C200U,
    UART_5 = (int)0x40008200U,
    UART_6 = (int)0x40040200U,
    UART_7 = (int)0x40034200U,
} UARTName;

typedef enum {
    ADC_0 = 0,
    ADC_1,
    ADC_2,
    ADC_3,
    ADC_4,
    ADC_5,
    ADC_6,
    ADC_7,
} ADCName;


#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_7


#ifdef __cplusplus
}
#endif

#endif
