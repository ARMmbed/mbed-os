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

/*
    The ADuCM4050 is made in two package variants.

    64 lead LFCSP & 72 ball WLCSP

    There are some differences for Port 2 between the two variants
    WLCSP also has Port 3.

    The #define ADUCM4050_LFCSP is used to determine which variant the code
    is built for.

    For LFCSP leave the #define in, to build for ADUCM4050_WLCSP remove.
*/
#define ADUCM4050_LFCSP

#include "PeripheralPins.h"

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P0_10, UART_0, 1},
    {P1_15, UART_1, 2},
    {NC,  NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {P0_11, UART_0, 1},
    {P2_00, UART_1, 2},
    {NC,  NC, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P0_00, SPI_0, 1},
    {P1_06, SPI_1, 1},
    {P1_02, SPI_2, 1},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {P0_01, SPI_0, 1},
    {P1_07, SPI_1, 1},
    {P1_03, SPI_2, 1},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {P0_02, SPI_0, 1},
    {P1_08, SPI_1, 1},
    {P1_04, SPI_2, 1},
    {NC, NC, 0}
};

#if defined(ADUCM4050_LFCSP)
const PinMap PinMap_SPI_SSEL[] = {
    {P0_03, SPI_0, 1},
    {P1_09, SPI_1, 1},
    {P2_10, SPI_2, 1},
    {NC, NC, 0}
};
#else
const PinMap PinMap_SPI_SSEL[] = {
    {P0_03, SPI_0, 1},
    {P1_09, SPI_1, 1},
    {P2_15, SPI_2, 1},
    {NC, NC, 0}
};
#endif

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {P0_05, I2C_0, 1},
    {NC, NC, 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {P0_04, I2C_0, 1},
    {NC, NC, 0}
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {P2_03, ADC0_VIN0, 1},
    {P2_04, ADC0_VIN1, 1},
    {P2_05, ADC0_VIN2, 1},
    {P2_06, ADC0_VIN3, 1},
    {P2_07, ADC0_VIN4, 1},
    {P2_08, ADC0_VIN5, 1},
    {P2_09, ADC0_VIN6, 1},
#ifdef ADUCM4050_LFCSP
    {P2_10, ADC0_VIN7, 1},
#endif
    {NC, NC, 0}
};

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};
