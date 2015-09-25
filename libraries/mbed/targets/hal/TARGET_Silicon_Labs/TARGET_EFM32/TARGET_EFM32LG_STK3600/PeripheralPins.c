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
 * of any proprietary rights of a third party.
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
    {PD0, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH0},
    {PD1, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH1},
    {PD2, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH2},
    {PD3, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH3},
    {PD4, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH4},
    {PD5, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH5},
    {PD6, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH6},
    {PD7, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH7},
    {NC  , NC   , NC}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {PB11, DAC_0, 0},
    {PB12, DAC_0, 1},
    {NC  , NC   , NC}
};

/************I2C SCL***********/
const PinMap PinMap_I2C_SCL[] = {
    /* I2C0 */
    {PA1,  I2C_0, 0},
    {PD7,  I2C_0, 1},
    {PC7,  I2C_0, 2},
    {PD15, I2C_0, 3},
    {PC1,  I2C_0, 4},
    {PF1,  I2C_0, 5},
    {PE13, I2C_0, 6},

    /* I2C1 */
    {PC5,  I2C_1, 0},
    {PB12,  I2C_1, 1},
    {PE1,  I2C_1, 2},

    /* Not connected */
    {NC  , NC   , NC}
};

/************I2C SDA***********/
const PinMap PinMap_I2C_SDA[] = {
    /* I2C0 */
    {PA0,  I2C_0, 0},
    {PD6,  I2C_0, 1},
    {PC6,  I2C_0, 2},
    {PD14, I2C_0, 3},
    {PC0,  I2C_0, 4},
    {PF0,  I2C_0, 5},
    {PE12, I2C_0, 6},

    /* I2C1 */
    {PC4,  I2C_1, 0},
    {PB11,  I2C_1, 1},
    {PE0,  I2C_1, 2},

    /* Not connected */
    {NC  , NC   , NC}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PA12, PWM_CH0, 0},
    {PA13, PWM_CH1, 0},
    {PA14, PWM_CH2, 0},
    {NC  , NC   , 0}
};

/*************SPI**************/
const PinMap PinMap_SPI_MOSI[] = {
    /* USART0 */
    {PE10, SPI_0, 0},
    {PE7, SPI_0, 1},
    {PC11, SPI_0, 2},
    {PC0, SPI_0, 5},
    /* USART1 */
    {PD0, SPI_1, 1},
    {PD7, SPI_1, 2},
    /* USART2 */
    {PC2, SPI_2, 0},
    {PB3, SPI_2, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_MISO[] = {
    /* USART0 */
    {PE11, SPI_0, 0},
    {PE6, SPI_0, 1},
    {PC10, SPI_0, 2},
    {PC1, SPI_0, 5},
    /* USART1 */
    {PD1, SPI_1, 1},
    {PD6, SPI_1, 2},
    /* USART2 */
    {PC3, SPI_2, 0},
    {PB4, SPI_2, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CLK[] = {
    /* USART0 */
    {PE12, SPI_0, 0},
    {PE5, SPI_0, 1},
    {PC9, SPI_0, 2},
    {PB13, SPI_0, 5},
    /* USART1 */
    {PD2, SPI_1, 1},
    {PF0, SPI_1, 2},
    /* USART2 */
    {PC4, SPI_2, 0},
    {PB5, SPI_2, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CS[] = {
    /* USART0 */
    {PE13, SPI_0, 0},
    {PE4, SPI_0, 1},
    {PC8, SPI_0, 2},
    {PB14, SPI_0, 5},
    /* USART1 */
    {PD3, SPI_1, 1},
    {PF1, SPI_1, 2},
    /* USART2 */
    {PC5, SPI_2, 0},
    {PB6, SPI_2, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

/************UART**************/
const PinMap PinMap_UART_TX[] = {
    /* UART0 */
    {PF6, UART_0, 0},
    {PE0, UART_0, 1},
    /* UART1 */
    {PF10, UART_1, 1},
    {PB9, UART_1, 2},
    {PE2, UART_1, 3},
    /* USART0 */
    {PE10, USART_0, 0},
    {PE7, USART_0, 1},
    {PC11, USART_0, 2},
    {PE13, USART_0, 3},
    {PB7, USART_0, 4},
    /* USART1 */
    {PC0, USART_1, 0},
    {PD0, USART_1, 1},
    {PD7, USART_1, 2},
    /* USART2 */
    {PC2, USART_2, 0},
    {PB3, USART_2, 1},
    /* LEUART0 */
    {PD4,  LEUART_0, 0},
    {PB13, LEUART_0, 1},
    {PE14, LEUART_0, 2},
    {PF0,  LEUART_0, 3},
    {PF2,  LEUART_0, 4},
    /* LEUART1 */
    {PC6,  LEUART_1, 0},
    {PA5,  LEUART_1, 1},
    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_UART_RX[] = {
    /* UART0 */
    {PF7, UART_0, 0},
    {PE1, UART_0, 1},
    /* UART1 */
    {PF11, UART_1, 1},
    {PB10, UART_1, 2},
    {PE3, UART_1, 3},
    /* USART0 */
    {PE11, USART_0, 0},
    {PE6, USART_0, 1},
    {PC10, USART_0, 2},
    {PE12, USART_0, 3},
    {PB8, USART_0, 4},
    /* USART1 */
    {PC1, USART_1, 0},
    {PD1, USART_1, 1},
    {PD6, USART_1, 2},
    /* USART2 */
    {PC3, USART_2, 0},
    {PB4, USART_2, 1},
    /* LEUART0 */
    {PD5,  LEUART_0, 0},
    {PB14, LEUART_0, 1},
    {PE15, LEUART_0, 2},
    {PF1,  LEUART_0, 3},
    {PA0, LEUART_0, 4},
    /* LEUART1 */
    {PC7,  LEUART_1, 0},
    {PA6,  LEUART_1, 1},
    /* Not connected */
    {NC  , NC   , NC}
};
