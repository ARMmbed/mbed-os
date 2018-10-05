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
#include "mbed_toolchain.h"

/************ADC***************/
MBED_WEAK const PinMap PinMap_ADC[] = {
#ifdef ADC0_BASE
    {PD4, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH4},
    {PD5, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH5},
    {PD6, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH6},
    {PD7, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH7},
#endif
    {NC  , NC   , NC}
};

/************I2C SCL***********/
MBED_WEAK const PinMap PinMap_I2C_SCL[] = {
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
MBED_WEAK const PinMap PinMap_I2C_SDA[] = {
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
MBED_WEAK const PinMap PinMap_PWM[] = {
#if defined(TIMER_ROUTE_CC0PEN) || defined(TIMER_ROUTEPEN_CC0PEN)
    /* PWM0 */
    {PA0, PWM_CH0, 0},
#endif
#if defined(TIMER_ROUTE_CC1PEN) || defined(TIMER_ROUTEPEN_CC1PEN)
    /* PWM1 */
    {PA1, PWM_CH1, 0},
#endif
#if defined(TIMER_ROUTE_CC2PEN) || defined(TIMER_ROUTEPEN_CC2PEN)
    /* PWM2 */
    {PA2, PWM_CH2, 0},
#endif
    {NC  , NC   , NC}
};

/*************SPI**************/
MBED_WEAK const PinMap PinMap_SPI_MOSI[] = {
#ifdef USART1_BASE
    /* USART1 */
    {PC0, SPI_1, 0},
    {PD7, SPI_1, 3},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

MBED_WEAK const PinMap PinMap_SPI_MISO[] = {
#ifdef USART1_BASE
    /* USART1 */
    {PC1, SPI_1, 0},
    {PD6, SPI_1, 3},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

MBED_WEAK const PinMap PinMap_SPI_CLK[] = {
#ifdef USART1_BASE
    /* USART1 */
    {PB7, SPI_1, 0},
    {PC15, SPI_1, 3},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

MBED_WEAK const PinMap PinMap_SPI_CS[] = {
#ifdef USART1_BASE
    /* USART1 */
    {PB8, SPI_1, 0},
    {PC14, SPI_1, 3},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

/************UART**************/
MBED_WEAK const PinMap PinMap_UART_TX[] = {
#ifdef USART1_BASE
    /* USART1 */
    {PC0, USART_1, 0},
    {PD7, USART_1, 3},
#endif
#ifdef LEUART0_BASE
    /* LEUART0 */
    {PD4,  LEUART_0, 0},
    {PB13, LEUART_0, 1},
    {PF0,  LEUART_0, 3},
    {PF2,  LEUART_0, 4},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

MBED_WEAK const PinMap PinMap_UART_RX[] = {
#ifdef USART1_BASE
    /* USART1 */
    {PC1, USART_1, 0},
    {PD6, USART_1, 3},
#endif
#ifdef LEUART0_BASE
    /* LEUART0 */
    {PD5,  LEUART_0, 0},
    {PB14, LEUART_0, 1},
    {PF1,  LEUART_0, 3},
    {PA0,  LEUART_0, 4},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};
