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
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = (int)UART0_BASE
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_0

typedef enum {
    ADC0_SE2  =  2,
    ADC0_SE3  =  3,
    ADC0_SE10 =  4,
    ADC0_SE11 =  5,
    ADC0_SE12 =  6,
    ADC0_SE13 =  7
} ADCName;

typedef enum {
    DAC_0 = 0
} DACName;

typedef enum {
    SPI_0 = (int)SPI0_BASE
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
