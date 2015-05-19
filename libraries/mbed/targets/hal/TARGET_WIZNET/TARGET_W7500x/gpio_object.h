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
#ifndef MBED_GPIO_OBJECT_H
#define MBED_GPIO_OBJECT_H

#include "mbed_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName  pin;
    uint32_t mask;
    uint32_t direction;
    __IO uint32_t *reg_data_in;
} gpio_t;


extern uint32_t Get_GPIO_BaseAddress(uint32_t port_idx);


static inline void gpio_write(gpio_t *obj, int value) {
    MBED_ASSERT(obj->pin != (PinName)NC);

    uint32_t port_index = WIZ_PORT(obj->pin);
    uint32_t pin_index  = WIZ_PIN(obj->pin);

    uint32_t gpio_add = Get_GPIO_BaseAddress(port_index);
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)gpio_add;


    if (value)
    {
        HAL_GPIO_SetBits(gpio,(0x01 << pin_index));
    }
    else
    {
        HAL_GPIO_ResetBits(gpio,(0x01 << pin_index));
    }
}

static inline int gpio_read(gpio_t *obj) {
    int ret;

    MBED_ASSERT(obj->pin != (PinName)NC);

    uint32_t port_index = WIZ_PORT(obj->pin);

    uint32_t gpio_add = Get_GPIO_BaseAddress(port_index);
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)gpio_add;

    if(obj->direction == PIN_OUTPUT)
    {
        ret = ( HAL_GPIO_ReadOutputData(gpio) & obj->mask ) ? 1 : 0;
    }
    else
    {
        ret = ((*obj->reg_data_in & obj->mask) ? 1 : 0);
    }

    return ret;
}

static inline int gpio_is_connected(const gpio_t *obj) {
    return obj->pin != (PinName)NC;
}

#ifdef __cplusplus
}
#endif

#endif
