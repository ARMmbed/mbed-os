/*
 * Copyright (c) 2018-2019 ARM Limited
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

// Suppress deprecation warnings since this whole
// class is deprecated already
#include "mbed_toolchain.h"
#undef MBED_DEPRECATED_SINCE
#define MBED_DEPRECATED_SINCE(...)

#include "platform/CallChain.h"
#include "cmsis.h"
#include "platform/mbed_critical.h"

namespace mbed {

class CallChainLink {
public:
    CallChainLink(): cb(), next(NULL)
    {
        // No work to do
    }

    CallChainLink(Callback<void()> &callback): cb(callback), next(NULL)
    {
        // No work to do
    }
    Callback<void()> cb;
    CallChainLink *next;
};

CallChain::CallChain(int size) : _chain(NULL)
{
    // No work to do
}

CallChain::~CallChain()
{
    clear();
}

pFunctionPointer_t CallChain::add(Callback<void()> func)
{
    CallChainLink *new_link = new CallChainLink(func);
    if (NULL == _chain) {
        _chain = new_link;
        return &new_link->cb;
    }

    CallChainLink *link = _chain;
    while (true) {
        if (NULL == link->next) {
            link->next = new_link;
            return &new_link->cb;
        }
        link = link->next;
    }
}

pFunctionPointer_t CallChain::add_front(Callback<void()> func)
{
    CallChainLink *link = new CallChainLink(func);
    link->next = _chain;
    _chain = link;
    return &link->cb;
}

int CallChain::size() const
{
    CallChainLink *link = _chain;
    int elements = 0;
    while (link != NULL) {
        elements++;
        link = link->next;
    }
    return elements;
}

pFunctionPointer_t CallChain::get(int idx) const
{
    CallChainLink *link = _chain;
    for (int i = 0; i < idx; i++) {
        if (NULL == link) {
            break;
        }
        link = link->next;
    }
    return &link->cb;
}

int CallChain::find(pFunctionPointer_t f) const
{
    CallChainLink *link = _chain;
    int i = 0;
    while (link != NULL) {
        if (f == &link->cb) {
            return i;
        }
        i++;
        link = link->next;
    }
    return -1;
}

void CallChain::clear()
{
    CallChainLink *link = _chain;
    _chain = NULL;
    while (link != NULL) {
        CallChainLink *temp = link->next;
        delete link;
        link = temp;
    }
}

bool CallChain::remove(pFunctionPointer_t f)
{
    CallChainLink *link = _chain;
    while (link != NULL) {
        if (f == &link->cb) {
            delete link;
            return true;
        }
        link = link->next;
    }
    return false;
}

void CallChain::call()
{
    CallChainLink *link = _chain;
    while (link != NULL) {
        link->cb.call();
        link = link->next;
    }
}

} // namespace mbed
