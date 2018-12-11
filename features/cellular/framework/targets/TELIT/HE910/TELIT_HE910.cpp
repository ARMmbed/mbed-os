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

#include "TELIT_HE910.h"
#include "TELIT_HE910_CellularPower.h"
#include "TELIT_HE910_CellularNetwork.h"
#include "TELIT_HE910_CellularContext.h"

using namespace mbed;
using namespace events;

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    0,  // AT_CGAUTH
};

TELIT_HE910::TELIT_HE910(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

TELIT_HE910::~TELIT_HE910()
{
}

AT_CellularNetwork *TELIT_HE910::open_network_impl(ATHandler &at)
{
    return new TELIT_HE910_CellularNetwork(at);
}

AT_CellularPower *TELIT_HE910::open_power_impl(ATHandler &at)
{
    return new TELIT_HE910_CellularPower(at);
}

AT_CellularContext *TELIT_HE910::create_context_impl(ATHandler &at, const char *apn)
{
    return new TELIT_HE910_CellularContext(at, this, apn);
}

uint16_t TELIT_HE910::get_send_delay() const
{
    return DEFAULT_DELAY_BETWEEN_AT_COMMANDS;
}

nsapi_error_t TELIT_HE910::init()
{
    nsapi_error_t err = AT_CellularDevice::init();
    if (err != NSAPI_ERROR_OK) {
        return err;
    }
    _at->lock();
    _at->cmd_start("AT&K0;&C1;&D0");
    _at->cmd_stop_read_resp();

    return _at->unlock_return_error();
}
