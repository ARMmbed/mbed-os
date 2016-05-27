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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_0 = (int)CMSDK_UART1_BASE,
    UART_1 = (int)CMSDK_UART3_BASE,
    UART_2 = (int)CMSDK_UART0_BASE,
    UART_3 = (int)CMSDK_UART2_BASE
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
    SPI_0 = (int)MPS2_SSP0_BASE,
    SPI_1 = (int)MPS2_SSP1_BASE,
      SPI_2 = (int)MPS2_SSP2_BASE,
    SPI_3 = (int)MPS2_SSP3_BASE,
    SPI_4 = (int)MPS2_SSP4_BASE
} SPIName;

typedef enum {
    PWM_1 = 0,
    PWM_2,
    PWM_3,
    PWM_4,
    PWM_5,
    PWM_6,
    PWM_7,
    PWM_8,
    PWM_9,
    PWM_10,
    PWM_11
} PWMName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        UART_0

#define MBED_UART0        USBTX, USBRX
#define MBED_UART1        XB_TX, XB_RX
#define MBED_UART2        SH0_TX, SH0_RX
#define MBED_UART3        SH1_TX, SH1_RX
#define MBED_UARTUSB      USBTX, USBRX

#ifdef __cplusplus
}
#endif

#endif
