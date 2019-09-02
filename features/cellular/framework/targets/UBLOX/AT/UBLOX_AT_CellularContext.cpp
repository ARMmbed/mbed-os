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
#include "UBLOX_AT_CellularContext.h"
#include "UBLOX_AT_CellularStack.h"
#include "APN_db.h"
#include "CellularLog.h"

namespace mbed {

UBLOX_AT_CellularContext::UBLOX_AT_CellularContext(ATHandler &at, CellularDevice *device, const char *apn, bool cp_req, bool nonip_req) :
    AT_CellularContext(at, device, apn, cp_req, nonip_req)
{
    // The authentication to use
    _auth = NOAUTH;
}

UBLOX_AT_CellularContext::~UBLOX_AT_CellularContext()
{
}

NetworkStack *UBLOX_AT_CellularContext::get_stack()
{
    if (_pdp_type == NON_IP_PDP_TYPE || _cp_in_use) {
        tr_error("Requesting stack for NON-IP context! Should request control plane netif: get_cp_netif()");
        return NULL;
    }
    if (!_stack) {
        _stack = new UBLOX_AT_CellularStack(_at, _cid, (nsapi_ip_stack_t)_pdp_type);
    }

    return _stack;
}

void UBLOX_AT_CellularContext::do_connect()
{
    _at.lock();
    _cb_data.error = NSAPI_ERROR_NO_CONNECTION;

    // Attempt to establish a connection
#ifndef TARGET_UBLOX_C030_R41XM
    _cb_data.error = define_context();
#elif TARGET_UBLOX_C030_R410M
    _is_context_active = true;
    _is_context_activated = true;
    _cb_data.error = NSAPI_ERROR_OK;
#elif TARGET_UBLOX_C030_R412M
    CellularNetwork::RadioAccessTechnology rat = read_radio_technology();
    if (rat == CellularNetwork::RadioAccessTechnology::RAT_EGPRS) {
        if (!_is_context_active) {
            _at.set_at_timeout(150 * 1000);
            _at.at_cmd_discard("+CGACT", "=", "%d%d", 1, 1);

            _at.cmd_start_stop("+CGACT", "?");
            _at.resp_start("+CGACT:");
            _at.skip_param(1);
            _is_context_activated = _at.read_int();
            _at.resp_stop();
            _at.restore_at_timeout();
            if (_is_context_activated == true) {
                _cid = 1;
                _is_context_active = true;
                _cb_data.error = NSAPI_ERROR_OK;
            }
        }
    } else if (rat == CellularNetwork::RadioAccessTechnology::RAT_CATM1 || rat == CellularNetwork::RadioAccessTechnology::RAT_NB1) {
        _is_context_active = true;
        _is_context_activated = true;
        _cb_data.error = NSAPI_ERROR_OK;
    }
#endif
    if (_cb_data.error != NSAPI_ERROR_OK) {
        // If new PSD context was created and failed to activate, delete it
        if (_new_context_set) {
            disconnect_modem_stack();
        }
        _connect_status = NSAPI_STATUS_DISCONNECTED;
    } else {
        _connect_status = NSAPI_STATUS_GLOBAL_UP;
    }
    _at.unlock();

    if (_status_cb) {
        _status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connect_status);
    }
}

#ifndef TARGET_UBLOX_C030_R41XM
nsapi_error_t UBLOX_AT_CellularContext::define_context()
{
    bool success = false;
    int active = 0;
    char *config = NULL;
    nsapi_error_t err = NSAPI_ERROR_NO_CONNECTION;
    char imsi[MAX_IMSI_LENGTH + 1];

    // do check for stack to validate that we have support for stack
    _stack = get_stack();
    if (!_stack) {
        return err;
    }

    _at.lock();
    _at.cmd_start_stop("+UPSND", "=", "%d%d", PROFILE, 8);
    _at.resp_start("+UPSND:");
    _at.skip_param(2);
    active = _at.read_int();
    _at.resp_stop();
    _at.unlock();

    if (active == 0) {
        // If the caller hasn't entered an APN, try to find it
        if (_apn == NULL) {
            err = get_imsi(imsi);
            if (err == NSAPI_ERROR_OK) {
                config = (char *)apnconfig(imsi);
            }
        }

        // Attempt to connect
        do {
            get_next_credentials(&config);
            if (_uname && _pwd) {
                _auth = (*_uname && *_pwd) ? _authentication_type : NOAUTH;
            } else {
                _auth = NOAUTH;
            }
            success = activate_profile(_apn, _uname, _pwd, _auth);
        } while (!success && config && *config);
    } else {
        // If the profile is already active, we're good
        success = true;
    }

    err = (_at.get_last_error() == NSAPI_ERROR_OK) ? NSAPI_ERROR_OK : NSAPI_ERROR_NO_CONNECTION;

    return err;
}

bool UBLOX_AT_CellularContext::activate_profile(const char *apn,
                                                const char *username,
                                                const char *password,
                                                AuthenticationType auth)
{
    bool activated = false;
    bool success = false;

    // Set up the APN
    if (apn) {
        success = false;
        if (_at.at_cmd_discard("+UPSD", "=", "%d%d%s", PROFILE, 1, apn) == NSAPI_ERROR_OK) {
            success = true;
        }
    }
    // Set up the UserName
    if (success && username) {
        success = false;
        if (_at.at_cmd_discard("+UPSD", "=", "%d%d%s", PROFILE, 2, username) == NSAPI_ERROR_OK) {
            success = true;
        }
    }
    // Set up the Password
    if (success && password) {
        success = false;
        if (_at.at_cmd_discard("+UPSD", "=", "%d%d%s", PROFILE, 3, password) == NSAPI_ERROR_OK) {
            success = true;
        }
    }

    if (success) {
        _at.at_cmd_discard("+UPSD", "=", "%d%d%s", PROFILE, 7, "0.0.0.0");

        if (_at.at_cmd_discard("+UPSD", "=", "%d%d%d", PROFILE, 6, nsapi_security_to_modem_security(auth)) == NSAPI_ERROR_OK) {
            // Activate, wait upto 30 seconds for the connection to be made
            _at.set_at_timeout(30000);

            nsapi_error_t err = _at.at_cmd_discard("+UPSDA", "=", "%d%d", PROFILE, 3);

            _at.restore_at_timeout();

            if (err == NSAPI_ERROR_OK) {
                Timer t1;
                t1.start();
                while (!(t1.read() >= 180)) {
                    _at.lock();
                    _at.cmd_start_stop("+UPSND", "=", "%d%d", PROFILE, 8);
                    _at.resp_start("+UPSND:");
                    _at.skip_param(2);
                    _at.read_int() ? activated = true : activated = false;
                    _at.resp_stop();
                    _at.unlock();

                    if (activated) {  //If context is activated, exit while loop and return status
                        break;
                    }
                    wait_ms(5000);    //Wait for 5 seconds and then try again
                }
                t1.stop();
            }
        }
    }

    return activated;
}
#endif

// Convert nsapi_security_t to the modem security numbers
int UBLOX_AT_CellularContext::nsapi_security_to_modem_security(AuthenticationType nsapi_security)
{
    int modem_security = 3;

    switch (nsapi_security) {
        case NOAUTH:
            modem_security = 0;
            break;
        case PAP:
            modem_security = 1;
            break;
        case CHAP:
            modem_security = 2;
            break;
#ifndef TARGET_UBLOX_C030_R41XM
        case AUTOMATIC:
            modem_security = 3;
            break;
        default:
            modem_security = 3;
            break;
#else
        default:
            modem_security = 0;
            break;
#endif
    }

    return modem_security;
}

// Disconnect the on board IP stack of the modem.
bool UBLOX_AT_CellularContext::disconnect_modem_stack()
{
    if (get_ip_address() != NULL) {
        if (_at.at_cmd_discard("+UPSDA", "=", "%d%d", PROFILE, 4) == NSAPI_ERROR_OK) {
            return true;
        }
    }

    return false;
}

nsapi_error_t UBLOX_AT_CellularContext::get_imsi(char *imsi)
{
    _at.lock();
    _at.cmd_start_stop("+CIMI", "");
    _at.resp_start();
    _at.read_string(imsi, MAX_IMSI_LENGTH + 1);
    _at.resp_stop();

    return _at.unlock_return_error();
}

// Get the next set of credentials, based on IMSI.
void UBLOX_AT_CellularContext::get_next_credentials(char **config)
{
    if (*config) {
        _apn    = _APN_GET(*config);
        _uname  = _APN_GET(*config);
        _pwd    = _APN_GET(*config);
    }
}

const char *UBLOX_AT_CellularContext::get_gateway()
{
    return get_ip_address();
}

const char *UBLOX_AT_CellularContext::get_apn()
{
    return _apn;
}

const char *UBLOX_AT_CellularContext::get_uname()
{
    return _uname;
}

const char *UBLOX_AT_CellularContext::get_pwd()
{
    return _pwd;
}

CellularContext::AuthenticationType UBLOX_AT_CellularContext::get_auth()
{
    return _authentication_type;
}

#ifdef TARGET_UBLOX_C030_R412M
CellularNetwork::RadioAccessTechnology UBLOX_AT_CellularContext::read_radio_technology()
{
    int act;
    CellularNetwork::RadioAccessTechnology rat;

    _at.at_cmd_int("+URAT", "?", act);

    switch (act) {
        case 0:
            rat = CellularNetwork::RadioAccessTechnology::RAT_GSM;
            break;
        case 1:
            rat = CellularNetwork::RadioAccessTechnology::RAT_GSM;
            break;
        case 2:
            rat = CellularNetwork::RadioAccessTechnology::RAT_UTRAN;
            break;
        case 7:
            rat = CellularNetwork::RadioAccessTechnology::RAT_CATM1;
            break;
        case 8:
            rat = CellularNetwork::RadioAccessTechnology::RAT_NB1;
            break;
        case 9:
            rat = CellularNetwork::RadioAccessTechnology::RAT_EGPRS;
            break;
        default:
            rat = CellularNetwork::RadioAccessTechnology::RAT_UNKNOWN;
            break;
    }

    return rat;
}
#endif // #ifdef TARGET_UBLOX_C030_R412M

} /* namespace mbed */
