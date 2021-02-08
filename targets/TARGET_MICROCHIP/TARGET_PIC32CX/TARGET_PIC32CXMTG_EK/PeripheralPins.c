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
    {NC, NC, 0}
};

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

// /*************SPI**************/
// MBED_WEAK const PinMap PinMap_SPI_MOSI[] = {
// #ifdef USART0_BASE
    // /* USART0 */
    // {PA0,  SPI_0,  0},
    // {PA1,  SPI_0,  1},
    // {PA2,  SPI_0,  2},
    // {PA3,  SPI_0,  3},
    // {PA4,  SPI_0,  4},
    // {PA5,  SPI_0,  5},
    // {PB11, SPI_0,  6},
    // {PB12, SPI_0,  7},
    // {PB13, SPI_0,  8},
    // {PB14, SPI_0,  9},
    // {PB15, SPI_0, 10},
    // {PD9,  SPI_0, 17},
    // {PD10, SPI_0, 18},
    // {PD11, SPI_0, 19},
    // {PD12, SPI_0, 20},
    // {PD13, SPI_0, 21},
    // {PD14, SPI_0, 22},
    // {PD15, SPI_0, 23},
// #endif
// #ifdef USART1_BASE
    // /* USART1 */
    // {PC6,  SPI_1, 11},
    // {PC7,  SPI_1, 12},
    // {PC8,  SPI_1, 13},
    // {PC9,  SPI_1, 14},
    // {PC10, SPI_1, 15},
    // {PC11, SPI_1, 16},
    // {PF0,  SPI_1, 24},
    // {PF1,  SPI_1, 25},
    // {PF2,  SPI_1, 26},
    // {PF3,  SPI_1, 27},
    // {PF4,  SPI_1, 28},
    // {PF5,  SPI_1, 29},
    // {PF6,  SPI_1, 30},
// #endif
// #ifdef USART2_BASE
    // /* USART2 */
    // {PA6,  SPI_2, 1},
    // {PA7,  SPI_2, 2},
    // {PA8,  SPI_2, 3},
    // {PA9,  SPI_2, 4},
    // {PI0,  SPI_2, 5},
    // {PI1,  SPI_2, 6},
    // {PI2,  SPI_2, 7},
    // {PI3,  SPI_2, 8},
    // {PB6,  SPI_2, 9},
    // {PB7,  SPI_2, 10},
    // {PB8,  SPI_2, 11},
    // {PB9,  SPI_2, 12},
    // {PB10, SPI_2, 13},
    // {PF7,  SPI_2, 20},
    // {PF8,  SPI_2, 21},
    // {PF9,  SPI_2, 22},
    // {PF10, SPI_2, 23},
    // {PF11, SPI_2, 24},
    // {PF12, SPI_2, 25},
    // {PF13, SPI_2, 26},
    // {PF14, SPI_2, 27},
    // {PF15, SPI_2, 28},
    // {PK0,  SPI_2, 29},
    // {PK1,  SPI_2, 30},
    // {PK2,  SPI_2, 31},
// #endif
// #ifdef USART3_BASE
    // /* USART3 */
    // {PJ14, SPI_3, 16},
    // {PJ15, SPI_3, 17},
    // {PC0,  SPI_3, 18},
    // {PC1,  SPI_3, 19},
    // {PC2,  SPI_3, 20},
    // {PC3,  SPI_3, 21},
    // {PC4,  SPI_3, 22},
    // {PC5,  SPI_3, 23},
// #endif
    // {NC  , NC   , NC}
// };

// MBED_WEAK const PinMap PinMap_SPI_MISO[] = {
// #ifdef USART0_BASE
    // /* USART0 */
    // {PA0,  SPI_0, 31},
    // {PA1,  SPI_0,  0},
    // {PA2,  SPI_0,  1},
    // {PA3,  SPI_0,  2},
    // {PA4,  SPI_0,  3},
    // {PA5,  SPI_0,  4},
    // {PB11, SPI_0,  5},
    // {PB12, SPI_0,  6},
    // {PB13, SPI_0,  7},
    // {PB14, SPI_0,  8},
    // {PB15, SPI_0,  9},
    // {PD9,  SPI_0, 16},
    // {PD10, SPI_0, 17},
    // {PD11, SPI_0, 18},
    // {PD12, SPI_0, 19},
    // {PD13, SPI_0, 20},
    // {PD14, SPI_0, 21},
    // {PD15, SPI_0, 22},
// #endif
// #ifdef USART1_BASE
    // /* USART1 */
    // {PC6,  SPI_1, 10},
    // {PC7,  SPI_1, 11},
    // {PC8,  SPI_1, 12},
    // {PC9,  SPI_1, 13},
    // {PC10, SPI_1, 14},
    // {PC11, SPI_1, 15},
    // {PF0,  SPI_1, 23},
    // {PF1,  SPI_1, 24},
    // {PF2,  SPI_1, 25},
    // {PF3,  SPI_1, 26},
    // {PF4,  SPI_1, 27},
    // {PF5,  SPI_1, 28},
    // {PF6,  SPI_1, 29},
    // {PA0,  SPI_1, 31},
// #endif
// #ifdef USART2_BASE
    // /* USART2 */
    // {PA6,  SPI_2, 0},
    // {PA7,  SPI_2, 1},
    // {PA8,  SPI_2, 2},
    // {PA9,  SPI_2, 3},
    // {PI0,  SPI_2, 4},
    // {PI1,  SPI_2, 5},
    // {PI2,  SPI_2, 6},
    // {PI3,  SPI_2, 7},
    // {PB6,  SPI_2, 8},
    // {PB7,  SPI_2, 9},
    // {PB8,  SPI_2, 10},
    // {PB9,  SPI_2, 11},
    // {PB10, SPI_2, 12},
    // {PF7,  SPI_2, 19},
    // {PF8,  SPI_2, 20},
    // {PF9,  SPI_2, 21},
    // {PF10, SPI_2, 22},
    // {PF11, SPI_2, 23},
    // {PF12, SPI_2, 24},
    // {PF13, SPI_2, 25},
    // {PF14, SPI_2, 26},
    // {PF15, SPI_2, 27},
    // {PK0,  SPI_2, 28},
    // {PK1,  SPI_2, 29},
    // {PK2,  SPI_2, 30},
// #endif
// #ifdef USART3_BASE
    // /* USART3 */
    // {PJ14, SPI_3, 15},
    // {PJ15, SPI_3, 16},
    // {PC0,  SPI_3, 17},
    // {PC1,  SPI_3, 18},
    // {PC2,  SPI_3, 19},
    // {PC3,  SPI_3, 20},
    // {PC4,  SPI_3, 21},
    // {PC5,  SPI_3, 22},
// #endif
    // {NC  , NC   , NC}
// };

// MBED_WEAK const PinMap PinMap_SPI_CLK[] = {
// #ifdef USART0_BASE
    // /* USART0 */
    // {PA0,  SPI_0, 30},
    // {PA1,  SPI_0, 31},
    // {PA2,  SPI_0,  0},
    // {PA3,  SPI_0,  1},
    // {PA4,  SPI_0,  2},
    // {PA5,  SPI_0,  3},
    // {PB11, SPI_0,  4},
    // {PB12, SPI_0,  5},
    // {PB13, SPI_0,  6},
    // {PB14, SPI_0,  7},
    // {PB15, SPI_0,  8},
    // {PD9,  SPI_0, 15},
    // {PD10, SPI_0, 16},
    // {PD11, SPI_0, 17},
    // {PD12, SPI_0, 18},
    // {PD13, SPI_0, 19},
    // {PD14, SPI_0, 20},
    // {PD15, SPI_0, 21},
// #endif
// #ifdef USART1_BASE
    // /* USART1 */
    // {PC6,  SPI_1,  9},
    // {PC7,  SPI_1, 10},
    // {PC8,  SPI_1, 11},
    // {PC9,  SPI_1, 12},
    // {PC10, SPI_1, 13},
    // {PC11, SPI_1, 14},
    // {PF0,  SPI_1, 22},
    // {PF1,  SPI_1, 23},
    // {PF2,  SPI_1, 24},
    // {PF3,  SPI_1, 25},
    // {PF4,  SPI_1, 26},
    // {PF5,  SPI_1, 27},
    // {PF6,  SPI_1, 28},
    // {PA0,  SPI_1, 30},
    // {PA1,  SPI_1, 31},
// #endif
// #ifdef USART2_BASE
    // /* USART2 */
    // {PA7,  SPI_2, 0},
    // {PA8,  SPI_2, 1},
    // {PA9,  SPI_2, 2},
    // {PI0,  SPI_2, 3},
    // {PI1,  SPI_2, 4},
    // {PI2,  SPI_2, 5},
    // {PI3,  SPI_2, 6},
    // {PB6,  SPI_2, 7},
    // {PB7,  SPI_2, 8},
    // {PB8,  SPI_2, 9},
    // {PB9,  SPI_2, 10},
    // {PB10, SPI_2, 11},
    // {PF7,  SPI_2, 18},
    // {PF8,  SPI_2, 19},
    // {PF9,  SPI_2, 20},
    // {PF10, SPI_2, 21},
    // {PF11, SPI_2, 22},
    // {PF12, SPI_2, 23},
    // {PF13, SPI_2, 24},
    // {PF14, SPI_2, 25},
    // {PF15, SPI_2, 26},
    // {PK0,  SPI_2, 27},
    // {PK1,  SPI_2, 28},
    // {PK2,  SPI_2, 29},
    // {PA6,  SPI_2, 31},
// #endif
// #ifdef USART3_BASE
    // /* USART3 */
    // {PJ14, SPI_3, 14},
    // {PJ15, SPI_3, 15},
    // {PC0,  SPI_3, 16},
    // {PC1,  SPI_3, 17},
    // {PC2,  SPI_3, 18},
    // {PC3,  SPI_3, 19},
    // {PC4,  SPI_3, 20},
    // {PC5,  SPI_3, 21},
// #endif
    // {NC  , NC   , NC}
// };

// MBED_WEAK const PinMap PinMap_SPI_CS[] = {
// #ifdef USART0_BASE
    // /* USART0 */
    // {PA0,  SPI_0,  29},
    // {PA1,  SPI_0,  30},
    // {PA2,  SPI_0,  31},
    // {PA3,  SPI_0,  0},
    // {PA4,  SPI_0,  1},
    // {PA5,  SPI_0,  2},
    // {PB11, SPI_0,  3},
    // {PB12, SPI_0,  4},
    // {PB13, SPI_0,  5},
    // {PB14, SPI_0,  6},
    // {PB15, SPI_0,  7},
    // {PD9,  SPI_0, 14},
    // {PD10, SPI_0, 15},
    // {PD11, SPI_0, 16},
    // {PD12, SPI_0, 17},
    // {PD13, SPI_0, 18},
    // {PD14, SPI_0, 19},
    // {PD15, SPI_0, 20},
// #endif
// #ifdef USART1_BASE
    // /* USART1 */
    // {PC6,  SPI_1,  8},
    // {PC7,  SPI_1,  9},
    // {PC8,  SPI_1, 10},
    // {PC9,  SPI_1, 11},
    // {PC10, SPI_1, 12},
    // {PC11, SPI_1, 13},
    // {PF0,  SPI_1, 21},
    // {PF1,  SPI_1, 22},
    // {PF2,  SPI_1, 23},
    // {PF3,  SPI_1, 24},
    // {PF4,  SPI_1, 25},
    // {PF5,  SPI_1, 26},
    // {PF6,  SPI_1, 27},
// #endif
// #ifdef USART2_BASE
    // /* USART2 */
    // {PA8,  SPI_2, 0},
    // {PA9,  SPI_2, 1},
    // {PI0,  SPI_2, 2},
    // {PI1,  SPI_2, 3},
    // {PI2,  SPI_2, 4},
    // {PI3,  SPI_2, 5},
    // {PB6,  SPI_2, 6},
    // {PB7,  SPI_2, 7},
    // {PB8,  SPI_2, 8},
    // {PB9,  SPI_2, 9},
    // {PB10, SPI_2, 10},
    // {PF7,  SPI_2, 17},
    // {PF8,  SPI_2, 18},
    // {PF9,  SPI_2, 19},
    // {PF10, SPI_2, 20},
    // {PF11, SPI_2, 21},
    // {PF12, SPI_2, 22},
    // {PF13, SPI_2, 23},
    // {PF14, SPI_2, 24},
    // {PF15, SPI_2, 25},
    // {PK0,  SPI_2, 26},
    // {PK1,  SPI_2, 27},
    // {PK2,  SPI_2, 28},
    // {PA6,  SPI_2, 30},
    // {PA7,  SPI_2, 31},
// #endif
// #ifdef USART3_BASE
    // /* USART3 */
    // {PJ14, SPI_3, 13},
    // {PJ15, SPI_3, 14},
    // {PC0,  SPI_3, 15},
    // {PC1,  SPI_3, 16},
    // {PC2,  SPI_3, 17},
    // {PC3,  SPI_3, 18},
    // {PC4,  SPI_3, 19},
    // {PC5,  SPI_3, 20},
// #endif
    // {NC  , NC   , NC}
// };

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
