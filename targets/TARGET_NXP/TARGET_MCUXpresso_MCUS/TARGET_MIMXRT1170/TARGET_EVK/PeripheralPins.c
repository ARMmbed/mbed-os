/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
    {GPIO_AD_10, ADC1_0, NC},
    {GPIO_AD_11, ADC1_1, NC},
    {GPIO_AD_12, ADC1_2, NC},
    {GPIO_AD_13, ADC1_3, NC},
    {GPIO_AD_09, ADC1_4, NC},
    {GPIO_AD_08, ADC1_5, NC},
    {NC   , NC       , 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
   {GPIO_LPSR_04   , I2C_5   , 0},
   {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
   {GPIO_LPSR_05   , I2C_5   , 0},
   {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {GPIO_AD_24, UART_1, 0},
    {GPIO_DISP_B2_10, UART_2, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_UART_RX[] = {
    {GPIO_AD_25, UART_1, 0},
    {GPIO_DISP_B2_11, UART_2, 2},
    {NC   , NC   , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {GPIO_AD_28  , SPI_1   , 0},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {GPIO_AD_30   , SPI_1   , 0},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {GPIO_AD_31   , SPI_1   , 0},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {GPIO_AD_29  , SPI_1   , 0},
    {NC   , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {NC   , NC    , 0}
};

/************SEMC***************/
const PinMap PinMap_SEMC_DATA00[] = {
    {ONFI_FLASH_D0  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_DATA01[] = {
    {ONFI_FLASH_D1  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_DATA02[] = {
    {ONFI_FLASH_D2  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_DATA03[] = {
    {ONFI_FLASH_D3  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_DATA04[] = {
    {ONFI_FLASH_D4  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_DATA05[] = {
    {ONFI_FLASH_D5  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_DATA06[] = {
    {ONFI_FLASH_D6  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_DATA07[] = {
    {ONFI_FLASH_D7  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_ADD09[] = {
    {ONFI_FLASH_ADD09  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_ADD11[] = {
    {ONFI_FLASH_ADD11  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_ADD12[] = {
    {ONFI_FLASH_ADD12  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_BA1[] = {
    {ONFI_FLASH_BA1  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_RDY[] = {
    {ONFI_FLASH_RDY  , SEMC_0, 0},
    {NC   , NC    , 0}
};

const PinMap PinMap_SEMC_CSX00[] = {
    {ONFI_FLASH_CSX  , SEMC_0, 0},
    {NC   , NC    , 0}
};

