/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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
    CG_INT_SRC_01 = 0U,
    CG_INT_SRC_02,
    CG_INT_SRC_03,
    CG_INT_SRC_04,
    CG_INT_SRC_05,
    CG_INT_SRC_06,
    CG_INT_SRC_07,
    CG_INT_SRC_08,
    CG_INT_SRC_09,
    CG_INT_SRC_0A,
    CG_INT_SRC_0B,
    CG_INT_SRC_0C,
    CG_INT_SRC_0D,
    CG_INT_SRC_0E,
    CG_INT_SRC_0F,
    CG_INT_SRC_10,
    CG_INT_SRC_11,
    CG_INT_SRC_12,
    CG_INT_SRC_13,
    CG_INT_SRC_14,
    CG_INT_SRC_15,
    CG_INT_SRC_16,
    CG_INT_SRC_17,
    CG_INT_SRC_18,
    CG_INT_SRC_19,
    CG_INT_SRC_1A,
    CG_INT_SRC_1B,
    CG_INT_SRC_1C,
    CG_INT_SRC_1D,
    CG_INT_SRC_1E,
    CG_INT_SRC_1F,
    CG_INT_SRC_20
} cg_intsrc;

typedef enum {
    CG_INT_ACTIVE_STATE_L = 0x00U,
    CG_INT_ACTIVE_STATE_H = 0x02U,
    CG_INT_ACTIVE_STATE_FALLING = 0x04U,
    CG_INT_ACTIVE_STATE_RISING = 0x06U,
    CG_INT_ACTIVE_STATE_BOTH_EDGES = 0x08U,
    CG_INT_ACTIVE_STATE_INVALID = 0x0AU
} cg_intactivestate;

typedef struct {
    PinName pin;
    uint32_t mask;
    PortName port;
} gpio_t;

struct gpio_irq_s {
    uint32_t mask;
    PortName port;
    PinName pin;
    uint32_t irq_id;
    cg_intactivestate event;
    cg_intsrc irq_src;
};

static inline int gpio_is_connected(const gpio_t *obj)
{
    return obj->pin != (PinName)NC;
}

#ifdef __cplusplus
}
#endif

#endif
