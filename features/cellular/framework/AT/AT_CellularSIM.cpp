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

#include "AT_CellularSIM.h"
#include "CellularLog.h"

using namespace mbed;

const int MAX_SIM_RESPONSE_LENGTH = 16;

AT_CellularSIM::AT_CellularSIM(ATHandler &at) : AT_CellularBase(at)
{
}

AT_CellularSIM::~AT_CellularSIM()
{
}

nsapi_error_t AT_CellularSIM::get_sim_state(SimState &state)
{
    char simstr[MAX_SIM_RESPONSE_LENGTH];
    _at.lock();
    _at.flush();
    _at.cmd_start("AT+CPIN?");
    _at.cmd_stop();
    _at.resp_start("+CPIN:");
    ssize_t len = _at.read_string(simstr, sizeof (simstr));
    if (len != -1) {
        if (len >= 5 && memcmp(simstr, "READY", 5) == 0) {
            state = SimStateReady;
        } else if (len >= 6 && memcmp(simstr, "SIM PIN", 6) == 0) {
            state = SimStatePinNeeded;
        } else if (len >= 6 && memcmp(simstr, "SIM PUK", 6) == 0) {
            state = SimStatePukNeeded;
        } else {
            simstr[len] = '\0';
            tr_error("Unknown SIM state %s", simstr);
            state = SimStateUnknown;
        }
    } else {
        tr_warn("SIM not readable.");
        state = SimStateUnknown; // SIM may not be ready yet or +CPIN may be unsupported command
    }
    _at.resp_stop();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularSIM::set_pin(const char *sim_pin)
{
    // if SIM is already in ready state then settings the PIN
    // will return error so let's check the state before settings the pin.
    SimState state;
    if (get_sim_state(state) == NSAPI_ERROR_OK && state == SimStateReady) {
        return NSAPI_ERROR_OK;
    }

    _at.lock();
    _at.cmd_start("AT+CPIN=");
    _at.write_string(sim_pin);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularSIM::change_pin(const char *sim_pin, const char *new_pin)
{
    _at.lock();
    _at.cmd_start("AT+CPWD=");
    _at.write_string("SC");
    _at.write_string(sim_pin);
    _at.write_string(new_pin);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularSIM::set_pin_query(const char *sim_pin, bool query_pin)
{
    _at.lock();
    if (query_pin) {
        /* use the SIM locked */
        _at.cmd_start("AT+CLCK=");
        _at.write_string("SC");
        _at.write_int(1);
        _at.write_string(sim_pin);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    } else {
        /* use the SIM unlocked */
        _at.cmd_start("AT+CLCK=");
        _at.write_string("SC");
        _at.write_int(0);
        _at.write_string(sim_pin);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularSIM::get_imsi(char* imsi)
{
    _at.lock();
    _at.cmd_start("AT+CIMI");
    _at.cmd_stop();
    _at.resp_start();
    int len = _at.read_string(imsi, MAX_IMSI_LENGTH);
    if (len > 0) {
        imsi[len] = '\0';
    }
    _at.resp_stop();
    return _at.unlock_return_error();
}
