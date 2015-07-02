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

#define CHANNEL_NUM 15
static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler;

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

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    struct extint_chan_conf config_extint_chan;
    int int_channel = 0;
    if (pin == NC)
        return -1;

    irq_handler = handler;

    extint_chan_get_config_defaults(&config_extint_chan);
    config_extint_chan.gpio_pin           = (uint32_t)pin;
    config_extint_chan.gpio_pin_mux       = 0;   // mux setting for ext int is 0
    config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
    config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
    int_channel = get_extint_channel(pin);
    if (int_channel != 0xFF) {
        extint_chan_set_config(int_channel, &config_extint_chan);
    }
}

void gpio_irq_free(gpio_irq_t *obj)
{
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{

}

void gpio_irq_enable(gpio_irq_t *obj)
{
}

void gpio_irq_disable(gpio_irq_t *obj)
{
}
