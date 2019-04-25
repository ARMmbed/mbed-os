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

#include "TELIT_ME910.h"
#include "AT_CellularNetwork.h"

#define TRACE_GROUP   "ME91"

using namespace mbed;
using namespace events;

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,    // C_EREG
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
    1,  // PROPERTY_IPV6_STACK
    1,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};

TELIT_ME910::TELIT_ME910(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

uint16_t TELIT_ME910::get_send_delay() const
{
    return DEFAULT_DELAY_BETWEEN_AT_COMMANDS;
}

nsapi_error_t TELIT_ME910::init()
{
    nsapi_error_t err = AT_CellularDevice::init();
    if (err != NSAPI_ERROR_OK) {
        return err;
    }
    _at->lock();
#if defined (MBED_CONF_TELIT_ME910_RTS) && defined (MBED_CONF_TELIT_ME910_CTS)
    _at->cmd_start("AT&K3;&C1;&D0");
#else
    _at->cmd_start("AT&K0;&C1;&D0");
#endif
    _at->cmd_stop_read_resp();

    return _at->unlock_return_error();
}

#if MBED_CONF_TELIT_ME910_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_TELIT_ME910_TX, MBED_CONF_TELIT_ME910_RX, MBED_CONF_TELIT_ME910_BAUDRATE);
#if defined (MBED_CONF_TELIT_ME910_RTS) && defined (MBED_CONF_TELIT_ME910_CTS)
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_TELIT_ME910_RTS, MBED_CONF_TELIT_ME910_CTS);
#endif
    static TELIT_ME910 device(&serial);
    return &device;
}
#endif
