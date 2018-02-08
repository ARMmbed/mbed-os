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
    {P0_15,  ADC0_SE3,   0},
    {P0_16,  ADC0_SE4,   0},
    {P0_23,  ADC0_SE11,  0},
    {P0_31,  ADC0_SE5,   0},
    {P1_0,   ADC0_SE6,   0},
    {P0_10,  ADC0_SE0,   0},
    {NC   ,  NC      ,   0}
};

/************CAN***************/
const PinMap PinMap_CAN_TD[] = {
    {P0_1,  CAN_1,  1},
    {NC  ,  NC   ,  0}
};

const PinMap PinMap_CAN_RD[] = {
    {P0_0, CAN_1,  1},
    {NC  , NC   ,  0}
};


/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {P0_26, I2C_2, 1},
    {P1_29, I2C_7, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {P0_27, I2C_2, 1},
    {P1_30, I2C_7, 1},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P0_30, UART_0, 1},
    {P0_27, UART_2, 1},
    {P1_30, UART_7, 1},
    {NC   ,  NC   , 0}
};

const PinMap PinMap_UART_RX[] = {
    {P0_29, UART_0, 1},
    {P0_26, UART_2, 1},
    {P1_29, UART_7, 1},
    {NC   ,  NC   , 0}
};

const PinMap PinMap_UART_CTS[] = {
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_RTS[] = {
    {NC   , NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P1_4,  SPI_0, 1},
    {P1_23, SPI_2, 1},
    {P0_6,  SPI_3, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {P1_5,  SPI_0, 1},
    {P1_24, SPI_2, 1},
    {P0_8,  SPI_3, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {P1_6,  SPI_0, 1},
    {P1_25, SPI_2, 1},
    {P0_9,  SPI_3, 1},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {P1_7,  SPI_0, 1},
    {P0_7,  SPI_3, 1},
    {NC  ,  NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {P0_17   , PWM_1,  4},
    {P0_18   , PWM_2,  4},
    {P0_19   , PWM_3,  4},
    {P0_22   , PWM_4,  4},
    {P0_28   , PWM_8,  4},
    {P0_29   , PWM_9,  4},
    {NC      , NC,     0}
};
