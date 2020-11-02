/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include "LinkedList.h"
#include "LinkEntry.h"
#include "mbed_assert.h"

LinkedListBase::LinkedListBase(): _head(0), _tail(0)
{

}

LinkedListBase::~LinkedListBase()
{

}

LinkEntry *LinkedListBase::head()
{
    return _head;
}

void LinkedListBase::enqueue(LinkEntry *entry)
{
    entry->_next = NULL;
    if (_tail == NULL) {
        _head = entry;
    } else {
        _tail->_next = entry;
    }
    _tail = entry;
}

LinkEntry *LinkedListBase::dequeue()
{
    if (_head == NULL) {
        return NULL;
    }
    if (_head->_next == NULL) {
        _tail = NULL;
    }
    LinkEntry *entry = _head;
    _head = _head->_next;
    entry->_next = NULL;
    return entry;
}

void LinkedListBase::remove(LinkEntry *entry)
{
    LinkEntry *prev = NULL;
    LinkEntry *cur = _head;
    while (cur != entry) {
        if (cur == NULL) {
            // Element is not in the list
            return;
        }
        prev = cur;
        cur = cur->_next;
    }

    if (prev != NULL) {
        prev->_next = entry->_next;
    }
    if (entry == _head) {
        _head = entry->_next;
    }
    if (entry == _tail) {
        _tail = prev;
    }
    entry->_next = NULL;
}
