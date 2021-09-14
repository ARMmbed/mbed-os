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
    ADC_1 = (int)ADC1_BASE,
} ADCName;

#if defined DAC_BASE
typedef enum {
    DAC_1 = (int)DAC_BASE,
} DACName;
#endif

typedef enum {
    UART_1   = (int)USART1_BASE,
    UART_2   = (int)USART2_BASE,
#if defined USART3_BASE
    UART_3   = (int)USART3_BASE,
#endif
#if defined USART4_BASE
    UART_4   = (int)USART4_BASE,
#endif
#if defined USART5_BASE
    UART_5   = (int)USART5_BASE,
#endif
#if defined USART6_BASE
    UART_6   = (int)USART6_BASE,
#endif
#if defined LPUART1_BASE
    LPUART_1 = (int)LPUART1_BASE,
#endif
#if defined LPUART2_BASE
    LPUART_2 = (int)LPUART2_BASE,
#endif
} UARTName;

#define DEVICE_SPI_COUNT 2
typedef enum {
    SPI_1 = (int)SPI1_BASE,
    SPI_2 = (int)SPI2_BASE,
#if defined SPI3_BASE
    SPI_3 = (int)SPI3_BASE,
#endif
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE,
#if defined I2C3_BASE
    I2C_3 = (int)I2C3_BASE,
#endif
} I2CName;

typedef enum {
    PWM_1  = (int)TIM1_BASE,
#if defined TIM2_BASE
    PWM_2  = (int)TIM2_BASE,
#endif
    PWM_3  = (int)TIM3_BASE,
#if defined TIM4_BASE
    PWM_4  = (int)TIM4_BASE,
#endif
    PWM_14 = (int)TIM14_BASE,
#if defined TIM15_BASE
    PWM_15 = (int)TIM15_BASE,
#endif
    PWM_16 = (int)TIM16_BASE,
    PWM_17 = (int)TIM17_BASE,
} PWMName;

#if defined FDCAN1_BASE
typedef enum {
    CAN_1 = (int)FDCAN1_BASE,
#if defined FDCAN2_BASE
    CAN_2 = (int)FDCAN2_BASE,
#endif
} CANName;
#endif

#if defined USB_BASE
typedef enum {
    USB_FS = (int)USB_BASE
} USBName;
#endif

#ifdef __cplusplus
}
#endif

#endif
