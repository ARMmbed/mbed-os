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

#include "GENERIC_AT3GPP.h"
#include "AT_CellularNetwork.h"

using namespace mbed;

// by default all properties are supported
static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,    // C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    1,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    1,  // PROPERTY_IPV6_STACK
    1,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};

GENERIC_AT3GPP::GENERIC_AT3GPP(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

#if MBED_CONF_GENERIC_AT3GPP_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_GENERIC_AT3GPP_TX, MBED_CONF_GENERIC_AT3GPP_RX, MBED_CONF_GENERIC_AT3GPP_BAUDRATE);
#if defined (MBED_CONF_GENERIC_AT3GPP_RTS) && defined(MBED_CONF_GENERIC_AT3GPP_CTS)
    tr_debug("GENERIC_AT3GPP flow control: RTS %d CTS %d", MBED_CONF_GENERIC_AT3GPP_RTS, MBED_CONF_GENERIC_AT3GPP_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_GENERIC_AT3GPP_RTS, MBED_CONF_GENERIC_AT3GPP_CTS);
#endif
    static GENERIC_AT3GPP device(&serial);
    return &device;
}
#endif
