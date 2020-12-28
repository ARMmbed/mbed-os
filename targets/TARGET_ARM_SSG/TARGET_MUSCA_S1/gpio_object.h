/* mbed Microcontroller Library
 * Copyright (c) 2019-2020 Arm Limited
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

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_PIN_NUMBER(pin)  ((pin) & 0xF)

#define PINS_NUMBER            (16)

/**
 * \brief Object to store GPIO specific data through the configuration
 */
typedef struct gpio_s {
    struct gpio_cmsdk_dev_t *gpio_dev;
    uint32_t pin_num;
    PinDirection direction;
} gpio_t;


struct gpio_irq_s {
    /** GPIO is not available in Musca-S1 in non-secure domain, so this dummy
     *  structure is needed for successful build.
     */
    uint32_t dummy;
};

#ifdef __cplusplus
}
#endif

#endif
