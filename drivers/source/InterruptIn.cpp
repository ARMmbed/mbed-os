/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "drivers/InterruptIn.h"

#if DEVICE_INTERRUPTIN

namespace mbed {

// Note: This single-parameter constructor exists to maintain binary
//       compatibility.
//       If not for that, we could simplify by having only the 2-param
//       constructor, with a default value for the PinMode.
InterruptIn::InterruptIn(PinName pin) : gpio(),
    gpio_irq(),
    _rise(),
    _fall()
{
    // No lock needed in the constructor
    irq_init(pin);
    gpio_init_in(&gpio, pin);
}

InterruptIn::InterruptIn(PinName pin, PinMode mode) :
    gpio(),
    gpio_irq(),
    _rise(),
    _fall()
{
    // No lock needed in the constructor
    irq_init(pin);
    gpio_init_in_ex(&gpio, pin, mode);
}

void InterruptIn::irq_init(PinName pin)
{
    gpio_irq_init(&gpio_irq, pin, (&InterruptIn::_irq_handler), (uint32_t)this);
}

InterruptIn::~InterruptIn()
{
    // No lock needed in the destructor
    gpio_irq_free(&gpio_irq);
}

int InterruptIn::read()
{
    // Read only
    return gpio_read(&gpio);
}

void InterruptIn::mode(PinMode pull)
{
    core_util_critical_section_enter();
    gpio_mode(&gpio, pull);
    core_util_critical_section_exit();
}

void InterruptIn::rise(Callback<void()> func)
{
    core_util_critical_section_enter();
    if (func) {
        _rise = func;
        gpio_irq_set(&gpio_irq, IRQ_RISE, 1);
    } else {
        _rise = nullptr;
        gpio_irq_set(&gpio_irq, IRQ_RISE, 0);
    }
    core_util_critical_section_exit();
}

void InterruptIn::fall(Callback<void()> func)
{
    core_util_critical_section_enter();
    if (func) {
        _fall = func;
        gpio_irq_set(&gpio_irq, IRQ_FALL, 1);
    } else {
        _fall = nullptr;
        gpio_irq_set(&gpio_irq, IRQ_FALL, 0);
    }
    core_util_critical_section_exit();
}

void InterruptIn::_irq_handler(uint32_t id, gpio_irq_event event)
{
    InterruptIn *handler = (InterruptIn *)id;
    switch (event) {
        case IRQ_RISE:
            if (handler->_rise) {
                handler->_rise();
            }
            break;
        case IRQ_FALL:
            if (handler->_fall) {
                handler->_fall();
            }
            break;
        case IRQ_NONE:
            break;
    }
}

void InterruptIn::enable_irq()
{
    core_util_critical_section_enter();
    gpio_irq_enable(&gpio_irq);
    core_util_critical_section_exit();
}

void InterruptIn::disable_irq()
{
    core_util_critical_section_enter();
    gpio_irq_disable(&gpio_irq);
    core_util_critical_section_exit();
}

InterruptIn::operator int()
{
    // Underlying call is atomic
    return read();
}

} // namespace mbed

#endif
