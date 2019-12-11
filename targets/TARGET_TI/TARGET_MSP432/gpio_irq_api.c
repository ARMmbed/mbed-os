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

/* Low-level implementation of gpio interrupts for MSP432.
 * This implementation uses bit-banding, so no masking of bits
 * is necessary during simple get/set operations for a single pin.
 *
 * See hal/gpio_api.h for a detailed description of the following
 * functions.
 */
#if DEVICE_INTERRUPTIN

#include "gpio_irq_api.h"
#include "mbed_error.h"
#include "gpio_msp432.h"

static uint32_t irq_ids   [6][8] = {0};
static uint8_t  both_edges[6][8] = {0};

static gpio_irq_handler irq_handler = 0;


/** Initialize the GPIO IRQ pin
 *
 * @param obj     The GPIO object to initialize
 * @param pin     The GPIO pin name
 * @param handler The handler to be attached to GPIO IRQ
 * @param id      The object ID (id != 0, 0 is reserved)
 * @return -1 if pin is NC, 0 otherwise
 */
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    if (pin == (PinName)NC) {
        obj->pin_base = 0;
        return -1;
    }
    uint8_t port_index = MSP432_PORT_IDX(pin);
    uint8_t pin_index  = MSP432_PIN_IDX(pin);
    if (port_index > 5 || pin_index > 7) {
        obj->pin_base = 0;
        error("IRQs not supported on GPIO P%d.%d\r\n", port_index + 1, pin_index);
        return -1;
    }
    // Initialize the gpio_t object
    obj->pin_base = PIN_BASE(port_index, pin_index);
    obj->port = port_index;
    obj->pin  = pin_index;
    obj->rise = 0;
    obj->fall = 0;
    // Store irq handler. We do not have to store
    // a separate handler for every pin, because the
    // InterruptIn-driver will always use the same one...
    irq_handler = handler;
    // Store the ID for this specific pin. It will be
    // used as a pointer to one InterruptIn instance by
    // the InterruptIn class.
    irq_ids[port_index][pin_index] = id;
    // Clear pending IRQ
    PIN_REG(obj->pin_base, REG_IFG) = 0;
    // Enable IRQs on this port
    NVIC_EnableIRQ((IRQn_Type)(35 + port_index));
    return 0;
}

/** Release the GPIO IRQ PIN
 *
 * @param obj The gpio object
 */
void gpio_irq_free(gpio_irq_t *obj)
{
    // Disable the interrupts
    gpio_irq_disable(obj);
    // Clear the ID
    irq_ids[obj->port][obj->pin] = 0;
    // Clear the gpio_irq_t object
    obj->pin_base = 0;
    obj->port = 0;
    obj->pin  = 0;
    obj->rise = 0;
    obj->fall = 0;
}

/** Enable/disable pin IRQ event
 *
 * @param obj    The GPIO object
 * @param event  The GPIO IRQ event
 * @param enable The enable flag
 */
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    // Disable irqs for this pin while we are in the
    // configuration process (so we have a chance to
    // clear spurious interrupts.
    PIN_REG(obj->pin_base, REG_IE) = 0;
    switch (event) {
        case IRQ_RISE: {
            // Listen to rising edges
            PIN_REG(obj->pin_base, REG_IES) = 0;
            obj->rise = enable;
            // Check if we have enabled both edges
            both_edges[obj->port][obj->pin] = obj->rise && obj->fall;
            break;
        }
        case IRQ_FALL: {
            // Listen to falling edges
            PIN_REG(obj->pin_base, REG_IES) = 1;
            obj->fall = enable;
            // Check if we have enabled both edges
            both_edges[obj->port][obj->pin] = obj->rise && obj->fall;
            break;
        }
        case IRQ_NONE:
        default:
            break;
    }
    // Enable IRQ as requested
    enable ? gpio_irq_enable(obj) : gpio_irq_disable(obj);
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    // Clear pending interrupts (just in case...)
    PIN_REG(obj->pin_base, REG_IFG) = 0;
    // Check if both edges need to be detected and
    // set initial edge select accordingly
    if (both_edges[obj->port][obj->pin]) {
        PIN_REG(obj->pin_base, REG_IES) = PIN_REG(obj->pin_base, REG_IN);
        // Make sure we don't accidently trigger a irq!
        PIN_REG(obj->pin_base, REG_IFG) = 0;
    }
    // Enable irqs for this pin
    PIN_REG(obj->pin_base, REG_IE) = 1;
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    // Disable irqs for this pin
    PIN_REG(obj->pin_base, REG_IE)  = 0;
    // Clear pending interrupts
    PIN_REG(obj->pin_base, REG_IFG) = 0;
}

/**********************************/
/* Interrupt handlers for P1...P6 */
/**********************************/
void handle_interrupt_in(uint8_t port, uint8_t pin)
{
    __IO uint8_t *pin_base = PIN_BASE(port, pin);
    // Call the second-stage handler
    if (irq_handler) {
        irq_handler(irq_ids[port][pin],
                    PIN_REG(pin_base, REG_IN) ? IRQ_RISE : IRQ_FALL);
    }
    // Change the edge select when listening on both edges
    if (both_edges[port][pin]) {
        PIN_REG(pin_base, REG_IES) = PIN_REG(pin_base, REG_IN);
        // Make sure we don't accidently trigger a irq!
        PIN_REG(pin_base, REG_IFG) = 0;
    }
}

void PORT1_IRQHandler(void)
{
    uint32_t pin = P1->IV >> 1;
    if (pin--) {
        handle_interrupt_in(0, pin);
    }
}
void PORT2_IRQHandler(void)
{
    uint32_t pin = P2->IV >> 1;
    if (pin--) {
        handle_interrupt_in(1, pin);
    }
}
void PORT3_IRQHandler(void)
{
    uint32_t pin = P3->IV >> 1;
    if (pin--) {
        handle_interrupt_in(2, pin);
    }
}
void PORT4_IRQHandler(void)
{
    uint32_t pin = P4->IV >> 1;
    if (pin--) {
        handle_interrupt_in(3, pin);
    }
}
void PORT5_IRQHandler(void)
{
    uint32_t pin = P5->IV >> 1;
    if (pin--) {
        handle_interrupt_in(4, pin);
    }
}
void PORT6_IRQHandler(void)
{
    uint32_t pin = P6->IV >> 1;
    if (pin--) {
        handle_interrupt_in(5, pin);
    }
}

#endif /* DEVICE_INTERRUPTIN */
