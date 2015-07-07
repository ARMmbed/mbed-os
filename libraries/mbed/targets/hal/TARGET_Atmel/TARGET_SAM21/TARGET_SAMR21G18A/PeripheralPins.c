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

#define SERCOM_NULL 0xFF
#define MUX_NULL    0xFF

#define SERCOM_USART_CTRLA_RXPO_Pos 20           /**< \brief (SERCOM_USART_CTRLA) Receive Data Pinout */
#define SERCOM_USART_CTRLA_RXPO_Msk (0x3ul << SERCOM_USART_CTRLA_RXPO_Pos)
#define SERCOM_USART_CTRLA_RXPO(value) ((SERCOM_USART_CTRLA_RXPO_Msk & ((value) << SERCOM_USART_CTRLA_RXPO_Pos)))

#define SERCOM_USART_CTRLA_TXPO_Pos 16           /**< \brief (SERCOM_USART_CTRLA) Transmit Data Pinout */
#define SERCOM_USART_CTRLA_TXPO_Msk (0x3ul << SERCOM_USART_CTRLA_TXPO_Pos)
#define SERCOM_USART_CTRLA_TXPO(value) ((SERCOM_USART_CTRLA_TXPO_Msk & ((value) << SERCOM_USART_CTRLA_TXPO_Pos)))


/************RTC***************/
const PinMap PinMap_RTC[] = {
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {PA04,  ADC_0, 1},
    {PA05,  ADC_1, 1},
    {PA06,  ADC_2, 1},
    {PA07,  ADC_3, 1},
    {PA08,  ADC_4, 1},
    {PA09,  ADC_5, 1},
    {PB02,  ADC_6, 1},
    {PB03,  ADC_7, 1}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
};

/************SERCOM Pins***********/
const PinMap PinMap_SERCOM_PAD[] = {
    {PA04, SERCOM0_PAD0, 3},
    {PA05, SERCOM0_PAD1, 3},
    {PA06, SERCOM0_PAD2, 3},
    {PA07, SERCOM0_PAD3, 3},
    {PA08, SERCOM0_PAD0, 2},
    {PA09, SERCOM0_PAD1, 2},
    {PA12, SERCOM2_PAD0, 2},
    {PA13, SERCOM2_PAD1, 2},
    {PA14, SERCOM2_PAD2, 2},
    {PA15, SERCOM2_PAD3, 2},
    {PA16, SERCOM1_PAD0, 2},
    {PA17, SERCOM1_PAD1, 2},
    {PA18, SERCOM1_PAD2, 2},
    {PA19, SERCOM1_PAD3, 2},
    {PA22, SERCOM3_PAD0, 2},
    {PA23, SERCOM3_PAD1, 2},
    {PA24, SERCOM3_PAD2, 2},
    {PA25, SERCOM3_PAD3, 2},
    {PA27, SERCOM3_PAD0, 5},
    {PA28, SERCOM3_PAD1, 5},
    {PB02, SERCOM5_PAD0, 3},
    {PB03, SERCOM5_PAD1, 3},
    {PB22, SERCOM5_PAD2, 3},
    {PB23, SERCOM5_PAD3, 3},
    {PB30, SERCOM4_PAD2, 5},
    {PB31, SERCOM4_PAD1, 5},
    {PC18, SERCOM4_PAD3, 5},
    {PC19, SERCOM4_PAD0, 5},

    /* Not connected */
    {NC  , NC   , NC}
};

/*******SERCOM Pins extended*******/
const PinMap PinMap_SERCOM_PADEx[] = {
    {PA00, SERCOM1_PAD0, 3},
    {PA01, SERCOM1_PAD1, 3},
    {PA08, SERCOM2_PAD0, 3},
    {PA09, SERCOM2_PAD1, 3},
    {PA16, SERCOM3_PAD0, 3},
    {PA17, SERCOM3_PAD1, 3},
    {PA18, SERCOM3_PAD2, 3},
    {PA19, SERCOM3_PAD3, 3},
    {PA22, SERCOM5_PAD0, 3},
    {PA23, SERCOM5_PAD1, 3},
    {PA24, SERCOM5_PAD2, 3},
    {PA25, SERCOM5_PAD3, 3},
    {PA30, SERCOM1_PAD2, 3},
    {PA31, SERCOM1_PAD3, 3},

    /* Not connected */
    {NC  , NC   , NC}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
};

const PinMap PinMap_I2C_SCL[] = {
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PA04,  UART_0, 0}
};

const PinMap PinMap_UART_RX[] = {
    {PA05,  UART_0, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
};

const PinMap PinMap_SPI_MOSI[] = {
};

const PinMap PinMap_SPI_MISO[] = {
};

const PinMap PinMap_SPI_SSEL[] = {
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
};

/**********EXTINT*************/
const PinMap PinMap_EXTINT[] = {
    {PA16,  EXTINT_0, 0},
    {PB00,  EXTINT_0, 0},
    {PB16,  EXTINT_0, 0},
    {PA00,  EXTINT_0, 0},

    {PA17,  EXTINT_1, 0},
    {PB17,  EXTINT_1, 0},
    {PA01,  EXTINT_1, 0},

    {PA18,  EXTINT_2, 0},
    {PA02,  EXTINT_2, 0},

    {PA19,  EXTINT_3, 0},
    {PB03,  EXTINT_3, 0},

    {PA04,  EXTINT_4, 0},
    {PA20,  EXTINT_4, 0},

    {PA05,  EXTINT_5, 0},

    {PA06,  EXTINT_6, 0},
    {PA22,  EXTINT_6, 0},
    {PB22,  EXTINT_6, 0},

    {PA07,  EXTINT_7, 0},
    {PA23,  EXTINT_7, 0},
    {PB28,  EXTINT_7, 0},

    {PA28,  EXTINT_8, 0},
    {PB08,  EXTINT_8, 0},

    {PA09,  EXTINT_9, 0},
    {PB09,  EXTINT_9, 0},

    {PA10,  EXTINT_10, 0},
    {PA30,  EXTINT_10, 0},

    {PA11,  EXTINT_11, 0},
    {PA31,  EXTINT_11, 0},

    {PA12,  EXTINT_12, 0},
    {PA24,  EXTINT_12, 0},

    {PA13,  EXTINT_13, 0},
    {PA25,  EXTINT_13, 0},

    {PB14,  EXTINT_14, 0},
    {PB30,  EXTINT_14, 0},
    {PA14,  EXTINT_14, 0},

    {PA15,  EXTINT_15, 0},
    {PA27,  EXTINT_15, 0},
    {PB15,  EXTINT_15, 0},
    {PB31,  EXTINT_15, 0}
};



