/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "analogin_api.h"
#include "i2c_api.h"
#include "gpio_api.h"
#include "mbed_toolchain.h"

// To be re-implemented in the target layer if required
MBED_WEAK void gpio_free(gpio_t *obj)
{
    // Do nothing
}

#if DEVICE_I2C
// To be re-implemented in the target layer if required
MBED_WEAK void i2c_free(i2c_t *obj)
{
    // Do nothing
}
#endif

#if DEVICE_ANALOGIN
// To be re-implemented in the target layer if required
MBED_WEAK void analogin_free(analogin_t *obj)
{
    // Do nothing
}
#endif