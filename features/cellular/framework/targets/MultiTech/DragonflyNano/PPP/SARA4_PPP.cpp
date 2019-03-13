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

#include "SARA4_PPP.h"
#include "SARA4_PPP_CellularNetwork.h"

using namespace mbed;
using namespace events;

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};

SARA4_PPP::SARA4_PPP(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

AT_CellularNetwork *SARA4_PPP::open_network_impl(ATHandler &at)
{
    return new SARA4_PPP_CellularNetwork(at);
}

#if MBED_CONF_SARA4_PPP_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_SARA4_PPP_TX, MBED_CONF_SARA4_PPP_RX, MBED_CONF_SARA4_PPP_BAUDRATE);
#if defined (MBED_CONF_SARA4_PPP_RTS) && defined (MBED_CONF_SARA4_PPP_CTS)
    tr_debug("SARA4_PPP flow control: RTS %d CTS %d", MBED_CONF_SARA4_PPP_RTS, MBED_CONF_SARA4_PPP_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_SARA4_PPP_RTS, MBED_CONF_SARA4_PPP_CTS);
#endif
    static SARA4_PPP device(&serial);
    return &device;
}
#endif
