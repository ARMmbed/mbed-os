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
    {P0_16, ADC0_SE4,  0},
    {P0_31, ADC0_SE5,  0},
    {P1_0,  ADC0_SE6,  0},
    {P2_0,  ADC0_SE7,  0},
    {NC   , NC      ,  0}
};

/************CAN***************/
const PinMap PinMap_CAN_TD[] = {
    {P3_18, CAN_0,  4},
    {P1_17, CAN_1,  5},
    {NC   , NC   ,  0}
};

const PinMap PinMap_CAN_RD[] = {
    {P3_19, CAN_0,  4},
    {P1_18, CAN_1,  5},
    {NC   , NC   ,  0}
};


/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {P0_13, I2C_0, 1},
    {P3_23, I2C_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {P0_14, I2C_0, 1},
    {P3_24, I2C_1, 1},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P0_30, UART_0, 1},
    {P3_27, UART_1, 1},
    {NC   ,  NC   , 0}
};

const PinMap PinMap_UART_RX[] = {
    {P0_29, UART_0, 1},
    {P3_26, UART_1, 1},
    {NC   ,  NC   , 0}
};

const PinMap PinMap_UART_CTS[] = {
    {P3_28, UART_1, 1},
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_RTS[] = {
    {P3_29, UART_1, 1},
    {NC   , NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P0_0,  SPI_0, 2},
    {P3_20, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {P0_3,  SPI_0, 1},
    {P3_21, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {P0_2,  SPI_0, 1},
    {P3_22, SPI_1, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {P0_1,  SPI_0, 2},
    {P3_30, SPI_1, 1},
    {P4_6,  SPI_1, 2},
    {NC  ,  NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {NC   , NC    , 0}
};
