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

#include "RM1000_AT.h"

#include "RM1000_AT_CellularContext.h"
#include "RM1000_AT_CellularNetwork.h"

#include "mbed-trace/mbed_trace.h"
#ifndef TRACE_GROUP
#define TRACE_GROUP  "RIOT"
#endif // TRACE_GROUP

using namespace mbed;
using namespace events;
static const uint16_t retry_timeout[] = {1, 2, 4};
static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,// C_EREG
    AT_CellularNetwork::RegistrationModeDisable,// C_GREG
    AT_CellularNetwork::RegistrationModeDisable,// C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    0,  // AT_CGAUTH
    0,  // AT_CNMI
    0,  // AT_CSMP
    0,  // AT_CMGF
    0,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    1,  // PROPERTY_IPV6_STACK
    1,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    0,  // PROPERTY_AT_CGEREP
};

RM1000_AT::RM1000_AT(FileHandle *fh) : AT_CellularDevice(fh)
{
    tr_debug("RM1000_AT::RM1000_AT");
    AT_CellularBase::set_cellular_properties(cellular_properties);
    set_retry_timeout_array(retry_timeout, sizeof(retry_timeout) / sizeof(retry_timeout[0]));
}

AT_CellularNetwork *RM1000_AT::open_network_impl(ATHandler &at)
{
    tr_debug("RM1000_AT::open_network_impl");
    return new RM1000_AT_CellularNetwork(at);
}

AT_CellularContext *RM1000_AT::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    tr_debug("RM1000_AT::create_context_impl");
    return new RM1000_AT_CellularContext(at, this, apn, cp_req, nonip_req);
}

nsapi_error_t RM1000_AT::init()
{
    tr_debug("RM1000_AT::init");

    _at->lock();
    _at->flush();
    _at->at_cmd_discard("E0", ""); // echo off

    _at->at_cmd_discard("+SIM", "=physical");

    _at->set_at_timeout(5000);
    _at->at_cmd_discard("+CFUN", "=1"); // set full functionality

    _at->at_cmd_discard("+VERBOSE", "=0"); // verbose responses

    return _at->unlock_return_error();
}

#if MBED_CONF_RM1000_AT_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    tr_debug("Calling CellularDevice::get_default_instance from RM1000_AT");

    static UARTSerial serial(MBED_CONF_RM1000_AT_TX, MBED_CONF_RM1000_AT_RX, MBED_CONF_RM1000_AT_BAUDRATE);
#if defined (MBED_CONF_RM1000_AT_RTS) && defined(MBED_CONF_RM1000_AT_CTS)
    tr_debug("RM1000_AT flow control: RTS %d CTS %d", MBED_CONF_RM1000_AT_RTS, MBED_CONF_RM1000_AT_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_RM1000_AT_RTS, MBED_CONF_RM1000_AT_CTS);
#endif
    static RM1000_AT device(&serial);
    return &device;
}
#endif

