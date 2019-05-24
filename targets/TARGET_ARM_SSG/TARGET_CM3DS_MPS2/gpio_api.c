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
#include "gpio_api.h"
#include "pinmap.h"
#include "objects.h"
#include "mbed_error.h"

#define RESERVED_MISC_PIN 7u

enum io_type {
    GPIO_DEVICE,
    MPS2_IO_DEVICE,
    DEVICE_UNKNOWN
};

/* Tell if the gpio is from GPIO device or MPS2 IO */
static enum io_type io_type(gpio_t *obj)
{
    if (obj->gpio_dev != NULL && obj->mps2_io_dev == NULL) {
        return GPIO_DEVICE;
    }
    if (obj->gpio_dev == NULL && obj->mps2_io_dev != NULL) {
        return MPS2_IO_DEVICE;
    }
    return DEVICE_UNKNOWN;
}

/* Return the correct mask of the given PIN */
uint32_t gpio_set(PinName pin)
{
    pin_function(pin, (int)GPIO_FUNC);

    if (pin >= EXP0 && pin <= EXP51) {
        /* GPIO pins */
        return (1 << GPIO_PIN_NUMBER(pin));
    } else if (pin == USERLED1 || pin == USERLED2) {
        /* User LEDs */
        return (1 << (pin - USERLED1));
    } else if (pin == USERSW1 || pin == USERSW2) {
        /* User Push buttons */
        return (1 << (pin - USERSW1));
    } else if (pin >= LED1 && pin <= LED8) {
        /* MCC LEDs */
        return (1 << (pin - LED1));
    } else if (pin >= SW1 && pin <= SW8) {
        /* MCC Switches */
        return (1 << (pin - SW1));
    } else {
        return 0;
    }
}

/*
 *  FPGA MISC bit mapping:
 *    [31:7] Reserved
 *    [6] CLCD_BL_CTRL
 *    [5] CLCD_RD
 *    [4] CLCD_RS
 *    [3] CLCD_RESET
 *    [2] Reserved
 *    [1] SPI_nSS
 *    [0] CLCD_CS
 */
static uint32_t gpio_fpga_misc_pin(PinName pin)
{
    uint32_t pin_number = RESERVED_MISC_PIN;

    if (pin == CLCD_SSEL) {
        pin_number = 0u;
    } else if (pin == SPI_SSEL) {
        pin_number = 1u;
    } else if (pin == CLCD_RESET) {
        pin_number = 3u;
    } else if (pin == CLCD_RS) {
        pin_number = 4u;
    } else if (pin == CLCD_RD) {
        pin_number = 5u;
    } else if (pin == CLCD_BL_CTRL){
        pin_number = 6u;
    }

    return pin_number;
}

void gpio_init(gpio_t *obj, PinName pin)
{
    struct arm_gpio_dev_t *gpio_dev;

    if (pin == NC) {
        obj->pin_number = NC;
        obj->gpio_dev = NULL;
        obj->mps2_io_dev = NULL;
        return;
    }

    if (pin >= EXP0 && pin <= EXP51) {
        /* GPIO pins */
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
                  GPIO_DEV_NUMBER(pin), pin);
            return;
        }

        arm_gpio_init(gpio_dev);

        obj->gpio_dev = gpio_dev;
        obj->mps2_io_dev = NULL;
        obj->arm_mps2_io_write = NULL;
        obj->pin_number = GPIO_PIN_NUMBER(pin);
        /* GPIO is input by default */
        obj->direction = PIN_INPUT;
        return;
    }

#ifdef ARM_MPS2_IO_FPGAIO
    if (pin == USERLED1 || pin == USERLED2) {
        /* User LEDs */
        obj->gpio_dev = NULL;
        obj->mps2_io_dev = &ARM_MPS2_IO_FPGAIO_DEV;
        obj->pin_number = pin - USERLED1;
        obj->arm_mps2_io_write = arm_mps2_io_write_leds;
        obj->direction = PIN_OUTPUT;
        return;
    } else if (pin == USERSW1 || pin == USERSW2) {
        /* User Push buttons */
        obj->gpio_dev = NULL;
        obj->mps2_io_dev = &ARM_MPS2_IO_FPGAIO_DEV;
        obj->arm_mps2_io_write = NULL;
        obj->pin_number = pin - USERSW1;
        obj->direction = PIN_INPUT;
        return;
    }
    /* Check if pin is a MISC pin */
    obj->pin_number = gpio_fpga_misc_pin(pin);
    if (obj->pin_number != RESERVED_MISC_PIN) {
        obj->gpio_dev = NULL;
        obj->mps2_io_dev = &ARM_MPS2_IO_FPGAIO_DEV;
        obj->arm_mps2_io_write = arm_mps2_io_write_misc;
        obj->direction = PIN_OUTPUT;
        return;
    }
#endif /* ARM_MPS2_IO_FPGAIO */

#ifdef ARM_MPS2_IO_SCC
    if (pin >= LED1 && pin <= LED8) {
        /* MCC LEDs */
        obj->gpio_dev = NULL;
        obj->mps2_io_dev = &ARM_MPS2_IO_SCC_DEV;
        obj->arm_mps2_io_write = arm_mps2_io_write_leds;
        obj->pin_number = pin - LED1;
        obj->direction = PIN_OUTPUT;
        return;
    } else if (pin >= SW1 && pin <= SW8) {
        /* MCC Switches */
        obj->gpio_dev = NULL;
        obj->mps2_io_dev = &ARM_MPS2_IO_SCC_DEV;
        obj->arm_mps2_io_write = NULL;
        obj->pin_number = pin - SW1;
        obj->direction = PIN_INPUT;
        return;
    }
#endif /* ARM_MPS2_IO_SCC */

    error("pin %d is not a GPIO", pin);
}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    /* PinMode is not supported */
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    uint32_t flags = ARM_GPIO_PIN_ENABLE;

    obj->direction = direction;

    switch (io_type(obj)) {
    case GPIO_DEVICE:
        switch (direction) {
        case PIN_INPUT:
            flags |= ARM_GPIO_INPUT;
            break;
        case PIN_OUTPUT:
            flags |= ARM_GPIO_OUTPUT;
            break;
        /* default: not added to force to cover all enumeration cases */
        }

        (void)arm_gpio_config(obj->gpio_dev, ARM_GPIO_ACCESS_PIN,
                              obj->pin_number, flags);
        return;
    case MPS2_IO_DEVICE:
        /* Do nothing as MPS2 IO direction can not be changed */
        return;
    case DEVICE_UNKNOWN:
        break;
    /* default:  The default is not defined intentionally to force the
     *           compiler to check that all the enumeration values are
     *           covered in the switch.*/
    }

    error("can not change the direction of pin");
}

int gpio_is_connected(const gpio_t *obj)
{
    if (obj->pin_number == (uint32_t)NC) {
        return 0;
    } else {
        return 1;
    }
}

void gpio_write(gpio_t *obj, int value)
{
    switch (io_type(obj)) {
    case GPIO_DEVICE:
        (void)arm_gpio_write(obj->gpio_dev, ARM_GPIO_ACCESS_PIN,
                             obj->pin_number, (uint32_t)value);
        return;
    case MPS2_IO_DEVICE:
        if (obj->direction == PIN_INPUT) {
            /*
             * If the given gpio is in fact a button, ignore the call to not
             * write to the corresponding LED instead.
             */
            return;
        }
        if (obj->arm_mps2_io_write != NULL) {
            obj->arm_mps2_io_write(obj->mps2_io_dev, ARM_MPS2_IO_ACCESS_PIN,
                               obj->pin_number, (uint32_t)value);
        }
        return;
    case DEVICE_UNKNOWN:
        break;
    /* default:  The default is not defined intentionally to force the
     *           compiler to check that all the enumeration values are
     *           covered in the switch.*/
    }

    error("can not write pin");
}

int gpio_read(gpio_t *obj)
{
    switch (io_type(obj)) {
    case GPIO_DEVICE:
        return (int)arm_gpio_read(obj->gpio_dev, ARM_GPIO_ACCESS_PIN,
                                  obj->pin_number);
    case MPS2_IO_DEVICE:
        switch (obj->direction) {
        case PIN_INPUT:

            return (int)arm_mps2_io_read_buttons(obj->mps2_io_dev,
                                                 ARM_MPS2_IO_ACCESS_PIN,
                                                 obj->pin_number);
        case PIN_OUTPUT:
            return (int)arm_mps2_io_read_leds(obj->mps2_io_dev,
                                              ARM_MPS2_IO_ACCESS_PIN,
                                              obj->pin_number);
        }

    case DEVICE_UNKNOWN:
        break;
    /* default:  The default is not defined intentionally to force the
     *           compiler to check that all the enumeration values are
     *           covered in the switch.*/
    }

    error("can not read pin");
    return 0;
}
