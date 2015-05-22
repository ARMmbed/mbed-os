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
#include "PeripheralNames.h"
#include "pinmap.h"


//*** ADC ***
const PinMap PinMap_ADC[] = {
    {PC_15, ADC_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF3)}, // ADC0_IN0
    {PC_14, ADC_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF3)}, // ADC0_IN1
    {PC_13, ADC_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF3)}, // ADC0_IN2
    {PC_12, ADC_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF3)}, // ADC0_IN3
    {PC_11, ADC_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF3)}, // ADC0_IN4
    {PC_10, ADC_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF3)}, // ADC0_IN5
    {NC,   NC,    0}
};


//*** SERIAL ***
const PinMap PinMap_UART_TX[] = {
    {PA_14, UART_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF0)},
    {PC_2,  UART_1, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF0)},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {PA_13, UART_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF0)},
    {PC_3,  UART_1, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_PULLUP, Px_AFSR_AF0)},
    {NC,    NC,     0}
};

//*** I2C ***
const PinMap PinMap_I2C_SDA[] = {
    {PA_10, I2C_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_NOPULL, Px_AFSR_AF0)},
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PA_9,  I2C_0, WIZ_PIN_DATA(WIZ_MODE_AF, WIZ_GPIO_NOPULL, Px_AFSR_AF0)},
    {NC,    NC,    0}
};

//*** SPI ***

const PinMap PinMap_SPI_SCLK[] = {
    {PA_6 , SPI_0, 0},
    {PB_1 , SPI_1, 0},
    {PC_12, SPI_0, 3},
    {PA_12, SPI_1, 3},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PA_8 , SPI_0, 0},
    {PB_3 , SPI_1, 0},
    {PC_10, SPI_0, 3},
    {PA_14, SPI_1, 3},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PA_7 , SPI_0, 0},
    {PB_2 , SPI_1, 0},
    {PC_11, SPI_0, 3},
    {PA_13, SPI_1, 3},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PA_5 , SPI_0, 0},
    {PB_0 , SPI_1, 0},
    {PC_13, SPI_0, 3},
    {PA_11, SPI_1, 3},
    {NC   , NC   , 0}
};

