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
#include "QUECTEL_UG96_CellularContext.h"

namespace mbed {

QUECTEL_UG96_CellularContext::QUECTEL_UG96_CellularContext(ATHandler &at, CellularDevice *device, const char *apn, bool cp_req, bool nonip_req) :
    AT_CellularContext(at, device, apn, cp_req, nonip_req)
{
}

QUECTEL_UG96_CellularContext::~QUECTEL_UG96_CellularContext()
{
}

nsapi_error_t QUECTEL_UG96_CellularContext::do_user_authentication()
{
    nsapi_error_t err = NSAPI_ERROR_OK;
    if (_pwd && _uname) {
        err = _at.at_cmd_discard("+QICSGP", "=", "%d%d%s%s%s%d", _cid, 1, _apn,
                                 _uname, _pwd, _authentication_type);
        if (err != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_AUTH_FAILURE;
        }
    }

    return err;
}

} /* namespace mbed */
