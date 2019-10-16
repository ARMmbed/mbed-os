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
#include "gpio_irq_api.h"
#include "psoc6_utils.h"
#include "cy_sysint.h"


#define NUM_GPIO_PORTS              IOSS_GPIO_GPIO_PORT_NR
#define NUM_GPIO_PORT_PINS          8
#define GPIO_DEFAULT_IRQ_PRIORITY   5

static gpio_irq_t *irq_objects[NUM_GPIO_PORTS][NUM_GPIO_PORT_PINS] = {NULL};

typedef struct irq_port_info_s {
    IRQn_Type   irqn;
    uint32_t    pin_mask;
} irq_port_info_t;

static irq_port_info_t irq_port_usage[NUM_GPIO_PORTS] = {{0, 0},};

static void gpio_irq_dispatcher(uint32_t port_id)
{
    uint32_t pin;
    gpio_irq_event event;
    GPIO_PRT_Type *port = Cy_GPIO_PortToAddr(port_id);

    for (pin = 0; pin < NUM_GPIO_PORT_PINS; ++pin) {
        if (Cy_GPIO_GetInterruptStatusMasked(port, pin)) {
            gpio_irq_t *obj = irq_objects[port_id][pin];
            MBED_ASSERT(obj);
            Cy_GPIO_ClearInterrupt(port, pin);
            event = (obj->mode == IRQ_FALL)? IRQ_FALL : IRQ_RISE;
            ((gpio_irq_handler) obj->handler)(obj->id_arg, event);
        }
    }
}

static void gpio_irq_dispatcher_port0(void)
{
    gpio_irq_dispatcher(0);
}

static void gpio_irq_dispatcher_port1(void)
{
    gpio_irq_dispatcher(1);
}

static void gpio_irq_dispatcher_port2(void)
{
    gpio_irq_dispatcher(2);
}

static void gpio_irq_dispatcher_port3(void)
{
    gpio_irq_dispatcher(3);
}

static void gpio_irq_dispatcher_port4(void)
{
    gpio_irq_dispatcher(4);
}

static void gpio_irq_dispatcher_port5(void)
{
    gpio_irq_dispatcher(5);
}

static void gpio_irq_dispatcher_port6(void)
{
    gpio_irq_dispatcher(6);
}

static void gpio_irq_dispatcher_port7(void)
{
    gpio_irq_dispatcher(7);
}

static void gpio_irq_dispatcher_port8(void)
{
    gpio_irq_dispatcher(8);
}

static void gpio_irq_dispatcher_port9(void)
{
    gpio_irq_dispatcher(9);
}

static void gpio_irq_dispatcher_port10(void)
{
    gpio_irq_dispatcher(10);
}

static void gpio_irq_dispatcher_port11(void)
{
    gpio_irq_dispatcher(11);
}

static void gpio_irq_dispatcher_port12(void)
{
    gpio_irq_dispatcher(12);
}

static void gpio_irq_dispatcher_port13(void)
{
    gpio_irq_dispatcher(13);
}

static void gpio_irq_dispatcher_port14(void)
{
    gpio_irq_dispatcher(14);
}

static void (*irq_dispatcher_table[])(void) = {
    gpio_irq_dispatcher_port0,
    gpio_irq_dispatcher_port1,
    gpio_irq_dispatcher_port2,
    gpio_irq_dispatcher_port3,
    gpio_irq_dispatcher_port4,
    gpio_irq_dispatcher_port5,
    gpio_irq_dispatcher_port6,
    gpio_irq_dispatcher_port7,
    gpio_irq_dispatcher_port8,
    gpio_irq_dispatcher_port9,
    gpio_irq_dispatcher_port10,
    gpio_irq_dispatcher_port11,
    gpio_irq_dispatcher_port12,
    gpio_irq_dispatcher_port13,
    gpio_irq_dispatcher_port14
};


static IRQn_Type gpio_irq_allocate_channel(gpio_irq_t *obj)
{
#if defined (TARGET_MCU_PSOC6_M0)
    obj->cm0p_irq_src = ioss_interrupts_gpio_0_IRQn + obj->port_id;
    return cy_m0_nvic_allocate_channel(CY_GPIO_IRQN_ID + obj->port_id);
#else
    return (IRQn_Type)(ioss_interrupts_gpio_0_IRQn + obj->port_id);
#endif // M0
}

static void gpio_irq_release_channel(IRQn_Type channel, uint32_t port_id)
{
#if defined (TARGET_MCU_PSOC6_M0)
    cy_m0_nvic_release_channel(channel, CY_GPIO_IRQN_ID + port_id);
#endif //M0
}

static int gpio_irq_setup_channel(gpio_irq_t *obj)
{
    cy_stc_sysint_t irq_config;

    if (irq_port_usage[obj->port_id].pin_mask == 0) {
        IRQn_Type irqn = gpio_irq_allocate_channel(obj);
        if (irqn < 0) {
            return (-1);
        }
        irq_port_usage[obj->port_id].irqn = irqn;
        // Configure NVIC
        irq_config.intrPriority = GPIO_DEFAULT_IRQ_PRIORITY;
        irq_config.intrSrc = irqn;
#if defined (TARGET_MCU_PSOC6_M0)
        irq_config.cm0pSrc = obj->cm0p_irq_src;
#endif
        if (Cy_SysInt_Init(&irq_config, irq_dispatcher_table[obj->port_id]) != CY_SYSINT_SUCCESS) {
            return(-1);
        }

        irq_port_usage[obj->port_id].pin_mask |= (1 << obj->pin);
        NVIC_EnableIRQ(irqn);
    }

    return 0;
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin != NC) {
        obj->port_id = CY_PORT(pin);
        obj->port = Cy_GPIO_PortToAddr(obj->port_id);
        obj->pin = CY_PIN(pin);
        if (obj->pin > NUM_GPIO_PORT_PINS) {
            MBED_ASSERT("Invalid pin ID!");
            return (-1);
        }
        obj->handler = (uint32_t)handler;
        obj->id_arg = id;
        irq_objects[obj->port_id][obj->pin] = obj;
        if (gpio_irq_setup_channel(obj) != 0) {
            irq_objects[obj->port_id][obj->pin] = NULL;
            return (-1);
        }
    } else {
        return (-1);
    }
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    gpio_irq_disable(obj);
    // TODO: Need atomicity for the following operations.
    NVIC_DisableIRQ(irq_port_usage[obj->port_id].irqn);
    irq_port_usage[obj->port_id].pin_mask &= ~(1 << obj->pin);
    if (irq_port_usage[obj->port_id].pin_mask == 0) {
        gpio_irq_release_channel(irq_port_usage[obj->port_id].irqn, obj->port_id);
        irq_objects[obj->port_id][obj->pin] = NULL;
        return;
    }
    NVIC_EnableIRQ(irq_port_usage[obj->port_id].irqn);
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    if (enable) {
        if (event == IRQ_RISE) {
            if (obj->mode == IRQ_FALL) {
                obj->mode += IRQ_RISE;
                Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_BOTH);
            } else if (obj->mode == IRQ_NONE) {
                obj->mode = IRQ_RISE;
                Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_RISING);
                gpio_irq_enable(obj);
            }
        } else if (event == IRQ_FALL) {
            if (obj->mode == IRQ_RISE) {
                obj->mode += IRQ_FALL;
                Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_BOTH);
            } else if (obj->mode == IRQ_NONE) {
                obj->mode = IRQ_FALL;
                Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_FALLING);
                gpio_irq_enable(obj);
            }
        } else {
            obj->mode = IRQ_NONE;
            gpio_irq_disable(obj);
            Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_DISABLE);
        }
    } else if (obj->mode != IRQ_NONE) {
        if (event == IRQ_RISE) {
            if (obj->mode == IRQ_RISE) {
                obj->mode = IRQ_NONE;
                gpio_irq_disable(obj);
                Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_DISABLE);
            } else {
                obj->mode = IRQ_FALL;
                Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_FALLING);
            }
        } else if (event == IRQ_FALL) {
            if (obj->mode == IRQ_FALL) {
                obj->mode = IRQ_NONE;
                gpio_irq_disable(obj);
                Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_DISABLE);
            } else {
                obj->mode = IRQ_RISE;
                Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_RISING);
            }
        } else {
            obj->mode = IRQ_NONE;
            Cy_GPIO_SetInterruptEdge(obj->port, obj->pin, CY_GPIO_INTR_DISABLE);
            gpio_irq_disable(obj);
        }
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    Cy_GPIO_SetInterruptMask(obj->port, obj->pin, 1);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    Cy_GPIO_SetInterruptMask(obj->port, obj->pin, 0);
}

