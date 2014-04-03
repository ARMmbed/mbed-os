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
#include "gpio_api.h"
#include "pinmap.h"
#include "fsl_port_hal.h"
#include "fsl_gpio_hal.h"
#include "fsl_sim_hal.h"

uint32_t gpio_set(PinName pin) {
    uint32_t pin_num = pin & 0xFF;

    pin_function(pin, (int)kPortMuxAsGpio);
    return 1 << pin_num;
}

void gpio_init(gpio_t *obj, PinName pin) {
    if (pin == NC) {
        return;
    }

    obj->pinName = pin;
    uint32_t port = pin >> GPIO_PORT_SHIFT;
    uint32_t pin_num = pin & 0xFF;
    clock_hal_set_gate(kSimClockModulePORT, port, true);
    port_hal_mux_control(port, pin_num, kPortMuxAsGpio);
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pinName, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    uint32_t port = obj->pinName >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pinName & 0xFF;

    switch (direction) {
        case PIN_INPUT:
            gpio_hal_set_pin_direction(port, pin_num, kGpioDigitalInput);
            break;
        case PIN_OUTPUT:
            gpio_hal_set_pin_direction(port, pin_num, kGpioDigitalOutput);
            break;
    }
}
