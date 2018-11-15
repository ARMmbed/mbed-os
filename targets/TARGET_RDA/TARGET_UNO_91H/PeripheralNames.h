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
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = (int)RDA_UART0_BASE,
    UART_1 = (int)RDA_UART1_BASE
} UARTName;

typedef enum {
    // Make sure GPIO_BASE & 0x1F == 0, store GPIO index at this field when mapping pins
    GPIO_0 = (int)RDA_GPIO_BASE
} GPIOName;

typedef enum {
    I2C_0  = (int)RDA_I2C0_BASE
} I2CName;

typedef enum {
    SPI_0  = (int)RDA_SPI0_BASE
} SPIName;

typedef enum {
    I2S_0  = (int)RDA_I2S_BASE
} I2SName;

typedef enum {
    WDT_0  = (int)RDA_WDT_BASE
} WDTName;

typedef enum {
    PWM_0 = 0,
    PWM_1,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    PWM_6,
    PWM_7
} PWMName;

typedef enum {
    ADC0_0 = 0,
    ADC0_1,
    ADC0_2
} ADCName;

typedef enum {
    GPADC0_0 = 0,
    GPADC0_1
} GPADCName;

#define STDIO_UART_TX     UART0_TX
#define STDIO_UART_RX     UART0_RX
#define STDIO_UART        UART_0

// Default peripherals
#define MBED_UART0        PA_0, PA_1
#define MBED_UART1        PB_1, PB_2
#define MBED_UARTUSB      USBTX, USBRX

#ifdef __cplusplus
}
#endif

#endif
