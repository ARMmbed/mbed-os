/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DYNAMIC_PIN_LIST_H
#define DYNAMIC_PIN_LIST_H

#include "pinmap.h"
#include <vector>

class DynamicPinList {
public:

    /**
     * Create an empty pin list
     */
    DynamicPinList();

    /**
     * Create a pin list with the given contents
     *
     * @param pin_list List of pins to create this list from
     */
    DynamicPinList(const PinList *pin_list);

    /**
     * Create a copy of another list
     *
     * @param other Other object to copy contruct this from
     */
    DynamicPinList(const DynamicPinList &other);

    /**
     * Add a pin to the pin list
     *
     * @param pin Pin to add to this pin list
     */
    void add(PinName pin);

    /**
     * Check if the given pin is in this list
     *
     * @param pin Pin to check for in the list
     * @return true if the pin is in the list, false otherwise
     */
    bool has_pin(PinName pin) const;

    /**
     * Empty this pin list
     */
    void clear();

    /**
     * Return the number of pins in this list
     *
     * @return Elements in this list
     */
    uint32_t count() const;

    /**
     * Get the pin at the given index
     *
     * @return Pin at this position
     */
    PinName get(uint32_t index) const;

    /**
     * Get the location of the given pin
     *
     * @param pin Pin to get the index of
     * @return pin index or -1 if pin is not in the list
     */
    int index(PinName pin) const;

private:
    std::vector<PinName> _pins;
};

#endif
