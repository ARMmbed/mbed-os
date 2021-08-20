/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2016-2020 STMicroelectronics.
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
    UART_1 = (int)USART1_BASE,
    UART_2 = (int)USART2_BASE,
    UART_3 = (int)USART3_BASE,
#if defined UART4_BASE
    UART_4 = (int)UART4_BASE,
#endif
#if defined UART5_BASE
    UART_5 = (int)UART5_BASE
#endif
} UARTName;

#define DEVICE_SPI_COUNT 3
typedef enum {
    SPI_1 = (int)SPI1_BASE,
    SPI_2 = (int)SPI2_BASE,
    SPI_3 = (int)SPI3_BASE
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE
} I2CName;

typedef enum {
    PWM_2  = (int)TIM2_BASE,
    PWM_3  = (int)TIM3_BASE,
    PWM_4  = (int)TIM4_BASE,
    PWM_5  = (int)TIM5_BASE,
    PWM_9  = (int)TIM9_BASE,
    PWM_10 = (int)TIM10_BASE,
    PWM_11 = (int)TIM11_BASE
} PWMName;

typedef enum {
    USB_FS = (int)USB_BASE,
} USBName;

#ifdef __cplusplus
}
#endif

#endif
