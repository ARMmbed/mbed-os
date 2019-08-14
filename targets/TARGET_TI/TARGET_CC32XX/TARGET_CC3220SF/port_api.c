/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#include "port_api.h"
#include "pinmap.h"
#include "gpio_api.h"
#include "PeripheralPins.h"

#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/driverlib/pin.h"
#include "ti/devices/cc32xx/driverlib/gpio.h"
#include "ti/devices/cc32xx/inc/hw_ints.h"
#include "ti/devices/cc32xx/driverlib/prcm.h"

#define NUM_PORTS            4
#define NUM_PINS_PER_PORT    8
#define PORT_MASK            0x3

static const uint8_t pinTable[] = {
    /* 00     01      02      03      04      05      06      07  */
    PIN_50, PIN_55, PIN_57, PIN_58, PIN_59, PIN_60, PIN_61, PIN_62,
    /* 08     09      10      11      12      13      14      15  */
    PIN_63, PIN_64, PIN_01, PIN_02, PIN_03, PIN_04, PIN_05, PIN_06,
    /* 16     17      18      19      20      21      22      23  */
    PIN_07, PIN_08, PIN_XX, PIN_XX, PIN_XX, PIN_XX, PIN_15, PIN_16,
    /* 24     25      26      27      28      29      30      31  */
    PIN_17, PIN_21, PIN_29, PIN_30, PIN_18, PIN_20, PIN_53, PIN_45,
    /* 32 */
    PIN_52
};

const uint16_t PortPinTypes[] = {
    PIN_TYPE_STD,       /* GPIO_CFG_IN_NOPULL */
    PIN_TYPE_STD_PU,    /* GPIO_CFG_IN_PU */
    PIN_TYPE_STD_PD,    /* GPIO_CFG_IN_PD */
    PIN_TYPE_OD,        /* GPIO_CFG_OUT_OD_NOPULL */
    PIN_TYPE_OD_PU,     /* GPIO_CFG_OUT_OD_PU */
    PIN_TYPE_OD_PD,     /* GPIO_CFG_OUT_OD_PD */
    (uint16_t)PIN_TYPE_ANALOG // Revisit this, PIN_TYPE_ANALOG gets truncated to 16b
};

PinName port_pin(PortName port, int pin_n)
{
    int gpio_num = (port * 8) + pin_n;
    PinName pin = (PinName)pinTable[gpio_num];
    return pin;
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    obj->port = port;
    obj->mask = mask;

    switch (port) {
        case Port0:
            obj->baseAddr = CC3220SF_GPIOA0_BASE;
            obj->peripheralId = PRCM_GPIOA0;
            break;
        case Port1:
            obj->baseAddr = CC3220SF_GPIOA1_BASE;
            obj->peripheralId = PRCM_GPIOA1;
            break;
        case Port2:
            obj->baseAddr = CC3220SF_GPIOA2_BASE;
            obj->peripheralId = PRCM_GPIOA2;
            break;
        case Port3:
            obj->baseAddr = CC3220SF_GPIOA3_BASE;
            obj->peripheralId = PRCM_GPIOA3;
            break;
    }

    // initialize GPIO PORT clock
    PRCMPeripheralClkEnable(obj->peripheralId, PRCM_RUN_MODE_CLK | PRCM_SLP_MODE_CLK);
    // wait for GPIO clock to settle
    while (!PRCMPeripheralStatusGet(obj->peripheralId));

    for (int i = 0; i < 8; i++) {
        if (obj->mask & (1 << i)) {
            PinName pin = port_pin(obj->port, i);
            PinModeSet(pin, PIN_MODE_0);
            pin_mode(pin, PullNone);
        }
    }

    port_dir(obj, dir);
}

void port_mode(port_t *obj, PinMode mode)
{
    for (int i = 0; i < 8; i++) {
        if (obj->mask & (1 << i)) {
            pin_mode(port_pin(obj->port, i), mode);
        }
    }
}

void port_dir(port_t *obj, PinDirection dir)
{
    switch (dir) {
        case PIN_INPUT:
            GPIODirModeSet(obj->baseAddr, obj->mask, GPIO_DIR_MODE_IN);
            break;
        case PIN_OUTPUT:
            GPIODirModeSet(obj->baseAddr, obj->mask, GPIO_DIR_MODE_OUT);
            break;
    }
}

void port_write(port_t *obj, int value)
{
    GPIOPinWrite(obj->baseAddr, obj->mask, value);
}

int port_read(port_t *obj)
{
    return (int)(GPIOPinRead(obj->baseAddr, obj->mask));
}
