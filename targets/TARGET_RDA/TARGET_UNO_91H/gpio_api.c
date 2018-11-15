/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include "gpio_api.h"
#include "pinmap.h"
#include "rda_ccfg_api.h"

#define GPIO_PIN_NUM    28

static const PinMap PinMap_GPIO[] = {
    {PB_0, (GPIO_0 |  0), 0},
    {PB_1, (GPIO_0 |  1), 0},
    {PB_2, (GPIO_0 |  2), 0},
    {PB_3, (GPIO_0 |  3), 0},
    {PB_4, (GPIO_0 |  4), 0},
    {PB_5, (GPIO_0 |  5), 0},
    {PB_6, (GPIO_0 |  6), 0},
    {PB_7, (GPIO_0 |  7), 0},
    {PB_8, (GPIO_0 |  8), 0},
    {PB_9, (GPIO_0 |  9), 0},
    {PA_8, (GPIO_0 | 10), 0},
    {PA_9, (GPIO_0 | 11), 0},
    {PC_0, (GPIO_0 | 12), 1},
    {PC_1, (GPIO_0 | 13), 1},
    {PC_2, (GPIO_0 | 14), 0},
    {PC_3, (GPIO_0 | 15), 0},
    {PC_4, (GPIO_0 | 16), 0},
    {PC_5, (GPIO_0 | 17), 0},
    {PC_6, (GPIO_0 | 18), 0},
    {PC_7, (GPIO_0 | 19), 0},
    {PC_8, (GPIO_0 | 20), 0},
    {PC_9, (GPIO_0 | 21), 0},
    {PD_0, (GPIO_0 | 22), 0},
    {PD_1, (GPIO_0 | 23), 0},
    {PD_2, (GPIO_0 | 24), 0},
    {PD_3, (GPIO_0 | 25), 0},
    {PA_0, (GPIO_0 | 26), 1},
    {PA_1, (GPIO_0 | 27), 1},
    {PA_2, (GPIO_0 | 14), 1},
    {PA_3, (GPIO_0 | 15), 1},
    {PA_4, (GPIO_0 | 16), 1},
    {PA_5, (GPIO_0 | 17), 1},
    {PA_6, (GPIO_0 | 18), 1},
    {PA_7, (GPIO_0 | 19), 1},
    {NC,   NC,            0}
};

#define PER_BITBAND_ADDR(reg, bit)      (uint32_t *)(RDA_PERBTBND_BASE + (((uint32_t)(reg)-RDA_PER_BASE)<<5U) + (((uint32_t)(bit))<<2U))

PinName gpio_pinname(int pin_n)
{
    MBED_ASSERT(pin_n < GPIO_PIN_NUM);
    return PinMap_GPIO[pin_n].pin;
}

uint32_t gpio_set(PinName pin)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t func = 0;
    uint32_t idx  = 0;

    func = pinmap_function(pin, PinMap_GPIO);
    idx  = pinmap_peripheral(pin, PinMap_GPIO) & 0x001F;
    pin_function(pin, func);

    return idx;
}

void gpio_init(gpio_t *obj, PinName pin)
{
    uint32_t gpio_idx   = 0;

    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    gpio_idx = gpio_set(pin);

    if ((6U <= gpio_idx) && (9U >= gpio_idx)) {
        rda_ccfg_gp((unsigned char)gpio_idx, 0x01U);
    }

    obj->reg_out = PER_BITBAND_ADDR(&RDA_GPIO->DOUT, gpio_idx);
    obj->reg_in  = PER_BITBAND_ADDR(&RDA_GPIO->DIN, gpio_idx);
    obj->reg_dir = PER_BITBAND_ADDR(&RDA_GPIO->DIR, gpio_idx);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    uint32_t dir = 0x00UL;
    MBED_ASSERT(obj->pin != (PinName)NC);
    if (PIN_INPUT == direction) {
        dir = 0x01UL;
    }
    if (rda_ccfg_hwver() >= 5) {
        uint32_t gpio_idx  = pinmap_peripheral(obj->pin, PinMap_GPIO) & 0x001F;
        /* Since U05, for gpio 2/3/8/14/15/16/17/20/21, 1'b1 means output */
        if (0x00UL != (0x0033C10CUL & (0x01UL << gpio_idx))) {
            dir ^= 0x01UL;
        }
    }
    *obj->reg_dir = dir;
}
