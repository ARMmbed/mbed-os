/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S1SBP6A_GPIO_H
#define __S1SBP6A_GPIO_H

#define GPIO_BASE               (BP_AHB_BASE)
#define GPIO_PORT_BASE(c)       (GPIO_BASE + (c << 12))

#define GPIO_DATA_OFFSET                                0x0000
#define GPIO_DATAOUT_OFFSET                             0x0004
#define GPIO_OUTENSET_OFFSET                            0x0010
#define GPIO_OUTENCLR_OFFSET                            0x0014
#define GPIO_ALTFUNCSET_OFFSET                          0x0018
#define GPIO_ALTFUNCCLR_OFFSET                          0x001C
#define GPIO_INTENSET_OFFSET                            0x0020
#define GPIO_INTENCLR_OFFSET                            0x0024
#define GPIO_INTTYPESET_OFFSET                          0x0028
#define GPIO_INTTYPECLR_OFFSET                          0x002C
#define GPIO_INTPOLSET_OFFSET                           0x0030
#define GPIO_INTPOLCLR_OFFSET                           0x0034
#define GPIO_INTSTAT_OFFSET                             0x0038
#define GPIO_INTCLR_OFFSET                              0x0038
#define GPIO_ALTFUNCSEL_OFFSET                          0x003c

typedef enum {
    GPIO_PORT_0 = 0,
    GPIO_PORT_1,
    GPIO_PORT_2,
    GPIO_PORT_MAX
} gpio_port_e;

typedef enum {
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
    GPIO_PIN_MAX
} gpio_pin_e;

typedef enum {
    GPIO_MODE_INPUT = 0x0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALT_FUNC_0,
    GPIO_MODE_ALT_FUNC_1,
    GPIO_MODE_MAX,
} gpio_mode_e;

void bp6a_gpio_set_dir(uint8_t port, uint8_t pin, bool is_input);
void bp6a_gpio_set_int(uint8_t port, uint8_t pin, bool enable);
void bp6a_gpio_clear_pend(uint8_t port, uint8_t pin);
void bp6a_gpio_set_int_type(uint8_t port, uint8_t pin, bool isHigh, bool isEadge);
void bp6a_set_pin_config(uint8_t port, uint8_t pin, uint8_t cfg);
#endif /* __S1SBP6A_GPIO_H */
