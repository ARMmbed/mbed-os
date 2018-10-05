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
#include "objects.h"
#include "gpio_irq_api.h"
#include "mbed_error.h"

#define ERROR_BIT_NUMBER 0xFF

struct gpio_irq_handler_t {
    gpio_irq_handler handler;
    gpio_irq_event event;
    uint32_t id;
};

/* Handlers registered */
static struct gpio_irq_handler_t gpio_irq[PINS_NUMBER];

/*
 * Return the bit number of the lowest significant bit set to 1 or
 * ERROR_BIT_NUMBER if there is no bit set.
 */
static uint8_t find_first_set_bit(uint32_t word)
{
    uint8_t bit_number = 0;

    if (word == 0) {
        return ERROR_BIT_NUMBER;
    }

    while (((word >> bit_number++) & 1UL) == 0);

    return (bit_number - 1);
}

static void handler(struct arm_gpio_dev_t* dev, uint32_t gpio_number,
                    uint32_t exp_pin_base)
{
    uint32_t irq_status = 0;
    /* Pin that triggered the IRQ in this GPIO */
    uint8_t pin_number;
    /* Pin number in the expension port */
    uint8_t exp_pin_number;

    (void)arm_gpio_get_irq_status(dev, ARM_GPIO_ACCESS_PORT, ARG_NOT_USED,
                                  &irq_status);

    pin_number = find_first_set_bit(irq_status);
    if (pin_number == ERROR_BIT_NUMBER) {
        /* There was no IRQ */
        return;
    }

    (void)arm_gpio_clear_interrupt(dev, pin_number);

    exp_pin_number = exp_pin_base + pin_number;

    gpio_irq[exp_pin_number].handler(gpio_irq[exp_pin_number].id,
                                     gpio_irq[exp_pin_number].event);
}

#ifdef ARM_GPIO0
void PORT0_IRQHandler(void)
{
    handler(&ARM_GPIO0_DEV, GPIO0_NUMBER, EXP_PIN_BASE0);
}
#endif /* ARM_GPIO0 */

#ifdef ARM_GPIO1
void PORT1_ALL_IRQHandler(void)
{
    handler(&ARM_GPIO1_DEV, GPIO1_NUMBER, EXP_PIN_BASE1);
}
#endif /* ARM_GPIO1 */

#ifdef ARM_GPIO2
void PORT2_ALL_IRQHandler(void)
{
    handler(&ARM_GPIO2_DEV, GPIO2_NUMBER, EXP_PIN_BASE2);
}
#endif /* ARM_GPIO2 */

#ifdef ARM_GPIO3
void PORT3_ALL_IRQHandler(void)
{
    handler(&ARM_GPIO3_DEV, GPIO3_NUMBER, EXP_PIN_BASE3);
}
#endif /* ARM_GPIO3 */

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler,
                  uint32_t id)
{
    struct arm_gpio_dev_t *gpio_dev;

    if (pin >= EXP0 && pin <= EXP51) {
        /* GPIO pins */
        switch (GPIO_DEV_NUMBER(pin)) {
#ifdef ARM_GPIO0
            case 0:
                gpio_dev = &ARM_GPIO0_DEV;
                obj->irq_number = PORT0_ALL_IRQn;
                break;
#endif /* ARM_GPIO0 */
#ifdef ARM_GPIO1
            case 1:
                gpio_dev = &ARM_GPIO1_DEV;
                obj->irq_number = PORT1_ALL_IRQn;
                break;
#endif /* ARM_GPIO1 */
#ifdef ARM_GPIO2
            case 2:
                gpio_dev = &ARM_GPIO2_DEV;
                obj->irq_number = PORT2_ALL_IRQn;
                break;
#endif /* ARM_GPIO2 */
#ifdef ARM_GPIO3
            case 3:
                gpio_dev = &ARM_GPIO3_DEV;
                obj->irq_number = PORT3_ALL_IRQn;
                break;
#endif /* ARM_GPIO3 */
            default:
                error("GPIO %d is not enabled", GPIO_DEV_NUMBER(pin));
                return -1;
        }

        obj->gpio_dev = gpio_dev;
        obj->pin_number = GPIO_PIN_NUMBER(pin);
        obj->exp_pin_number = pin;

        arm_gpio_init(gpio_dev);

        /* Save the handler and id into the global structure */
        gpio_irq[pin].handler = handler;
        gpio_irq[pin].id = id;

        return 0;
    } else {
        /* The pin is not concerned with GPIO IRQ */
        error("Pin %d is not a GPIO", pin);
        return -1;
    }
}

void gpio_irq_free(gpio_irq_t *obj)
{
    /* Not implemented because the device can not be uninitialized. */
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    /* Interrupt is set on an input pin on rising or falling edge */
    uint32_t flags = ARM_GPIO_PIN_ENABLE | ARM_GPIO_INPUT | ARM_GPIO_IRQ |
                     ARM_GPIO_IRQ_EDGE;

    switch (event) {
        case IRQ_RISE:
            flags |= ARM_GPIO_IRQ_ACTIVE_HIGH;
            break;
        case IRQ_FALL:
            flags |= ARM_GPIO_IRQ_ACTIVE_LOW;
            break;
        case IRQ_NONE:
            return;
    }

    (void)arm_gpio_config(obj->gpio_dev, ARM_GPIO_ACCESS_PIN, obj->pin_number,
                          flags);

    /* Record the event type of this pin */
    gpio_irq[obj->exp_pin_number].event = event;

    NVIC_EnableIRQ(obj->irq_number);

    if (enable) {
        gpio_irq_enable(obj);
    } else {
        gpio_irq_disable(obj);
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    (void)arm_gpio_set_interrupt(obj->gpio_dev, ARM_GPIO_ACCESS_PIN,
                                 obj->pin_number, ARM_GPIO_IRQ_ENABLE);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    (void)arm_gpio_set_interrupt(obj->gpio_dev, ARM_GPIO_ACCESS_PIN,
                                 obj->pin_number, ARM_GPIO_IRQ_DISABLE);
}
