/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
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
 * 3. Neither the name of ARM Limited nor the names of its contributors
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
#include "PinNames.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_0 = (int)W7500x_ADC_BASE
} ADCName;

typedef enum {
    UART_0 = (int)W7500x_UART0_BASE,
    UART_1 = (int)W7500x_UART1_BASE,
    UART_2 = (int)W7500x_UART2_BASE
} UARTName;


typedef enum {
    SPI_0 = (int)SSP0_BASE,
    SPI_1 = (int)SSP1_BASE
} SPIName;

typedef enum {
    I2C_0 = (int)I2C0_BASE,
    I2C_1 = (int)I2C1_BASE
} I2CName;

typedef enum {    
    PWM_0 = (int)PWM_CH0_BASE,
    PWM_1 = (int)PWM_CH1_BASE,
    PWM_2 = (int)PWM_CH2_BASE,
    PWM_3 = (int)PWM_CH3_BASE,
    PWM_4 = (int)PWM_CH4_BASE,
    PWM_5 = (int)PWM_CH5_BASE,
    PWM_6 = (int)PWM_CH6_BASE,
    PWM_7 = (int)PWM_CH7_BASE
} PWMName;

#ifdef __cplusplus
}
#endif

#endif
