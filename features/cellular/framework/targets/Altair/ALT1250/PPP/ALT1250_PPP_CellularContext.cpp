/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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
#include "ALT1250_PPP_CellularContext.h"

namespace mbed {

ALT1250_PPP_CellularContext::ALT1250_PPP_CellularContext(ATHandler &at, CellularDevice *device, const char *apn, bool cp_req, bool nonip_req) :
    AT_CellularContext(at, device, apn, cp_req, nonip_req)
{
}

ALT1250_PPP_CellularContext::~ALT1250_PPP_CellularContext()
{
}

nsapi_error_t ALT1250_PPP_CellularContext::do_user_authentication()
{
    nsapi_error_t err = NSAPI_ERROR_OK;
    if (_pwd && _uname) {
        err = _at.at_cmd_discard("%PPPAUTH", "=", "%d%d%s%s", _cid, _authentication_type,
                                 _uname, _pwd);
        if (err != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_AUTH_FAILURE;
        }
    }

    return err;
}

} /* namespace mbed */
