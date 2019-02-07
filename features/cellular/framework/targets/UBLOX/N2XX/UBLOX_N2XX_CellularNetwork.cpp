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

UBLOX_N2XX_CellularNetwork::UBLOX_N2XX_CellularNetwork(ATHandler &atHandler) : AT_CellularNetwork(atHandler)
{
    _op_act = RAT_UNKNOWN;
}

UBLOX_N2XX_CellularNetwork::~UBLOX_N2XX_CellularNetwork()
{
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_ERROR_CONNECTION_LOST);
    }
}

AT_CellularNetwork::RegistrationMode UBLOX_N2XX_CellularNetwork::has_registration(RegistrationType reg_type)
{
    return (reg_type == C_EREG) ? RegistrationModeLAC : RegistrationModeDisable;
}

nsapi_error_t UBLOX_N2XX_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opRat) // Not understandable, RAT (+CEDRXS)
{
    switch (opRat) {
#if defined(TARGET_UBLOX_C030_N211)
    case RAT_NB1:
        break;
#endif
    default: {
        _op_act = RAT_UNKNOWN;
        return NSAPI_ERROR_UNSUPPORTED;
    }
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_N2XX_CellularNetwork::get_signal_quality(int &rssi, int &ber)
{
    _at.lock();

    _at.cmd_start("AT+CSQ");
    _at.cmd_stop();

    _at.resp_start("+CSQ:");
    rssi = _at.read_int();
    ber = _at.read_int();
    _at.resp_stop();
    if (rssi < 0 || ber < 0) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (rssi == 99) {
        rssi = 0;
    }

    return _at.unlock_return_error();
}
