/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "UBLOX_N2XX.h"

using namespace mbed;
using namespace events;

static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,        // C_EREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_GREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_REG
    1,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    0,  // AT_CGAUTH
    0,  // AT_CNMI
    0,  // AT_CSMP
    0,  // AT_CMGF
    0,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    1,  // PROPERTY_NON_IP_PDP_TYPE
    0,  // PROPERTY_AT_CGEREP
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    7,  // PROPERTY_SOCKET_COUNT
    0,  // PROPERTY_IP_TCP
    1,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};

UBLOX_N2XX::UBLOX_N2XX(FileHandle *fh): AT_CellularDevice(fh)
{
    set_cellular_properties(cellular_properties);
    memset(simstr, 0, sizeof(simstr));
}

void UBLOX_N2XX::set_at_urcs_impl()
{
    _at.set_urc_handler("+NPIN:", mbed::Callback<void()>(this, &UBLOX_N2XX::NPIN_URC));
}

UBLOX_N2XX::~UBLOX_N2XX()
{
    _at.set_urc_handler("+NPIN:", nullptr);
}

// Callback for Sim Pin.
void UBLOX_N2XX::NPIN_URC()
{
    _at.read_string(simstr, sizeof(simstr));
}

AT_CellularNetwork *UBLOX_N2XX::open_network_impl(ATHandler &at)
{
    return new UBLOX_N2XX_CellularNetwork(at, *this);
}

AT_CellularContext *UBLOX_N2XX::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new UBLOX_N2XX_CellularContext(at, this, apn, cp_req, nonip_req);
}

#if MBED_CONF_CELLULAR_USE_SMS
AT_CellularSMS *UBLOX_N2XX::open_sms_impl(ATHandler &at)
{
    return new UBLOX_N2XX_CellularSMS(at, *this);
}
#endif // MBED_CONF_CELLULAR_USE_SMS

nsapi_error_t UBLOX_N2XX::init()
{
    setup_at_handler();

    _at.lock();
    _at.flush();
    _at.at_cmd_discard("", "");

    _at.at_cmd_discard("+CMEE", "=1"); // verbose responses

#ifdef MBED_CONF_NSAPI_DEFAULT_CELLULAR_SIM_PIN
    set_pin(MBED_CONF_NSAPI_DEFAULT_CELLULAR_SIM_PIN);
#endif
    return _at.unlock_return_error();
}

nsapi_error_t UBLOX_N2XX::get_sim_state(SimState &state)
{
    nsapi_error_t error = NSAPI_ERROR_DEVICE_ERROR;

    _at.lock();
    _at.flush();
    //Special case: Command put in cmd_chr to make a 1 liner
    error = _at.at_cmd_str("", "+CFUN=1", simstr, sizeof(simstr));
    _at.unlock();

    int len = strlen(simstr);
    if (len > 0 || error == NSAPI_ERROR_OK) {
        if (error == NSAPI_ERROR_OK) {
            state = SimStateReady;
        } else if (len >= 6 && memcmp(simstr, "ENTER PIN", 9) == 0) {
            state = SimStatePinNeeded;
        } else {
            simstr[len] = '\0';
            tr_error("Unknown SIM state %s", simstr);
            state = SimStateUnknown;
        }
        error = NSAPI_ERROR_OK;
    } else {
        tr_warn("SIM not readable.");
        state = SimStateUnknown; // SIM may not be ready yet or pin command may not be supported
    }

#if MBED_CONF_MBED_TRACE_ENABLE
    switch (state) {
        case SimStatePinNeeded:
            tr_info("SIM PIN required");
            break;
        case SimStatePukNeeded:
            tr_error("SIM PUK required");
            break;
        case SimStateUnknown:
            tr_warn("SIM state unknown");
            break;
        default:
            tr_info("SIM is ready");
            break;
    }
#endif
    return error;
}

nsapi_error_t UBLOX_N2XX::set_pin(const char *sim_pin)
{
    // if SIM is already in ready state then settings the PIN
    // will return error so let's check the state before settings the pin.
    SimState state;
    if (get_sim_state(state) == NSAPI_ERROR_OK && state == SimStateReady) {
        return NSAPI_ERROR_OK;
    }

    if (sim_pin == NULL) {
        return NSAPI_ERROR_PARAMETER;
    }

    return _at.at_cmd_discard("+NPIN", "=", "%d%s", 0, sim_pin);
}

#if MBED_CONF_UBLOX_N2XX_PROVIDE_DEFAULT
#include "drivers/BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_UBLOX_N2XX_TX, MBED_CONF_UBLOX_N2XX_RX, MBED_CONF_UBLOX_N2XX_BAUDRATE);
#if defined (MBED_CONF_UBLOX_N2XX_RTS) && defined(MBED_CONF_UBLOX_N2XX_CTS)
    tr_debug("UBLOX_N2XX flow control: RTS %d CTS %d", MBED_CONF_UBLOX_N2XX_RTS, MBED_CONF_UBLOX_N2XX_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_UBLOX_N2XX_RTS, MBED_CONF_UBLOX_N2XX_CTS);
#endif
    static UBLOX_N2XX device(&serial);
    return &device;
}
#endif
