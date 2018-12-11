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
#include "QUECTEL_BC95_CellularPower.h"
#include "QUECTEL_BC95_CellularContext.h"
#include "QUECTEL_BC95_CellularInformation.h"
#include "QUECTEL_BC95.h"
#include "CellularLog.h"

#define CONNECT_DELIM         "\r\n"
#define CONNECT_BUFFER_SIZE   (1280 + 80 + 80) // AT response + sscanf format
#define CONNECT_TIMEOUT       8000

using namespace events;
using namespace mbed;

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,        // C_EREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_GREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_REG
    1,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    0,  // AT_CGAUTH, BC95_AT_Commands_Manual_V1.9
};

QUECTEL_BC95::QUECTEL_BC95(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

QUECTEL_BC95::~QUECTEL_BC95()
{
}

nsapi_error_t QUECTEL_BC95::get_sim_state(SimState &state)
{
    _at->lock();
    _at->flush();
    _at->cmd_start("AT+NCCID?");
    _at->cmd_stop();
    _at->resp_start("+NCCID:");
    if (_at->info_resp()) {
        state = SimStateReady;
    } else {
        tr_warn("SIM not readable.");
        state = SimStateUnknown; // SIM may not be ready yet
    }
    _at->resp_stop();
    return _at->unlock_return_error();
}

AT_CellularNetwork *QUECTEL_BC95::open_network_impl(ATHandler &at)
{
    return new QUECTEL_BC95_CellularNetwork(at);
}

AT_CellularPower *QUECTEL_BC95::open_power_impl(ATHandler &at)
{
    return new QUECTEL_BC95_CellularPower(at);
}

AT_CellularContext *QUECTEL_BC95::create_context_impl(ATHandler &at, const char *apn)
{
    return new QUECTEL_BC95_CellularContext(at, this, apn);
}

AT_CellularInformation *QUECTEL_BC95::open_information_impl(ATHandler &at)
{
    return new QUECTEL_BC95_CellularInformation(at);
}

nsapi_error_t QUECTEL_BC95::init()
{
    _at->lock();
    _at->flush();
    _at->cmd_start("AT");
    _at->cmd_stop_read_resp();

    _at->cmd_start("AT+CMEE="); // verbose responses
    _at->write_int(1);
    _at->cmd_stop_read_resp();

    return _at->unlock_return_error();
}

nsapi_error_t QUECTEL_BC95::reset()
{
    _at->lock();
    AT_CellularDevice::shutdown();
    _at->cmd_start("AT+NRB"); // reset to full power levels
    _at->cmd_stop();
    _at->resp_start("REBOOTING", true);
    return _at->unlock_return_error();
}
