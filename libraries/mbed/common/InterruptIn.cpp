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
#include "InterruptIn.h"

#if DEVICE_INTERRUPTIN

namespace mbed {

InterruptIn::InterruptIn(PinName pin) {
    gpio_irq_init(&gpio_irq, pin, (&InterruptIn::_irq_handler), (uint32_t)this);
    gpio_init(&gpio, pin, PIN_INPUT);
}

InterruptIn::~InterruptIn() {
    gpio_irq_free(&gpio_irq);
}

int InterruptIn::read() {
    return gpio_read(&gpio);
}

void InterruptIn::mode(PinMode pull) {
    gpio_mode(&gpio, pull);
}

pFunctionPointer_t InterruptIn::rise(void (*fptr)(void)) {
    pFunctionPointer_t pf = NULL;
    _rise.clear();
    if (fptr) {
        pf = _rise.add(fptr);
        gpio_irq_set(&gpio_irq, IRQ_RISE, 1);
    } else {
        gpio_irq_set(&gpio_irq, IRQ_RISE, 0);
    }
    return pf;
}

pFunctionPointer_t InterruptIn::rise_add_common(void (*fptr)(void), bool front) {
    if (NULL == fptr)
        return NULL;
    pFunctionPointer_t pf = front ? _rise.add_front(fptr) : _rise.add(fptr);
    gpio_irq_set(&gpio_irq, IRQ_RISE, 1);
    return pf;
}

bool InterruptIn::rise_remove(pFunctionPointer_t pf) {
    bool res = _rise.remove(pf);
    if (res && _rise.size() == 0)
        gpio_irq_set(&gpio_irq, IRQ_RISE, 0);
    return res;
}

pFunctionPointer_t InterruptIn::fall(void (*fptr)(void)) {
    pFunctionPointer_t pf = NULL;
    _fall.clear();
    if (fptr) {
        pf = _fall.add(fptr);
        gpio_irq_set(&gpio_irq, IRQ_FALL, 1);
    } else {
        gpio_irq_set(&gpio_irq, IRQ_FALL, 0);
    }
    return pf;
}

pFunctionPointer_t InterruptIn::fall_add_common(void (*fptr)(void), bool front) {
    if (NULL == fptr)
        return NULL;
    pFunctionPointer_t pf = front ? _fall.add_front(fptr) : _fall.add(fptr);
    gpio_irq_set(&gpio_irq, IRQ_FALL, 1);
    return pf;
}

bool InterruptIn::fall_remove(pFunctionPointer_t pf) {
    bool res = _fall.remove(pf);
    if (res && _fall.size() == 0)
        gpio_irq_set(&gpio_irq, IRQ_FALL, 0);
    return res;
}

void InterruptIn::_irq_handler(uint32_t id, gpio_irq_event event) {
    InterruptIn *handler = (InterruptIn*)id;
    switch (event) {
        case IRQ_RISE: handler->_rise.call(); break;
        case IRQ_FALL: handler->_fall.call(); break;
        case IRQ_NONE: break;
    }
}

#ifdef MBED_OPERATORS
InterruptIn::operator int() {
    return read();
}
#endif

} // namespace mbed

#endif
