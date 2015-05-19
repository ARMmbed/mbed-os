/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "cmsis.h"
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "mbed_error.h"

#define EDGE_NONE (0)
#define EDGE_RISE (1)
#define EDGE_FALL (2)
#define EDGE_BOTH (3)

static gpio_irq_handler irq_handler;


typedef struct gpio_channel {
    uint32_t pin_mask;      // bitmask representing which pins are configured for receiving interrupts
    uint32_t channel_ids;   // mbed "gpio_irq_t gpio_irq" field of instance
    uint32_t channel_pin;   // pin number in port group
    gpio_irq_t * obj;
} gpio_channel_t;

static gpio_channel_t channel;

#ifdef __cplusplus
extern "C"{
#endif
void EXTI_Handler()
{
    // To Do
    //irq_handler(channel.channel_ids);
}
#ifdef __cplusplus
}
#endif


int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t gpio_pin;

    if (pin == NC) return -1;

    uint32_t port_index = WIZ_PORT(pin);
    uint32_t pin_index  = WIZ_PIN(pin);

    // If port_index is not GPIOC, return -1
    if (port_index != 2 ) {
        error("InterruptIn error: pin not supported.\n");
        return -1;
    }

    // Select irq number and interrupt routine
    if ((pin_index == 0) || (pin_index == 1) || (pin_index == 4) || (pin_index == 6) || (pin_index == 7)) {
        irq_n = EXTI_IRQn;
        gpio_pin = 1 << pin_index;
    } else {
        error("InterruptIn error: pin not supported.\n");
        return -1;
    }

    // Enable GPIO clock
    uint32_t gpio_add = Get_GPIO_BaseAddress(port_index);

    // Configure GPIO
    EXTI_InitTypeDef EXTI_InitDef;
    EXTI_InitDef.EXTI_Line = gpio_pin;
    EXTI_InitDef.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(gpio_add, &EXTI_InitDef);

    // Enable EXTI interrupt
    NVIC_ClearPendingIRQ(irq_n); // Pending bit Clear
    NVIC_EnableIRQ(irq_n);

    obj->irq_n = irq_n;
//    obj->irq_index = irq_index;
    obj->event = EDGE_FALL;
    obj->pin = pin;

    channel.channel_pin = pin_index;
    channel.pin_mask = gpio_pin;
    channel.channel_ids = id;
    channel.obj = obj;

    irq_handler = handler;

    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    // Disable EXTI line
    NVIC_ClearPendingIRQ(obj->irq_n);
    EXTI_DeInit();
    pin_function(obj->pin, WIZ_PIN_DATA(WIZ_MODE_INPUT, WIZ_GPIO_NOPULL, 0));
    obj->event = EDGE_NONE;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint16_t porarity;

    if (enable) {
        if (event == IRQ_RISE) {
            porarity = EXTI_Trigger_Rising;
            obj->event = EDGE_RISE;
        }
        if (event == IRQ_FALL) {
            porarity = EXTI_Trigger_Falling;
            obj->event = EDGE_FALL;
        }

        uint32_t port_index = WIZ_PORT(obj->pin);
        uint32_t pin_index  = WIZ_PIN(obj->pin);

        // Enable GPIO clock
        uint32_t gpio_add = Get_GPIO_BaseAddress(port_index);
        EXTI_Polarity_Set(port_index,(1<<pin_index),porarity);
    } else { // Disable
        gpio_irq_free(obj);
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    NVIC_EnableIRQ(obj->irq_n);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    NVIC_DisableIRQ(obj->irq_n);
    obj->event = EDGE_NONE;
}
