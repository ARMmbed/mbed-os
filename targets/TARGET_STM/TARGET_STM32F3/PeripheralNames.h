/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
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
#if defined ADC4_BASE
    ADC_4 = (int)ADC4_BASE
#endif
} ADCName;

typedef enum {
    DAC_1 = (int)DAC1_BASE,
#if defined DAC2_BASE
    DAC_2 = (int)DAC2_BASE
#endif
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

#define DEVICE_SPI_COUNT 4
typedef enum {
#if defined SPI1_BASE
    SPI_1 = (int)SPI1_BASE,
#endif
#if defined SPI2_BASE
    SPI_2 = (int)SPI2_BASE,
#endif
#if defined SPI3_BASE
    SPI_3 = (int)SPI3_BASE,
#endif
#if defined SPI4_BASE
    SPI_4 = (int)SPI4_BASE
#endif
} SPIName;

typedef enum {
    I2C_1 = (int)I2C1_BASE,
#if defined I2C2_BASE
    I2C_2 = (int)I2C2_BASE,
#endif
#if defined I2C3_BASE
    I2C_3 = (int)I2C3_BASE
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
#if defined TIM8_BASE
    PWM_8  = (int)TIM8_BASE,
#endif
    PWM_15 = (int)TIM15_BASE,
    PWM_16 = (int)TIM16_BASE,
    PWM_17 = (int)TIM17_BASE,
#if defined TIM20_BASE
    PWM_20 = (int)TIM20_BASE
#endif
} PWMName;

typedef enum {
    CAN_1 = (int)CAN_BASE
} CANName;

#if defined USB_BASE
typedef enum {
    USB_FS = (int)USB_BASE,
} USBName;
#endif

#ifdef __cplusplus
}
#endif

#endif
