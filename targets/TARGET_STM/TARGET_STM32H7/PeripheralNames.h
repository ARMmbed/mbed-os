/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
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
    ADC_2 = (int)ADC2_BASE,
#if ADC3_BASE
    ADC_3 = (int)ADC3_BASE
#endif
} ADCName;

typedef enum {
    DAC_1 = DAC1_BASE,
#if DAC2_BASE
    DAC_2 = DAC2_BASE,
#endif
} DACName;

typedef enum {
    UART_1 = (int)USART1_BASE,
    UART_2 = (int)USART2_BASE,
    UART_3 = (int)USART3_BASE,
    UART_4 = (int)UART4_BASE,
    UART_5 = (int)UART5_BASE,
    UART_6 = (int)USART6_BASE,
    UART_7 = (int)UART7_BASE,
    UART_8 = (int)UART8_BASE,
#if UART9_BASE
    UART_9 = (int)UART9_BASE,
#endif
#if USART10_BASE
    UART_10 = (int)USART10_BASE,
#endif
    LPUART_1 = (int)LPUART1_BASE
} UARTName;

#define DEVICE_SPI_COUNT 6
typedef enum {
    SPI_1 = (int)SPI1_BASE,
    SPI_2 = (int)SPI2_BASE,
    SPI_3 = (int)SPI3_BASE,
    SPI_4 = (int)SPI4_BASE,
    SPI_5 = (int)SPI5_BASE,
    SPI_6 = (int)SPI6_BASE
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
    I2C_2 = (int)I2C2_BASE,
    I2C_3 = (int)I2C3_BASE,
    I2C_4 = (int)I2C4_BASE,
#if I2C5_BASE
    I2C_5 = (int)I2C5_BASE,
#endif
} I2CName;

typedef enum {
#if HRTIM1_BASE
    PWM_I  = (int)HRTIM1_BASE,
#endif
    PWM_1  = (int)TIM1_BASE,
    PWM_2  = (int)TIM2_BASE,
    PWM_3  = (int)TIM3_BASE,
    PWM_4  = (int)TIM4_BASE,
    PWM_5  = (int)TIM5_BASE,
    PWM_6  = (int)TIM6_BASE,
    PWM_7  = (int)TIM7_BASE,
    PWM_8  = (int)TIM8_BASE,
    PWM_12 = (int)TIM12_BASE,
    PWM_13 = (int)TIM13_BASE,
    PWM_14 = (int)TIM14_BASE,
    PWM_15 = (int)TIM15_BASE,
    PWM_16 = (int)TIM16_BASE,
    PWM_17 = (int)TIM17_BASE,
#if TIM23_BASE
    PWM_23 = (int)TIM23_BASE,
#endif
#if TIM24_BASE
    PWM_24 = (int)TIM24_BASE,
#endif
} PWMName;

typedef enum {
    CAN_1 = (int)FDCAN1_BASE,
    CAN_2 = (int)FDCAN2_BASE,
#if FDCAN3_BASE
    CAN_3 = (int)FDCAN3_BASE,
#endif
} CANName;

typedef enum {
#if QSPI_R_BASE
    QSPI_1 = (int)QSPI_R_BASE,
#endif
#if OCTOSPI1_R_BASE
    QSPI_1 = (int)OCTOSPI1_R_BASE,
#endif
#if OCTOSPI2_R_BASE
    QSPI_2 = (int)OCTOSPI2_R_BASE,
#endif
} QSPIName;

#if defined OCTOSPI1_R_BASE
typedef enum {
    OSPI_1 = (int)OCTOSPI1_R_BASE,
#if defined OCTOSPI2_R_BASE
    OSPI_2 = (int)OCTOSPI2_R_BASE,
#endif
} OSPIName;
#endif

typedef enum {
#if USB_OTG_FS_PERIPH_BASE
    USB_FS = (int)USB_OTG_FS_PERIPH_BASE,
#endif
    USB_HS = (int)USB_OTG_HS_PERIPH_BASE
} USBName;

#ifdef __cplusplus
}
#endif

#endif
