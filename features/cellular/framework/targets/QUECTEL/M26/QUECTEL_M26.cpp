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

#include "QUECTEL_M26_CellularNetwork.h"
#include "QUECTEL_M26_CellularPower.h"
#include "QUECTEL_M26_CellularInformation.h"
#include "QUECTEL_M26_CellularContext.h"
#include "QUECTEL_M26.h"

using namespace events;
using namespace mbed;

#define CONNECT_DELIM         "\r\n"
#define CONNECT_BUFFER_SIZE   (1280 + 80 + 80) // AT response + sscanf format
#define CONNECT_TIMEOUT       8000

#define MAX_STARTUP_TRIALS 5
#define MAX_RESET_TRIALS 5

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,    // C_EREG
    AT_CellularNetwork::RegistrationModeLAC,        // C_GREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
};

QUECTEL_M26::QUECTEL_M26(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

QUECTEL_M26::~QUECTEL_M26()
{
}

AT_CellularNetwork *QUECTEL_M26::open_network_impl(ATHandler &at)
{
    return new QUECTEL_M26_CellularNetwork(at);
}

AT_CellularPower *QUECTEL_M26::open_power_impl(ATHandler &at)
{
    return new QUECTEL_M26_CellularPower(at);
}

AT_CellularInformation *QUECTEL_M26::open_information_impl(ATHandler &at)
{
    return new QUECTEL_M26_CellularInformation(at);
}

AT_CellularContext *QUECTEL_M26::create_context_impl(ATHandler &at, const char *apn)
{
    return new QUECTEL_M26_CellularContext(at, this, apn);
}

nsapi_error_t QUECTEL_M26::init()
{
    _at->lock();
    _at->cmd_start("AT");
    _at->cmd_stop_read_resp();

    _at->cmd_start("AT+CMEE="); // verbose responses
    _at->write_int(1);
    _at->cmd_stop_read_resp();

    return _at->unlock_return_error();
}

nsapi_error_t QUECTEL_M26::get_sim_state(SimState &state)
{
    char buf[13];

    _at->lock();
    _at->cmd_start("AT+CPIN?");
    _at->cmd_stop();
    _at->resp_start("+CPIN:");
    if (_at->info_resp()) {
        _at->read_string(buf, 13);
        if (memcmp(buf, "READY", 5) == 0) {
            state = SimStateReady;
        } else if (memcmp(buf, "Information PIN", 7) == 0) {
            state = SimStatePinNeeded;
        } else if (memcmp(buf, "Information PUK", 7) == 0) {
            state = SimStatePukNeeded;
        } else if (memcmp(buf, "PH_Information PIN", 10) == 0) {
            state = SimStatePinNeeded;
        } else if (memcmp(buf, "PH_Information PUK", 10) == 0) {
            state = SimStatePukNeeded;
        } else if (memcmp(buf, "Information PIN2", 8) == 0) {
            state = SimStatePinNeeded;
        } else if (memcmp(buf, "Information PUK2", 8) == 0) {
            state = SimStatePukNeeded;
        } else {
            state = SimStateUnknown; // Information may not be ready yet
        }

    }
    _at->resp_stop();
    return _at->unlock_return_error();
}
