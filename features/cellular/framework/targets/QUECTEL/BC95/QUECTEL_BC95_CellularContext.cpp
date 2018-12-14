/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include "QUECTEL_BC95_CellularContext.h"
#include "QUECTEL_BC95_CellularStack.h"

namespace mbed {

QUECTEL_BC95_CellularContext::QUECTEL_BC95_CellularContext(ATHandler &at, CellularDevice *device, const char *apn) :
    AT_CellularContext(at, device, apn)
{
}

QUECTEL_BC95_CellularContext::~QUECTEL_BC95_CellularContext()
{
}

#if !NSAPI_PPP_AVAILABLE
NetworkStack *QUECTEL_BC95_CellularContext::get_stack()
{
    if (!_stack) {
        _stack = new QUECTEL_BC95_CellularStack(_at, _cid, _ip_stack_type);
    }
    return _stack;
}
#endif // #if !NSAPI_PPP_AVAILABLE

bool QUECTEL_BC95_CellularContext::stack_type_supported(nsapi_ip_stack_t stack_type)
{
    return stack_type == IPV4_STACK ? true : false;
}

} /* namespace mbed */
