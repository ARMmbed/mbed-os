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

#include "UBLOX_PPP.h"
#include "UBLOX_PPP_CellularPower.h"
#include "UBLOX_PPP_CellularContext.h"
#include "AT_CellularNetwork.h"

using namespace mbed;
using namespace events;

#ifdef TARGET_UBLOX_C027
static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    1,  // AT_CGAUTH, BC95_AT_Commands_Manual_V1.9
};
#endif

UBLOX_PPP::UBLOX_PPP(FileHandle *fh) : AT_CellularDevice(fh)
{
#ifdef TARGET_UBLOX_C027
    AT_CellularBase::set_cellular_properties(cellular_properties);
#endif
}

UBLOX_PPP::~UBLOX_PPP()
{
}

AT_CellularPower *UBLOX_PPP::open_power_impl(ATHandler &at)
{
    return new UBLOX_PPP_CellularPower(at);
}

AT_CellularContext *UBLOX_PPP::create_context_impl(ATHandler &at, const char *apn)
{
    return new UBLOX_PPP_CellularContext(at, this, apn);
}
