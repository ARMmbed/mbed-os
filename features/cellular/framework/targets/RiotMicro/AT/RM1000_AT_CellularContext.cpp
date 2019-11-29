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
#include "RM1000_AT_CellularContext.h"

#include "APN_db.h"
#include "RM1000_AT_CellularStack.h"
#include "mbed-trace/mbed_trace.h"
#ifndef TRACE_GROUP
#define TRACE_GROUP  "RIOT"
#endif // TRACE_GROUP

namespace mbed {

RM1000_AT_CellularContext::RM1000_AT_CellularContext(ATHandler &at, CellularDevice *device, const char *apn, bool cp_req, bool nonip_req) :
    AT_CellularContext(at, device, apn, cp_req, nonip_req)
{
    tr_debug("Calling RM1000_AT_CellularContext::RM1000_AT_CellularContext");
}

RM1000_AT_CellularContext::~RM1000_AT_CellularContext()
{
    tr_debug("Calling RM1000_AT_CellularContext::~RM1000_AT_CellularContext");
}

NetworkStack *RM1000_AT_CellularContext::get_stack()
{
    tr_debug("Calling RM1000_AT_CellularContext::get_stack");

    if (_pdp_type == NON_IP_PDP_TYPE || _cp_in_use) {
        tr_error("Requesting stack for NON-IP context! Should request control plane netif: get_cp_netif()");
        return NULL;
    }
    if (!_stack) {
        _stack = new RM1000_AT_CellularStack(_at, _cid, (nsapi_ip_stack_t)_pdp_type, *get_device());
    }

    return _stack;
}

} /* namespace mbed */
