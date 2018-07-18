/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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

#define GPIO_BIT_VALUE_1                ((uint8_t)0x01)
#define GPIO_BIT_VALUE_0                ((uint8_t)0x00)
#define GPIO_BIT_ALL                    ((uint8_t)0xFF)

typedef enum {
    cg_int_src_0 = 0U,
    cg_int_src_1,
    cg_int_src_2,
    cg_int_src_3,
    cg_int_src_4,
    cg_int_src_5,
    cg_int_src_6,
    cg_int_src_7,
    cg_int_src_8,
    cg_int_src_9,
    cg_int_src_a,
    cg_int_src_b,
    cg_int_src_c,
    cg_int_src_d,
    cg_int_src_e,
    cg_int_src_f
} cg_intsrc;

typedef enum {
    cg_int_active_state_l          = 0x00U,
    cg_int_active_state_h          = 0x02U,
    cg_int_active_state_falling    = 0x04U,
    cg_int_active_state_rising     = 0x06U,
    cg_int_active_state_both_edges = 0x08U,
    cg_int_active_state_invalid    = 0x0AU
} cg_intactivestate;

typedef struct {
    PinName  pin;
    uint32_t mask;
    PortName port;
} gpio_t;

struct gpio_irq_s {
    uint32_t          mask;
    PortName          port;
    PinName           pin;
    uint32_t          irq_id;
    cg_intactivestate event;
    cg_intsrc         irq_src;
};

static inline int gpio_is_connected(const gpio_t *obj)
{
    return (obj->pin != (PinName)NC);
}

#ifdef __cplusplus
}
#endif

#endif
