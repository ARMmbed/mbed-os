/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H
#include "cmsis.h"
#include "PeripheralPins.h"

const PinMap PinMap_empty[] = {
    { NC, NC, 0 }
};

#if DEVICE_SERIAL
//*** SERIAL ***
const PinMap PinMap_UART_TX[] = {
    { UART0_TX, BP_UART0_BASE, BP6A_PIN_DATA(ALT0, PIN_OUTPUT, PullNone, 0) },
    { UART1_TX, BP_UART1_BASE, BP6A_PIN_DATA(ALT0, PIN_OUTPUT, PullNone, 1) },
    { UART2_TX, BP_UART2_BASE, BP6A_PIN_DATA(ALT0, PIN_OUTPUT, PullNone, 2) },
    { NC, NC, 0 }
};

const PinMap PinMap_UART_RX[] = {
    { UART0_RX, BP_UART0_BASE, BP6A_PIN_DATA(ALT0, PIN_INPUT, PullNone, 0) },
    { UART1_RX, BP_UART1_BASE, BP6A_PIN_DATA(ALT0, PIN_INPUT, PullNone, 1) },
    { UART2_RX, BP_UART2_BASE, BP6A_PIN_DATA(ALT0, PIN_INPUT, PullNone, 2) },
    { NC, NC, 0 }
};

const PinMap PinMap_UART_CTS[] = {
    { UART0_CTS, BP_UART0_BASE, BP6A_PIN_DATA(ALT0, PIN_INPUT, PullNone, 0)},
    { UART1_CTS, BP_UART1_BASE, BP6A_PIN_DATA(ALT0, PIN_INPUT, PullNone, 1)},
    { UART2_CTS, BP_UART2_BASE, BP6A_PIN_DATA(ALT0, PIN_INPUT, PullNone, 2)},
    {NC, NC, 0}
};

const PinMap PinMap_UART_RTS[] = {
    { UART0_RTS, BP_UART0_BASE, BP6A_PIN_DATA(ALT0, PIN_OUTPUT, PullNone, 0)},
    { UART1_RTS, BP_UART1_BASE, BP6A_PIN_DATA(ALT0, PIN_OUTPUT, PullNone, 1)},
    { UART2_RTS, BP_UART2_BASE, BP6A_PIN_DATA(ALT0, PIN_OUTPUT, PullNone, 2)},
    {NC, NC, 0}
};
#endif


#if DEVICE_SPI
//*** SPI ***
const PinMap PinMap_SPI_SCLK[] = {
    { SPI0_SCLK, BP_SPI0_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 0) },
    { SPI1_SCLK, BP_SPI1_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 1) },
    { SPI2_SCLK, BP_SPI2_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 2) },
    { SPI3_SCLK, BP_SPI3_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 3) },
    { SPI4_SCLK, BP_SPI4_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 4) },

    { NC, NC, 0 }
};

const PinMap PinMap_SPI_SSEL[] = {
    { SPI0_SSEL, BP_SPI0_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 0) },
    { SPI1_SSEL, BP_SPI1_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 1) },
    { SPI2_SSEL, BP_SPI2_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 2) },
    { SPI3_SSEL, BP_SPI3_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 3) },
    { SPI4_SSEL, BP_SPI4_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 4) },
    { NC, NC, 0 }
};

const PinMap PinMap_SPI_MOSI[] = {
    { SPI0_MOSI, BP_SPI0_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 0) },
    { SPI1_MOSI, BP_SPI1_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 1) },
    { SPI2_MOSI, BP_SPI2_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 2) },
    { SPI3_MOSI, BP_SPI3_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 3) },
    { SPI4_MOSI, BP_SPI4_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 4) },
    { NC, NC, 0 }
};

const PinMap PinMap_SPI_MISO[] = {
    { SPI0_MISO, BP_SPI0_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 0) },
    { SPI1_MISO, BP_SPI1_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 1) },
    { SPI2_MISO, BP_SPI2_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 2) },
    { SPI3_MISO, BP_SPI3_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 3) },
    { SPI4_MISO, BP_SPI4_BASE, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullNone, 4) },
    { NC, NC, 0 }
};
#endif
//*** I2C ***
#if DEVICE_I2C
const PinMap PinMap_I2C_SDA[] = {
    { I2C0_SDA, 0, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 0) },
    { I2C1_SDA, 1, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 1) },
    { I2C2_SDA, 2, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 2) },
    { I2C3_SDA, 3, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 3) },
    { I2C4_SDA, 4, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 4) },
    { NC, NC, 0 }
};

const PinMap PinMap_I2C_SCL[] = {
    { I2C0_SCL, 0, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 0) },
    { I2C1_SCL, 1, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 1) },
    { I2C2_SCL, 2, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 2) },
    { I2C3_SCL, 3, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 3) },
    { I2C4_SCL, 4, BP6A_PIN_DATA(ALT0, (PIN_INPUT | PIN_OUTPUT), PullUp_2200, 4) },
    { NC, NC, 0 }
};
#endif

#if DEVICE_ANALOGIN
static const PinMap PinMap_ADC[] = {
    {AN0, ADC_0, GPIO_FUNC1},
    {AN1, ADC_1, GPIO_FUNC1},
    {AN2, ADC_2, GPIO_FUNC1},
    {NC,   NC, 0}
};

#endif

#endif
