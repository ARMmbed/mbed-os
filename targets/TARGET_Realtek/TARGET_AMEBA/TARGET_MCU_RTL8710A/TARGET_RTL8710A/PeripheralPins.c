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

// for spi_api.c
const PinMap PinMap_SSI_MOSI[] = {
    {PC_2,  RTL_PIN_PERI(SPI0, 0, S1), RTL_PIN_FUNC(SPI0, S1)},
    {NC,    NC,     0}
};

const PinMap PinMap_SSI_MISO[] = {
    {PC_3,  RTL_PIN_PERI(SPI0, 0, S1), RTL_PIN_FUNC(SPI0, S1)},
    {NC,    NC,     0}
};

// for serial_api.c
const PinMap PinMap_UART_TX[] = {
    {PA_4,  RTL_PIN_PERI(UART2, 2, S0), RTL_PIN_FUNC(UART2, S0)},
    {PB_0,  RTL_PIN_PERI(LOG_UART, 3, S0), RTL_PIN_FUNC(LOG_UART, S0)},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {PA_0,  RTL_PIN_PERI(UART2, 2, S0), RTL_PIN_FUNC(UART2, S0)},
    {PB_1,  RTL_PIN_PERI(LOG_UART, 3, S0), RTL_PIN_FUNC(LOG_UART, S0)},
    {NC,    NC,     0}
};

// for i2c_api.c
const PinMap PinMap_I2C_SDA[] = {
    {PB_3,  RTL_PIN_PERI(I2C3, 3, S0), RTL_PIN_FUNC(I2C3, S0)},
    {NC,    NC,     0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PB_2,  RTL_PIN_PERI(I2C3, 3, S0), RTL_PIN_FUNC(I2C3, S0)}, 
    {NC,    NC,     0}
};
