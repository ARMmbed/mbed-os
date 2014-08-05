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

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {PTC11, I2C_1, 2},
    {PTB3 , I2C_0, 7},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PTB2 , I2C_0, 2},
    {PTC10, I2C_1, 2},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PTB17, UART_0, 3},
    {PTD3 , UART_2, 3},
    {PTB11, UART_3, 3},
    {PTE8, UART_5, 0},
    {NC, NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {PTB16, UART_0, 3},
    {PTB10, UART_3, 3},
    {PTD2, UART_2, 3},
    {PTE9, UART_5, 0},
    {NC,  NC, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {PTE2 , SPI_1, 2},
    {PTC5 , SPI_0, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PTE3 , SPI_1, 7},
    {PTC6 , SPI_0, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PTE1 , SPI_1, 7},
    {PTC7 , SPI_0, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PTE4 , SPI_1, 2},
    {PTC4 , SPI_0, 2},
    {NC   , NC   , 0}
};

