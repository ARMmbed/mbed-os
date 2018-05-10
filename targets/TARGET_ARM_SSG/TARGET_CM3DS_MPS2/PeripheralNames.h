/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
    UART_0 = 0, /* MCC UART */
    UART_1,     /* MPS2+ UART */
    UART_2,     /* Shield 0 UART */
    UART_3,     /* Shield 1 UART */
    UART_4      /* Shield BT UART */
} UARTName;

typedef enum {
    I2C_0 = (int)MPS2_TSC_I2C_BASE,
    I2C_1 = (int)MPS2_AAIC_I2C_BASE,
    I2C_2 = (int)MPS2_SHIELD0_I2C_BASE,
    I2C_3 = (int)MPS2_SHIELD1_I2C_BASE
} I2CName;

typedef enum {
    ADC0_0 = 0,
    ADC0_1,
    ADC0_2,
    ADC0_3,
    ADC0_4,
    ADC0_5,
    ADC0_6,
    ADC0_7,
    ADC0_8,
    ADC0_9,
    ADC0_10,
    ADC0_11
} ADCName;

typedef enum {
    SPI_0 = 0,
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_4,
    SPI_NC = (SPI_4 + 1)
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
