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

#ifndef MBED_LINKED_LIST_BASE_H
#define MBED_LINKED_LIST_BASE_H

#include "LinkEntry.h"

/**
 * \defgroup drivers_LinkedListBase LinkedListBase class
 * \ingroup drivers-internal-api-usb
 * @{
 */
class LinkedListBase {
public:
    LinkedListBase();
    ~LinkedListBase();

    /**
     * Return the element at the head of the list
     *
     * @return The element at the head of the list or NULL if the list is empty
     */
    LinkEntry *head();

    /**
     * Add an element to the tail of the list
     *
     * @param entry New element to add
     */
    void enqueue(LinkEntry *entry);

    /**
     * Remove the element at the head of the list
     *
     * @return The element at the head of the list or NULL if the list is empty
     */
    LinkEntry *dequeue();

    /**
     * Remove the specified element if it is in the list
     *
     * @param entry Element to remove from the list
     */
    void remove(LinkEntry *entry);

private:
    LinkEntry *_head;
    LinkEntry *_tail;
};

/** @}*/

#endif
