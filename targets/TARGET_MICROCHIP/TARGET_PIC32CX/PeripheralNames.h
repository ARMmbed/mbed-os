/***************************************************************************//**
 * @file PeripheralNames.h
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
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

#include "pic32cx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STDIO_UART        USART_0

// #if DEVICE_ANALOGIN
// typedef enum {
// #ifdef ADC0_BASE
    // ADC_0 = ADC0_BASE,
// #endif
// } ADCName;
// #endif

// #if DEVICE_ANALOGOUT
// typedef enum {
// #ifdef DAC0_BASE
    // DAC_0 = DAC0_BASE,
// #endif
// } DACName;
// #endif

#if DEVICE_I2C
typedef enum {
    I2C_0 = (uint32_t)TWI0,
    I2C_1 = (uint32_t)TWI1,
    I2C_2 = (uint32_t)TWI2,
    I2C_3 = (uint32_t)TWI3,
    I2C_4 = (uint32_t)TWI4,
    I2C_5 = (uint32_t)TWI5,
    I2C_6 = (uint32_t)TWI6,
    I2C_7 = (uint32_t)TWI7,
} I2CName;
#endif

// #if DEVICE_PWMOUT
// typedef enum {
// #if defined(TIMER_ROUTE_CC0PEN) || defined(TIMER_ROUTEPEN_CC0PEN)
    // PWM_CH0 = 0,
// #endif
// #if defined(TIMER_ROUTE_CC1PEN) || defined(TIMER_ROUTEPEN_CC1PEN)
    // PWM_CH1 = 1,
// #endif
// #if defined(TIMER_ROUTE_CC2PEN) || defined(TIMER_ROUTEPEN_CC2PEN)
    // PWM_CH2 = 2,
// #endif
// #if defined(TIMER_ROUTE_CC3PEN) || defined(TIMER_ROUTEPEN_CC3PEN)
    // PWM_CH3 = 3,
// #endif
// } PWMName;
// #endif

// #if DEVICE_SPI
// typedef enum {
// #ifdef USART0_BASE
    // SPI_0 = USART0_BASE,
// #endif
// #ifdef USART1_BASE
    // SPI_1 = USART1_BASE,
// #endif
// #ifdef USART2_BASE
    // SPI_2 = USART2_BASE,
// #endif
// #ifdef USART3_BASE
    // SPI_3 = USART3_BASE,
// #endif
// #ifdef USART4_BASE
    // SPI_4 = USART4_BASE,
// #endif
// #ifdef USART5_BASE
    // SPI_5 = USART5_BASE,
// #endif
// } SPIName;
// #endif

#if DEVICE_SERIAL
typedef enum {
    UART_0 = (uint32_t)UART,
    USART_0 = (uint32_t)USART0,
    USART_1 = (uint32_t)USART1,
    USART_2 = (uint32_t)USART2,
    USART_3 = (uint32_t)USART3,
    USART_4 = (uint32_t)USART4,
    USART_5 = (uint32_t)USART5,
    USART_6 = (uint32_t)USART6,
    USART_7 = (uint32_t)USART7,
} UARTName;
#endif

// #if DEVICE_CAN
// typedef enum {
// #ifdef CAN0_BASE
    // CAN_0 = (int)CAN0_BASE,
// #endif
// #ifdef CAN1_BASE
    // CAN_1 = (int)CAN1_BASE,
// #endif
// } CANName;
// #endif

// #if DEVICE_QSPI
// typedef enum {
// #ifdef QSPI0_BASE
    // QSPI_0 = QSPI0_BASE,
// #endif
// } QSPIName;
// #endif

#ifdef __cplusplus
}
#endif

#endif
