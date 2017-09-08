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

typedef enum {
    ADC_0 = ADC0_BASE
} ADCName;

typedef enum {
    I2C_0 = I2C0_BASE,
} I2CName;

typedef enum {
    PWM_CH0 = 0,
    PWM_CH1 = 1,
    PWM_CH2 = 2,
    PWM_CH3 = 3
} PWMName;

typedef enum {
    USART_0 = USART0_BASE,
    USART_1 = USART1_BASE,
    LEUART_0 = LEUART0_BASE,
} UARTName;

typedef enum {
    SPI_0 = USART0_BASE,
    SPI_1 = USART1_BASE,
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
