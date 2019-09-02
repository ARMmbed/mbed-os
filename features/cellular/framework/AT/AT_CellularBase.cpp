/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "AT_CellularBase.h"
#include "CellularLog.h"

using namespace mbed;

AT_CellularBase::AT_CellularBase(ATHandler &at) : _at(at)
{
}

ATHandler &AT_CellularBase::get_at_handler()
{
    return _at;
}

device_err_t AT_CellularBase::get_device_error() const
{
    return _at.get_last_device_error();
}

const intptr_t *AT_CellularBase::_property_array = NULL;

void AT_CellularBase::set_cellular_properties(const intptr_t *property_array)
{
    if (!property_array) {
        tr_warning("trying to set an empty cellular property array");
        return;
    }

    _property_array = property_array;
}

intptr_t AT_CellularBase::get_property(CellularProperty key)
{
    if (_property_array) {
        return _property_array[key];
    } else {
        return 0;
    }
}
