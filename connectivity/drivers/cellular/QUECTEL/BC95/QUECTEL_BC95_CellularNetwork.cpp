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

#include "QUECTEL_BC95_CellularNetwork.h"

using namespace mbed;

QUECTEL_BC95_CellularNetwork::QUECTEL_BC95_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularNetwork(atHandler, device)
{
    _op_act = RAT_NB1;
}

QUECTEL_BC95_CellularNetwork::~QUECTEL_BC95_CellularNetwork()
{
}

nsapi_error_t QUECTEL_BC95_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opRat)
{
    if (opRat != RAT_NB1) {
        // only rat that is supported by this modem
        _op_act = RAT_NB1;
        return NSAPI_ERROR_UNSUPPORTED;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_BC95_CellularNetwork::clear()
{
    nsapi_error_t err = AT_CellularNetwork::clear();
#if MBED_CONF_CELLULAR_CONTROL_PLANE_OPT
    if (!err) {
        err = _at.at_cmd_discard("+CGDCONT", "=", "%d", 0);
#ifdef MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN
        err = _at.at_cmd_discard("+CGDCONT", "=", "%d%s%s", 1, "NONIP", MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN);
#endif
        if (!err) {
            err = _at.at_cmd_discard("+CIPCA", "=", "%d%d", 3, 1); // EPS Attach without PDN connection
        }
        if (!err) {
            _at.lock();
            _at.cmd_start("AT+NCONFIG=\"AUTOCONNECT\",\"TRUE\""); // disable auto connect to IP context
            _at.cmd_stop_read_resp();
            err = _at.unlock_return_error();
        }
    }
#endif
    return err;
}
