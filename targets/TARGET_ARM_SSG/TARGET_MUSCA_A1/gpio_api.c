/* mbed Microcontroller Library
 * Copyright (c) 2019 Arm Limited
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

/*
 * This file implements APIS defined in hal/gpio_api.h
 * Note: Due to a HW limitation, GPIO in Musca-A1 is Secure only, so secure
 *       service should be used for GPIO in NS domain.
 */

#include <stddef.h>
#include "gpio_api.h"
#include "pinmap.h"
#include "objects.h"
#include "mbed_error.h"

uint32_t gpio_set(PinName pin)
{
    /* Due to a HW limitation, GPIO in Musca-A1 is Secure only, so secure
     * service should be used for GPIO in NS domain.
     */
    return 0;
}

void gpio_init(gpio_t *obj, PinName pin)
{
    /* Due to a HW limitation, GPIO in Musca-A1 is Secure only, so secure
     * service should be used for GPIO in NS domain.
     */
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    /* Due to a HW limitation, GPIO in Musca-A1 is Secure only, so secure
     * service should be used for GPIO in NS domain.
     */
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    /* Due to a HW limitation, GPIO in Musca-A1 is Secure only, so secure
     * service should be used for GPIO in NS domain.
     */
}

int gpio_is_connected(const gpio_t *obj)
{
    /* Due to a HW limitation, GPIO in Musca-A1 is Secure only, so secure
     * service should be used for GPIO in NS domain.
     */
    return 0;
}

void gpio_write(gpio_t *obj, int value)
{
    /* Due to a HW limitation, GPIO in Musca-A1 is Secure only, so secure
     * service should be used for GPIO in NS domain.
     */
}

int gpio_read(gpio_t *obj)
{
    /* Due to a HW limitation, GPIO in Musca-A1 is Secure only, so secure
     * service should be used for GPIO in NS domain.
     */
    return 0;
}
