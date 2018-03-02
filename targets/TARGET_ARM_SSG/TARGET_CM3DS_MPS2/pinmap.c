/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "objects.h"

void pin_function(PinName pin, int function)
{
    struct arm_gpio_dev_t *gpio_dev;
    uint32_t flags;

    MBED_ASSERT(pin != NC);

    /* The pin has to be a GPIO pin */
    if (pin >= EXP0 && pin <= EXP51) {
        switch (function) {
            case ALTERNATE_FUNC:
                flags = ARM_GPIO_PIN_DISABLE;
                break;
            case GPIO_FUNC:
                flags = ARM_GPIO_PIN_ENABLE;
                break;
            default:
                return;
        }

        switch (GPIO_DEV_NUMBER(pin)) {
#ifdef ARM_GPIO0
        case GPIO0_NUMBER:
            gpio_dev = &ARM_GPIO0_DEV;
            break;
#endif /* ARM_GPIO0 */
#ifdef ARM_GPIO1
        case GPIO1_NUMBER:
            gpio_dev = &ARM_GPIO1_DEV;
            break;
#endif /* ARM_GPIO1 */
#ifdef ARM_GPIO2
        case GPIO2_NUMBER:
            gpio_dev = &ARM_GPIO2_DEV;
            break;
#endif /* ARM_GPIO2 */
#ifdef ARM_GPIO3
        case GPIO3_NUMBER:
            gpio_dev = &ARM_GPIO3_DEV;
            break;
#endif /* ARM_GPIO3 */
        default:
            error("GPIO %d, associated with expansion pin %d, is disabled",
                  pin, GPIO_DEV_NUMBER(pin));
            return;
        }

        arm_gpio_init(gpio_dev);
        (void)arm_gpio_config(gpio_dev, ARM_GPIO_ACCESS_PIN,
                              GPIO_PIN_NUMBER(pin), flags);
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != NC);

    /* PinMode is not supported */
}
