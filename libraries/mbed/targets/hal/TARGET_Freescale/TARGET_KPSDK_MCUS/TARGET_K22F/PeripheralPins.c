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
    
    {NC   , NC       , 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {

    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
   
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
   
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PTE0, UART_1, 3},
    {NC  ,  NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {PTE1, UART_1, 3},
    {NC  ,  NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {NC   , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {

    {NC   , NC    , 0}
};
