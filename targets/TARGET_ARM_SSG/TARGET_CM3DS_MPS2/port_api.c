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
#include <stddef.h>
#include "port_api.h"
#include "objects.h"
#include "mbed_error.h"

PinName port_pin(PortName port, int pin_n)
{
    if (pin_n < 0 || pin_n >= PINS_PER_GPIO ||
        ((port == Port3) && (pin_n >= GPIO3_PIN_NUMBER))) {
        return NC;
    }

    return (PinName)(PINS_PER_GPIO * port + pin_n);
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir)
{
    struct arm_gpio_dev_t *gpio_dev;
    uint32_t flags = ARM_GPIO_PIN_ENABLE;

    switch (port) {
#ifdef ARM_GPIO0
    case Port0:
        gpio_dev = &ARM_GPIO0_DEV;
        break;
#endif /* ARM_GPIO0 */
#ifdef ARM_GPIO1
    case Port1:
        gpio_dev = &ARM_GPIO1_DEV;
        break;
#endif /* ARM_GPIO1 */
#ifdef ARM_GPIO2
    case Port2:
        gpio_dev = &ARM_GPIO2_DEV;
        break;
#endif /* ARM_GPIO2 */
#ifdef ARM_GPIO3
    case Port3:
        gpio_dev = &ARM_GPIO3_DEV;
        break;
#endif /* ARM_GPIO3 */
    default:
        error("Port%d is not enabled", port);
        return;
    }

    arm_gpio_init(gpio_dev);
    obj->gpio_dev = gpio_dev;

    arm_gpio_set_port_mask(gpio_dev, mask);

    switch (dir) {
    case PIN_INPUT:
        flags |= ARM_GPIO_INPUT;
        break;
    case PIN_OUTPUT:
        flags |= ARM_GPIO_OUTPUT;
        break;
    /* default: not added to force to cover all enumeration cases */
    }

    (void)arm_gpio_config(gpio_dev, ARM_GPIO_ACCESS_PORT, ARG_NOT_USED, flags);
}

void port_mode(port_t *obj, PinMode mode)
{
    /* PinMode is not supported */
}

void port_dir(port_t *obj, PinDirection dir)
{
    uint32_t flags = (dir == PIN_OUTPUT) ? ARM_GPIO_OUTPUT : ARM_GPIO_INPUT;
    (void)arm_gpio_config(obj->gpio_dev, ARM_GPIO_ACCESS_PORT, ARG_NOT_USED,
                          flags);
}

void port_write(port_t *obj, int value)
{
    (void)arm_gpio_write(obj->gpio_dev, ARM_GPIO_ACCESS_PORT, ARG_NOT_USED,
                         (uint32_t)value);
}

int port_read(port_t *obj)
{
    return (int)arm_gpio_read(obj->gpio_dev, ARM_GPIO_ACCESS_PORT,
                              ARG_NOT_USED);
}
