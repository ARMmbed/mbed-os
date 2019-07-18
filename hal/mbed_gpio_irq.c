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
#include "hal/gpio_irq_api.h"

#if DEVICE_INTERRUPTIN

#include "platform/mbed_toolchain.h"
#include "hal/gpio_api.h"

MBED_WEAK const PinMap *gpio_irq_pinmap()
{
    // Targets should override this weak implementation to provide correct data.
    // By default, this is exactly the same as GPIO PinMap.
    return gpio_pinmap();
}

#endif
