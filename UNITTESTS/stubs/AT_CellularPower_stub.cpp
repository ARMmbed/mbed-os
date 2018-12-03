/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "AT_CellularPower_stub.h"
#include "CellularUtil.h"
#include "CellularLog.h"

using namespace mbed_cellular_util;
using namespace mbed;

nsapi_error_t AT_CellularPower_stub::nsapi_error_value = 0;
int AT_CellularPower_stub::fail_counter = 0;
int AT_CellularPower_stub::set_at_fail_counter = 0;

AT_CellularPower::AT_CellularPower(ATHandler &at) : AT_CellularBase(at)
{
}

AT_CellularPower::~AT_CellularPower()
{
}

nsapi_error_t AT_CellularPower::on()
{
    if (AT_CellularPower_stub::fail_counter) {
        AT_CellularPower_stub::fail_counter--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return AT_CellularPower_stub::nsapi_error_value;
}

nsapi_error_t AT_CellularPower::off()
{
    if (AT_CellularPower_stub::fail_counter) {
        AT_CellularPower_stub::fail_counter--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return AT_CellularPower_stub::nsapi_error_value;
}
