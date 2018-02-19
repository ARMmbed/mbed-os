/***************************************************************************//**
 * @file PeripheralNames.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "em_adc.h"
#include "em_usart.h"
#include "em_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_ANALOGIN
typedef enum {
#ifdef ADC0_BASE
    ADC_0 = ADC0_BASE,
#endif
} ADCName;
#endif

#if DEVICE_ANALOGOUT
typedef enum {
#ifdef DAC0_BASE
    DAC_0 = DAC0_BASE,
#endif
} DACName;
#endif

#if DEVICE_I2C
typedef enum {
#ifdef I2C0_BASE
    I2C_0 = I2C0_BASE,
#endif
#ifdef I2C1_BASE
    I2C_1 = I2C1_BASE,
#endif
} I2CName;
#endif

#if DEVICE_PWMOUT
typedef enum {
#if defined(TIMER_ROUTE_CC0PEN) || defined(TIMER_ROUTEPEN_CC0PEN)
    PWM_CH0 = 0,
#endif
#if defined(TIMER_ROUTE_CC1PEN) || defined(TIMER_ROUTEPEN_CC1PEN)
    PWM_CH1 = 1,
#endif
#if defined(TIMER_ROUTE_CC2PEN) || defined(TIMER_ROUTEPEN_CC2PEN)
    PWM_CH2 = 2,
#endif
#if defined(TIMER_ROUTE_CC3PEN) || defined(TIMER_ROUTEPEN_CC3PEN)
    PWM_CH3 = 3,
#endif
} PWMName;
#endif

#if DEVICE_SPI
typedef enum {
#ifdef USART0_BASE
    SPI_0 = USART0_BASE,
#endif
#ifdef USART1_BASE
    SPI_1 = USART1_BASE,
#endif
#ifdef USART2_BASE
    SPI_2 = USART2_BASE,
#endif
#ifdef USART3_BASE
    SPI_3 = USART3_BASE,
#endif
} SPIName;
#endif

#if DEVICE_SERIAL
typedef enum {
#ifdef USART0_BASE
    USART_0 = USART0_BASE,
#endif
#ifdef USART1_BASE
    USART_1 = USART1_BASE,
#endif
#ifdef USART2_BASE
    USART_2 = USART2_BASE,
#endif
#ifdef USART3_BASE
    USART_3 = USART3_BASE,
#endif
#ifdef UART0_BASE
    UART_0 = UART0_BASE,
#endif
#ifdef UART1_BASE
    UART_1 = UART1_BASE,
#endif
#ifdef LEUART0_BASE
    LEUART_0 = LEUART0_BASE,
#endif
#ifdef LEUART1_BASE
    LEUART_1 = LEUART1_BASE,
#endif
} UARTName;
#endif

#ifdef __cplusplus
}
#endif

#endif
