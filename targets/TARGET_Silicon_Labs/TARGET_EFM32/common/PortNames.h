/***************************************************************************//**
 * @file PortNames.h
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
#ifndef MBED_PORTNAMES_H
#define MBED_PORTNAMES_H

#include "em_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
#if ( _GPIO_PORT_A_PIN_COUNT > 0 )
    PortA = gpioPortA, /**< Port A */
#endif
#if ( _GPIO_PORT_B_PIN_COUNT > 0 )
    PortB = gpioPortB, /**< Port B */
#endif
#if ( _GPIO_PORT_C_PIN_COUNT > 0 )
    PortC = gpioPortC, /**< Port C */
#endif
#if ( _GPIO_PORT_D_PIN_COUNT > 0 )
    PortD = gpioPortD, /**< Port D */
#endif
#if ( _GPIO_PORT_E_PIN_COUNT > 0 )
    PortE = gpioPortE, /**< Port E */
#endif
#if ( _GPIO_PORT_F_PIN_COUNT > 0 )
    PortF = gpioPortF, /**< Port F */
#endif
#if defined( _GPIO_PORT_G_PIN_COUNT ) && ( _GPIO_PORT_G_PIN_COUNT > 0 )
    PortG = gpioPortG, /**< Port G */
#endif
#if defined( _GPIO_PORT_H_PIN_COUNT ) && ( _GPIO_PORT_H_PIN_COUNT > 0 )
    PortH = gpioPortH, /**< Port H */
#endif
#if defined( _GPIO_PORT_I_PIN_COUNT ) && ( _GPIO_PORT_I_PIN_COUNT > 0 )
    PortI = gpioPortI, /**< Port I */
#endif
#if defined( _GPIO_PORT_J_PIN_COUNT ) && ( _GPIO_PORT_J_PIN_COUNT > 0 )
    PortJ = gpioPortJ, /**< Port J */
#endif
#if defined( _GPIO_PORT_K_PIN_COUNT ) && ( _GPIO_PORT_K_PIN_COUNT > 0 )
    PortK = gpioPortK, /**< Port K */
#endif
} PortName;

#ifdef __cplusplus
}
#endif
#endif
