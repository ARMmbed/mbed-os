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

#include "UBLOX_N2XX_CellularSIM.h"
#include "CellularLog.h"
#include "platform/mbed_wait_api.h"

using namespace mbed;

UBLOX_N2XX_CellularSIM::UBLOX_N2XX_CellularSIM(ATHandler &atHandler) : AT_CellularSIM(atHandler)
{
    _at.set_urc_handler("+NPIN:", callback(this, &UBLOX_N2XX_CellularSIM::NPIN_URC));
    memset(simstr,0,sizeof(simstr));
}

UBLOX_N2XX_CellularSIM::~UBLOX_N2XX_CellularSIM()
{
}

// Callback for Sim Pin.
void UBLOX_N2XX_CellularSIM::NPIN_URC()
{
    _at.read_string(simstr, sizeof(simstr));
}

nsapi_error_t UBLOX_N2XX_CellularSIM::get_sim_state(SimState &state)
{
    nsapi_error_t error = NSAPI_ERROR_DEVICE_ERROR;

    _at.lock();
    _at.flush();
    _at.cmd_start("AT+CFUN=1");
    _at.cmd_stop();

    _at.resp_start();
    _at.read_string(simstr, sizeof(simstr));
    _at.resp_stop();
    error = _at.unlock_return_error();

    int len = strlen(simstr);
    if (len > 0 || error == NSAPI_ERROR_OK) {
        if (error == NSAPI_ERROR_OK) {
            state = SimStateReady;
        } else if (len >= 6 && memcmp(simstr, "ENTER PIN", 9) == 0) {
            state = SimStatePinNeeded;
        } else {
            simstr[len] = '\0';
            tr_error("Unknown SIM state %s", simstr);
            state = SimStateUnknown;
        }
        error = NSAPI_ERROR_OK;
    } else {
        tr_warn("SIM not readable.");
        state = SimStateUnknown; // SIM may not be ready yet or pin command may not be supported
    }

#if MBED_CONF_MBED_TRACE_ENABLE
    switch (state) {
    case SimStatePinNeeded:
        tr_info("SIM PIN required");
        break;
    case SimStatePukNeeded:
        tr_error("SIM PUK required");
        break;
    case SimStateUnknown:
        tr_warn("SIM state unknown");
        break;
    default:
        tr_info("SIM is ready");
        break;
    }
#endif
    return error;
}

nsapi_error_t UBLOX_N2XX_CellularSIM::set_pin(const char *sim_pin)
{
    // if SIM is already in ready state then settings the PIN
    // will return error so let's check the state before settings the pin.
    SimState state;
    if (get_sim_state(state) == NSAPI_ERROR_OK && state == SimStateReady) {
        return NSAPI_ERROR_OK;
    }

    _at.lock();
    _at.cmd_start("AT+NPIN=0,");
    _at.write_string(sim_pin);
    _at.cmd_stop_read_resp();
    return _at.unlock_return_error();
}

nsapi_error_t UBLOX_N2XX_CellularSIM::change_pin(const char *sim_pin, const char *new_pin)
{
    _at.lock();
    _at.cmd_start("AT+NPIN=1,");
    _at.write_string(sim_pin);
    _at.write_string(new_pin);
    _at.cmd_stop_read_resp();
    return _at.unlock_return_error();
}

nsapi_error_t UBLOX_N2XX_CellularSIM::set_pin_query(const char *sim_pin, bool query_pin)
{
    _at.lock();
    if (query_pin) {
        /* use the SIM locked */
        _at.cmd_start("AT+NPIN=2,");
        _at.write_string(sim_pin);
        _at.cmd_stop_read_resp();
    } else {
        /* use the SIM unlocked */
        _at.cmd_start("AT+NPIN=3,");
        _at.write_string(sim_pin);
        _at.cmd_stop_read_resp();
    }
    return _at.unlock_return_error();
}
