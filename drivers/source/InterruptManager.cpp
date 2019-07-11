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
#include "cmsis.h"
#if defined(NVIC_NUM_VECTORS)

// Suppress deprecation warnings since this whole
// class is deprecated already
#include "mbed_toolchain.h"
#undef MBED_DEPRECATED_SINCE
#define MBED_DEPRECATED_SINCE(...)

#include "drivers/InterruptManager.h"
#include "platform/mbed_critical.h"
#include <string.h>

#define CHAIN_INITIAL_SIZE    4

namespace mbed {

typedef void (*pvoidf)(void);

InterruptManager *InterruptManager::_instance = (InterruptManager *)NULL;

InterruptManager *InterruptManager::get()
{

    if (NULL == _instance) {
        InterruptManager *temp = new InterruptManager();

        // Atomically set _instance
        core_util_critical_section_enter();
        if (NULL == _instance) {
            _instance = temp;
        }
        core_util_critical_section_exit();

        // Another thread got there first so delete ours
        if (temp != _instance) {
            delete temp;
        }

    }
    return _instance;
}

InterruptManager::InterruptManager()
{
    // No mutex needed in constructor
    memset(_chains, 0, NVIC_NUM_VECTORS * sizeof(CallChain *));
}

void InterruptManager::destroy()
{
    // Not a good idea to call this unless NO interrupt at all
    // is under the control of the handler; otherwise, a system crash
    // is very likely to occur
    if (NULL != _instance) {
        delete _instance;
        _instance = (InterruptManager *)NULL;
    }
}

InterruptManager::~InterruptManager()
{
    for (int i = 0; i < NVIC_NUM_VECTORS; i++)
        if (NULL != _chains[i]) {
            delete _chains[i];
        }
}

bool InterruptManager::must_replace_vector(IRQn_Type irq)
{
    lock();

    int ret = false;
    int irq_pos = get_irq_index(irq);
    if (NULL == _chains[irq_pos]) {
        _chains[irq_pos] = new CallChain(CHAIN_INITIAL_SIZE);
        _chains[irq_pos]->add((pvoidf)NVIC_GetVector(irq));
        ret = true;
    }
    unlock();
    return ret;
}

pFunctionPointer_t InterruptManager::add_common(void (*function)(void), IRQn_Type irq, bool front)
{
    lock();
    int irq_pos = get_irq_index(irq);
    bool change = must_replace_vector(irq);

    pFunctionPointer_t pf = front ? _chains[irq_pos]->add_front(function) : _chains[irq_pos]->add(function);
    if (change) {
        NVIC_SetVector(irq, (uint32_t)&InterruptManager::static_irq_helper);
    }
    unlock();
    return pf;
}

bool InterruptManager::remove_handler(pFunctionPointer_t handler, IRQn_Type irq)
{
    int irq_pos = get_irq_index(irq);
    bool ret = false;

    lock();
    if (_chains[irq_pos] != NULL) {
        if (_chains[irq_pos]->remove(handler)) {
            ret = true;
        }
    }
    unlock();

    return ret;
}

void InterruptManager::irq_helper()
{
    _chains[__get_IPSR()]->call();
}

int InterruptManager::get_irq_index(IRQn_Type irq)
{
    // Pure function - no lock needed
    return (int)irq + NVIC_USER_IRQ_OFFSET;
}

void InterruptManager::static_irq_helper()
{
    InterruptManager::get()->irq_helper();
}

void InterruptManager::lock()
{
    _mutex.lock();
}

void InterruptManager::unlock()
{
    _mutex.unlock();
}

} // namespace mbed

#endif
