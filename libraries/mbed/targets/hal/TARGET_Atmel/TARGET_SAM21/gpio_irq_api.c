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
#include <stddef.h>
#include "cmsis.h"

#include "gpio_irq_api.h"
#include "gpio_api.h"
#include "mbed_error.h"
#include "extint.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "port.h"

#define CHANNEL_NUM 16
#define pEXT_CONF(obj) (obj->config_extint_chan)
static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;
uint8_t ext_int_pins[EIC_NUMBER_OF_INTERRUPTS] = {0xFF};
uint8_t fallcount = 0;
uint8_t risecount = 0;

int get_extint_channel(PinName pin)
{
    switch (pin) {
        case PA16:
        case PB00:
        case PB16:
        case PA00:
            return 0;

        case PA17:
        case PB17:
        case PA01:
            return 1;

        case PA18:
        case PA02:
            return 2;

        case PA19:
        case PB03:
            return 3;

        case PA04:
        case PA20:
            return 4;

        case PA05:
            return 5;

        case PA06:
        case PA22:
        case PB22:
            return 6;

        case PA07:
        case PA23:
        case PB28:
            return 7;

        case PA28:
        case PB08:
            return 8;

        case PA09:
        case PB09:
            return 9;

        case PA10:
        case PA30:
            return 10;

        case PA11:
        case PA31:
            return 11;

        case PA12:
        case PA24:
            return 12;

        case PA13:
        case PA25:
            return 13;

        case PB14:
        case PB30:
        case PA14:
            return 14;

        case PA15:
        case PA27:
        case PB15:
        case PB31:
            return 15;

        default :
            return 0xFF;
    }
}

uint32_t find_peripheral_index (PinName pin, const PinMap* map)
{
    uint8_t count = 0;
    while (map->pin != NC) {
        if (map->pin == pin)
            return map[count].peripheral;
        map++;
        count++;
    }
    return (uint32_t)NC;
}

uint32_t find_pin_index (PinName pin, const PinMap* map)
{
    uint8_t count = 0;
    while (map->pin != NC) {
        if (map->pin == pin)
            return count;
        map++;
        count++;
    }
    return (uint32_t)NC;
}

void gpio_irq(void)
{
    uint32_t current_channel;
    uint32_t config_pos;
    uint32_t new_config;
    uint32_t mask;
    Eic *const EIC_module = EIC;
    uint32_t config = 0;
    gpio_irq_event event = IRQ_NONE;
    PortGroup *port_base;
    uint32_t index = 0;

    for (current_channel = 0; current_channel < EIC_NUMBER_OF_INTERRUPTS ; current_channel++) {
        if (extint_chan_is_detected(current_channel)) {
            extint_chan_clear_detected(current_channel);
            port_base = (PortGroup*)port_get_group_from_gpio_pin(ext_int_pins[current_channel]);
            mask = gpio_set(ext_int_pins[current_channel]);
            if ((port_base->IN.reg & mask) != 0) {
                event = IRQ_RISE;
                risecount++;
            } else {
                event = IRQ_FALL;
                fallcount++;
            }
            if (event != IRQ_NONE) {
                index = find_pin_index(ext_int_pins[current_channel], PinMap_EXTINT);
                irq_handler(channel_ids[current_channel], event);
            }
        }
    }
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector, ab = 0;
    irq_handler = handler;

    obj->pin = pin;
    int int_channel = 0;
    if (pin == NC)
        return -1;
    extint_chan_get_config_defaults(&pEXT_CONF(obj));
    pEXT_CONF(obj).gpio_pin           = (uint32_t)pin;
    pEXT_CONF(obj).gpio_pin_mux       = 0;   // mux setting for ext int is 0
    pEXT_CONF(obj).gpio_pin_pull      = EXTINT_PULL_NONE;
    pEXT_CONF(obj).detection_criteria = EXTINT_DETECT_BOTH;
    int_channel = get_extint_channel(pin);
    if (int_channel != 0xFF) {
        extint_chan_set_config(int_channel, &pEXT_CONF(obj));
    }
    ext_int_pins[int_channel] = pin;

    irq_n = EIC_IRQn;
    vector = (uint32_t)gpio_irq;
    NVIC_SetVector(irq_n, vector);
    NVIC_EnableIRQ(irq_n);

    obj->int_ch = find_pin_index(pin, PinMap_EXTINT);
    obj->ch = int_channel;
    channel_ids[obj->ch] = id;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    channel_ids[obj->int_ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    Eic *const eic = _extint_get_eic_from_channel(obj->ch);

    if (enable) {
        pEXT_CONF(obj).detection_criteria = EXTINT_DETECT_BOTH;
        extint_chan_set_config(obj->ch, &pEXT_CONF(obj));
        eic->INTENSET.reg = (1UL << obj->ch);
    } else {
        pEXT_CONF(obj).detection_criteria = EXTINT_DETECT_BOTH;
        extint_chan_set_config(obj->ch, &pEXT_CONF(obj));
//		channel_ids[obj->ch] = 0;
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
//	NVIC_EnableIRQ(EIC_IRQn);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
//	NVIC_DisableIRQ(EIC_IRQn);
}
