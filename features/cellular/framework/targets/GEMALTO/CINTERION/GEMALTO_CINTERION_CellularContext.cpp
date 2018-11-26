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
#include "GEMALTO_CINTERION_CellularContext.h"
#include "GEMALTO_CINTERION_CellularStack.h"
#include "GEMALTO_CINTERION.h"

namespace mbed {

GEMALTO_CINTERION_CellularContext::GEMALTO_CINTERION_CellularContext(ATHandler &at, CellularDevice *device,
                                                                     const char *apn) : AT_CellularContext(at, device, apn)
{
}

GEMALTO_CINTERION_CellularContext::~GEMALTO_CINTERION_CellularContext()
{
}

#if !NSAPI_PPP_AVAILABLE
NetworkStack *GEMALTO_CINTERION_CellularContext::get_stack()
{
    if (!_stack) {
        _stack = new GEMALTO_CINTERION_CellularStack(_at, _apn, _cid, _ip_stack_type);
    }
    return _stack;
}
#endif // NSAPI_PPP_AVAILABLE

bool GEMALTO_CINTERION_CellularContext::stack_type_supported(nsapi_ip_stack_t requested_stack)
{
    if (GEMALTO_CINTERION::get_module() == GEMALTO_CINTERION::ModuleBGS2) {
        return (requested_stack == IPV4_STACK);
    }
    return (requested_stack == IPV4_STACK || requested_stack == IPV6_STACK);
}

} /* namespace mbed */
