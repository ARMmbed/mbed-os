/* mbed Microcontroller Library
 * Copyright (c) 2016-2020 STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
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
    ADC_1 = (int)ADC1_BASE,
#if defined ADC2_BASE
    ADC_2 = (int)ADC2_BASE,
#endif
#if defined ADC3_BASE
    ADC_3 = (int)ADC3_BASE,
#endif
} ADCName;

#if defined DAC_BASE
typedef enum {
    DAC_1 = (int)DAC_BASE,
} DACName;
#endif

typedef enum {
    UART_1 = (int)USART1_BASE,
    UART_2 = (int)USART2_BASE,
#if defined USART3_BASE
    UART_3 = (int)USART3_BASE,
#endif
#if defined UART4_BASE
    UART_4 = (int)UART4_BASE,
#endif
#if defined UART5_BASE
    UART_5 = (int)UART5_BASE,
#endif
    LPUART_1 = (int)LPUART1_BASE,
} UARTName;

#define DEVICE_SPI_COUNT 3
typedef enum {
    SPI_1 = (int)SPI1_BASE,
#if defined SPI2_BASE
    SPI_2 = (int)SPI2_BASE,
#endif
#if defined SPI3_BASE
    SPI_3 = (int)SPI3_BASE,
#endif
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
#if defined I2C2_BASE
    I2C_2 = (int)I2C2_BASE,
#endif
    I2C_3 = (int)I2C3_BASE,
#if defined I2C4_BASE
    I2C_4 = (int)I2C4_BASE,
#endif
} I2CName;

typedef enum {
    PWM_1  = (int)TIM1_BASE,
    PWM_2  = (int)TIM2_BASE,
#if defined TIM3_BASE
    PWM_3  = (int)TIM3_BASE,
#endif
#if defined TIM4_BASE
    PWM_4  = (int)TIM4_BASE,
#endif
#if defined TIM5_BASE
    PWM_5  = (int)TIM5_BASE,
#endif
#if defined TIM8_BASE
    PWM_8  = (int)TIM8_BASE,
#endif
    PWM_15 = (int)TIM15_BASE,
    PWM_16 = (int)TIM16_BASE,
#if defined TIM17_BASE
    PWM_17 = (int)TIM17_BASE,
#endif
} PWMName;

#if defined CAN1_BASE
typedef enum {
    CAN_1 = (int)CAN1_BASE,
#if defined CAN2_BASE
    CAN_2 = (int)CAN2_BASE,
#endif
} CANName;
#endif

#if defined QSPI_R_BASE || defined OCTOSPI1_R_BASE
typedef enum {
#if defined QSPI_R_BASE
    QSPI_1 = (int)QSPI_R_BASE,
#endif
#if defined OCTOSPI1_R_BASE
    QSPI_1 = (int)OCTOSPI1_R_BASE,
#endif
#if defined OCTOSPI2_R_BASE
    QSPI_2 = (int)OCTOSPI2_R_BASE,
#endif
} QSPIName;
#endif

#if defined OCTOSPI1_R_BASE
typedef enum {
    OSPI_1 = (int)OCTOSPI1_R_BASE,
#if defined OCTOSPI2_R_BASE
    OSPI_2 = (int)OCTOSPI2_R_BASE,
#endif
} OSPIName;
#endif

#if defined USB_OTG_FS_PERIPH_BASE || defined USB_BASE
typedef enum {
#if defined USB_OTG_FS_PERIPH_BASE
    USB_FS = (int)USB_OTG_FS_PERIPH_BASE,
#endif
#if defined USB_BASE
    USB_FS = (int)USB_BASE,
#endif
} USBName;
#endif

#ifdef __cplusplus
}
#endif

#endif
