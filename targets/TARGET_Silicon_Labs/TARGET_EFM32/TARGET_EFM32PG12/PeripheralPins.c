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
/* The third "function" value is used to select the correct ADC channel */
const PinMap PinMap_ADC[] = {
#ifdef ADC0_BASE
    {PA0,  ADC_0, adcPosSelAPORT3XCH8},
    {PA1,  ADC_0, adcPosSelAPORT4XCH9},
    {PA2,  ADC_0, adcPosSelAPORT3XCH10},
    {PA3,  ADC_0, adcPosSelAPORT4XCH11},
    {PA4,  ADC_0, adcPosSelAPORT3XCH12},
    {PA5,  ADC_0, adcPosSelAPORT4XCH13},
    
    {PB11, ADC_0, adcPosSelAPORT4XCH27},
    {PB12, ADC_0, adcPosSelAPORT3XCH28},
    {PB14, ADC_0, adcPosSelAPORT3XCH30},
    {PB15, ADC_0, adcPosSelAPORT4XCH31},
    
    {PC6,  ADC_0, adcPosSelAPORT1XCH6},
    {PC7,  ADC_0, adcPosSelAPORT2XCH7},
    {PC8,  ADC_0, adcPosSelAPORT1XCH8},
    {PC9,  ADC_0, adcPosSelAPORT2XCH9},
    {PC10, ADC_0, adcPosSelAPORT1XCH10},
    {PC11, ADC_0, adcPosSelAPORT2XCH11},
    
    {PD9,  ADC_0, adcPosSelAPORT4XCH1},
    {PD10, ADC_0, adcPosSelAPORT3XCH2},
    {PD11, ADC_0, adcPosSelAPORT3YCH3},
    {PD12, ADC_0, adcPosSelAPORT3XCH4},
    {PD13, ADC_0, adcPosSelAPORT3YCH5},
    {PD14, ADC_0, adcPosSelAPORT3XCH6},
    {PD15, ADC_0, adcPosSelAPORT4XCH7},
    
    {PF0,  ADC_0, adcPosSelAPORT1XCH16},
    {PF1,  ADC_0, adcPosSelAPORT2XCH17},
    {PF2,  ADC_0, adcPosSelAPORT1XCH18},
    {PF3,  ADC_0, adcPosSelAPORT2XCH19},
    {PF4,  ADC_0, adcPosSelAPORT1XCH20},
    {PF5,  ADC_0, adcPosSelAPORT2XCH21},
    {PF6,  ADC_0, adcPosSelAPORT1XCH22},
    {PF7,  ADC_0, adcPosSelAPORT2XCH23},
#endif
    {NC ,  NC   , NC}
};

/************I2C SCL***********/
const PinMap PinMap_I2C_SCL[] = {
#ifdef I2C0_BASE
    /* I2C0 */
    {PA1,  I2C_0,  0},
    {PA2,  I2C_0,  1},
    {PA3,  I2C_0,  2},
    {PA4,  I2C_0,  3},
    {PA5,  I2C_0,  4},
    {PB11, I2C_0,  5},
    {PB12, I2C_0,  6},
    {PB13, I2C_0,  7},
    {PB14, I2C_0,  8},
    {PB15, I2C_0,  9},
    {PC6,  I2C_0, 10},
    {PC7,  I2C_0, 11},
    {PC8,  I2C_0, 12},
    {PC9,  I2C_0, 13},
    {PC10, I2C_0, 14},
    {PC11, I2C_0, 15},
    {PD9,  I2C_0, 16},
    {PD10, I2C_0, 17},
    {PD11, I2C_0, 18},
    {PD12, I2C_0, 19},
    {PD13, I2C_0, 20},
    {PD14, I2C_0, 21},
    {PD15, I2C_0, 22},
    {PF0,  I2C_0, 23},
    {PF1,  I2C_0, 24},
    {PF2,  I2C_0, 25},
    {PF3,  I2C_0, 26},
    {PF4,  I2C_0, 27},
    {PF5,  I2C_0, 28},
    {PF6,  I2C_0, 29},
    {PF7,  I2C_0, 30},
    {PA0,  I2C_0, 31},
#endif
#ifdef I2C1_BASE
    {PA7, I2C_1, 0},
    {PA8, I2C_1, 1},
    {PA9, I2C_1, 2},
    {PI2, I2C_1, 3},
    {PI3, I2C_1, 4},
    {PB6, I2C_1, 5},
    {PB7, I2C_1, 6},
    {PB8, I2C_1, 7},
    {PB9, I2C_1, 8},
    {PB10, I2C_1, 9},
    {PJ14, I2C_1, 10},
    {PJ15, I2C_1, 11},
    {PC0, I2C_1, 12},
    {PC1, I2C_1, 13},
    {PC2, I2C_1, 14},
    {PC3, I2C_1, 15},
    {PC4, I2C_1, 16},
    {PC5, I2C_1, 17},
    {PF8, I2C_1, 20},
    {PF9, I2C_1, 21},
    {PF10, I2C_1, 22},
    {PF11, I2C_1, 23},
    {PF12, I2C_1, 24},
    {PF13, I2C_1, 25},
    {PF14, I2C_1, 26},
    {PF15, I2C_1, 27},
    {PK0, I2C_1, 28},
    {PK1, I2C_1, 29},
    {PK2, I2C_1, 30},
    {PA6, I2C_1, 31},
#endif
    {NC  , NC   , NC}
};

/************I2C SDA***********/
const PinMap PinMap_I2C_SDA[] = {
#ifdef I2C0_BASE
    /* I2C0 */
    {PA0,  I2C_0,  0},
    {PA1,  I2C_0,  1},
    {PA2,  I2C_0,  2},
    {PA3,  I2C_0,  3},
    {PA4,  I2C_0,  4},
    {PA5,  I2C_0,  5},
    {PB11, I2C_0,  6},
    {PB12, I2C_0,  7},
    {PB13, I2C_0,  8},
    {PB14, I2C_0,  9},
    {PB15, I2C_0, 10},
    {PC6,  I2C_0, 11},
    {PC7,  I2C_0, 12},
    {PC8,  I2C_0, 13},
    {PC9,  I2C_0, 14},
    {PC10, I2C_0, 15},
    {PC11, I2C_0, 16},
    {PD9,  I2C_0, 17},
    {PD10, I2C_0, 18},
    {PD11, I2C_0, 19},
    {PD12, I2C_0, 20},
    {PD13, I2C_0, 21},
    {PD14, I2C_0, 22},
    {PD15, I2C_0, 23},
    {PF0,  I2C_0, 24},
    {PF1,  I2C_0, 25},
    {PF2,  I2C_0, 26},
    {PF3,  I2C_0, 27},
    {PF4,  I2C_0, 28},
    {PF5,  I2C_0, 29},
    {PF6,  I2C_0, 30},
    {PF7,  I2C_0, 31},
#endif
#ifdef I2C1_BASE
    {PA6, I2C_1, 0},
    {PA7, I2C_1, 1},
    {PA8, I2C_1, 2},
    {PA9, I2C_1, 3},
    {PI2, I2C_1, 4},
    {PI3, I2C_1, 5},
    {PB6, I2C_1, 6},
    {PB7, I2C_1, 7},
    {PB8, I2C_1, 8},
    {PB9, I2C_1, 9},
    {PB10, I2C_1, 10},
    {PJ14, I2C_1, 11},
    {PJ15, I2C_1, 12},
    {PC0, I2C_1, 13},
    {PC1, I2C_1, 14},
    {PC2, I2C_1, 15},
    {PC3, I2C_1, 16},
    {PC4, I2C_1, 17},
    {PC5, I2C_1, 18},
    {PF8, I2C_1, 21},
    {PF9, I2C_1, 22},
    {PF10, I2C_1, 23},
    {PF11, I2C_1, 24},
    {PF12, I2C_1, 25},
    {PF13, I2C_1, 26},
    {PF14, I2C_1, 27},
    {PF15, I2C_1, 28},
    {PK0, I2C_1, 29},
    {PK1, I2C_1, 30},
    {PK2, I2C_1, 31},
#endif
    /* Not connected */
    {NC  , NC   , NC}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
#if defined(TIMER_ROUTE_CC0PEN) || defined(TIMER_ROUTEPEN_CC0PEN)
    /* PWM0 */
    {PA0,  PWM_CH0,  0},
    {PB14, PWM_CH0,  9},
    {PB15, PWM_CH0, 10},
    {PC6,  PWM_CH0, 11},
    {PD10, PWM_CH0, 18},
    {PD14, PWM_CH0, 22},
    {PF0,  PWM_CH0, 24},
    {PF4,  PWM_CH0, 28},
#endif
#if defined(TIMER_ROUTE_CC1PEN) || defined(TIMER_ROUTEPEN_CC1PEN)
    /* PWM1 */
    {PA1,  PWM_CH1,  0},
    {PB11, PWM_CH1,  5},
    {PC7,  PWM_CH1, 11},
    {PD11, PWM_CH1, 18},
    {PD15, PWM_CH1, 22},
    {PF1,  PWM_CH1, 24},
    {PF5,  PWM_CH1, 28},
#endif
#if defined(TIMER_ROUTE_CC2PEN) || defined(TIMER_ROUTEPEN_CC2PEN)
    /* PWM2 */
    {PA2,  PWM_CH2,  0},
    {PA4,  PWM_CH2,  2},
    {PB12, PWM_CH2,  5},
    {PC8,  PWM_CH2, 11},
    {PC10, PWM_CH2, 13},
    {PD12, PWM_CH2, 18},
    {PF2,  PWM_CH2, 24},
    {PF6,  PWM_CH2, 28},
#endif
#if defined(TIMER_ROUTE_CC3PEN) || defined(TIMER_ROUTEPEN_CC3PEN)
    /* PWM3 */
    {PA3,  PWM_CH3,  0},
    {PA5,  PWM_CH3,  2},
    {PB13, PWM_CH3,  5},
    {PC9,  PWM_CH3, 11},
    {PC11, PWM_CH3, 13},
    {PD9,  PWM_CH3, 14},
    {PD13, PWM_CH3, 18},
    {PF3,  PWM_CH3, 24},
    {PF7,  PWM_CH3, 28},
#endif
    {NC  , NC     , NC}
};

/*************SPI**************/
const PinMap PinMap_SPI_MOSI[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PA0,  SPI_0,  0},
    {PA1,  SPI_0,  1},
    {PA2,  SPI_0,  2},
    {PA3,  SPI_0,  3},
    {PA4,  SPI_0,  4},
    {PA5,  SPI_0,  5},
    {PB11, SPI_0,  6},
    {PB12, SPI_0,  7},
    {PB13, SPI_0,  8},
    {PB14, SPI_0,  9},
    {PB15, SPI_0, 10},
    {PD9,  SPI_0, 17},
    {PD10, SPI_0, 18},
    {PD11, SPI_0, 19},
    {PD12, SPI_0, 20},
    {PD13, SPI_0, 21},
    {PD14, SPI_0, 22},
    {PD15, SPI_0, 23},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC6,  SPI_1, 11},
    {PC7,  SPI_1, 12},
    {PC8,  SPI_1, 13},
    {PC9,  SPI_1, 14},
    {PC10, SPI_1, 15},
    {PC11, SPI_1, 16},
    {PF0,  SPI_1, 24},
    {PF1,  SPI_1, 25},
    {PF2,  SPI_1, 26},
    {PF3,  SPI_1, 27},
    {PF4,  SPI_1, 28},
    {PF5,  SPI_1, 29},
    {PF6,  SPI_1, 30},
    {PF7,  SPI_1, 31},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PA6,  SPI_2, 1},
    {PA7,  SPI_2, 2},
    {PA8,  SPI_2, 3},
    {PA9,  SPI_2, 4},
    {PI0,  SPI_2, 5},
    {PI1,  SPI_2, 6},
    {PI2,  SPI_2, 7},
    {PI3,  SPI_2, 8},
    {PB6,  SPI_2, 9},
    {PB7,  SPI_2, 10},
    {PB8,  SPI_2, 11},
    {PB9,  SPI_2, 12},
    {PB10, SPI_2, 13},
    {PF8,  SPI_2, 21},
    {PF9,  SPI_2, 22},
    {PF10, SPI_2, 23},
    {PF11, SPI_2, 24},
    {PF12, SPI_2, 25},
    {PF13, SPI_2, 26},
    {PF14, SPI_2, 27},
    {PF15, SPI_2, 28},
    {PK0,  SPI_2, 29},
    {PK1,  SPI_2, 30},
    {PK2,  SPI_2, 31},
#endif
#ifdef USART3_BASE
    /* USART3 */
    {PJ14, SPI_3, 16},
    {PJ15, SPI_3, 17},
    {PC0,  SPI_3, 18},
    {PC1,  SPI_3, 19},
    {PC2,  SPI_3, 20},
    {PC3,  SPI_3, 21},
    {PC4,  SPI_3, 22},
    {PC5,  SPI_3, 23},
#endif
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_MISO[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PA0,  SPI_0, 31},
    {PA1,  SPI_0,  0},
    {PA2,  SPI_0,  1},
    {PA3,  SPI_0,  2},
    {PA4,  SPI_0,  3},
    {PA5,  SPI_0,  4},
    {PB11, SPI_0,  5},
    {PB12, SPI_0,  6},
    {PB13, SPI_0,  7},
    {PB14, SPI_0,  8},
    {PB15, SPI_0,  9},
    {PD9,  SPI_0, 16},
    {PD10, SPI_0, 17},
    {PD11, SPI_0, 18},
    {PD12, SPI_0, 19},
    {PD13, SPI_0, 20},
    {PD14, SPI_0, 21},
    {PD15, SPI_0, 22},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC6,  SPI_1, 10},
    {PC7,  SPI_1, 11},
    {PC8,  SPI_1, 12},
    {PC9,  SPI_1, 13},
    {PC10, SPI_1, 14},
    {PC11, SPI_1, 15},
    {PF0,  SPI_1, 23},
    {PF1,  SPI_1, 24},
    {PF2,  SPI_1, 25},
    {PF3,  SPI_1, 26},
    {PF4,  SPI_1, 27},
    {PF5,  SPI_1, 28},
    {PF6,  SPI_1, 29},
    {PF7,  SPI_1, 30},
    {PA0,  SPI_1, 31},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PA6,  SPI_2, 0},
    {PA7,  SPI_2, 1},
    {PA8,  SPI_2, 2},
    {PA9,  SPI_2, 3},
    {PI0,  SPI_2, 4},
    {PI1,  SPI_2, 5},
    {PI2,  SPI_2, 6},
    {PI3,  SPI_2, 7},
    {PB6,  SPI_2, 8},
    {PB7,  SPI_2, 9},
    {PB8,  SPI_2, 10},
    {PB9,  SPI_2, 11},
    {PB10, SPI_2, 12},
    {PF8,  SPI_2, 20},
    {PF9,  SPI_2, 21},
    {PF10, SPI_2, 22},
    {PF11, SPI_2, 23},
    {PF12, SPI_2, 24},
    {PF13, SPI_2, 25},
    {PF14, SPI_2, 26},
    {PF15, SPI_2, 27},
    {PK0,  SPI_2, 28},
    {PK1,  SPI_2, 29},
    {PK2,  SPI_2, 30},
#endif
#ifdef USART3_BASE
    /* USART3 */
    {PJ14, SPI_3, 15},
    {PJ15, SPI_3, 16},
    {PC0,  SPI_3, 17},
    {PC1,  SPI_3, 18},
    {PC2,  SPI_3, 19},
    {PC3,  SPI_3, 20},
    {PC4,  SPI_3, 21},
    {PC5,  SPI_3, 22},
#endif
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CLK[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PA0,  SPI_0, 30},
    {PA1,  SPI_0, 31},
    {PA2,  SPI_0,  0},
    {PA3,  SPI_0,  1},
    {PA4,  SPI_0,  2},
    {PA5,  SPI_0,  3},
    {PB11, SPI_0,  4},
    {PB12, SPI_0,  5},
    {PB13, SPI_0,  6},
    {PB14, SPI_0,  7},
    {PB15, SPI_0,  8},
    {PD9,  SPI_0, 15},
    {PD10, SPI_0, 16},
    {PD11, SPI_0, 17},
    {PD12, SPI_0, 18},
    {PD13, SPI_0, 19},
    {PD14, SPI_0, 20},
    {PD15, SPI_0, 21},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC6,  SPI_1,  9},
    {PC7,  SPI_1, 10},
    {PC8,  SPI_1, 11},
    {PC9,  SPI_1, 12},
    {PC10, SPI_1, 13},
    {PC11, SPI_1, 14},
    {PF0,  SPI_1, 22},
    {PF1,  SPI_1, 23},
    {PF2,  SPI_1, 24},
    {PF3,  SPI_1, 25},
    {PF4,  SPI_1, 26},
    {PF5,  SPI_1, 27},
    {PF6,  SPI_1, 28},
    {PF7,  SPI_1, 29},
    {PA0,  SPI_1, 30},
    {PA1,  SPI_1, 31},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PA7,  SPI_2, 0},
    {PA8,  SPI_2, 1},
    {PA9,  SPI_2, 2},
    {PI0,  SPI_2, 3},
    {PI1,  SPI_2, 4},
    {PI2,  SPI_2, 5},
    {PI3,  SPI_2, 6},
    {PB6,  SPI_2, 7},
    {PB7,  SPI_2, 8},
    {PB8,  SPI_2, 9},
    {PB9,  SPI_2, 10},
    {PB10, SPI_2, 11},
    {PF8,  SPI_2, 19},
    {PF9,  SPI_2, 20},
    {PF10, SPI_2, 21},
    {PF11, SPI_2, 22},
    {PF12, SPI_2, 23},
    {PF13, SPI_2, 24},
    {PF14, SPI_2, 25},
    {PF15, SPI_2, 26},
    {PK0,  SPI_2, 27},
    {PK1,  SPI_2, 28},
    {PK2,  SPI_2, 29},
    {PA6,  SPI_2, 31},
#endif
#ifdef USART3_BASE
    /* USART3 */
    {PJ14, SPI_3, 14},
    {PJ15, SPI_3, 15},
    {PC0,  SPI_3, 16},
    {PC1,  SPI_3, 17},
    {PC2,  SPI_3, 18},
    {PC3,  SPI_3, 19},
    {PC4,  SPI_3, 20},
    {PC5,  SPI_3, 21},
#endif
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CS[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PA0,  SPI_0,  29},
    {PA1,  SPI_0,  30},
    {PA2,  SPI_0,  31},
    {PA3,  SPI_0,  0},
    {PA4,  SPI_0,  1},
    {PA5,  SPI_0,  2},
    {PB11, SPI_0,  3},
    {PB12, SPI_0,  4},
    {PB13, SPI_0,  5},
    {PB14, SPI_0,  6},
    {PB15, SPI_0,  7},
    {PD9,  SPI_0, 14},
    {PD10, SPI_0, 15},
    {PD11, SPI_0, 16},
    {PD12, SPI_0, 17},
    {PD13, SPI_0, 18},
    {PD14, SPI_0, 19},
    {PD15, SPI_0, 20},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC6,  SPI_1,  8},
    {PC7,  SPI_1,  9},
    {PC8,  SPI_1, 10},
    {PC9,  SPI_1, 11},
    {PC10, SPI_1, 12},
    {PC11, SPI_1, 13},
    {PF0,  SPI_1, 21},
    {PF1,  SPI_1, 22},
    {PF2,  SPI_1, 23},
    {PF3,  SPI_1, 24},
    {PF4,  SPI_1, 25},
    {PF5,  SPI_1, 26},
    {PF6,  SPI_1, 27},
    {PF7,  SPI_1, 28},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PA8,  SPI_2, 0},
    {PA9,  SPI_2, 1},
    {PI0,  SPI_2, 2},
    {PI1,  SPI_2, 3},
    {PI2,  SPI_2, 4},
    {PI3,  SPI_2, 5},
    {PB6,  SPI_2, 6},
    {PB7,  SPI_2, 7},
    {PB8,  SPI_2, 8},
    {PB9,  SPI_2, 9},
    {PB10, SPI_2, 10},
    {PF8,  SPI_2, 18},
    {PF9,  SPI_2, 19},
    {PF10, SPI_2, 20},
    {PF11, SPI_2, 21},
    {PF12, SPI_2, 22},
    {PF13, SPI_2, 23},
    {PF14, SPI_2, 24},
    {PF15, SPI_2, 25},
    {PK0,  SPI_2, 26},
    {PK1,  SPI_2, 27},
    {PK2,  SPI_2, 28},
    {PA6,  SPI_2, 30},
    {PA7,  SPI_2, 31},
#endif
#ifdef USART3_BASE
    /* USART3 */
    {PJ14, SPI_3, 13},
    {PJ15, SPI_3, 14},
    {PC0,  SPI_3, 15},
    {PC1,  SPI_3, 16},
    {PC2,  SPI_3, 17},
    {PC3,  SPI_3, 18},
    {PC4,  SPI_3, 19},
    {PC5,  SPI_3, 20},
#endif
    {NC  , NC   , NC}
};

/************UART**************/
const PinMap PinMap_UART_TX[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PA0,  USART_0,  0},
    {PA1,  USART_0,  1},
    {PA2,  USART_0,  2},
    {PA3,  USART_0,  3},
    {PA4,  USART_0,  4},
    {PA5,  USART_0,  5},
    {PB11, USART_0,  6},
    {PB12, USART_0,  7},
    {PB13, USART_0,  8},
    {PB14, USART_0,  9},
    {PB15, USART_0, 10},
#endif
#ifdef LEUART0_BASE
    /* LEUART0 */
    {PD9,  LEUART_0, 17},
    {PD10, LEUART_0, 18},
    {PD11, LEUART_0, 19},
    {PD12, LEUART_0, 20},
    {PD13, LEUART_0, 21},
    {PD14, LEUART_0, 22},
    {PD15, LEUART_0, 23},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC6,  USART_1, 11},
    {PC7,  USART_1, 12},
    {PC8,  USART_1, 13},
    {PC9,  USART_1, 14},
    {PC10, USART_1, 15},
    {PC11, USART_1, 16},
    {PF0,  USART_1, 24},
    {PF1,  USART_1, 25},
    {PF2,  USART_1, 26},
    {PF3,  USART_1, 27},
    {PF4,  USART_1, 28},
    {PF5,  USART_1, 29},
    {PF6,  USART_1, 30},
    {PF7,  USART_1, 31},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PA6,  USART_2, 1},
    {PA7,  USART_2, 2},
    {PA8,  USART_2, 3},
    {PA9,  USART_2, 4},
    {PI0,  USART_2, 5},
    {PI1,  USART_2, 6},
    {PI2,  USART_2, 7},
    {PI3,  USART_2, 8},
    {PB6,  USART_2, 9},
    {PB7,  USART_2, 10},
    {PB8,  USART_2, 11},
    {PB9,  USART_2, 12},
    {PB10, USART_2, 13},
    {PF8,  USART_2, 21},
    {PF9,  USART_2, 22},
    {PF10, USART_2, 23},
    {PF11, USART_2, 24},
    {PF12, USART_2, 25},
    {PF13, USART_2, 26},
    {PF14, USART_2, 27},
    {PF15, USART_2, 28},
    {PK0,  USART_2, 29},
    {PK1,  USART_2, 30},
    {PK2,  USART_2, 31},
#endif
#ifdef USART3_BASE
    /* USART3 */
    {PJ14, USART_3, 16},
    {PJ15, USART_3, 17},
    {PC0,  USART_3, 18},
    {PC1,  USART_3, 19},
    {PC2,  USART_3, 20},
    {PC3,  USART_3, 21},
    {PC4,  USART_3, 22},
    {PC5,  USART_3, 23},
#endif
    {NC  , NC   , NC}
};

const PinMap PinMap_UART_RX[] = {
#ifdef USART0_BASE
    /* USART0 */
    {PA0,  USART_0, 31},
    {PA1,  USART_0,  0},
    {PA2,  USART_0,  1},
    {PA3,  USART_0,  2},
    {PA4,  USART_0,  3},
    {PA5,  USART_0,  4},
    {PB11, USART_0,  5},
    {PB12, USART_0,  6},
    {PB13, USART_0,  7},
    {PB14, USART_0,  8},
    {PB15, USART_0,  9},
#endif
#ifdef LEUART0_BASE
    /* LEUART0 */
    {PD9,  LEUART_0, 16},
    {PD10, LEUART_0, 17},
    {PD11, LEUART_0, 18},
    {PD12, LEUART_0, 19},
    {PD13, LEUART_0, 20},
    {PD14, LEUART_0, 21},
    {PD15, LEUART_0, 22},
#endif
#ifdef USART1_BASE
    /* USART1 */
    {PC6,  USART_1, 10},
    {PC7,  USART_1, 11},
    {PC8,  USART_1, 12},
    {PC9,  USART_1, 13},
    {PC10, USART_1, 14},
    {PC11, USART_1, 15},
    {PF0,  USART_1, 23},
    {PF1,  USART_1, 24},
    {PF2,  USART_1, 25},
    {PF3,  USART_1, 26},
    {PF4,  USART_1, 27},
    {PF5,  USART_1, 28},
    {PF6,  USART_1, 29},
    {PF7,  USART_1, 30},
#endif
#ifdef USART2_BASE
    /* USART2 */
    {PA6,  USART_2, 0},
    {PA7,  USART_2, 1},
    {PA8,  USART_2, 2},
    {PA9,  USART_2, 3},
    {PI0,  USART_2, 4},
    {PI1,  USART_2, 5},
    {PI2,  USART_2, 6},
    {PI3,  USART_2, 7},
    {PB6,  USART_2, 8},
    {PB7,  USART_2, 9},
    {PB8,  USART_2, 10},
    {PB9,  USART_2, 11},
    {PB10, USART_2, 12},
    {PF8,  USART_2, 20},
    {PF9,  USART_2, 21},
    {PF10, USART_2, 22},
    {PF11, USART_2, 23},
    {PF12, USART_2, 24},
    {PF13, USART_2, 25},
    {PF14, USART_2, 26},
    {PF15, USART_2, 27},
    {PK0,  USART_2, 28},
    {PK1,  USART_2, 29},
    {PK2,  USART_2, 30},
#endif
#ifdef USART3_BASE
    /* USART3 */
    {PJ14, USART_3, 15},
    {PJ15, USART_3, 16},
    {PC0,  USART_3, 17},
    {PC1,  USART_3, 18},
    {PC2,  USART_3, 19},
    {PC3,  USART_3, 20},
    {PC4,  USART_3, 21},
    {PC5,  USART_3, 22},
#endif
    {NC  , NC   , NC}
};
