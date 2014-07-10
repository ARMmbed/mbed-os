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

#include "em_adc.h"
#include "em_usart.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC0_PD0 = NC,
    ADC0_PD1 = NC,
    ADC0_PD2 = NC,
    ADC0_PD3 = NC,
    ADC0_PD4 = _ADC_SINGLECTRL_INPUTSEL_CH4,
    ADC0_PD5 = _ADC_SINGLECTRL_INPUTSEL_CH5,
    ADC0_PD6 = _ADC_SINGLECTRL_INPUTSEL_CH6,
    ADC0_PD7 = _ADC_SINGLECTRL_INPUTSEL_CH7
} ADCName;

typedef enum {
    PWM_CH0 = 0,
    PWM_CH1 = 1,
    PWM_CH2 = 2,
} PWMName;

typedef enum {
    UART_0 = NC,
    UART_1 = NC,
    USART_0 = NC,
    USART_1 = USART1_BASE,
    USART_2 = NC
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        USART1

typedef enum {
    SPI_0 = NC,
    SPI_1 = USART1_BASE,
    SPI_2 = NC
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
