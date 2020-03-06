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

#include "UBLOX_AT.h"

using namespace mbed;
using namespace events;

#ifdef UBX_MDM_SARA_R41XM
static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    0,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    7,  // PROPERTY_SOCKET_COUNT
    1,  // PROPERTY_IP_TCP
    1,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};
#elif defined(UBX_MDM_SARA_U2XX) || defined(UBX_MDM_SARA_G3XX)
static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
#ifdef UBX_MDM_SARA_G3XX
    0,  // AT_CGSN_WITH_TYPE
#else
    1,  // AT_CGSN_WITH_TYPE
#endif
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
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    7,  // PROPERTY_SOCKET_COUNT
    1,  // PROPERTY_IP_TCP
    1,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};
#else
static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    0,  // C_EREG
    0,  // C_GREG
    0,  // C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    0,  // AT_CGAUTH
    0,  // AT_CNMI
    0,  // AT_CSMP
    0,  // AT_CMGF
    0,  // AT_CSDH
    0,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    0,  // PROPERTY_AT_CGEREP
    0,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    0,  // PROPERTY_SOCKET_COUNT
    0,  // PROPERTY_IP_TCP
    0,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};
#endif

UBLOX_AT::UBLOX_AT(FileHandle *fh) : AT_CellularDevice(fh), ubx_context(0)
{
    set_cellular_properties(cellular_properties);
}

AT_CellularNetwork *UBLOX_AT::open_network_impl(ATHandler &at)
{
    return new UBLOX_AT_CellularNetwork(at, *this);
}

AT_CellularContext *UBLOX_AT::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    ubx_context = new UBLOX_AT_CellularContext(at, this, apn, cp_req, nonip_req);
    return  ubx_context;
}

#if MBED_CONF_UBLOX_AT_PROVIDE_DEFAULT
#include "drivers/BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_UBLOX_AT_TX, MBED_CONF_UBLOX_AT_RX, MBED_CONF_UBLOX_AT_BAUDRATE);
#if defined (MBED_CONF_UBLOX_AT_RTS) && defined(MBED_CONF_UBLOX_AT_CTS)
    tr_debug("UBLOX_AT flow control: RTS %d CTS %d", MBED_CONF_UBLOX_AT_RTS, MBED_CONF_UBLOX_AT_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_UBLOX_AT_RTS, MBED_CONF_UBLOX_AT_CTS);
#endif
    static UBLOX_AT device(&serial);
    return &device;
}
#endif

nsapi_error_t UBLOX_AT::init()
{
    setup_at_handler();
    _at.lock();
    _at.flush();
    _at.at_cmd_discard("", "");

#if defined(UBX_MDM_SARA_U2XX) || defined(UBX_MDM_SARA_G3XX)
    nsapi_error_t err = _at.at_cmd_discard("E0", ""); // echo off

    if (err == NSAPI_ERROR_OK) {
        _at.at_cmd_discard("+CMEE", "=1"); // verbose responses
        err = _at.at_cmd_discard("+CFUN", "=1"); // set full functionality
    }
#elif defined(UBX_MDM_SARA_R41XM)
    nsapi_error_t err = _at.at_cmd_discard("+CFUN", "=", "%d", 4);

    if (err == NSAPI_ERROR_OK) {
        _at.at_cmd_discard("E0", ""); // echo off
        _at.at_cmd_discard("+CMEE", "=1"); // verbose responses
        config_authentication_parameters();
        err = _at.at_cmd_discard("+CFUN", "=1"); // set full functionality
    }
#else
    _at.unlock();
    return NSAPI_ERROR_UNSUPPORTED;
#endif

    return _at.unlock_return_error();
}

nsapi_error_t UBLOX_AT::config_authentication_parameters()
{
    char *config = NULL;
    nsapi_error_t err;
    const char *apn;
    const char *uname;
    const char *pwd;
    CellularContext::AuthenticationType auth = CellularContext::NOAUTH;
    char imsi[MAX_IMSI_LENGTH + 1];

    if (ubx_context->get_apn() == NULL) {
        err = get_imsi(imsi);
        if (err == NSAPI_ERROR_OK) {
            config = (char *)apnconfig(imsi);
        }
        ubx_context->get_next_credentials(&config);
    }

    apn = ubx_context->get_apn();
    pwd = ubx_context->get_pwd();
    uname = ubx_context->get_uname();

    if (*uname && *pwd) {
        auth = ubx_context->get_auth();
    }
    err = set_authentication_parameters(apn, uname, pwd, auth);

    return err;
}

nsapi_error_t UBLOX_AT::set_authentication_parameters(const char *apn,
                                                      const char *username,
                                                      const char *password,
                                                      CellularContext::AuthenticationType auth)
{
    int modem_security = ubx_context->nsapi_security_to_modem_security(auth);

    nsapi_error_t err = _at.at_cmd_discard("+CGDCONT", "=", "%d%s%s", 1, "IP", apn);

    if (err == NSAPI_ERROR_OK) {
#ifdef UBX_MDM_SARA_R41XM
        if (modem_security == CellularContext::CHAP) {
            err = _at.at_cmd_discard("+UAUTHREQ", "=", "%d%d%s%s", 1, modem_security, password, username);
        } else if (modem_security == CellularContext::NOAUTH) {
            err = _at.at_cmd_discard("+UAUTHREQ", "=", "%d%d", 1, modem_security);
        } else {
            err = _at.at_cmd_discard("+UAUTHREQ", "=", "%d%d%s%s", 1, modem_security, username, password);
        }
#else
        err = _at.at_cmd_discard("+UAUTHREQ", "=", "%d%d%s%s", 1, modem_security, username, password);
#endif
    }

    return err;
}

nsapi_error_t UBLOX_AT::get_imsi(char *imsi)
{
    //Special case: Command put in cmd_chr to make a 1 liner
    return _at.at_cmd_str("", "+CIMI", imsi, MAX_IMSI_LENGTH + 1);
}
