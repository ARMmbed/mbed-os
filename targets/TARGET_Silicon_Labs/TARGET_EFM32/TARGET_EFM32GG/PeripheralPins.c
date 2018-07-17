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
/* The third "function" value is used to select the correct ADC channel */
MBED_WEAK const PinMap PinMap_ADC[] = {
#ifdef ADC0_BASE
    {PD0, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH0},
    {PD1, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH1},
    {PD2, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH2},
    {PD3, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH3},
    {PD4, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH4},
    {PD5, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH5},
    {PD6, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH6},
    {PD7, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH7},
#endif
    {NC  , NC   , NC}
};

/************DAC***************/
MBED_WEAK const PinMap PinMap_DAC[] = {
#ifdef DAC0_BASE
    {PB11, DAC_0, 0},
    {PB12, DAC_0, 1},
#endif
    {NC  , NC   , NC}
};

/************I2C SCL***********/
MBED_WEAK const PinMap PinMap_I2C_SCL[] = {
#ifdef I2C0_BASE
    /* I2C0 */
    {PA1,  I2C_0, 0},
    {PD7,  I2C_0, 1},
    {PC7,  I2C_0, 2},
    {PD15, I2C_0, 3},
    {PC1,  I2C_0, 4},
    {PF1,  I2C_0, 5},
    {PE13, I2C_0, 6},
#endif
#ifdef I2C1_BASE
    /* I2C1 */
    {PC5,  I2C_1, 0},
    {PB12,  I2C_1, 1},
    {PE1,  I2C_1, 2},
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
    {PC6,  I2C_0, 2},
    {PD14, I2C_0, 3},
    {PC0,  I2C_0, 4},
    {PF0,  I2C_0, 5},
    {PE12, I2C_0, 6},
#endif
#ifdef I2C1_BASE
    /* I2C1 */
    {PC4,  I2C_1, 0},
    {PB11,  I2C_1, 1},
    {PE0,  I2C_1, 2},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

/************PWM***************/
MBED_WEAK const PinMap PinMap_PWM[] = {
#if defined(TIMER_ROUTE_CC0PEN) || defined(TIMER_ROUTEPEN_CC0PEN)
    /* PWM0 */
    {PA8,  PWM_CH0, 0},
    {PA12, PWM_CH0, 1},
    {PC8,  PWM_CH0, 2},
#endif
#if defined(TIMER_ROUTE_CC1PEN) || defined(TIMER_ROUTEPEN_CC1PEN)
    /* PWM1 */
    {PA9,  PWM_CH1, 0},
    {PA13, PWM_CH1, 1},
    {PC9,  PWM_CH1, 2},
#endif
#if defined(TIMER_ROUTE_CC2PEN) || defined(TIMER_ROUTEPEN_CC2PEN)
    /* PWM2 */
    {PA10, PWM_CH2, 0},
    {PA14, PWM_CH2, 1},
    {PC10, PWM_CH2, 2},
#endif
    {NC  , NC   , 0}
};

/*************SPI**************/
MBED_WEAK const PinMap PinMap_SPI_MOSI[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE10, SPI_0, 0},
    {PE7, SPI_0, 1},
    {PC11, SPI_0, 2},
    {PC0, SPI_0, 5},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PD0, SPI_1, 1},
    {PD7, SPI_1, 2},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PC2, SPI_2, 0},
    {PB3, SPI_2, 1},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

MBED_WEAK const PinMap PinMap_SPI_MISO[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE11, SPI_0, 0},
    {PE6, SPI_0, 1},
    {PC10, SPI_0, 2},
    {PC1, SPI_0, 5},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PD1, SPI_1, 1},
    {PD6, SPI_1, 2},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PC3, SPI_2, 0},
    {PB4, SPI_2, 1},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

MBED_WEAK const PinMap PinMap_SPI_CLK[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE12, SPI_0, 0},
    {PE5, SPI_0, 1},
    {PC9, SPI_0, 2},
    {PB13, SPI_0, 5},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PD2, SPI_1, 1},
    {PF0, SPI_1, 2},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PC4, SPI_2, 0},
    {PB5, SPI_2, 1},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

MBED_WEAK const PinMap PinMap_SPI_CS[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PE13, SPI_0, 0},
    {PE4, SPI_0, 1},
    {PC8, SPI_0, 2},
    {PB14, SPI_0, 5},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PD3, SPI_1, 1},
    {PF1, SPI_1, 2},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PC5, SPI_2, 0},
    {PB6, SPI_2, 1},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

/************UART**************/
MBED_WEAK const PinMap PinMap_UART_TX[] = {
#ifdef UART0_BASE
    /* UART0 */
    {PF6, UART_0, 0},
    {PE0, UART_0, 1},
#endif
#ifdef UART1_BASE
    /* UART1 */
    {PF10, UART_1, 1},
    {PB9, UART_1, 2},
    {PE2, UART_1, 3},
#endif
#ifdef USART0_BASE
    /* USART0 */
    {PE10, USART_0, 0},
    {PE7, USART_0, 1},
    {PC11, USART_0, 2},
    {PE13, USART_0, 3},
    {PB7, USART_0, 4},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC0, USART_1, 0},
    {PD0, USART_1, 1},
    {PD7, USART_1, 2},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PC2, USART_2, 0},
    {PB3, USART_2, 1},
#endif
#ifdef LEUART0_BASE
    /* LEUART0 */
    {PD4,  LEUART_0, 0},
    {PB13, LEUART_0, 1},
    {PE14, LEUART_0, 2},
    {PF0,  LEUART_0, 3},
    {PF2,  LEUART_0, 4},
#endif
#ifdef LEUART1_BASE
    /* LEUART1 */
    {PC6,  LEUART_1, 0},
    {PA5,  LEUART_1, 1},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

MBED_WEAK const PinMap PinMap_UART_RX[] = {
#ifdef UART0_BASE
    /* UART0 */
    {PF7, UART_0, 0},
    {PE1, UART_0, 1},
#endif
#ifdef UART1_BASE
    /* UART1 */
    {PF11, UART_1, 1},
    {PB10, UART_1, 2},
    {PE3, UART_1, 3},
#endif
#ifdef USART0_BASE
    /* USART0 */
    {PE11, USART_0, 0},
    {PE6, USART_0, 1},
    {PC10, USART_0, 2},
    {PE12, USART_0, 3},
    {PB8, USART_0, 4},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC1, USART_1, 0},
    {PD1, USART_1, 1},
    {PD6, USART_1, 2},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PC3, USART_2, 0},
    {PB4, USART_2, 1},
#endif
#ifdef LEUART0_BASE
    /* LEUART0 */
    {PD5,  LEUART_0, 0},
    {PB14, LEUART_0, 1},
    {PE15, LEUART_0, 2},
    {PF1,  LEUART_0, 3},
    {PA0, LEUART_0, 4},
#endif
#ifdef LEUART1_BASE
    /* LEUART1 */
    {PC7,  LEUART_1, 0},
    {PA6,  LEUART_1, 1},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};
