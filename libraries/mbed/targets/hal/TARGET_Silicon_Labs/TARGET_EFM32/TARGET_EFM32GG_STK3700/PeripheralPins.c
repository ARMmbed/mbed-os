/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "PeripheralPins.h"

/************ADC***************/
/* The third "function" value is used to select the correct ADC channel */
const PinMap PinMap_ADC[] = {
    {PD_0, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH0},
    {PD_1, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH1},
    {PD_2, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH2},
    {PD_3, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH3},
    {PD_4, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH4},
    {PD_5, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH5},
    {PD_6, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH6},
    {PD_7, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH7},
    {NC  , NC   , NC}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {PB_11, DAC0_CH0, 0},
    {PB_12, DAC0_CH1, 0},
    {NC  , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PA_12, PWM_CH0, 0},
    {PA_13, PWM_CH1, 0},
    {PA_14, PWM_CH2, 0},
    {NC  , NC   , 0}
};

/*************SPI**************/
const PinMap PinMap_SPI_MOSI[] = {
        /* USART0 */
        {PE_10, SPI_0, 0},
        {PE_7, SPI_0, 1},
        {PC_11, SPI_0, 2},
        {PC_0, SPI_0, 5},
        /* USART1 */
        {PD_0, SPI_1, 1},
        {PD_7, SPI_1, 2},
        /* USART2 */
        {PC_2, SPI_2, 0},
        {PB_3, SPI_2, 1},
        /* Not connected */
        {NC  , NC   , NC}
};

const PinMap PinMap_SPI_MISO[] = {
        /* USART0 */
        {PE_11, SPI_0, 0},
        {PE_6, SPI_0, 1},
        {PC_10, SPI_0, 2},
        {PC_1, SPI_0, 5},
        /* USART1 */
        {PD_1, SPI_1, 1},
        {PD_6, SPI_1, 2},
        /* USART2 */
        {PC_3, SPI_2, 0},
        {PB_4, SPI_2, 1},
        /* Not connected */
        {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CLK[] = {
        /* USART0 */
        {PE_12, SPI_0, 0},
        {PE_5, SPI_0, 1},
        {PC_9, SPI_0, 2},
        {PB_13, SPI_0, 5},
        /* USART1 */
        {PD_2, SPI_1, 1},
        {PF_0, SPI_1, 2},
        /* USART2 */
        {PC_4, SPI_2, 0},
        {PB_5, SPI_2, 1},
        /* Not connected */
        {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CS[] = {
        /* USART0 */
        {PE_13, SPI_0, 0},
        {PE_4, SPI_0, 1},
        {PC_8, SPI_0, 2},
        {PB_14, SPI_0, 5},
        /* USART1 */
        {PD_3, SPI_1, 1},
        {PF_1, SPI_1, 2},
        /* USART2 */
        {PC_5, SPI_2, 0},
        {PB_6, SPI_2, 1},
        /* Not connected */
        {NC  , NC   , NC}
};

/************UART**************/
const PinMap PinMap_UART_TX[] = {
        /* UART0 */
        {PF_6, UART_0, 0},
        {PE_0, UART_0, 1},
        /* UART1 */
        {PF_10, UART_1, 1},
        {PB_9, UART_1, 2},
        {PE_2, UART_1, 3},
        /* USART0 */
        {PE_10, USART_0, 0},
        {PE_7, USART_0, 1},
        {PC_11, USART_0, 2},
        {PE_13, USART_0, 3},
        {PB_7, USART_0, 4},
        /* USART1 */
        {PC_0, USART_1, 0},
        {PD_0, USART_1, 1},
        {PD_7, USART_1, 2},
        /* USART2 */
        {PC_2, USART_2, 0},
        {PB_3, USART_2, 1},
        /* Not connected */
        {NC  , NC   , NC}
};

const PinMap PinMap_UART_RX[] = {
        /* UART0 */
        {PF_7, UART_0, 0},
        {PE_1, UART_0, 1},
        /* UART1 */
        {PF_11, UART_1, 1},
        {PB_10, UART_1, 2},
        {PE_3, UART_1, 3},
        /* USART0 */
        {PE_11, USART_0, 0},
        {PE_6, USART_0, 1},
        {PC_10, USART_0, 2},
        {PE_12, USART_0, 3},
        {PB_8, USART_0, 4},
        /* USART1 */
        {PC_1, USART_1, 0},
        {PD_1, USART_1, 1},
        {PD_6, USART_1, 2},
        /* USART2 */
        {PC_3, USART_2, 0},
        {PB_4, USART_2, 1},
        /* Not connected */
        {NC  , NC   , NC}
};
