/* mbed Microcontroller Library
 *
 * Copyright (C) 2019, Toshiba Electronic Device Solutions Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "txz_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t pin_num;
    uint32_t mask;
    PinName  pin;
    PortName port;
} gpio_t;

typedef enum {
    CG_INT_SRC_0 = 0U,
    CG_INT_SRC_1,
    CG_INT_SRC_2,
    CG_INT_SRC_3,
    CG_INT_SRC_4,
    CG_INT_SRC_5,
    CG_INT_SRC_6,
    CG_INT_SRC_7,
    CG_INT_SRC_8,
    CG_INT_SRC_9,
    CG_INT_SRC_A,
    CG_INT_SRC_B,
    CG_INT_SRC_C,
    CG_INT_SRC_D,
    CG_INT_SRC_E,
    CG_INT_SRC_F
} CG_INTSrc;

typedef enum {
    CG_INT_ACTIVE_STATE_L          = 0x00U,
    CG_INT_ACTIVE_STATE_H          = 0x02U,
    CG_INT_ACTIVE_STATE_FALLING    = 0x04U,
    CG_INT_ACTIVE_STATE_RISING     = 0x06U,
    CG_INT_ACTIVE_STATE_BOTH_EDGES = 0x08U,
    CG_INT_ACTIVE_STATE_INVALID    = 0x0AU
} CG_INTActiveState;

static inline int gpio_is_connected(const gpio_t *obj)
{
    return (obj->pin != (PinName)NC);
}

#ifdef __cplusplus
}
#endif

#endif
