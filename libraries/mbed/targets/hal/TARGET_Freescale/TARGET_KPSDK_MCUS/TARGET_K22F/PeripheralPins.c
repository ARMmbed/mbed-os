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
    {PTA2, UART_0, 2},
    {PTE0, UART_1, 3},
    {PTD3, UART_2, 3},
    {NC  ,  NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {PTA1, UART_0, 2},
    {PTE1, UART_1, 3},
    {PTD2, UART_2, 3},
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
    {PTA0 , PWM_05, 3},
    {PTA1 , PWM_06, 3},
    {PTA2 , PWM_07, 3},
    {PTA3 , PWM_00, 3},
    {PTA4 , PWM_01, 3},
    {PTA5 , PWM_02, 3},
    {PTA10, PWM_20, 3},
    {PTA11, PWM_21, 3},
    {PTA12, PWM_10, 3},
    {PTA13, PWM_11, 3},
    
    {PTB0 , PWM_10, 3},
    {PTB1 , PWM_11, 3},
    {PTB18, PWM_20, 3},
    {PTB19, PWM_21, 3},
    
    {PTC1 , PWM_00, 4},
    {PTC2 , PWM_01, 4},
    {PTC3 , PWM_02, 4},
    {PTC4 , PWM_03, 4},
    {PTC5 , PWM_02, 7},
    
    {PTD0 , PWM_30, 4},
    {PTD1 , PWM_31, 4},
    {PTD2 , PWM_32, 4},
    {PTD3 , PWM_33, 4},
    {PTD4 , PWM_04, 4},
    {PTD5 , PWM_05, 4},
    {PTD6 , PWM_06, 4},
    {PTD7 , PWM_07, 4},
    
    {PTE5 , PWM_30, 6},
    {PTE6 , PWM_31, 6},
    {NC   ,  NC   , 0}
};
