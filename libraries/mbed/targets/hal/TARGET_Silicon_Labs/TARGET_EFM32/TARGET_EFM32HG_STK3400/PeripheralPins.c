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
const PinMap PinMap_ADC[] = {
    {PD4, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH4},
    {PD5, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH5},
    {PD6, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH6},
    {PD7, ADC_0, ADC_SINGLECTRL_INPUTSEL_CH7},
    {NC  , NC   , NC}
};

/************I2C SCL***********/
const PinMap PinMap_I2C_SCL[] = {
    /* I2C0 */
    {PA1,  I2C_0, 0},
    {PD7,  I2C_0, 1},
    {PC1,  I2C_0, 4},
    {PF1,  I2C_0, 5},
    {PE13, I2C_0, 6},

    /* Not connected */
    {NC  , NC   , NC}
};

/************I2C SDA***********/
const PinMap PinMap_I2C_SDA[] = {
    /* I2C0 */
    {PA0,  I2C_0, 0},
    {PD6,  I2C_0, 1},
    {PC0,  I2C_0, 4},
    {PF0,  I2C_0, 5},
    {PE12, I2C_0, 6},

    /* Not connected */
    {NC  , NC   , NC}
};
/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PA0, PWM_CH0, 0},
    {PA1, PWM_CH1, 0},
    {PA2, PWM_CH2, 0},
    {NC  , NC   , NC}
};

/*************SPI**************/
const PinMap PinMap_SPI_MOSI[] = {
    /* USART0 */
    {PE10, SPI_0, 0},
    //{NC, SPI_0, 2},     /* SPI_0 loc2 is not bonded */
    {PE13, SPI_0, 3},
    {PB7,  SPI_0, 4},

    /* USART1 */
    {PC0,  SPI_1, 0},
    {PD7,  SPI_1, 3},
    {PF2,  SPI_1, 4},

    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_MISO[] = {
    /* USART0 */
    {PE11, SPI_0, 0},
    {PC10, SPI_0, 2},
    {PE12, SPI_0, 3},
    {PB8,  SPI_0, 4},

    /* USART1 */
    {PC1,  SPI_1, 0},
    {PD6,  SPI_1, 3},
    {PA0,  SPI_1, 4},

    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CLK[] = {
    /* USART0 */
    {PE12, SPI_0, 0},
    {PC9,  SPI_0, 2},
    //{PC15, SPI_0, 3},     /* Conflict with SPI_0 loc4 */
    {PB13, SPI_0, 4},

    /* USART1 */
    {PB7,  SPI_1, 0},
    {PC15, SPI_1, 3},
    {PB11, SPI_1, 4},

    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_SPI_CS[] = {
    /* USART0 */
    {PE13, SPI_0, 0},
    {PC8,  SPI_0, 2},
    //{PC14, SPI_0, 3},     /* Conflict with SPI_1 loc3 */
    {PB14, SPI_0, 4},

    /* USART1 */
    {PB8,  SPI_1, 0},
    {PC14, SPI_1, 3},

    /* Not connected */
    {NC  , NC   , NC}
};

/************UART**************/
const PinMap PinMap_UART_TX[] = {
    /* USART0 */
    {PE10, USART_0, 0},
    //{NC, USART_0, 2},     /* USART_0 loc2 is not bonded */
    {PE13, USART_0, 3},
    {PB7,  USART_0, 4},

    /* USART1 */
    {PC0,  USART_1, 0},
    {PD7,  USART_1, 3},
    {PF2,  USART_1, 4},

    /* LEUART0 */
    {PD4,  LEUART_0, 0},
    {PB13, LEUART_0, 1},
    {PF0,  LEUART_0, 3},
    {PC14, LEUART_0, 5},

    /* Not connected */
    {NC  , NC   , NC}
};

const PinMap PinMap_UART_RX[] = {
    /* USART0 */
    {PE11, USART_0, 0},
    //{PC10, USART_0, 2},
    {PE12, USART_0, 3},
    {PB8,  USART_0, 4},

    /* USART1 */
    {PC1,  USART_1, 0},
    {PD6,  USART_1, 3},
    {PA0,  USART_1, 4},

    /* LEUART0 */
    {PD5,  LEUART_0, 0},
    {PB14, LEUART_0, 1},
    {PF1,  LEUART_0, 3},
    {PC15, LEUART_0, 5},

    /* Not connected */
    {NC  , NC   , NC}
};
