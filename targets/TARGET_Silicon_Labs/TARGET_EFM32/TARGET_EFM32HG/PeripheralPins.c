/***************************************************************************//**
 * @file PeripheralPins.c
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

#include "PeripheralPins.h"

/************ADC***************/
const PinMap PinMap_ADC[] = {
#ifdef ADC0_BASE
    {PD4, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH4},
    {PD5, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH5},
    {PD6, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH6},
    {PD7, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH7},
#endif
    {NC  , NC   , NC}
};

/************I2C SCL***********/
const PinMap PinMap_I2C_SCL[] = {
#ifdef I2C0_BASE
    /* I2C0 */
    {PA1,  I2C_0, 0},
    {PD7,  I2C_0, 1},
    {PC1,  I2C_0, 4},
    {PF1,  I2C_0, 5},
    {PE13, I2C_0, 6},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

/************I2C SDA***********/
const PinMap PinMap_I2C_SDA[] = {
#ifdef I2C0_BASE
    /* I2C0 */
    {PA0,  I2C_0, 0},
    {PD6,  I2C_0, 1},
    {PC0,  I2C_0, 4},
    {PF0,  I2C_0, 5},
    {PE12, I2C_0, 6},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};
/************PWM***************/
const PinMap PinMap_PWM[] = {
#if defined(TIMER_ROUTE_CC0PEN) || defined(TIMER_ROUTEPEN_CC0PEN)
    /* PWM0 */
    {PA0, PWM_CH0, 0},
    {PF0, PWM_CH0, 5},
#endif
#if defined(TIMER_ROUTE_CC1PEN) || defined(TIMER_ROUTEPEN_CC1PEN)
    /* PWM1 */
    {PA1, PWM_CH1, 0},
    {PF1, PWM_CH1, 5},
#endif
#if defined(TIMER_ROUTE_CC2PEN) || defined(TIMER_ROUTEPEN_CC2PEN)
    /* PWM2 */
    {PA2, PWM_CH2, 0},
    {PF2, PWM_CH2, 5},
#endif
    {NC  , NC   , NC}
};

/*************SPI**************/
const PinMap PinMap_SPI_MOSI[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE10, SPI_0, 0},
    //{NC, SPI_0, 2},     /* SPI_0 loc2 is not bonded */
    {PE13, SPI_0, 3},
    {PB7,  SPI_0, 4},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC0,  SPI_1, 0},
    {PD7,  SPI_1, 3},
    {PF2,  SPI_1, 4},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_MISO[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE11, SPI_0, 0},
    {PC10, SPI_0, 2},
    {PE12, SPI_0, 3},
    {PB8,  SPI_0, 4},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC1,  SPI_1, 0},
    {PD6,  SPI_1, 3},
    {PA0,  SPI_1, 4},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CLK[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE12, SPI_0, 0},
    {PC9,  SPI_0, 2},
    //{PC15, SPI_0, 3},     /* Conflict with SPI_0 loc4 */
    {PB13, SPI_0, 4},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PB7,  SPI_1, 0},
    {PC15, SPI_1, 3},
    {PB11, SPI_1, 4},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CS[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE13, SPI_0, 0},
    {PC8,  SPI_0, 2},
    //{PC14, SPI_0, 3},     /* Conflict with SPI_1 loc3 */
    {PB14, SPI_0, 4},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PB8,  SPI_1, 0},
    {PC14, SPI_1, 3},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

/************UART**************/
const PinMap PinMap_UART_TX[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE10, USART_0, 0},
    //{NC, USART_0, 2},     /* USART_0 loc2 is not bonded */
    {PE13, USART_0, 3},
    {PB7,  USART_0, 4},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC0,  USART_1, 0},
    {PD7,  USART_1, 3},
    {PF2,  USART_1, 4},
#endif
#ifdef LEUART0_BASE
    /* LEUART0 */
    {PD4,  LEUART_0, 0},
    {PB13, LEUART_0, 1},
    {PF0,  LEUART_0, 3},
    {PC14, LEUART_0, 5},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_UART_RX[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE11, USART_0, 0},
    //{PC10, USART_0, 2},
    {PE12, USART_0, 3},
    {PB8,  USART_0, 4},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC1,  USART_1, 0},
    {PD6,  USART_1, 3},
    {PA0,  USART_1, 4},
#endif
#ifdef LEUART0_BASE
    /* LEUART0 */
    {PD5,  LEUART_0, 0},
    {PB14, LEUART_0, 1},
    {PF1,  LEUART_0, 3},
    {PC15, LEUART_0, 5},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};
