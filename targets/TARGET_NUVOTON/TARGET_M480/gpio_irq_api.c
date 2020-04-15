/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
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

#include "gpio_irq_api.h"

#if DEVICE_INTERRUPTIN

#include "gpio_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "nu_bitutil.h"
#include "mbed_assert.h"

#define NU_MAX_PIN_PER_PORT     16

struct nu_gpio_irq_var {
    gpio_irq_t *    obj_arr[NU_MAX_PIN_PER_PORT];
    IRQn_Type       irq_n;
    void            (*vec)(void);
    uint32_t        port_index;
};

static void gpio_irq_0_vec(void);
static void gpio_irq_1_vec(void);
static void gpio_irq_2_vec(void);
static void gpio_irq_3_vec(void);
static void gpio_irq_4_vec(void);
static void gpio_irq_5_vec(void);
static void gpio_irq_6_vec(void);
static void gpio_irq_7_vec(void);
static void gpio_irq(struct nu_gpio_irq_var *var);

//EINT0_IRQn
static struct nu_gpio_irq_var gpio_irq_var_arr[] = {
    {{NULL}, GPA_IRQn, gpio_irq_0_vec, 0},
    {{NULL}, GPB_IRQn, gpio_irq_1_vec, 1},
    {{NULL}, GPC_IRQn, gpio_irq_2_vec, 2},
    {{NULL}, GPD_IRQn, gpio_irq_3_vec, 3},
    {{NULL}, GPE_IRQn, gpio_irq_4_vec, 4},
    {{NULL}, GPF_IRQn, gpio_irq_5_vec, 5},
    {{NULL}, GPG_IRQn, gpio_irq_6_vec, 6},
    {{NULL}, GPH_IRQn, gpio_irq_7_vec, 7},
};

#define NU_MAX_PORT     (sizeof (gpio_irq_var_arr) / sizeof (gpio_irq_var_arr[0]))

#ifndef MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE
#define MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE 0
#endif

#ifndef MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST
#define MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST NC
#endif
static PinName gpio_irq_debounce_arr[] = {
    MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE_LIST
};

#ifndef MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE
#define MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE GPIO_DBCTL_DBCLKSRC_LIRC
#endif

#ifndef MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE
#define MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE GPIO_DBCTL_DBCLKSEL_16
#endif

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == NC) {
        return -1;
    }

    uint32_t pin_index = NU_PINNAME_TO_PIN(pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(pin);
    if (pin_index >= NU_MAX_PIN_PER_PORT || port_index >= NU_MAX_PORT) {
        return -1;
    }

    obj->pin = pin;
    obj->irq_types = 0;
    obj->irq_handler = (uint32_t) handler;
    obj->irq_id = id;

    GPIO_T *gpio_base = NU_PORT_BASE(port_index);
    // NOTE: In InterruptIn constructor, gpio_irq_init() is called with gpio_init_in() which is responsible for multi-function pin setting.
    //       There is no need to call gpio_set() redundantly.

    {
#if MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_ENABLE
        // Suppress compiler warning
        (void) gpio_irq_debounce_arr;

        // Configure de-bounce clock source and sampling cycle time
        GPIO_SET_DEBOUNCE_TIME(MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE, MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE);
        GPIO_ENABLE_DEBOUNCE(gpio_base, 1 << pin_index);
#else
        // Enable de-bounce if the pin is in the de-bounce enable list

        // De-bounce defaults to disabled.
        GPIO_DISABLE_DEBOUNCE(gpio_base, 1 << pin_index);

        PinName *debounce_pos = gpio_irq_debounce_arr;
        PinName *debounce_end = gpio_irq_debounce_arr + sizeof (gpio_irq_debounce_arr) / sizeof (gpio_irq_debounce_arr[0]);
        for (; debounce_pos != debounce_end && *debounce_pos != NC; debounce_pos ++) {
            uint32_t pin_index_debunce = NU_PINNAME_TO_PIN(*debounce_pos);
            uint32_t port_index_debounce = NU_PINNAME_TO_PORT(*debounce_pos);

            if (pin_index == pin_index_debunce &&
                    port_index == port_index_debounce) {
                // Configure de-bounce clock source and sampling cycle time
                GPIO_SET_DEBOUNCE_TIME(MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_CLOCK_SOURCE, MBED_CONF_TARGET_GPIO_IRQ_DEBOUNCE_SAMPLE_RATE);
                GPIO_ENABLE_DEBOUNCE(gpio_base, 1 << pin_index);
                break;
            }
        }
#endif
    }

    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;

    var->obj_arr[pin_index] = obj;

    // NOTE: InterruptIn requires IRQ enabled by default.
    gpio_irq_enable(obj);

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(obj->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;

    NVIC_DisableIRQ(var->irq_n);
    NU_PORT_BASE(port_index)->INTEN = 0;

    MBED_ASSERT(pin_index < NU_MAX_PIN_PER_PORT);
    var->obj_arr[pin_index] = NULL;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint32_t pin_index = NU_PINNAME_TO_PIN(obj->pin);
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    /* We assume BSP has such coding so that we can easily add/remove either irq type. */
    MBED_STATIC_ASSERT(GPIO_INT_BOTH_EDGE == (GPIO_INT_RISING | GPIO_INT_FALLING),
        "GPIO_INT_BOTH_EDGE must be bitwise OR of GPIO_INT_RISING and GPIO_INT_FALLING");
    uint32_t irq_type;
    switch (event) {
        case IRQ_RISE:
            irq_type = GPIO_INT_RISING;
            break;

        case IRQ_FALL:
            irq_type = GPIO_INT_FALLING;
            break;

        default:
            irq_type = 0;
    }

    /* We can handle invalid/null irq type. */
    if (enable) {
        obj->irq_types |= irq_type;
    } else {
        obj->irq_types &= ~irq_type;
    }

    /* Update irq types:
     *
     * Implementations of GPIO_EnableInt(...) are inconsistent: disable or not irq type not enabled.
     * For consistency, disable GPIO_INT_BOTH_EDGE and then enable OR'ed irq types, GPIO_INT_RISING,
     * GPIO_INT_FALLING, or both.
     */
    GPIO_DisableInt(gpio_base, pin_index);
    GPIO_EnableInt(gpio_base, pin_index, obj->irq_types);
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;

    NVIC_SetVector(var->irq_n, (uint32_t) var->vec);
    NVIC_EnableIRQ(var->irq_n);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    uint32_t port_index = NU_PINNAME_TO_PORT(obj->pin);
    struct nu_gpio_irq_var *var = gpio_irq_var_arr + port_index;

    NVIC_DisableIRQ(var->irq_n);
}

static void gpio_irq_0_vec(void)
{
    gpio_irq(gpio_irq_var_arr + 0);
}
static void gpio_irq_1_vec(void)
{
    gpio_irq(gpio_irq_var_arr + 1);
}
static void gpio_irq_2_vec(void)
{
    gpio_irq(gpio_irq_var_arr + 2);
}
static void gpio_irq_3_vec(void)
{
    gpio_irq(gpio_irq_var_arr + 3);
}
static void gpio_irq_4_vec(void)
{
    gpio_irq(gpio_irq_var_arr + 4);
}
static void gpio_irq_5_vec(void)
{
    gpio_irq(gpio_irq_var_arr + 5);
}
static void gpio_irq_6_vec(void)
{
    gpio_irq(gpio_irq_var_arr + 6);
}
static void gpio_irq_7_vec(void)
{
    gpio_irq(gpio_irq_var_arr + 7);
}

static void gpio_irq(struct nu_gpio_irq_var *var)
{
    // NOTE: GPA_IRQn, GPB_IRQn, ... are not arranged sequentially, so we cannot calculate out port_index through offset from GPA_IRQn.
    //       Instead, we add port_index into gpio_irq_var_arr table.
    uint32_t port_index = var->port_index;
    GPIO_T *gpio_base = NU_PORT_BASE(port_index);

    uint32_t intsrc = gpio_base->INTSRC;
    uint32_t inten = gpio_base->INTEN;
    while (intsrc) {
        int pin_index = nu_ctz(intsrc);
        gpio_irq_t *obj = var->obj_arr[pin_index];
        if (inten & (GPIO_INT_RISING << pin_index)) {
            if (GPIO_PIN_DATA(port_index, pin_index)) {
                if (obj->irq_handler) {
                    ((gpio_irq_handler) obj->irq_handler)(obj->irq_id, IRQ_RISE);
                }
            }
        }

        if (inten & (GPIO_INT_FALLING << pin_index)) {
            if (! GPIO_PIN_DATA(port_index, pin_index)) {
                if (obj->irq_handler) {
                    ((gpio_irq_handler) obj->irq_handler)(obj->irq_id, IRQ_FALL);
                }
            }
        }

        intsrc &= ~(1 << pin_index);
    }
    // Clear all interrupt flags
    gpio_base->INTSRC = gpio_base->INTSRC;
}

#endif
