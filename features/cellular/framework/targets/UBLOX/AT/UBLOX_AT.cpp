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

#ifdef TARGET_UBLOX_C030_R41XM
static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
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
#else
static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,// C_EREG
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
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};
#endif

UBLOX_AT::UBLOX_AT(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

AT_CellularNetwork *UBLOX_AT::open_network_impl(ATHandler &at)
{
    return new UBLOX_AT_CellularNetwork(at);
}

AT_CellularContext *UBLOX_AT::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    ubx_context = new UBLOX_AT_CellularContext(at, this, apn, cp_req, nonip_req);
    return  ubx_context;
}

#if MBED_CONF_UBLOX_AT_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_UBLOX_AT_TX, MBED_CONF_UBLOX_AT_RX, MBED_CONF_UBLOX_AT_BAUDRATE);
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

    _at->lock();
    _at->flush();
    _at->cmd_start("AT");
    _at->cmd_stop_read_resp();

#ifdef TARGET_UBLOX_C027
    _at->cmd_start("AT+CFUN=0");
    _at->cmd_stop_read_resp();
    if (_at->get_last_error() == NSAPI_ERROR_OK) {
        _at->cmd_start("ATE0"); // echo off
        _at->cmd_stop_read_resp();
        _at->cmd_start("AT+CMEE=1"); // verbose responses
        _at->cmd_stop_read_resp();
        config_authentication_parameters();
        _at->cmd_start("AT+CFUN=1"); // set full functionality
        _at->cmd_stop_read_resp();
    }
#else
    _at->cmd_start("AT+CFUN=4");
    _at->cmd_stop_read_resp();
    if (_at->get_last_error() == NSAPI_ERROR_OK) {
        _at->cmd_start("ATE0"); // echo off
        _at->cmd_stop_read_resp();
        _at->cmd_start("AT+CMEE=1"); // verbose responses
        _at->cmd_stop_read_resp();
        config_authentication_parameters();
        _at->cmd_start("AT+CFUN=1"); // set full functionality
        _at->cmd_stop_read_resp();
    }
#endif

    return _at->unlock_return_error();
}

nsapi_error_t UBLOX_AT::config_authentication_parameters()
{
    char *config = NULL;
    nsapi_error_t err;
    char imsi[MAX_IMSI_LENGTH + 1];

    if (apn == NULL) {
        err = get_imsi(imsi);
        if (err == NSAPI_ERROR_OK) {
            config = (char *)apnconfig(imsi);
        }
    }

    ubx_context->get_next_credentials(&config);
    apn = ubx_context->get_apn();
    pwd = ubx_context->get_pwd();
    uname = ubx_context->get_uname();
    auth = ubx_context->get_auth();

    auth = (*uname && *pwd) ? auth : CellularContext::NOAUTH;
    err = set_authentication_parameters(apn, uname, pwd, auth);

    return err;
}

nsapi_error_t UBLOX_AT::set_authentication_parameters(const char *apn,
                                                      const char *username,
                                                      const char *password,
                                                      CellularContext::AuthenticationType auth)
{
    int modem_security = ubx_context->nsapi_security_to_modem_security(auth);

    _at->cmd_start("AT+CGDCONT=1,\"IP\",");
    _at->write_string(apn);
    _at->cmd_stop();
    _at->resp_start();
    _at->resp_stop();

    if (_at->get_last_error() == NSAPI_ERROR_OK) {
#ifdef TARGET_UBLOX_C030_R41XM
        if (modem_security == CellularContext::CHAP) {
            _at->cmd_start("AT+UAUTHREQ=1,");
            _at->write_int(modem_security);
            _at->write_string(password);
            _at->write_string(username);
            _at->cmd_stop();
            _at->resp_start();
            _at->resp_stop();
        } else if (modem_security == CellularContext::NOAUTH) {
            _at->cmd_start("AT+UAUTHREQ=1,");
            _at->write_int(modem_security);
            _at->cmd_stop();
            _at->resp_start();
            _at->resp_stop();
        } else {
            _at->cmd_start("AT+UAUTHREQ=1,");
            _at->write_int(modem_security);
            _at->write_string(username);
            _at->write_string(password);
            _at->cmd_stop();
            _at->resp_start();
            _at->resp_stop();
        }
#else
        _at->cmd_start("AT+UAUTHREQ=1,");
        _at->write_int(modem_security);
        _at->write_string(username);
        _at->write_string(password);
        _at->cmd_stop();
        _at->resp_start();
        _at->resp_stop();
#endif
    }

    return _at->get_last_error();
}

nsapi_error_t UBLOX_AT::get_imsi(char *imsi)
{
    _at->lock();
    _at->cmd_start("AT+CIMI");
    _at->cmd_stop();
    _at->resp_start();
    _at->read_string(imsi, MAX_IMSI_LENGTH + 1);
    _at->resp_stop();

    return _at->unlock_return_error();
}
