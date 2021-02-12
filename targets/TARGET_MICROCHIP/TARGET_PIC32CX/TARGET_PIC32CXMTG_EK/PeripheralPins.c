/***************************************************************************//**
 * @file PeripheralPins.c
 *******************************************************************************
 * @section License
 * <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
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
//MBED_WEAK const PinMap PinMap_ADC[] = {
// #ifdef ADC0_BASE
    // {PA0,  ADC_0, adcPosSelAPORT3XCH8},
    // {PA1,  ADC_0, adcPosSelAPORT4XCH9},
    // {PA2,  ADC_0, adcPosSelAPORT3XCH10},
    // {PA3,  ADC_0, adcPosSelAPORT4XCH11},
    // {PA4,  ADC_0, adcPosSelAPORT3XCH12},
    // {PA5,  ADC_0, adcPosSelAPORT4XCH13},

    // {PB11, ADC_0, adcPosSelAPORT4XCH27},
    // {PB12, ADC_0, adcPosSelAPORT3XCH28},
    // {PB14, ADC_0, adcPosSelAPORT3XCH30},
    // {PB15, ADC_0, adcPosSelAPORT4XCH31},

    // {PC6,  ADC_0, adcPosSelAPORT1XCH6},
    // {PC7,  ADC_0, adcPosSelAPORT2XCH7},
    // {PC8,  ADC_0, adcPosSelAPORT1XCH8},
    // {PC9,  ADC_0, adcPosSelAPORT2XCH9},
    // {PC10, ADC_0, adcPosSelAPORT1XCH10},
    // {PC11, ADC_0, adcPosSelAPORT2XCH11},

    // {PD9,  ADC_0, adcPosSelAPORT4XCH1},
    // {PD10, ADC_0, adcPosSelAPORT3XCH2},
    // {PD11, ADC_0, adcPosSelAPORT3YCH3},
    // {PD12, ADC_0, adcPosSelAPORT3XCH4},
    // {PD13, ADC_0, adcPosSelAPORT3YCH5},
    // {PD14, ADC_0, adcPosSelAPORT3XCH6},
    // {PD15, ADC_0, adcPosSelAPORT4XCH7},

    // {PF0,  ADC_0, adcPosSelAPORT1XCH16},
    // {PF1,  ADC_0, adcPosSelAPORT2XCH17},
    // {PF2,  ADC_0, adcPosSelAPORT1XCH18},
    // {PF3,  ADC_0, adcPosSelAPORT2XCH19},
    // {PF4,  ADC_0, adcPosSelAPORT1XCH20},
    // {PF5,  ADC_0, adcPosSelAPORT2XCH21},
    // {PF6,  ADC_0, adcPosSelAPORT1XCH22},
    // {PF7,  ADC_0, adcPosSelAPORT2XCH23},
// #endif
    // {NC ,  NC   , NC}
// };

/************I2C SDA***********/
MBED_WEAK const PinMap PinMap_I2C_SDA[] = {
	/* I2C0 */
    {A4, I2C_0, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    /* I2C1 */
    {A8, I2C_1, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    {C2, I2C_1, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C2 */
    {A12, I2C_2, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    /* I2C3 */
    {B9, I2C_3, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C4 */
    {B5, I2C_4, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {B24, I2C_4, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C5 */
    {A24, I2C_5, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {B20, I2C_5, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C6 */
    {A0, I2C_6, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    {A25, I2C_6, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {C6, I2C_6, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C7 */
    {B4, I2C_7, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {C13, I2C_7, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {C21, I2C_7, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    {NC, NC, 0}
};

/************I2C SCL***********/
MBED_WEAK const PinMap PinMap_I2C_SCL[] = {
    /* I2C0 */
    {A5, I2C_0, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    /* I2C1 */
    {A9, I2C_1, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    {C3, I2C_1, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C2 */
    {A13, I2C_2, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    /* I2C3 */
    {B10, I2C_3, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C4 */
    {B6, I2C_4, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {B25, I2C_4, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C5 */
    {A23, I2C_5, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {B21, I2C_5, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C6 */
    {A1, I2C_6, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    {A26, I2C_6, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {C7, I2C_6, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    /* I2C7 */
    {B3, I2C_7, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {C12, I2C_7, IOPORT_MODE_MUX_B | IOPORT_MODE_PULLUP},
    {C20, I2C_7, IOPORT_MODE_MUX_A | IOPORT_MODE_PULLUP},
    {NC, NC, 0}
};

// /************PWM***************/
// MBED_WEAK const PinMap PinMap_PWM[] = {
// #if defined(TIMER_ROUTE_CC0PEN) || defined(TIMER_ROUTEPEN_CC0PEN)
    // /* PWM0 */
    // {PA0,  PWM_CH0,  0},
    // {PB14, PWM_CH0,  9},
    // {PB15, PWM_CH0, 10},
    // {PC6,  PWM_CH0, 11},
    // {PD10, PWM_CH0, 18},
    // {PD14, PWM_CH0, 22},
    // {PF0,  PWM_CH0, 24},
    // {PF4,  PWM_CH0, 28},
// #endif
// #if defined(TIMER_ROUTE_CC1PEN) || defined(TIMER_ROUTEPEN_CC1PEN)
    // /* PWM1 */
    // {PA1,  PWM_CH1,  0},
    // {PB11, PWM_CH1,  5},
    // {PC7,  PWM_CH1, 11},
    // {PD11, PWM_CH1, 18},
    // {PD15, PWM_CH1, 22},
    // {PF1,  PWM_CH1, 24},
    // {PF5,  PWM_CH1, 28},
// #endif
// #if defined(TIMER_ROUTE_CC2PEN) || defined(TIMER_ROUTEPEN_CC2PEN)
    // /* PWM2 */
    // {PA2,  PWM_CH2,  0},
    // {PA4,  PWM_CH2,  2},
    // {PB12, PWM_CH2,  5},
    // {PC8,  PWM_CH2, 11},
    // {PC10, PWM_CH2, 13},
    // {PD12, PWM_CH2, 18},
    // {PF2,  PWM_CH2, 24},
    // {PF6,  PWM_CH2, 28},
// #endif
// #if defined(TIMER_ROUTE_CC3PEN) || defined(TIMER_ROUTEPEN_CC3PEN)
    // /* PWM3 */
    // {PA3,  PWM_CH3,  0},
    // {PA5,  PWM_CH3,  2},
    // {PB13, PWM_CH3,  5},
    // {PC9,  PWM_CH3, 11},
    // {PC11, PWM_CH3, 13},
    // {PD9,  PWM_CH3, 14},
    // {PD13, PWM_CH3, 18},
    // {PF3,  PWM_CH3, 24},
    // {PF7,  PWM_CH3, 28},
// #endif
    // {NC  , NC     , NC}
// };

/*************SPI**************/
MBED_WEAK const PinMap PinMap_SPI_MOSI[] = {
    /* SPI0 */
    {A4, SPI_0, IOPORT_MODE_MUX_A},
    /* SPI2 */
    {A12, SPI_2, IOPORT_MODE_MUX_A},
    /* SPI3 */
    {B9, SPI_3, IOPORT_MODE_MUX_B},
    /* SPI4 */
    {B5, SPI_4, IOPORT_MODE_MUX_B},
    {B24, SPI_4, IOPORT_MODE_MUX_B},
    /* SPI5 */
    {A24, SPI_5, IOPORT_MODE_MUX_B},
    {B20, SPI_5, IOPORT_MODE_MUX_B},
    /* SPI6 */
    {A0, SPI_6, IOPORT_MODE_MUX_A},
    {A25, SPI_6, IOPORT_MODE_MUX_B},
    {C6, SPI_6, IOPORT_MODE_MUX_B},
    /* SPI7 */
    {B4, SPI_7, IOPORT_MODE_MUX_B},
    {C13, SPI_7, IOPORT_MODE_MUX_B},
    {C21, SPI_7, IOPORT_MODE_MUX_A},
    {NC, NC, 0}
};

MBED_WEAK const PinMap PinMap_SPI_MISO[] = {
    /* SPI0 */
    {A5, SPI_0, IOPORT_MODE_MUX_A},
    /* SPI2 */
    {A13, SPI_2, IOPORT_MODE_MUX_A},
    /* SPI3 */
    {B10, SPI_3, IOPORT_MODE_MUX_B},
    /* SPI4 */
    {B6, SPI_4, IOPORT_MODE_MUX_B},
    {B25, SPI_4, IOPORT_MODE_MUX_B},
    /* SPI5 */
    {A23, SPI_5, IOPORT_MODE_MUX_B},
    {B21, SPI_5, IOPORT_MODE_MUX_B},
    /* SPI6 */
    {A1, SPI_6, IOPORT_MODE_MUX_A},
    {A26, SPI_6, IOPORT_MODE_MUX_B},
    {C7, SPI_6, IOPORT_MODE_MUX_B},
    /* SPI7 */
    {B3, SPI_7, IOPORT_MODE_MUX_B},
    {C12, SPI_7, IOPORT_MODE_MUX_B},
    {C20, SPI_7, IOPORT_MODE_MUX_A},
    {NC, NC, 0}
};

MBED_WEAK const PinMap PinMap_SPI_CLK[] = {
    /* SPI0 */
    {A6, SPI_0, IOPORT_MODE_MUX_A},
    /* SPI2 */
    {A14, SPI_2, IOPORT_MODE_MUX_A},
    /* SPI3 */
    {B11, SPI_3, IOPORT_MODE_MUX_B},
    /* SPI4 */
    {B26, SPI_4, IOPORT_MODE_MUX_B},
    /* SPI5 */
    {A22, SPI_5, IOPORT_MODE_MUX_B},
    {B22, SPI_5, IOPORT_MODE_MUX_B},
    {C7, SPI_5, IOPORT_MODE_MUX_A},
    /* SPI6 */
    {A27, SPI_6, IOPORT_MODE_MUX_B},
    {C18, SPI_6, IOPORT_MODE_MUX_A},
    /* SPI7 */
    {B2, SPI_7, IOPORT_MODE_MUX_B},
    {C15, SPI_7, IOPORT_MODE_MUX_B},
    {NC, NC, 0}
};

MBED_WEAK const PinMap PinMap_SPI_CS[] = {
    /* SPI0 CS0 */
    {A7, SPI_0, IOPORT_MODE_MUX_A},
    /* SPI0 CS1 */
    {A29, SPI_0, IOPORT_MODE_MUX_B},
    /* SPI2 CS0 */
    {A15, SPI_2, IOPORT_MODE_MUX_A},
    /* SPI2 CS1 */
    {A29, SPI_2, IOPORT_MODE_MUX_D},
    /* SPI3 CS0 */
    {B12, SPI_3, IOPORT_MODE_MUX_B},
    /* SPI3 CS1 */
    {B0, SPI_3, IOPORT_MODE_MUX_B},
    /* SPI4 CS0 */
    {C0, SPI_4, IOPORT_MODE_MUX_B},
    /* SPI5 CS0 */
    {A21, SPI_5, IOPORT_MODE_MUX_B},
    {B23, SPI_5, IOPORT_MODE_MUX_B},
    {C6, SPI_5, IOPORT_MODE_MUX_A},
    /* SPI5 CS1 */
    {B22, SPI_5, IOPORT_MODE_MUX_C},
    {C7, SPI_5, IOPORT_MODE_MUX_C},
    /* SPI6 CS0 */
    {A28, SPI_6, IOPORT_MODE_MUX_B},
    {C19, SPI_6, IOPORT_MODE_MUX_A},
    /* SPI6 CS1 */
    {A27, SPI_6, IOPORT_MODE_MUX_C},
    {C18, SPI_6, IOPORT_MODE_MUX_B},
    /* SPI7 CS0 */
    {B1, SPI_7, IOPORT_MODE_MUX_B},
    {C14, SPI_7, IOPORT_MODE_MUX_B},
    /* SPI7 CS1 */
    {B2, SPI_7, IOPORT_MODE_MUX_C},
    {C11, SPI_7, IOPORT_MODE_MUX_B},
    {NC, NC, 0}
};

MBED_WEAK const PinMap PinMap_SPI_CS0[] = {
    /* SPI0 */
    {A7, SPI_0, IOPORT_MODE_MUX_A},
    /* SPI2 */
    {A15, SPI_2, IOPORT_MODE_MUX_A},
    /* SPI3 */
    {B12, SPI_3, IOPORT_MODE_MUX_B},
    /* SPI4 */
    {C0, SPI_4, IOPORT_MODE_MUX_B},
    /* SPI5 */
    {A21, SPI_5, IOPORT_MODE_MUX_B},
    {B23, SPI_5, IOPORT_MODE_MUX_B},
    {C6, SPI_5, IOPORT_MODE_MUX_A},
    /* SPI6 */
    {A28, SPI_6, IOPORT_MODE_MUX_B},
    {C19, SPI_6, IOPORT_MODE_MUX_A},
    /* SPI7 */
    {B1, SPI_7, IOPORT_MODE_MUX_B},
    {C14, SPI_7, IOPORT_MODE_MUX_B},
    {NC, NC, 0}
};

MBED_WEAK const PinMap PinMap_SPI_CS1[] = {
    /* SPI0 */
    {A29, SPI_0, IOPORT_MODE_MUX_B},
    /* SPI2 */
    {A29, SPI_2, IOPORT_MODE_MUX_D},
    /* SPI3 */
    {B0, SPI_3, IOPORT_MODE_MUX_B},
    /* SPI5 */
    {B22, SPI_5, IOPORT_MODE_MUX_C},
    {C7, SPI_5, IOPORT_MODE_MUX_C},
    /* SPI6 */
    {A27, SPI_6, IOPORT_MODE_MUX_C},
    {C18, SPI_6, IOPORT_MODE_MUX_B},
    /* SPI7 */
    {B2, SPI_7, IOPORT_MODE_MUX_C},
    {C11, SPI_7, IOPORT_MODE_MUX_B},
    {NC, NC, 0}
};

/************UART**************/
MBED_WEAK const PinMap PinMap_UART_TX[] = {
    /* USART0 */
    {A4, USART_0, IOPORT_MODE_MUX_A},
    /* USART1 */
    {A8, USART_1, IOPORT_MODE_MUX_A},
    {C2, USART_1, IOPORT_MODE_MUX_B},
    /* USART2 */
    {A12, USART_2, IOPORT_MODE_MUX_A},
    /* USART3 */
    {B9, USART_3, IOPORT_MODE_MUX_B},
    /* USART4 */
    {B5, USART_4, IOPORT_MODE_MUX_B},
    {B24, USART_4, IOPORT_MODE_MUX_B},
    /* USART5 */
    {A24, USART_5, IOPORT_MODE_MUX_B},
    {B20, USART_5, IOPORT_MODE_MUX_B},
    /* USART6 */
    {A0, USART_6, IOPORT_MODE_MUX_A},
    {A25, USART_6, IOPORT_MODE_MUX_B},
    {C6, USART_6, IOPORT_MODE_MUX_B},
    /* USART7 */
    {B4, USART_7, IOPORT_MODE_MUX_B},
    /* UART */
    {D2, UART_0, IOPORT_MODE_MUX_A},
    {NC, NC, 0}
};

MBED_WEAK const PinMap PinMap_UART_RX[] = {
    /* USART0 */
    {A5, USART_0, IOPORT_MODE_MUX_A},
    /* USART1 */
    {A9, USART_1, IOPORT_MODE_MUX_A},
    {C3, USART_1, IOPORT_MODE_MUX_B},
    /* USART2 */
    {A13, USART_2, IOPORT_MODE_MUX_A},
    /* USART3 */
    {B10, USART_3, IOPORT_MODE_MUX_B},
    /* USART4 */
    {B6, USART_4, IOPORT_MODE_MUX_B},
    {B25, USART_4, IOPORT_MODE_MUX_B},
    /* USART5 */
    {A23, USART_5, IOPORT_MODE_MUX_B},
    {B21, USART_5, IOPORT_MODE_MUX_B},
    /* USART6 */
    {A1, USART_6, IOPORT_MODE_MUX_A},
    {A26, USART_6, IOPORT_MODE_MUX_B},
    {C7, USART_6, IOPORT_MODE_MUX_B},
    /* USART7 */
    {B3, USART_7, IOPORT_MODE_MUX_B},
    /* UART */
    {D1, UART_0, IOPORT_MODE_MUX_A},
    {NC, NC, 0}
};
