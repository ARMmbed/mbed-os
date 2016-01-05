/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include <stddef.h>

#include "cmsis.h"
#include "mbed_assert.h"
#include "compiler.h"

#include "pinmux.h"
#include "pinmap.h"

extern uint8_t g_sys_init;


static inline void pinmux_get_current_config(PinName pin, struct system_pinmux_config *const config)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t pin_index = pin % 32;
    uint32_t pin_mask = (uint32_t)(1UL << pin_index);

    PortGroup *const port = system_pinmux_get_group_from_gpio_pin(pin);
    MBED_ASSERT(port);

    config->mux_position = system_pinmux_pin_get_mux_position(pin);

    if (port->PINCFG[pin_index].reg & PORT_PINCFG_INEN) {
        if (port->DIR.reg & pin_mask) {
            config->direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT_WITH_READBACK;
            config->input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
        } else {
            config->direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
            if (port->PINCFG[pin_index].reg & PORT_PINCFG_PULLEN) {
                if (port->OUT.reg & pin_mask) {
                    config->input_pull = SYSTEM_PINMUX_PIN_PULL_UP;
                } else {
                    config->input_pull = SYSTEM_PINMUX_PIN_PULL_DOWN;
                }
            } else {
                config->input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
            }
        }
    } else {
        config->input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
        config->direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
    }

    /* Not relevant for now */
    config->powersave = false;
}

/** Change the MUX padding of input pin
 *
 * Configure the pin for specific module
 * @param[in]  pin      Pin name whose MUX padding is to be changed
 * @param[in]  function The MUX mode to be selected
 * @return              void
 */
void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);

    struct system_pinmux_config pin_conf;

    pinmux_get_current_config(pin, &pin_conf);
    pin_conf.mux_position = function;

    system_pinmux_pin_set_config(pin, &pin_conf);
}

/** Change the pin pull mode
 *
 * Configure the pin pull mode
 * @param[in]  pin      Pin name whose MUX padding is to be changed
 * @param[in]  mode     Pin pull mode to be set
 * @return              void
 */
void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);

    struct system_pinmux_config pin_conf;

    pinmux_get_current_config(pin, &pin_conf);
    if (mode == PullUp) {
        pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_UP;
    } else if (mode == PullDown) {
        pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_DOWN;
    } else {
        pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
    }

    system_pinmux_pin_set_config(pin, &pin_conf);
}
