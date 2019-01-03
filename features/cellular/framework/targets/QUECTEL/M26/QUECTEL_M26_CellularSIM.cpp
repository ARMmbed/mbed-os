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

#include "QUECTEL_M26_CellularSIM.h"
#include "CellularLog.h"

using namespace mbed;

QUECTEL_M26_CellularSIM::QUECTEL_M26_CellularSIM(ATHandler &atHandler) : AT_CellularSIM(atHandler)
{

}

QUECTEL_M26_CellularSIM::~QUECTEL_M26_CellularSIM()
{

}

nsapi_error_t QUECTEL_M26_CellularSIM::get_sim_state(SimState &state)
{
    char buf[13];

    _at.lock();
    _at.cmd_start("AT+CPIN?");
    _at.cmd_stop();
    _at.resp_start("+CPIN:");
    if (_at.info_resp()) {
        _at.read_string(buf, 13);
        tr_debug("CPIN: %s", buf);

        if (memcmp(buf, "READY", 5) == 0) {
            state = SimStateReady;
        } else if (memcmp(buf, "SIM PIN", 7) == 0) {
            state = SimStatePinNeeded;
        } else if (memcmp(buf, "SIM PUK", 7) == 0) {
            state = SimStatePukNeeded;
        } else if (memcmp(buf, "PH_SIM PIN", 10) == 0) {
            state = SimStatePinNeeded;
        } else if (memcmp(buf, "PH_SIM PUK", 10) == 0) {
            state = SimStatePukNeeded;
        } else if (memcmp(buf, "SIM PIN2", 8) == 0) {
            state = SimStatePinNeeded;
        } else if (memcmp(buf, "SIM PUK2", 8) == 0) {
            state = SimStatePukNeeded;
        } else {
            state = SimStateUnknown; // SIM may not be ready yet
        }

    }
    _at.resp_stop();
    return _at.unlock_return_error();
}

// According to M26_AT_Commands_Manual_V1.9
nsapi_error_t QUECTEL_M26_CellularSIM::get_iccid(char *buf, size_t buf_size)
{
    _at.lock();
    _at.cmd_start("AT+CCID");
    _at.cmd_stop();
    _at.resp_start("+CCID:");
    _at.read_string(buf, buf_size);
    _at.resp_stop();
    return _at.unlock_return_error();
}

nsapi_error_t QUECTEL_M26_CellularSIM::change_pin(const char *sim_pin, const char *new_pin)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t QUECTEL_M26_CellularSIM::set_pin_query(const char *sim_pin, bool query_pin)
{
    return NSAPI_ERROR_UNSUPPORTED;
}
