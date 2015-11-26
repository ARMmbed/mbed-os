/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
 
#ifndef MBED_GPIO_OBJECT_H
#define MBED_GPIO_OBJECT_H

#include "mbed_assert.h"
#include "W7500x_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName  pin;
    uint32_t pin_index;
    uint32_t port_num;
    uint32_t direction;
    uint32_t mode;
    __IO uint32_t *reg_data_in;
} gpio_t;


extern uint32_t Get_GPIO_BaseAddress(uint32_t port_idx);


static inline void gpio_write(gpio_t *obj, int value) {
    MBED_ASSERT(obj->pin != (PinName)NC);

    uint32_t port_num = WIZ_PORT(obj->pin);
    uint32_t pin_index  = WIZ_PIN_INDEX(obj->pin);

    GPIO_TypeDef *gpio = (GPIO_TypeDef *)Get_GPIO_BaseAddress(port_num);
    

    if (value)
    {
        HAL_GPIO_SetBits(gpio, pin_index);
    }
    else
    {
        HAL_GPIO_ResetBits(gpio, pin_index);
    }
}

static inline int gpio_read(gpio_t *obj) {
    int ret;

    MBED_ASSERT(obj->pin != (PinName)NC);

    uint32_t port_num = WIZ_PORT(obj->pin);

    GPIO_TypeDef *gpio = (GPIO_TypeDef *)Get_GPIO_BaseAddress(port_num);

    if(obj->direction == PIN_OUTPUT)
    {
        ret = ( HAL_GPIO_ReadOutputData(gpio) & obj->pin_index ) ? 1 : 0;
    }
    else
    {
        ret = ((*obj->reg_data_in & obj->pin_index) ? 1 : 0);
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
