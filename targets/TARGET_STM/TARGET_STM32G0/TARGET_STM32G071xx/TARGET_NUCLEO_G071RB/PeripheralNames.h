/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_1 = (int)ADC1_BASE
} ADCName;

typedef enum {
    DAC_1 = (int)DAC_BASE
} DACName;

typedef enum {
    UART_1   = (int)USART1_BASE,
    UART_2   = (int)USART2_BASE,
    UART_3   = (int)USART3_BASE,
    UART_4   = (int)USART4_BASE,
    LPUART_1 = (int)LPUART1_BASE
} UARTName;

#define DEVICE_SPI_COUNT 2
typedef enum {
    SPI_1 = (int)SPI1_BASE,
    SPI_2 = (int)SPI2_BASE
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE
} I2CName;

typedef enum {
    PWM_1  = (int)TIM1_BASE,
    PWM_2  = (int)TIM2_BASE,
    PWM_3  = (int)TIM3_BASE,
    PWM_14 = (int)TIM14_BASE,
    PWM_15 = (int)TIM15_BASE,
    PWM_16 = (int)TIM16_BASE,
    PWM_17 = (int)TIM17_BASE
} PWMName;

#ifdef __cplusplus
}
#endif

#endif
