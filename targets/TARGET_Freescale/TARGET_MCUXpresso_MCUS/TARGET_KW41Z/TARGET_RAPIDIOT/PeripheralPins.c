/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {PTB1,  ADC0_SE1, 0},
    {PTB3,  ADC0_SE2, 0},
    {PTB2,  ADC0_SE3, 0},
    {PTB18, ADC0_SE4, 0},
    {PTA19, ADC0_SE5, 0},
    {NC   , NC       , 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {DAC0_OUT, DAC_0, 0},
    {NC,       NC,    0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {PTB1,  I2C_0, 3},
    {PTB17, I2C_1, 3},
    {PTC1, I2C_0, 3},
    {PTC3, I2C_1, 3},
    {PTC7, I2C_1, 3},
    {PTC16, I2C_0, 3},
    {PTC18, I2C_1, 3},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PTB0,  I2C_0, 3},
    {PTB16, I2C_1, 3},
    {PTB18, I2C_1, 3},
    {PTC2, I2C_1, 3},
    {PTC6, I2C_1, 3},
    {PTC17, I2C_1, 3},
    {PTC19, I2C_0, 3},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PTC3,  LPUART_0, 4},
    {PTC7,  LPUART_0, 4},
    {PTC18, LPUART_0, 4},
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {PTC2,  LPUART_0, 4},
    {PTC6,  LPUART_0, 4},
    {PTC17, LPUART_0, 4},
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_CTS[] = {
    {PTC4,  LPUART_0, 4},
    {PTC19, LPUART_0, 4},
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_RTS[] = {
    {PTC1,  LPUART_0, 4},
    {PTC5,  LPUART_0, 4},
    {PTC16, LPUART_0, 4},
    {NC   , NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {PTA18, SPI_1, 2},
    {PTC16,  SPI_0, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PTA16, SPI_1, 2},
    {PTC17,  SPI_0, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PTA17, SPI_1, 2},
    {PTC18,  SPI_0, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PTA1,  SPI_1, 2},
    {PTA19, SPI_1, 2},
    {PTC19,  SPI_0, 2},
    {NC   , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    /* TPM 0 */
    {PTA16, PWM_1, 5},
    {PTB0,  PWM_2, 5},
    {PTB1,  PWM_3, 5},
    {PTA2,  PWM_4, 5},
    {PTB18, PWM_1, 5},
    {PTC3,  PWM_2, 5},
    {PTC1,  PWM_3, 5},
    {PTC16, PWM_4, 5},
    /* TPM 1 */
    {PTA0,  PWM_5, 5},
    {PTA1,  PWM_6, 5},
    {PTB2,  PWM_5, 5},
    {PTB3,  PWM_6, 5},
    {PTC4,  PWM_5, 5},
    {PTC5,  PWM_6, 5},
    {NC   , NC   , 0}
};
