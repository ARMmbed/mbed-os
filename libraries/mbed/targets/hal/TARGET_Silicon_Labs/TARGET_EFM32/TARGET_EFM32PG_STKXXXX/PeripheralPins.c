/***************************************************************************//**
 * @file PeripheralPins.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third particularty.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "PeripheralPins.h"

/************ADC***************/
/* The third "function" value is used to select the correct ADC channel */
const PinMap PinMap_ADC[] = {
    // Pearl ok
    {PA0,  ADC_0, adcPosSelBUS3XCH8},
    {PA2,  ADC_0, adcPosSelBUS3XCH10},  
    {PA4,  ADC_0, adcPosSelBUS3XCH12},  
    {PB12, ADC_0, adcPosSelBUS3XCH28},  
    {PB14, ADC_0, adcPosSelBUS3XCH30},  
    {PC6,  ADC_0, adcPosSelBUS1XCH6},  
    {PC7,  ADC_0, adcPosSelBUS2XCH7},  
    {PC8,  ADC_0, adcPosSelBUS1XCH8}, 
    {PC9,  ADC_0, adcPosSelBUS2XCH9},  
    {PC10, ADC_0, adcPosSelBUS1XCH10},
    {PC11, ADC_0, adcPosSelBUS2XCH11},  
    {PD10, ADC_0, adcPosSelBUS3XCH2},  
    {PD11, ADC_0, adcPosSelBUS3YCH3},
    {PD12, ADC_0, adcPosSelBUS3XCH4},
    {PD13, ADC_0, adcPosSelBUS3YCH5},
    {PD14, ADC_0, adcPosSelBUS3XCH6},
    {PF0,  ADC_0, adcPosSelBUS1XCH16},
    {PF1,  ADC_0, adcPosSelBUS2XCH17},
    {PF2,  ADC_0, adcPosSelBUS1XCH18},
    {PF3,  ADC_0, adcPosSelBUS2XCH19},
    {PF4,  ADC_0, adcPosSelBUS1XCH20},
    {PF5,  ADC_0, adcPosSelBUS2XCH21},
    {PF6,  ADC_0, adcPosSelBUS1XCH22},
    {PF7,  ADC_0, adcPosSelBUS2XCH23},
    {NC ,  NC   , NC}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    // Pearl ok
    {NC  , NC   , NC}
};

/************I2C SCL***********/
const PinMap PinMap_I2C_SCL[] = {
    // Pearl ok

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

    {NC  , NC   , NC}
};

/************I2C SDA***********/
const PinMap PinMap_I2C_SDA[] = {

    // Pearl ok

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

    /* Not connected */
    {NC  , NC   , NC}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {

    // Pearl ok

    {PA0,  PWM_CH0,  0},
    {PA1,  PWM_CH0,  1},
    {PA2,  PWM_CH0,  2},
    {PA3,  PWM_CH0,  3},
    {PA4,  PWM_CH0,  4},
    {PA5,  PWM_CH0,  5},
    {PB11, PWM_CH0,  6},
    {PB12, PWM_CH0,  7},
    {PB13, PWM_CH0,  8},
    {PB14, PWM_CH0,  9},
    {PB15, PWM_CH0, 10},
    {PC6,  PWM_CH0, 11},
    {PC7,  PWM_CH0, 12},
    {PC8,  PWM_CH0, 13},
    {PC9,  PWM_CH0, 14},
    {PC10, PWM_CH0, 15},
    {PC11, PWM_CH0, 16},

    {PD10, PWM_CH0, 18},
    {PD11, PWM_CH0, 19},
    {PD12, PWM_CH0, 20},
    {PD13, PWM_CH0, 21},
    {PD14, PWM_CH0, 22},
    {PD15, PWM_CH0, 23},
    {PF0,  PWM_CH0, 24},
    {PF1,  PWM_CH0, 25},
    {PF2,  PWM_CH0, 26},
    {PF3,  PWM_CH0, 27},
    {PF4,  PWM_CH0, 28},
    {PF5,  PWM_CH0, 29},
    {PF6,  PWM_CH0, 30},
    {PF7,  PWM_CH0, 31},

    {NC  , NC     , NC}
};

/*************SPI**************/
const PinMap PinMap_SPI_MOSI[] = {

    // Pearl ok

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
    {PC6,  SPI_0, 11},
    {PC7,  SPI_0, 12},
    {PC8,  SPI_0, 13},
    {PC9,  SPI_0, 14},
    {PC10, SPI_0, 15},
    {PC11, SPI_0, 16},

    {PD10, SPI_0, 18},
    {PD11, SPI_0, 19},
    {PD12, SPI_0, 20},
    {PD13, SPI_0, 21},
    {PD14, SPI_0, 22},
    {PD15, SPI_0, 23},
    {PF0,  SPI_0, 24},
    {PF1,  SPI_0, 25},
    {PF2,  SPI_0, 26},
    {PF3,  SPI_0, 27},
    {PF4,  SPI_0, 28},
    {PF5,  SPI_0, 29},
    {PF6,  SPI_0, 30},
    {PF7,  SPI_0, 31},

    /* USART1 */
    {PA0,  SPI_1,  0},
    {PA1,  SPI_1,  1},
    {PA2,  SPI_1,  2},
    {PA3,  SPI_1,  3},
    {PA4,  SPI_1,  4},
    {PA5,  SPI_1,  5},
    {PB11, SPI_1,  6},
    {PB12, SPI_1,  7},
    {PB13, SPI_1,  8},
    {PB14, SPI_1,  9},
    {PB15, SPI_1, 10},
    {PC6,  SPI_1, 11},
    {PC7,  SPI_1, 12},
    {PC8,  SPI_1, 13},
    {PC9,  SPI_1, 14},
    {PC10, SPI_1, 15},
    {PC11, SPI_1, 16},

    {PD10, SPI_1, 18},
    {PD11, SPI_1, 19},
    {PD12, SPI_1, 20},
    {PD13, SPI_1, 21},
    {PD14, SPI_1, 22},
    {PD15, SPI_1, 23},
    {PF0,  SPI_1, 24},
    {PF1,  SPI_1, 25},
    {PF2,  SPI_1, 26},
    {PF3,  SPI_1, 27},
    {PF4,  SPI_1, 28},
    {PF5,  SPI_1, 29},
    {PF6,  SPI_1, 30},
    {PF7,  SPI_1, 31},

    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_MISO[] = {

    // Pearl ok

    /* USART0 */
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
    {PC6,  SPI_0, 10},
    {PC7,  SPI_0, 11},
    {PC8,  SPI_0, 12},
    {PC9,  SPI_0, 13},
    {PC10, SPI_0, 14},
    {PC11, SPI_0, 15},

    {PD10, SPI_0, 17},
    {PD11, SPI_0, 18},
    {PD12, SPI_0, 19},
    {PD13, SPI_0, 20},
    {PD14, SPI_0, 21},
    {PD15, SPI_0, 22},
    {PF0,  SPI_0, 23},
    {PF1,  SPI_0, 24},
    {PF2,  SPI_0, 25},
    {PF3,  SPI_0, 26},
    {PF4,  SPI_0, 27},
    {PF5,  SPI_0, 28},
    {PF6,  SPI_0, 29},
    {PF7,  SPI_0, 30},
    {PA0,  SPI_0, 31},

    /* USART1 */
    {PA1,  SPI_1,  0},
    {PA2,  SPI_1,  1},
    {PA3,  SPI_1,  2},
    {PA4,  SPI_1,  3},
    {PA5,  SPI_1,  4},
    {PB11, SPI_1,  5},
    {PB12, SPI_1,  6},
    {PB13, SPI_1,  7},
    {PB14, SPI_1,  8},
    {PB15, SPI_1,  9},
    {PC6,  SPI_1, 10},
    {PC7,  SPI_1, 11},
    {PC8,  SPI_1, 12},
    {PC9,  SPI_1, 13},
    {PC10, SPI_1, 14},
    {PC11, SPI_1, 15},

    {PD10, SPI_1, 17},
    {PD11, SPI_1, 18},
    {PD12, SPI_1, 19},
    {PD13, SPI_1, 20},
    {PD14, SPI_1, 21},
    {PD15, SPI_1, 22},
    {PF0,  SPI_1, 23},
    {PF1,  SPI_1, 24},
    {PF2,  SPI_1, 25},
    {PF3,  SPI_1, 26},
    {PF4,  SPI_1, 27},
    {PF5,  SPI_1, 28},
    {PF6,  SPI_1, 29},
    {PF7,  SPI_1, 30},
    {PA0,  SPI_1, 31},

    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CLK[] = {

    // Pearl ok

    /* USART0 */
    {PA2,  SPI_0,  0},
    {PA3,  SPI_0,  1},
    {PA4,  SPI_0,  2},
    {PA5,  SPI_0,  3},
    {PB11, SPI_0,  4},
    {PB12, SPI_0,  5},
    {PB13, SPI_0,  6},
    {PB14, SPI_0,  7},
    {PB15, SPI_0,  8},
    {PC6,  SPI_0,  9},
    {PC7,  SPI_0, 10},
    {PC8,  SPI_0, 11},
    {PC9,  SPI_0, 12},
    {PC10, SPI_0, 13},
    {PC11, SPI_0, 14},

    {PD10, SPI_0, 16},
    {PD11, SPI_0, 17},
    {PD12, SPI_0, 18},
    {PD13, SPI_0, 19},
    {PD14, SPI_0, 20},
    {PD15, SPI_0, 21},
    {PF0,  SPI_0, 22},
    {PF1,  SPI_0, 23},
    {PF2,  SPI_0, 24},
    {PF3,  SPI_0, 25},
    {PF4,  SPI_0, 26},
    {PF5,  SPI_0, 27},
    {PF6,  SPI_0, 28},
    {PF7,  SPI_0, 29},
    {PA0,  SPI_0, 30},
    {PA1,  SPI_0, 31},

    /* USART1 */
    {PA2,  SPI_1,  0},
    {PA3,  SPI_1,  1},
    {PA4,  SPI_1,  2},
    {PA5,  SPI_1,  3},
    {PB11, SPI_1,  4},
    {PB12, SPI_1,  5},
    {PB13, SPI_1,  6},
    {PB14, SPI_1,  7},
    {PB15, SPI_1,  8},
    {PC6,  SPI_1,  9},
    {PC7,  SPI_1, 10},
    {PC8,  SPI_1, 11},
    {PC9,  SPI_1, 12},
    {PC10, SPI_1, 13},
    {PC11, SPI_1, 14},

    {PD10, SPI_1, 16},
    {PD11, SPI_1, 17},
    {PD12, SPI_1, 18},
    {PD13, SPI_1, 19},
    {PD14, SPI_1, 20},
    {PD15, SPI_1, 21},
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

    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CS[] = {

    /* USART0 */
    {PA3,  SPI_0,  0},
    {PA4,  SPI_0,  1},
    {PA5,  SPI_0,  2},
    {PB11, SPI_0,  3},
    {PB12, SPI_0,  4},
    {PB13, SPI_0,  5},
    {PB14, SPI_0,  6},
    {PB15, SPI_0,  7},
    {PC6,  SPI_0,  8},
    {PC7,  SPI_0,  9},
    {PC8,  SPI_0, 10},
    {PC9,  SPI_0, 11},
    {PC10, SPI_0, 12},
    {PC11, SPI_0, 13},

    {PD10, SPI_0, 15},
    {PD11, SPI_0, 16},
    {PD12, SPI_0, 17},
    {PD13, SPI_0, 18},
    {PD14, SPI_0, 19},
    {PD15, SPI_0, 20},
    {PF0,  SPI_0, 21},
    {PF1,  SPI_0, 22},
    {PF2,  SPI_0, 23},
    {PF3,  SPI_0, 24},
    {PF4,  SPI_0, 25},
    {PF5,  SPI_0, 26},
    {PF6,  SPI_0, 27},
    {PF7,  SPI_0, 28},
    {PA0,  SPI_0, 29},
    {PA1,  SPI_0, 30},
    {PA2,  SPI_0, 31},

    /* USART1 */
    {PA3,  SPI_1,  0},
    {PA4,  SPI_1,  1},
    {PA5,  SPI_1,  2},
    {PB11, SPI_1,  3},
    {PB12, SPI_1,  4},
    {PB13, SPI_1,  5},
    {PB14, SPI_1,  6},
    {PB15, SPI_1,  7},
    {PC6,  SPI_1,  8},
    {PC7,  SPI_1,  9},
    {PC8,  SPI_1, 10},
    {PC9,  SPI_1, 11},
    {PC10, SPI_1, 12},
    {PC11, SPI_1, 13},

    {PD10, SPI_1, 15},
    {PD11, SPI_1, 16},
    {PD12, SPI_1, 17},
    {PD13, SPI_1, 18},
    {PD14, SPI_1, 19},
    {PD15, SPI_1, 20},
    {PF0,  SPI_1, 21},
    {PF1,  SPI_1, 22},
    {PF2,  SPI_1, 23},
    {PF3,  SPI_1, 24},
    {PF4,  SPI_1, 25},
    {PF5,  SPI_1, 26},
    {PF6,  SPI_1, 27},
    {PF7,  SPI_1, 28},
    {PA0,  SPI_1, 29},
    {PA1,  SPI_1, 30},
    {PA2,  SPI_1, 31},

    {NC  , NC   , NC}
};

/************UART**************/
const PinMap PinMap_UART_TX[] = {

    // Pearl ok

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
    {PC6,  USART_0, 11},
    {PC7,  USART_0, 12},
    {PC8,  USART_0, 13},
    {PC9,  USART_0, 14},
    {PC10, USART_0, 15},
    {PC11, USART_0, 16},

    {PD10, USART_0, 18},
    {PD11, USART_0, 19},
    {PD12, USART_0, 20},
    {PD13, USART_0, 21},
    {PD14, USART_0, 22},
    {PD15, USART_0, 23},
    {PF0,  USART_0, 24},
    {PF1,  USART_0, 25},
    {PF2,  USART_0, 26},
    {PF3,  USART_0, 27},
    {PF4,  USART_0, 28},
    {PF5,  USART_0, 29},
    {PF6,  USART_0, 30},
    {PF7,  USART_0, 31},

    /* USART1, LEUART0, LEUART1 excluded as they can not
       be selected through the Mbed API since their
       pin maps completely overlap each other - FIXME */

    {NC  , NC   , NC}
};

const PinMap PinMap_UART_RX[] = {

    // Pearl ok

    /* USART0 */
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
    {PC6,  USART_0, 10},
    {PC7,  USART_0, 11},
    {PC8,  USART_0, 12},
    {PC9,  USART_0, 13},
    {PC10, USART_0, 14},
    {PC11, USART_0, 15},

    {PD10, USART_0, 17},
    {PD11, USART_0, 18},
    {PD12, USART_0, 19},
    {PD13, USART_0, 20},
    {PD14, USART_0, 21},
    {PD15, USART_0, 22},
    {PF0,  USART_0, 23},
    {PF1,  USART_0, 24},
    {PF2,  USART_0, 25},
    {PF3,  USART_0, 26},
    {PF4,  USART_0, 27},
    {PF5,  USART_0, 28},
    {PF6,  USART_0, 29},
    {PF7,  USART_0, 30},
    {PA0,  USART_0, 31},

    /* USART1, LEUART0, LEUART1 excluded as they can not
       be selected through the Mbed API since their
       pin maps completely overlap each other - FIXME */

    {NC  , NC   , NC}
};
