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

#include "onboard_modem_api.h"
#include "UBLOX_AT.h"
#include "UBLOX_AT_CellularNetwork.h"
#include "UBLOX_AT_CellularContext.h"

using namespace mbed;
using namespace events;

#ifdef TARGET_UBLOX_C030_R410M
static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    1,  // AT_CGAUTH,
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
};
#else
static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    1,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
};
#endif

UBLOX_AT::UBLOX_AT(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

UBLOX_AT::~UBLOX_AT()
{
}

AT_CellularNetwork *UBLOX_AT::open_network_impl(ATHandler &at)
{
    return new UBLOX_AT_CellularNetwork(at);
}

nsapi_error_t UBLOX_AT::power_on()
{
#if MODEM_ON_BOARD
    ::onboard_modem_init();
    ::onboard_modem_power_up();
#endif
    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_AT::power_off()
{
#if MODEM_ON_BOARD
    ::onboard_modem_power_down();
#endif
    return NSAPI_ERROR_OK;
}

AT_CellularContext *UBLOX_AT::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new UBLOX_AT_CellularContext(at, this, apn, cp_req, nonip_req);
}
