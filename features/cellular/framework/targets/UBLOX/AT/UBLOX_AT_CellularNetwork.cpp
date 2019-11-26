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

#include "UBLOX_AT_CellularNetwork.h"
#include "rtos/ThisThread.h"

using namespace mbed;

UBLOX_AT_CellularNetwork::UBLOX_AT_CellularNetwork(ATHandler &atHandler) : AT_CellularNetwork(atHandler)
{
    _op_act = RAT_UNKNOWN;
}

UBLOX_AT_CellularNetwork::~UBLOX_AT_CellularNetwork()
{
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_ERROR_CONNECTION_LOST);
    }
}

nsapi_error_t UBLOX_AT_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opRat)
{
    nsapi_error_t ret = NSAPI_ERROR_OK;
    CellularNetwork::AttachStatus status;

    get_attach(status);
    if (status == Attached) {
        tr_debug("RAT should only be set in detached state");
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _at.lock();
    switch (opRat) {
        case RAT_EGPRS:
#if defined (UBX_MDM_SARA_R412M)
            _at.at_cmd_discard("+URAT", "=", "%d%d", 9, 8);
            break;
#endif
#if defined(UBX_MDM_SARA_U201)
        case RAT_GSM:
            _at.at_cmd_discard("+URAT", "=", "%d%d", 0, 0);
            break;
        case RAT_UTRAN:
        case RAT_HSDPA:
        case RAT_HSUPA:
        case RAT_HSDPA_HSUPA:
            _at.at_cmd_discard("+URAT", "=", "%d%d", 2, 2);
            break;
#elif defined(UBX_MDM_SARA_R41XM)
        case RAT_CATM1:
            _at.at_cmd_discard("+URAT", "=", "%d%d", 7, 8);
            break;
        case RAT_NB1:
            _at.at_cmd_discard("+URAT", "=", "%d%d", 8, 7);
            break;
#endif
        default:
            _op_act = RAT_UNKNOWN;
            ret = NSAPI_ERROR_UNSUPPORTED;
            break;
    }
    _at.unlock();
    ubx_reboot();

    return (ret);
}

nsapi_error_t UBLOX_AT_CellularNetwork::ubx_reboot()
{
    _at.lock();
    _at.at_cmd_discard("+CFUN", "=15");

    nsapi_error_t err = NSAPI_ERROR_OK;
    Timer t1;
    t1.start();
    while (!(t1.read() >= 30)) {
        err = _at.at_cmd_discard("E0", "");
        if (err == NSAPI_ERROR_OK) {
            break;
        } else {
            //Don't clear err here so that we get some error in case of failure
            _at.clear_error();
            rtos::ThisThread::sleep_for(1000);
        }
    }
    t1.stop();
    _at.unlock();
    return err;
}
