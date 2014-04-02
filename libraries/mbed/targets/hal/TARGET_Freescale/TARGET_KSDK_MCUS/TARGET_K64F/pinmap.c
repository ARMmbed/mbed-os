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
#include "pinmap.h"
#include "error.h"
#include "fsl_clock_manager.h"
#include "fsl_port_hal.h"

void pin_function(PinName pin, int function) {
    if (pin == (PinName)NC) {
        return;
    }

    clock_manager_set_gate(kClockModulePORT, pin >> GPIO_PORT_SHIFT, true);
    port_hal_mux_control(pin >> GPIO_PORT_SHIFT, pin & 0xFF, (port_mux_t)function);
}

void pin_mode(PinName pin, PinMode mode) {
    if (pin == (PinName)NC) {
        return;
    }

    uint32_t instance = pin >> GPIO_PORT_SHIFT;
    uint32_t pinName = pin & 0xFF;

    switch (mode) {
        case PullNone:
            port_hal_configure_pull(instance, pinName, false);
            port_hal_pull_select(instance, pinName, kPortPullDown);
            break;
        case PullDown:
            port_hal_configure_pull(instance, pinName, true);
            port_hal_pull_select(instance, pinName, kPortPullDown);
            break;
        case PullUp:
            port_hal_configure_pull(instance, pinName, true);
            port_hal_pull_select(instance, pinName, kPortPullUp);
            break;
        default:
            break;
    }
}
