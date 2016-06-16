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
#include "CAN.h"

#if DEVICE_CAN

#include "cmsis.h"

namespace mbed {

CAN::CAN(PinName rd, PinName td) : _can(), _irq() {
    // No lock needed in constructor
    can_init(&_can, rd, td);
    can_irq_init(&_can, (&CAN::_irq_handler), (uint32_t)this);
}

CAN::~CAN() {
    // No lock needed in destructor
    can_irq_free(&_can);
    can_free(&_can);
}

int CAN::frequency(int f) {
    lock();
    int ret = can_frequency(&_can, f);
    unlock();
    return ret;
}

int CAN::write(CANMessage msg) {
    lock();
    int ret = can_write(&_can, msg, 0);
    unlock();
    return ret;
}

int CAN::read(CANMessage &msg, int handle) {
    lock();
    int ret = can_read(&_can, &msg, handle);
    unlock();
    return ret;
}

void CAN::reset() {
    lock();
    can_reset(&_can);
    unlock();
}

unsigned char CAN::rderror() {
    lock();
    int ret = can_rderror(&_can);
    unlock();
    return ret;
}

unsigned char CAN::tderror() {
    lock();
    int ret = can_tderror(&_can);
    unlock();
    return ret;
}

void CAN::monitor(bool silent) {
    lock();
    can_monitor(&_can, (silent) ? 1 : 0);
    unlock();
}

int CAN::mode(Mode mode) {
    lock();
    int ret = can_mode(&_can, (CanMode)mode);
    unlock();
    return ret;
}

int CAN::filter(unsigned int id, unsigned int mask, CANFormat format, int handle) {
    lock();
    int ret = can_filter(&_can, id, mask, format, handle);
    unlock();
    return ret;
}

void CAN::attach(Callback<void()> func, IrqType type) {
    lock();
    if (func) {
        _irq[(CanIrqType)type].attach(func);
        can_irq_set(&_can, (CanIrqType)type, 1);
    } else {
        can_irq_set(&_can, (CanIrqType)type, 0);
    }
    unlock();
}

void CAN::_irq_handler(uint32_t id, CanIrqType type) {
    CAN *handler = (CAN*)id;
    handler->_irq[type].call();
}

void CAN::lock() {
    _mutex.lock();
}

void CAN::unlock() {
    _mutex.unlock();
}

} // namespace mbed

#endif
