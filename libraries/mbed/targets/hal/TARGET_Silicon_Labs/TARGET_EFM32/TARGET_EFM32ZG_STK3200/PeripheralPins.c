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
const PinMap PinMap_ADC[] = {
    {PD_4, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH4},
    {PD_5, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH5},
    {PD_6, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH6},
    {PD_7, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH7},
    {NC  , NC   , NC}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PA_0, PWM_CH0, 0},
    {PA_1, PWM_CH1, 0},
    {PA_2, PWM_CH2, 0},
    {NC  , NC   , NC}
};

/*************SPI**************/
const PinMap PinMap_SPI_MOSI[] = {
        /* USART1 */
        {PC_0, SPI_1, 0},
        {PD_7, SPI_1, 3},
        /* Not connected */
        {NC  , NC   , NC}
};

const PinMap PinMap_SPI_MISO[] = {
        /* USART1 */
        {PC_1, SPI_1, 0},
        {PD_6, SPI_1, 3},
        /* Not connected */
        {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CLK[] = {
        /* USART1 */
        {PB_7, SPI_1, 0},
        {PC_15, SPI_1, 3},
        /* Not connected */
        {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CS[] = {
        /* USART1 */
        {PB_8, SPI_1, 0},
        {PC_14, SPI_1, 3},
        /* Not connected */
        {NC  , NC   , NC}
};

/************UART**************/
const PinMap PinMap_UART_TX[] = {
        /* USART1 */
        {PC_0, USART_1, 0},
        {PD_7, USART_1, 3},
        /* Not connected */
        {NC  , NC   , NC}
};

const PinMap PinMap_UART_RX[] = {
        /* USART1 */
        {PC_1, USART_1, 0},
        {PD_6, USART_1, 3},
        /* Not connected */
        {NC  , NC   , NC}
};
