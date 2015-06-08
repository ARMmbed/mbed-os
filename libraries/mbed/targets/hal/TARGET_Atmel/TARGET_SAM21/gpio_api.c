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
#include "mbed_assert.h"
#include "PinNames.h"
#include "gpio_object.h"
#include "gpio_api.h"
#include "compiler.h"
#include "port.h"

void pin_mode(PinName pin, PinMode mode) { // Kept as dummy function. configuration settings already taken care in gpio_dir function
	MBED_ASSERT(pin != (PinName)NC);
}

uint32_t gpio_set(PinName pin) {
    MBED_ASSERT(pin != (PinName)NC);
    return (1UL << (pin % 32));
}

void gpio_init(gpio_t *obj, PinName pin) {
    MBED_ASSERT(pin != (PinName)NC);
    struct port_config pin_conf;
    PortGroup *const port_base = (PortGroup*)port_get_group_from_gpio_pin(pin);
    
    obj->pin = pin;
    if (pin == (PinName)NC)
    return;
    
    obj->mask = gpio_set(pin);
    port_get_config_defaults(&pin_conf);
    obj->powersave = pin_conf.powersave;
    port_pin_set_config(pin, &pin_conf);
    
    obj->OUTCLR = &port_base->OUTCLR.reg;
    obj->OUTSET = &port_base->OUTSET.reg;
    obj->IN = &port_base->IN.reg;
}

void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    MBED_ASSERT(obj->pin != (PinName)NC);
    struct port_config pin_conf;
    port_get_config_defaults(&pin_conf);
    switch (direction) {
	    case PIN_INPUT :
	    pin_conf.direction  = PORT_PIN_DIR_INPUT;
	    pin_conf.input_pull = PORT_PIN_PULL_UP;
	    break;
	    case PIN_OUTPUT:
	    pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	    break;
	    case PIN_INPUT_OUTPUT:
	    pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	    break;
    }
    port_pin_set_config(obj->pin, &pin_conf);  
}
