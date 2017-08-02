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

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {P0_30, ADC0_SE1,  0},
    {P1_4,  ADC0_SE7,  0},
    {P1_5,  ADC0_SE8,  0},
    {P1_8,  ADC0_SE11, 0},
    {NC   , NC      ,  0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {P0_24, I2C_0, 1},
    {P0_26, I2C_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {P0_23, I2C_0, 1},
    {P0_25, I2C_1, 1},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P0_1, UART_0, 1},
    {P0_9, UART_1, 1},
    {NC  ,  NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {P0_0, UART_0, 1},
    {P0_8, UART_1, 1},
    {NC  ,  NC   , 0}
};

const PinMap PinMap_UART_CTS[] = {
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_RTS[] = {
    {NC   , NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P0_11, SPI_0, 1},
    {P0_19, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {P0_12, SPI_0, 1},
    {P0_20, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {P0_13, SPI_0, 1},
    {P0_18, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {P0_14, SPI_0, 1},
    {P1_1,  SPI_1, 4},
    {P1_2,  SPI_1, 4},
    {NC  ,  NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {NC   , NC    , 0}
};
