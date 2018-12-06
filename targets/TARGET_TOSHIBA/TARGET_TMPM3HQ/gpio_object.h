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
    cg_int_src_01 = 0U,
    cg_int_src_02,
    cg_int_src_03,
    cg_int_src_04,
    cg_int_src_05,
    cg_int_src_06,
    cg_int_src_07,
    cg_int_src_08,
    cg_int_src_09,
    cg_int_src_0a,
    cg_int_src_0b,
    cg_int_src_0c,
    cg_int_src_0d,
    cg_int_src_0e,
    cg_int_src_0f,
    cg_int_src_10,
    cg_int_src_11,
    cg_int_src_12,
    cg_int_src_13,
    cg_int_src_14,
    cg_int_src_15,
    cg_int_src_16,
    cg_int_src_17,
    cg_int_src_18,
    cg_int_src_19,
    cg_int_src_1a,
    cg_int_src_1b,
    cg_int_src_1c,
    cg_int_src_1d,
    cg_int_src_1e,
    cg_int_src_1f,
    cg_int_src_20
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
