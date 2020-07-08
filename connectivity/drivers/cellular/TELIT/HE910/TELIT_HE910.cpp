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
#include "AT_CellularNetwork.h"

using namespace mbed;
using namespace events;

static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP,
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    0,  // PROPERTY_SOCKET_COUNT
    0,  // PROPERTY_IP_TCP
    0,  // PROPERTY_IP_UDP
    20, // PROPERTY_AT_SEND_DELAY
};

TELIT_HE910::TELIT_HE910(FileHandle *fh) : AT_CellularDevice(fh)
{
    set_cellular_properties(cellular_properties);
}

nsapi_error_t TELIT_HE910::init()
{
    nsapi_error_t err = AT_CellularDevice::init();
    if (err != NSAPI_ERROR_OK) {
        return err;
    }
    return _at.at_cmd_discard("&K0;&C1;&D0", "");
}

#if MBED_CONF_TELIT_HE910_PROVIDE_DEFAULT
#include "drivers/BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_TELIT_HE910_TX, MBED_CONF_TELIT_HE910_RX, MBED_CONF_TELIT_HE910_BAUDRATE);
#if defined (MBED_CONF_TELIT_HE910_RTS) && defined (MBED_CONF_TELIT_HE910_CTS)
    tr_debug("TELIT_HE910 flow control: RTS %d CTS %d", MBED_CONF_TELIT_HE910_RTS, MBED_CONF_TELIT_HE910_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_TELIT_HE910_RTS, MBED_CONF_TELIT_HE910_CTS);
#endif
    static TELIT_HE910 device(&serial);
    return &device;
}
#endif
