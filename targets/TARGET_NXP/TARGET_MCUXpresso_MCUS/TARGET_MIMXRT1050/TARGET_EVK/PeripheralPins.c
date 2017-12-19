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
    {GPIO_AD_B1_11, ADC1_0, 0},
    {GPIO_AD_B1_04, ADC1_9, 0},
    {NC   , NC       , 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {GPIO_AD_B1_01, I2C_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4D0 << DAISY_REG_SHIFT) | 3)},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {GPIO_AD_B1_00, I2C_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4CC << DAISY_REG_SHIFT) | 3)},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {GPIO_AD_B0_12, UART_1, 2},
    {NC  ,  NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {GPIO_AD_B0_13, UART_1, 2},
    {NC  ,  NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {GPIO_SD_B0_00, SPI_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4F0 << DAISY_REG_SHIFT) | 4)},
    {GPIO_AD_B0_00, SPI_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x510 << DAISY_REG_SHIFT) | 7)},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {GPIO_SD_B0_02, SPI_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4F8 << DAISY_REG_SHIFT) | 4)},
    {GPIO_AD_B0_01, SPI_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x518 << DAISY_REG_SHIFT) | 7)},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {GPIO_SD_B0_03, SPI_1, ((1U << DAISY_REG_VALUE_SHIFT) | (0x4F4 << DAISY_REG_SHIFT) | 4)},
    {GPIO_AD_B0_02, SPI_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x514 << DAISY_REG_SHIFT) | 7)},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {GPIO_SD_B0_01, SPI_1, ((0U << DAISY_REG_VALUE_SHIFT) | (0x4EC << DAISY_REG_SHIFT) | 4)},
    {GPIO_AD_B0_03, SPI_3, ((0U << DAISY_REG_VALUE_SHIFT) | (0x50C << DAISY_REG_SHIFT) | 7)},
    {NC   , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {GPIO_AD_B0_10, PWM_7, ((3U << DAISY_REG_VALUE_SHIFT) | (0x454 << DAISY_REG_SHIFT) | 1)},
    {GPIO_AD_B0_11, PWM_8, ((3U << DAISY_REG_VALUE_SHIFT) | (0x464 << DAISY_REG_SHIFT) | 1)},
    {NC   , NC    , 0}
};

