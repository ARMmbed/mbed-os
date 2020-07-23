/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "UBLOX_N2XX_CellularContext.h"
#include "UBLOX_N2XX_CellularStack.h"
#include "CellularLog.h"
#include "APN_db.h"

namespace mbed {

UBLOX_N2XX_CellularContext::UBLOX_N2XX_CellularContext(ATHandler &at, CellularDevice *device, const char *apn, bool cp_req, bool nonip_req):
    AT_CellularContext(at, device, apn, cp_req, nonip_req)
{
}

UBLOX_N2XX_CellularContext::~UBLOX_N2XX_CellularContext()
{
}

#if !NSAPI_PPP_AVAILABLE
NetworkStack *UBLOX_N2XX_CellularContext::get_stack()
{
    if (_pdp_type == NON_IP_PDP_TYPE || _cp_in_use) {
        tr_error("Requesting stack for NON-IP context! Should request control plane netif: get_cp_netif()");
        return NULL;
    }
    if (!_stack) {
        _stack = new UBLOX_N2XX_CellularStack(_at, _cid, (nsapi_ip_stack_t)_pdp_type, *get_device());
    }
    return _stack;
}
#endif

const char *UBLOX_N2XX_CellularContext::get_nonip_context_type_str()
{
    return "NONIP";
}

} /* namespace mbed */
