/*
#include <M26/QUECTEL_M26_CellularInformation.h>
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

#include "AT_CellularNetwork.h"
#include "QUECTEL_M26_CellularContext.h"
#include "QUECTEL_M26.h"

#include "CellularLog.h"

using namespace mbed;

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeDisable,// C_REG
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
    1,  // PROPERTY_AT_CGEREP
};

QUECTEL_M26::QUECTEL_M26(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

nsapi_error_t QUECTEL_M26::get_sim_state(SimState &state)
{
    char buf[13];

    _at->lock();
    nsapi_error_t err = _at->at_cmd_str("+CPIN", "?", buf, 13);
    tr_debug("CPIN: %s", buf);

    if (memcmp(buf, "READY", 5) == 0) {
        state = SimStateReady;
    } else if (memcmp(buf, "SIM PIN", 7) == 0) {
        state = SimStatePinNeeded;
    } else if (memcmp(buf, "SIM PUK", 7) == 0) {
        state = SimStatePukNeeded;
    } else if (memcmp(buf, "PH_SIM PIN", 10) == 0) {
        state = SimStatePinNeeded;
    } else if (memcmp(buf, "PH_SIM PUK", 10) == 0) {
        state = SimStatePukNeeded;
    } else if (memcmp(buf, "SIM PIN2", 8) == 0) {
        state = SimStatePinNeeded;
    } else if (memcmp(buf, "SIM PUK2", 8) == 0) {
        state = SimStatePukNeeded;
    } else {
        state = SimStateUnknown; // SIM may not be ready yet
    }

    return err;
}

AT_CellularContext *QUECTEL_M26::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new QUECTEL_M26_CellularContext(at, this, apn, cp_req, nonip_req);
}

nsapi_error_t QUECTEL_M26::shutdown()
{
    return _at->at_cmd_discard("+QPOWD", "=0");
}


#if MBED_CONF_QUECTEL_M26_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_QUECTEL_M26_TX, MBED_CONF_QUECTEL_M26_RX, MBED_CONF_QUECTEL_M26_BAUDRATE);
#if defined (MBED_CONF_QUECTEL_M26_RTS) && defined(MBED_CONF_QUECTEL_M26_CTS)
    tr_debug("QUECTEL_M26 flow control: RTS %d CTS %d", MBED_CONF_QUECTEL_M26_RTS, MBED_CONF_QUECTEL_M26_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_QUECTEL_M26_RTS, MBED_CONF_QUECTEL_M26_CTS);
#endif
    static QUECTEL_M26 device(&serial);
    return &device;
}
#endif
