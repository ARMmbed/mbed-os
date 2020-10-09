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

#include "DynamicPinList.h"

DynamicPinList::DynamicPinList()
{

}

DynamicPinList::DynamicPinList(const PinList *pin_list)
{
    for (uint32_t i = 0; i < pin_list->count; i++) {
        _pins.push_back(pin_list->pins[i]);
    }
}

DynamicPinList::DynamicPinList(const DynamicPinList &other)
{
    _pins = other._pins;
}

void DynamicPinList::add(PinName pin)
{
    _pins.push_back(pin);
}

bool DynamicPinList::has_pin(PinName pin) const
{
    for (uint32_t i = 0; i < _pins.size(); i++) {
        if (pin == _pins[i]) {
            return true;
        }
    }
    return false;
}

void DynamicPinList::clear()
{
    _pins.clear();
}

uint32_t DynamicPinList::count() const
{
    return _pins.size();
}

PinName DynamicPinList::get(uint32_t index) const
{
    return index < _pins.size() ? _pins[index] : NC;
}

int DynamicPinList::index(PinName pin) const
{
    for (uint32_t i = 0; i < _pins.size(); i++) {
        if (pin == _pins[i]) {
            return i;
        }
    }
    return -1;
}
