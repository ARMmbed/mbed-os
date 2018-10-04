/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "mbed_assert.h"
#include "pinmap.h"
#include "gpio_include.h"

void pin_function(PinName pin, int function)
{
    int      port      = 0;
    uint8_t  bit       = 0;
    uint8_t  func      = 0;
    uint8_t  dir       = 0;
    uint32_t port_base = 0;
    uint32_t mode_base = 0;

    // Assert that pin is valid
    MBED_ASSERT(pin != NC);

    // Calculate pin function and pin direction
    func = PIN_FUNC(function);
    dir  = PIN_DIR(function);

    // Calculate port and pin position
    port = PIN_PORT(pin);
    bit  = PIN_POS(pin);

    port_base = BITBAND_PORT_BASE(port);
    // Initialization PxFR OFF
    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR1);
    BITBAND_PORT_CLR(mode_base, bit);
    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR2);
    BITBAND_PORT_CLR(mode_base, bit);
    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR3);
    BITBAND_PORT_CLR(mode_base, bit);
    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR4);
    BITBAND_PORT_CLR(mode_base, bit);
    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR5);
    BITBAND_PORT_CLR(mode_base, bit);
    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR6);
    BITBAND_PORT_CLR(mode_base, bit);

    // Initialize Input
    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
    BITBAND_PORT_CLR(mode_base, bit);
    mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
    BITBAND_PORT_SET(mode_base, bit);

    switch (func) {
        case 0:
            break;
        case 1:
            mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR1);
            break;
        case 2:
            mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR2);
            break;
        case 3:
            mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR3);
            break;
        case 4:
            mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR4);
            break;
        case 5:
            mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR5);
            break;
        case 6:
            mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_FR6);
            break;
        default:
            break;
    }

    if (func != 0) {
        BITBAND_PORT_SET(mode_base, bit);
    }
    if (dir == PIN_OUTPUT) {
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
        BITBAND_PORT_CLR(mode_base, bit);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
        BITBAND_PORT_SET(mode_base, bit);
    } else if (dir == PIN_INOUT) {
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_IE);
        BITBAND_PORT_SET(mode_base, bit);
        mode_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_CR);
        BITBAND_PORT_SET(mode_base, bit);
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    int     port = 0;
    uint8_t bit  = 0;
    uint8_t val  = 0;

    // Assert that pin is valid
    MBED_ASSERT(pin != NC);

    // Check if function is in range
    if (mode > OpenDrain) {
        return;
    }

    // Calculate port and pin position
    port = PIN_PORT(pin);
    bit  = PIN_POS(pin);
    val  = (1 << bit);

    switch (port) {
        case PortA:
            if (mode == OpenDrain)   TSB_PA->OD  |= val;
            else if (mode == PullUp) TSB_PA->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PA->PDN |= val;
            break;
        case PortB:
            if (mode == OpenDrain)   TSB_PB->OD  |= val;
            else if (mode == PullUp) TSB_PB->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PB->PDN |= val;
            break;
        case PortC:
            if (mode == OpenDrain)   TSB_PC->OD  |= val;
            else if (mode == PullUp) TSB_PC->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PC->PDN |= val;
            break;
        case PortD:
            if (mode == OpenDrain)   TSB_PD->OD  |= val;
            else if (mode == PullUp) TSB_PD->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PD->PDN |= val;
            break;
        case PortE:
            if (mode == OpenDrain)   TSB_PE->OD  |= val;
            else if (mode == PullUp) TSB_PE->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PE->PDN |= val;
            break;
        case PortF:
            if (mode == OpenDrain)   TSB_PF->OD  |= val;
            else if (mode == PullUp) TSB_PF->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PF->PDN |= val;
            break;
        case PortG:
            if (mode == OpenDrain)   TSB_PG->OD  |= val;
            else if (mode == PullUp) TSB_PG->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PG->PDN |= val;
            break;
        case PortH:
            if (mode == PullDown) TSB_PH->PDN |= val;
            break;
        case PortJ:
            if (mode == OpenDrain)  TSB_PJ->OD   |= val;
            else if (mode == PullUp) TSB_PJ->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PJ->PDN |= val;
            break;
        case PortK:
            if (mode == OpenDrain)  TSB_PK->OD   |= val;
            else if (mode == PullUp) TSB_PK->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PK->PDN |= val;
            break;
        case PortL:
            if (mode == OpenDrain)  TSB_PL->OD   |= val;
            else if (mode == PullUp) TSB_PL->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PL->PDN |= val;
            break;
        case PortM:
            if (mode == OpenDrain)  TSB_PM->OD   |= val;
            else if (mode == PullUp) TSB_PM->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PM->PDN |= val;
            break;
        case PortN:
            if (mode == OpenDrain)  TSB_PN->OD   |= val;
            else if (mode == PullUp) TSB_PN->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PN->PDN |= val;
            break;
        case PortP:
            if (mode == OpenDrain)  TSB_PP->OD   |= val;
            else if (mode == PullUp) TSB_PP->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PP->PDN |= val;
            break;
        case PortR:
            if (mode == OpenDrain)  TSB_PR->OD   |= val;
            else if (mode == PullUp) TSB_PR->PUP |= val;
            else if (mode == PullDown || mode == PullDefault) TSB_PR->PDN |= val;
            break;
        default:
            break;
    }
}
