/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "cmsis.h"
#include "device.h"
#include "mbed_assert.h"
#include "gpio_object.h"
#include "psoc6_utils.h"
#include "mbed_error.h"
#include "rtx_lib.h"

static inline void gpio_set_dir_mode(gpio_t *obj)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->port);
    MBED_ASSERT(obj->pin != NC);

    uint32_t pin  = CY_PIN(obj->pin);
    uint32_t cymode = gpio_get_cy_drive_mode(obj->dir, obj->mode);

    Cy_GPIO_SetDrivemode(obj->port, pin, cymode);

    if (obj->dir == PIN_INPUT) {
        // Force output to enable pulls.
        switch (cymode) {
            case CY_GPIO_DM_PULLUP:
                Cy_GPIO_Write(obj->port, pin, 1);
                break;
            case CY_GPIO_DM_PULLDOWN:
                Cy_GPIO_Write(obj->port, pin, 0);
                break;
        }
    }
}

void gpio_init(gpio_t *obj, PinName pin)
{
    MBED_ASSERT(obj);
    obj->pin = pin;
    obj->dir = PIN_INPUT;
    obj->mode = PullNone;

    if (pin == NC) {
        return;
    }

    MBED_ASSERT(CY_PIN(obj->pin) < 8); // PSoC6 architecture supports 8 pins per port.

    /*
     * Perform i/o reservation only if this is called outside of critical section/interrupt context.
     * This is a workaround for mbed_die() implementation, which configures LED1 inside critical section.
     * Normally user is advised to perform all of the i/o configuration at the program beginning,
     * or elsewhere in the running thread context. when we detect that we are in the wrong context here,
     * we assume it's explicitly called from mbed_die() or other fault handling, so eventual forcing
     * of the pin mode is deliberate and should not cause more problems.
     */
    if (!(IsIrqMode() || IsIrqMasked())) {
        if (cy_reserve_io_pin(pin)) {
            error("GPIO pin reservation conflict.");
        }
    }
    obj->port = Cy_GPIO_PortToAddr(CY_PORT(obj->pin));

    const uint32_t outputVal = 0;

    Cy_GPIO_Pin_FastInit(obj->port, CY_PIN(obj->pin), CY_GPIO_DM_HIGHZ, outputVal, HSIOM_SEL_GPIO);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->pin != NC);

    obj->mode = mode;
    gpio_set_dir_mode(obj);
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(obj->pin != NC);

    obj->dir = direction;
    gpio_set_dir_mode(obj);
}
