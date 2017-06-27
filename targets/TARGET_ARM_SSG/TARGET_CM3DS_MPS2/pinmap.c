/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#include "mbed_error.h"

#define GET_GPIO_PIN_POS(pin)  (pin & 0x0F)  /* pin % 16 */
#define GET_GPIO_MAP_NUM(pin)  (pin >> 4)    /* pin / 16 */
#define GPIO_NUM               4

static CMSDK_GPIO_TypeDef* GPIO_MAP[GPIO_NUM] = {
    CMSDK_GPIO0,
    CMSDK_GPIO1,
    CMSDK_GPIO2,
    CMSDK_GPIO3
};

void pin_function(PinName pin, int function)
{
    CMSDK_GPIO_TypeDef* p_gpio_map = 0;

    MBED_ASSERT(pin != (PinName)NC);

    if (pin >= EXP0 && pin <= EXP51) {
        if (function == ALTERNATE_FUNC) {
            p_gpio_map = GPIO_MAP[GET_GPIO_MAP_NUM(pin)];
            p_gpio_map->ALTFUNCSET = (1 << GET_GPIO_PIN_POS(pin));
        } else if(function == GPIO_FUNC) {
            p_gpio_map = GPIO_MAP[GET_GPIO_MAP_NUM(pin)];
            p_gpio_map->ALTFUNCCLR = (1 << GET_GPIO_PIN_POS(pin));
        } else {
            error("Invalid pin_function value %d", function);
        }
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);

    /* Pin modes configuration is not supported */
}
