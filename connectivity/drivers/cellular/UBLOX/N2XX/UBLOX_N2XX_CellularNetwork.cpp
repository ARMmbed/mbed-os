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

#include "UBLOX_N2XX_CellularNetwork.h"

using namespace mbed;

UBLOX_N2XX_CellularNetwork::UBLOX_N2XX_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularNetwork(atHandler, device)
{
}

nsapi_error_t UBLOX_N2XX_CellularNetwork::clear()
{
    nsapi_error_t err = AT_CellularNetwork::clear();
#if MBED_CONF_CELLULAR_CONTROL_PLANE_OPT
    if (!err) {
#ifdef MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN
        (void) _at.at_cmd_discard("+CGDCONT", "=", "%d%s%s", 1, "NONIP", MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN);
#endif
        (void) _at.at_cmd_discard("+CIPCA", "=", "%d%d", 3, 1); // EPS Attach without PDN connection
        _at.lock();
        _at.cmd_start("AT+NCONFIG=\"AUTOCONNECT\",\"FALSE\""); // disable auto connect to IP context
        _at.cmd_stop_read_resp();
        _at.unlock();
    }
#endif
    return err;
}

nsapi_error_t UBLOX_N2XX_CellularNetwork::set_ciot_optimization_config(CIoT_Supported_Opt supported_opt,
                                                                       CIoT_Preferred_UE_Opt preferred_opt,
                                                                       Callback<void(CIoT_Supported_Opt)> network_support_cb)
{
    _ciotopt_network_support_cb = network_support_cb;
    nsapi_error_t err = _at.at_cmd_discard("+CRTDCP", "=", "%d", 1);
    return err;
}
