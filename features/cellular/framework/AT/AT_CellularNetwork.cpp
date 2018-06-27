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

#include <stdlib.h>
#include "AT_CellularNetwork.h"
#include "nsapi_ppp.h"
#include "CellularUtil.h"
#include "CellularLog.h"
#include "CellularCommon.h"

using namespace std;
using namespace mbed_cellular_util;
using namespace mbed;

struct at_reg_t {
    const CellularNetwork::RegistrationType type;
    const char *const cmd;
    const char *const urc_prefix;
};

static const at_reg_t at_reg[] = {
    { CellularNetwork::C_EREG, "AT+CEREG", "+CEREG:"},
    { CellularNetwork::C_GREG, "AT+CGREG", "+CGREG:"},
    { CellularNetwork::C_REG,  "AT+CREG", "+CREG:"}
};

AT_CellularNetwork::AT_CellularNetwork(ATHandler &atHandler) : AT_CellularBase(atHandler),
    _stack(NULL), _apn(NULL), _uname(NULL), _pwd(NULL), _ip_stack_type_requested(DEFAULT_STACK),
    _ip_stack_type(DEFAULT_STACK), _cid(-1), _connection_status_cb(NULL), _op_act(RAT_UNKNOWN),
    _authentication_type(CHAP), _cell_id(-1), _connect_status(NSAPI_STATUS_DISCONNECTED), _new_context_set(false),
    _is_context_active(false), _reg_status(NotRegistered), _current_act(RAT_UNKNOWN)
{
}

AT_CellularNetwork::~AT_CellularNetwork()
{
#if NSAPI_PPP_AVAILABLE
    (void)disconnect();
#else
    delete _stack;
#endif // NSAPI_PPP_AVAILABLE

    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (has_registration((RegistrationType)type)) {
            _at.remove_urc_handler(at_reg[type].urc_prefix, _urc_funcs[type]);
        }
    }

    _at.remove_urc_handler("NO CARRIER", callback(this, &AT_CellularNetwork::urc_no_carrier));
    _at.remove_urc_handler("+CGEV:", callback(this, &AT_CellularNetwork::urc_cgev));
    free_credentials();
}

nsapi_error_t AT_CellularNetwork::init()
{
    _urc_funcs[C_EREG] = callback(this, &AT_CellularNetwork::urc_cereg);
    _urc_funcs[C_GREG] = callback(this, &AT_CellularNetwork::urc_cgreg);
    _urc_funcs[C_REG] = callback(this, &AT_CellularNetwork::urc_creg);

    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (has_registration((RegistrationType)type)) {
            if (_at.set_urc_handler(at_reg[type].urc_prefix, _urc_funcs[type]) != NSAPI_ERROR_OK) {
                return NSAPI_ERROR_NO_MEMORY;
            }
        }
    }

    return _at.set_urc_handler("NO CARRIER", callback(this, &AT_CellularNetwork::urc_no_carrier));
}

void AT_CellularNetwork::free_credentials()
{
    if (_uname) {
        free(_uname);
        _uname = NULL;
    }

    if (_pwd) {
        free(_pwd);
        _pwd = NULL;
    }

    if (_apn) {
        free(_apn);
        _apn = NULL;
    }
}

void AT_CellularNetwork::urc_no_carrier()
{
    tr_error("Data call failed: no carrier");
    call_network_cb(NSAPI_STATUS_DISCONNECTED);
}

void AT_CellularNetwork::urc_cgev()
{
    char buf[13];
    if (_at.read_string(buf, 13) < 8) { // smallest string length we wan't to compare is 8
        return;
    }
    tr_debug("urc_cgev: %s", buf);

    bool call_cb = false;
    // NOTE! If in future there will be 2 or more active contexts we might wan't to read context id also but not for now.

    if (memcmp(buf, "NW DETACH", 9) == 0) { // The network has forced a PS detach
        call_cb = true;
    } else if (memcmp(buf, "ME DETACH", 9) == 0) {// The mobile termination has forced a PS detach.
        call_cb = true;
    } else if (memcmp(buf, "NW DEACT", 8) == 0) {// The network has forced a context deactivation
        call_cb = true;
    } else if (memcmp(buf, "ME DEACT", 8) == 0) {// The mobile termination has forced a context deactivation
        call_cb = true;
    } else if (memcmp(buf, "NW PDN DEACT", 12) == 0) {// The network has deactivated a context
        call_cb = true;
    } else if (memcmp(buf, "ME PDN DEACT", 12) == 0) {// The mobile termination has deactivated a context.
        call_cb = true;
    }

    if (call_cb) {
        call_network_cb(NSAPI_STATUS_DISCONNECTED);
    }
}

void AT_CellularNetwork::read_reg_params_and_compare(RegistrationType type)
{
    RegistrationStatus reg_status = NotRegistered;
    int lac = -1, cell_id = -1, act = -1;

    read_reg_params(type, reg_status, lac, cell_id, act);

#if MBED_CONF_MBED_TRACE_ENABLE
    switch (reg_status) {
        case NotRegistered:
            tr_warn("not registered");
            break;
        case RegistrationDenied:
            tr_warn("registration denied");
            break;
        case Unknown:
            tr_warn("registration status unknown");
            break;
        default:
            break;
    }
#endif

    if (_at.get_last_error() == NSAPI_ERROR_OK && _connection_status_cb) {
        tr_debug("stat: %d, lac: %d, cellID: %d, act: %d", reg_status, lac, cell_id, act);
        if (act != -1 && (RadioAccessTechnology)act != _current_act) {
            _current_act = (RadioAccessTechnology)act;
            _connection_status_cb((nsapi_event_t)CellularRadioAccessTechnologyChanged, _current_act);
        }
        if (reg_status != _reg_status) {
            _reg_status = reg_status;
            _connection_status_cb((nsapi_event_t)CellularRegistrationStatusChanged, _reg_status);
        }
        if (cell_id != -1 && cell_id != _cell_id) {
            _cell_id = cell_id;
            _connection_status_cb((nsapi_event_t)CellularCellIDChanged, _cell_id);
        }
    }
}

void AT_CellularNetwork::urc_creg()
{
    tr_debug("urc_creg");
    read_reg_params_and_compare(C_REG);
}

void AT_CellularNetwork::urc_cereg()
{
    tr_debug("urc_cereg");
    read_reg_params_and_compare(C_EREG);
}

void AT_CellularNetwork::urc_cgreg()
{
    tr_debug("urc_cgreg");
    read_reg_params_and_compare(C_GREG);
}

nsapi_error_t AT_CellularNetwork::set_credentials(const char *apn,
        const char *username, const char *password)
{
    free_credentials();

    size_t len;
    if (apn && (len = strlen(apn)) > 0) {
        _apn = (char *)malloc(len * sizeof(char) + 1);
        if (_apn) {
            memcpy(_apn, apn, len + 1);
        } else {
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    if (username && (len = strlen(username)) > 0) {
        _uname = (char *)malloc(len * sizeof(char) + 1);
        if (_uname) {
            memcpy(_uname, username, len + 1);
        } else {
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    if (password && (len = strlen(password)) > 0) {
        _pwd = (char *)malloc(len * sizeof(char) + 1);
        if (_pwd) {
            memcpy(_pwd, password, len + 1);
        } else {
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_credentials(const char *apn,
        AuthenticationType type, const char *username, const char *password)
{
    nsapi_error_t err = set_credentials(apn, username, password);
    if (err) {
        return err;
    }

    _authentication_type = type;

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::connect(const char *apn,
        const char *username, const char *password)
{
    nsapi_error_t err = set_credentials(apn, username, password);
    if (err) {
        return err;
    }

    return connect();
}

nsapi_error_t AT_CellularNetwork::delete_current_context()
{
    tr_info("Delete context %d", _cid);
    _at.clear_error();
    _at.cmd_start("AT+CGDCONT=");
    _at.write_int(_cid);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        _cid = -1;
        _new_context_set = false;
    }

    return _at.get_last_error();
}

nsapi_error_t AT_CellularNetwork::activate_context()
{
    _at.lock();

    nsapi_error_t err = NSAPI_ERROR_OK;

    // try to find or create context with suitable stack
    if (get_context()) {
        // try to authenticate user before activating or modifying context
        err = do_user_authentication();
    } else {
        err = NSAPI_ERROR_NO_CONNECTION;
    }

    if (err != NSAPI_ERROR_OK) {
        _at.unlock();
        tr_error("Failed to activate network context! (%d)", err);
        call_network_cb(NSAPI_STATUS_DISCONNECTED);

        return err;
    }

    // do check for stack to validate that we have support for stack
    _stack = get_stack();
    if (!_stack) {
        tr_error("No cellular stack!");
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _is_context_active = false;
    _at.cmd_start("AT+CGACT?");
    _at.cmd_stop();
    _at.resp_start("+CGACT:");
    while (_at.info_resp()) {
        int context_id = _at.read_int();
        int context_activation_state = _at.read_int();
        if (context_id == _cid && context_activation_state == 1) {
            _is_context_active = true;
        }
    }
    _at.resp_stop();

    if (!_is_context_active) {
        tr_info("Activate PDP context %d", _cid);
        _at.cmd_start("AT+CGACT=1,");
        _at.write_int(_cid);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    err = (_at.get_last_error() == NSAPI_ERROR_OK) ? NSAPI_ERROR_OK : NSAPI_ERROR_NO_CONNECTION;

    // If new PDP context was created and failed to activate, delete it
    if (err != NSAPI_ERROR_OK && _new_context_set) {
        delete_current_context();
    } else if (err == NSAPI_ERROR_OK) {
        _is_context_active = true;
    }

    _at.unlock();

    return err;
}

nsapi_error_t AT_CellularNetwork::connect()
{
    call_network_cb(NSAPI_STATUS_CONNECTING);

    nsapi_error_t err = NSAPI_ERROR_OK;
    if (!_is_context_active) {
        err = activate_context();
    }
    if (err) {
        call_network_cb(NSAPI_STATUS_DISCONNECTED);
        return err;
    }

#if NSAPI_PPP_AVAILABLE
    _at.lock();
    err = open_data_channel();
    _at.unlock();
    if (err != NSAPI_ERROR_OK) {
        tr_error("Failed to open data channel!");
        call_network_cb(NSAPI_STATUS_DISCONNECTED);
        return err;
    }
#else
    // additional urc to get better disconnect info for application. Not critical so not returning an error in case of failure
    err = _at.set_urc_handler("+CGEV:", callback(this, &AT_CellularNetwork::urc_cgev));
    if (err == NSAPI_ERROR_OK) {
        _at.lock();
        _at.cmd_start("AT+CGEREP=1");
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
        _at.unlock();
    }

    call_network_cb(NSAPI_STATUS_GLOBAL_UP);
#endif

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::open_data_channel()
{
#if NSAPI_PPP_AVAILABLE
    tr_info("Open data channel in PPP mode");
    _at.cmd_start("AT+CGDATA=\"PPP\",");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("CONNECT", true);
    if (_at.get_last_error()) {
        tr_error("Failed to CONNECT");
        return _at.get_last_error();
    }

    _at.set_is_filehandle_usable(false);

    /* Initialize PPP
     * If blocking: mbed_ppp_init() is a blocking call, it will block until
                  connected, or timeout after 30 seconds*/
    return nsapi_ppp_connect(_at.get_file_handle(), callback(this, &AT_CellularNetwork::ppp_status_cb), NULL, NULL, _ip_stack_type);
#else
    return NSAPI_ERROR_OK;
#endif // #if NSAPI_PPP_AVAILABLE
}

/**
 * User initiated disconnect
 *
 * Disconnects from PPP connection only and brings down the underlying network
 * interface
 */
nsapi_error_t AT_CellularNetwork::disconnect()
{
#if NSAPI_PPP_AVAILABLE
    nsapi_error_t err = nsapi_ppp_disconnect(_at.get_file_handle());
    // after ppp disconnect if we wan't to use same at handler we need to set filehandle again to athandler so it
    // will set the correct sigio and nonblocking
    _at.lock();
    _at.set_file_handle(_at.get_file_handle());
    _at.set_is_filehandle_usable(true);
    _at.unlock();
    return err;
#else
    _at.lock();
    _at.cmd_start("AT+CGACT=0,");
    _at.write_int(_cid);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    _at.restore_at_timeout();

    _at.remove_urc_handler("+CGEV:", callback(this, &AT_CellularNetwork::urc_cgev));
    call_network_cb(NSAPI_STATUS_DISCONNECTED);

    return _at.unlock_return_error();
#endif
}

void AT_CellularNetwork::call_network_cb(nsapi_connection_status_t status)
{
    if (_connect_status != status) {
        _connect_status = status;
        if (_connection_status_cb) {
            _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connect_status);
        }
    }
}

void AT_CellularNetwork::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
}

nsapi_connection_status_t AT_CellularNetwork::get_connection_status() const
{
    return _connect_status;
}

nsapi_error_t AT_CellularNetwork::set_blocking(bool blocking)
{
#if NSAPI_PPP_AVAILABLE
    return nsapi_ppp_set_blocking(blocking);
#else
    return NSAPI_ERROR_OK;
#endif
}

#if NSAPI_PPP_AVAILABLE
void AT_CellularNetwork::ppp_status_cb(nsapi_event_t event, intptr_t parameter)
{
    _connect_status = (nsapi_connection_status_t)parameter;

    if (_connection_status_cb) {
        _connection_status_cb(event, parameter);
    }
}
#endif

nsapi_error_t AT_CellularNetwork::do_user_authentication()
{
    // if user has defined user name and password we need to call CGAUTH before activating or modifying context
    if (_pwd && _uname) {
        _at.cmd_start("AT+CGAUTH=");
        _at.write_int(_cid);
        _at.write_int(_authentication_type);
        _at.write_string(_uname);
        _at.write_string(_pwd);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_AUTH_FAILURE;
        }
    }

    return NSAPI_ERROR_OK;
}

bool AT_CellularNetwork::set_new_context(int cid)
{
    nsapi_ip_stack_t tmp_stack = _ip_stack_type_requested;

    if (tmp_stack == DEFAULT_STACK) {
        bool modem_supports_ipv6 = get_modem_stack_type(IPV6_STACK);
        bool modem_supports_ipv4 = get_modem_stack_type(IPV4_STACK);

        if (modem_supports_ipv6 && modem_supports_ipv4) {
            tmp_stack = IPV4V6_STACK;
        } else if (modem_supports_ipv6) {
            tmp_stack = IPV6_STACK;
        } else if (modem_supports_ipv4) {
            tmp_stack = IPV4_STACK;
        }
    }

    char pdp_type[8 + 1] = {0};

    switch (tmp_stack) {
        case IPV4_STACK:
            strncpy(pdp_type, "IP", sizeof(pdp_type));
            break;
        case IPV6_STACK:
            strncpy(pdp_type, "IPV6", sizeof(pdp_type));
            break;
        case IPV4V6_STACK:
            strncpy(pdp_type, "IPV6", sizeof(pdp_type)); // try first IPV6 and then fall-back to IPv4
            break;
        default:
            break;
    }

    //apn: "If the value is null or omitted, then the subscription value will be requested."
    bool success = false;
    _at.cmd_start("AT+CGDCONT=");
    _at.write_int(cid);
    _at.write_string(pdp_type);
    _at.write_string(_apn);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    success = (_at.get_last_error() == NSAPI_ERROR_OK);

    // Fall back to ipv4
    if (!success && tmp_stack == IPV4V6_STACK) {
        tmp_stack = IPV4_STACK;
        _at.cmd_start("AT+FCLASS=0;+CGDCONT=");
        _at.write_int(cid);
        _at.write_string("IP");
        _at.write_string(_apn);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
        success = (_at.get_last_error() == NSAPI_ERROR_OK);
    }

    if (success) {
        _ip_stack_type = tmp_stack;
        _cid = cid;
        _new_context_set = true;
        tr_info("New PDP context id %d was created", _cid);
    }

    return success;
}

bool AT_CellularNetwork::get_context()
{
    if (_apn) {
        tr_debug("APN in use: %s", _apn);
    } else {
        tr_debug("NO APN");
    }

    _at.cmd_start("AT+CGDCONT?");
    _at.cmd_stop();
    _at.resp_start("+CGDCONT:");
    _cid = -1;
    int cid_max = 0; // needed when creating new context
    char apn[MAX_ACCESSPOINT_NAME_LENGTH];
    int apn_len = 0;

    bool modem_supports_ipv6 = get_modem_stack_type(IPV6_STACK);
    bool modem_supports_ipv4 = get_modem_stack_type(IPV4_STACK);

    while (_at.info_resp()) {
        int cid = _at.read_int();
        if (cid > cid_max) {
            cid_max = cid;
        }
        char pdp_type_from_context[10];
        int pdp_type_len = _at.read_string(pdp_type_from_context, sizeof(pdp_type_from_context) - 1);
        if (pdp_type_len > 0) {
            apn_len = _at.read_string(apn, sizeof(apn) - 1);
            if (apn_len >= 0) {
                if (_apn && (strcmp(apn, _apn) != 0)) {
                    continue;
                }
                nsapi_ip_stack_t pdp_stack = string_to_stack_type(pdp_type_from_context);
                // Accept dual PDP context for IPv4/IPv6 only modems
                if (pdp_stack != DEFAULT_STACK && (get_modem_stack_type(pdp_stack) || pdp_stack == IPV4V6_STACK)) {
                    if (_ip_stack_type_requested == IPV4_STACK) {
                        if (pdp_stack == IPV4_STACK || pdp_stack == IPV4V6_STACK) {
                            _ip_stack_type = _ip_stack_type_requested;
                            _cid = cid;
                            break;
                        }
                    } else if (_ip_stack_type_requested == IPV6_STACK) {
                        if (pdp_stack == IPV6_STACK || pdp_stack == IPV4V6_STACK) {
                            _ip_stack_type = _ip_stack_type_requested;
                            _cid = cid;
                            break;
                        }
                    } else {
                        // requested dual stack or stack is not specified
                        // If dual PDP need to check for IPV4 or IPV6 modem support. Prefer IPv6.
                        if (pdp_stack == IPV4V6_STACK) {
                            if (modem_supports_ipv6) {
                                _ip_stack_type = IPV6_STACK;
                                _cid = cid;
                                break;
                            } else if (modem_supports_ipv4) {
                                _ip_stack_type = IPV4_STACK;
                                _cid = cid;
                                break;
                            }
                            // If PDP is IPV4 or IPV6 they are already checked if supported
                        } else {
                            _ip_stack_type = pdp_stack;
                            _cid = cid;

                            if (pdp_stack == IPV6_STACK) {
                                break;
                            }
                            if (pdp_stack == IPV4_STACK && !modem_supports_ipv6) {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    _at.resp_stop();
    if (_cid == -1) { // no suitable context was found so create a new one
        if (!set_new_context(cid_max + 1)) {
            return false;
        }
    }

    // save the apn
    if (apn_len > 0 && !_apn) {
        _apn = (char *)malloc(apn_len * sizeof(char) + 1);
        if (_apn) {
            memcpy(_apn, apn, apn_len + 1);
        } else {
            return false;
        }
    }

    tr_debug("Context id %d", _cid);
    return true;
}

nsapi_ip_stack_t AT_CellularNetwork::string_to_stack_type(const char *pdp_type)
{
    nsapi_ip_stack_t stack = DEFAULT_STACK;
    int len = strlen(pdp_type);

    if (len == 6 && memcmp(pdp_type, "IPV4V6", len) == 0) {
        stack = IPV4V6_STACK;
    } else if (len == 4 && memcmp(pdp_type, "IPV6", len) == 0) {
        stack = IPV6_STACK;
    } else if (len == 2 && memcmp(pdp_type, "IP", len) == 0) {
        stack = IPV4_STACK;
    }
    return stack;
}

nsapi_error_t AT_CellularNetwork::set_registration_urc(RegistrationType type, bool urc_on)
{
    int index = (int)type;
    MBED_ASSERT(index >= 0 && index < C_MAX);

    if (!has_registration(type)) {
        return NSAPI_ERROR_UNSUPPORTED;
    } else {
        _at.lock();
        if (urc_on) {
            _at.cmd_start(at_reg[index].cmd);
            _at.write_string("=2", false);
            _at.cmd_stop();
        } else {
            _at.cmd_start(at_reg[index].cmd);
            _at.write_string("=0", false);
            _at.cmd_stop();
        }

        _at.resp_start();
        _at.resp_stop();
        return _at.unlock_return_error();
    }
}

nsapi_error_t AT_CellularNetwork::get_network_registering_mode(NWRegisteringMode &mode)
{
    _at.lock();
    _at.cmd_start("AT+COPS?");
    _at.cmd_stop();
    _at.resp_start("+COPS:");
    mode = (NWRegisteringMode)_at.read_int();
    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::set_registration(const char *plmn)
{
    _at.lock();

    if (!plmn) {
        tr_debug("Automatic network registration");
        _at.cmd_start("AT+COPS?");
        _at.cmd_stop();
        _at.resp_start("+COPS:");
        int mode = _at.read_int();
        _at.resp_stop();
        if (mode != 0) {
            _at.clear_error();
            _at.cmd_start("AT+COPS=0");
            _at.cmd_stop();
            _at.resp_start();
            _at.resp_stop();
        }
    } else {
        tr_debug("Manual network registration to %s", plmn);
        _at.cmd_start("AT+COPS=4,2,");
        _at.write_string(plmn);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    return _at.unlock_return_error();
}

void AT_CellularNetwork::read_reg_params(RegistrationType type, RegistrationStatus &reg_status, int &lac, int &cell_id, int &act)
{
    const int LAC_LENGTH = 5, CELL_ID_LENGTH = 9;
    char lac_string[LAC_LENGTH] = {0}, cell_id_string[CELL_ID_LENGTH] = {0};
    bool lac_read = false, cell_id_read = false;

    reg_status = (RegistrationStatus)_at.read_int();

    int len = _at.read_string(lac_string, LAC_LENGTH);
    if (memcmp(lac_string, "ffff", LAC_LENGTH - 1) && len >= 0) {
        lac_read = true;
    }

    len = _at.read_string(cell_id_string, CELL_ID_LENGTH);
    if (memcmp(cell_id_string, "ffffffff", CELL_ID_LENGTH - 1) && len >= 0) {
        cell_id_read = true;
    }

    act = _at.read_int();

    if (lac_read) {
        lac = hex_str_to_int(lac_string, LAC_LENGTH);
        tr_debug("lac %s %d", lac_string, lac);
    }

    if (cell_id_read) {
        cell_id = hex_str_to_int(cell_id_string, CELL_ID_LENGTH);
        tr_debug("cell_id %s %d", cell_id_string, cell_id);
    }
}

nsapi_error_t AT_CellularNetwork::get_registration_status(RegistrationType type, RegistrationStatus &status)
{
    int i = (int)type;
    MBED_ASSERT(i >= 0 && i < C_MAX);

    if (!has_registration(at_reg[i].type)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _at.lock();

    const char *rsp[] = { "+CEREG:", "+CGREG:", "+CREG:"};
    _at.cmd_start(at_reg[i].cmd);
    _at.write_string("?", false);
    _at.cmd_stop();
    _at.resp_start(rsp[i]);

    (void)_at.read_int(); // ignore urc mode subparam
    int lac = -1, cell_id = -1, act = -1;
    read_reg_params(type, status, lac, cell_id, act);
    _at.resp_stop();
    _reg_status = status;

    if (cell_id != -1) {
        _cell_id = cell_id;
    }
    if (act != -1) {
        _current_act = (RadioAccessTechnology)act;
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_cell_id(int &cell_id)
{
    cell_id = _cell_id;
    return NSAPI_ERROR_OK;
}

bool AT_CellularNetwork::has_registration(RegistrationType reg_type)
{
    (void)reg_type;
    return true;
}

nsapi_error_t AT_CellularNetwork::set_attach(int /*timeout*/)
{
    _at.lock();

    _at.cmd_start("AT+CGATT?");
    _at.cmd_stop();
    _at.resp_start("+CGATT:");
    int attached_state = _at.read_int();
    _at.resp_stop();
    if (attached_state != 1) {
        tr_debug("Network attach");
        _at.cmd_start("AT+CGATT=1");
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_attach(AttachStatus &status)
{
    _at.lock();

    _at.cmd_start("AT+CGATT?");
    _at.cmd_stop();

    _at.resp_start("+CGATT:");
    if (_at.info_resp()) {
        int attach_status = _at.read_int();
        status = (attach_status == 1) ? Attached : Detached;
    }
    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::detach()
{
    _at.lock();

    tr_debug("Network detach");
    _at.cmd_start("AT+CGATT=0");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    call_network_cb(NSAPI_STATUS_DISCONNECTED);

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_apn_backoff_timer(int &backoff_timer)
{
    // If apn is set
    if (_apn) {
        _at.lock();
        _at.cmd_start("AT+CABTRDP=");
        _at.write_string(_apn);
        _at.cmd_stop();
        _at.resp_start("+CABTRDP:");
        if (_at.info_resp()) {
            _at.skip_param();
            backoff_timer = _at.read_int();
        }
        _at.resp_stop();
        return _at.unlock_return_error();
    }

    return NSAPI_ERROR_PARAMETER;
}

NetworkStack *AT_CellularNetwork::get_stack()
{
#if NSAPI_PPP_AVAILABLE
    // use lwIP/PPP if modem does not have IP stack
    if (!_stack) {
        _stack = nsapi_ppp_get_stack();
    }
#endif
    return _stack;
}

const char *AT_CellularNetwork::get_ip_address()
{
#if NSAPI_PPP_AVAILABLE
    return nsapi_ppp_get_ip_addr(_at.get_file_handle());
#else
    if (!_stack) {
        _stack = get_stack();
    }
    if (_stack) {
        return _stack->get_ip_address();
    }
    return NULL;
#endif
}

nsapi_error_t AT_CellularNetwork::set_stack_type(nsapi_ip_stack_t stack_type)
{
    if (get_modem_stack_type(stack_type)) {
        _ip_stack_type_requested = stack_type;
        return NSAPI_ERROR_OK;
    } else {
        return NSAPI_ERROR_PARAMETER;
    }
}

nsapi_ip_stack_t AT_CellularNetwork::get_stack_type()
{
    return _ip_stack_type;
}

bool AT_CellularNetwork::get_modem_stack_type(nsapi_ip_stack_t requested_stack)
{
    if (requested_stack == _ip_stack_type) {
        return true;
    } else {
        return false;
    }
}

nsapi_error_t AT_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularNetwork::get_access_technology(RadioAccessTechnology &rat)
{
    rat = _current_act;
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_access_technology(RadioAccessTechnology opAct)
{
    if (opAct == RAT_UNKNOWN) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _op_act = opAct;

    return set_access_technology_impl(opAct);
}

nsapi_error_t AT_CellularNetwork::scan_plmn(operList_t &operators, int &opsCount)
{
    int idx = 0;

    _at.lock();

    _at.cmd_start("AT+COPS=?");
    _at.cmd_stop();

    _at.resp_start("+COPS:");

    int ret, error_code = -1;
    operator_t *op = NULL;

    while (_at.info_elem('(')) {

        op = operators.add_new();
        if (!op) {
            tr_warn("Could not allocate new operator");
            _at.resp_stop();
            _at.unlock();
            operators.delete_all();
            opsCount = 0;
            return NSAPI_ERROR_NO_MEMORY;
        }

        op->op_status = (operator_t::Status)_at.read_int();
        _at.read_string(op->op_long, sizeof(op->op_long));
        _at.read_string(op->op_short, sizeof(op->op_short));
        _at.read_string(op->op_num, sizeof(op->op_num));

        // Optional - try read an int
        ret = _at.read_int();
        op->op_rat = (ret == error_code) ? RAT_UNKNOWN : (RadioAccessTechnology)ret;

        if ((_op_act == RAT_UNKNOWN) ||
                ((op->op_rat != RAT_UNKNOWN) && (op->op_rat == _op_act))) {
            idx++;
        } else {
            operators.delete_last();
        }
    }

    _at.resp_stop();

    opsCount = idx;
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::set_ciot_optimization_config(Supported_UE_Opt supported_opt,
        Preferred_UE_Opt preferred_opt)
{
    _at.lock();

    _at.cmd_start("AT+CCIOTOPT=");
    _at.write_int(_cid);
    _at.write_int(supported_opt);
    _at.write_int(preferred_opt);
    _at.cmd_stop();

    _at.resp_start();
    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_ciot_optimization_config(Supported_UE_Opt &supported_opt,
        Preferred_UE_Opt &preferred_opt)
{
    _at.lock();

    _at.cmd_start("AT+CCIOTOPT?");
    _at.cmd_stop();

    _at.resp_start("+CCIOTOPT:");
    _at.read_int();
    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        supported_opt = (Supported_UE_Opt)_at.read_int();
        preferred_opt = (Preferred_UE_Opt)_at.read_int();
    }

    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_rate_control(
    CellularNetwork::RateControlExceptionReports &reports,
    CellularNetwork::RateControlUplinkTimeUnit &timeUnit, int &uplinkRate)
{

    _at.lock();

    _at.cmd_start("AT+CGAPNRC=");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("+CGAPNRC:");
    _at.read_int();
    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        bool comma_found = true;
        int next_element = _at.read_int();
        if (next_element >= 0) {
            reports = (RateControlExceptionReports)next_element;
            tr_debug("reports %d", reports);
            next_element = _at.read_int();
        } else {
            comma_found = false;
        }

        if (comma_found && next_element >= 0) {
            timeUnit = (RateControlUplinkTimeUnit)next_element;
            tr_debug("time %d", timeUnit);
            next_element = _at.read_int();
        } else {
            comma_found = false;
        }

        if (comma_found && next_element >= 0) {
            uplinkRate = next_element;
            tr_debug("rate %d", uplinkRate);
        }
    }
    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_pdpcontext_params(pdpContextList_t &params_list)
{
    const int ipv6_subnet_size = 128;
    const int max_ipv6_size = 64;
    char *ipv6_and_subnetmask = (char *)malloc(ipv6_subnet_size);
    if (!ipv6_and_subnetmask) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    char *temp = (char *)malloc(max_ipv6_size);
    if (!temp) {
        free(ipv6_and_subnetmask);
        return NSAPI_ERROR_NO_MEMORY;
    }

    _at.lock();

    _at.cmd_start("AT+CGCONTRDP=");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("+CGCONTRDP:");
    pdpcontext_params_t *params = NULL;
    while (_at.info_resp()) { // response can be zero or many +CGDCONT lines
        params = params_list.add_new();
        if (!params) {
            tr_warn("Could not allocate new pdpcontext_params_t");
            _at.resp_stop();
            _at.unlock();
            params_list.delete_all();
            free(temp);
            free(ipv6_and_subnetmask);
            return NSAPI_ERROR_NO_MEMORY;
        }

        params->cid = _at.read_int();
        params->bearer_id = _at.read_int();
        _at.read_string(params->apn, sizeof(params->apn));

        // rest are optional params
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';
        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->local_addr, sizeof(params->local_addr), params->local_subnet_mask, sizeof(params->local_subnet_mask));
        ipv6_and_subnetmask[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->gateway_addr, sizeof(params->gateway_addr), temp, max_ipv6_size);
        prefer_ipv6(params->gateway_addr, sizeof(params->gateway_addr), temp, max_ipv6_size);
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->dns_primary_addr, sizeof(params->dns_primary_addr), temp, max_ipv6_size);
        prefer_ipv6(params->dns_primary_addr, sizeof(params->dns_primary_addr), temp, max_ipv6_size);
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->dns_secondary_addr, sizeof(params->dns_secondary_addr), temp, max_ipv6_size);
        prefer_ipv6(params->dns_secondary_addr, sizeof(params->dns_secondary_addr), temp, max_ipv6_size);
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->p_cscf_prim_addr, sizeof(params->p_cscf_prim_addr), temp, max_ipv6_size);
        prefer_ipv6(params->p_cscf_prim_addr, sizeof(params->p_cscf_prim_addr), temp, max_ipv6_size);
        ipv6_and_subnetmask[0] = '\0';
        temp[0] = '\0';

        _at.read_string(ipv6_and_subnetmask, ipv6_subnet_size);
        separate_ip_addresses(ipv6_and_subnetmask, params->p_cscf_sec_addr, sizeof(params->p_cscf_sec_addr), temp, max_ipv6_size);
        prefer_ipv6(params->p_cscf_sec_addr, sizeof(params->p_cscf_sec_addr), temp, max_ipv6_size);

        params->im_signalling_flag = _at.read_int();
        params->lipa_indication = _at.read_int();
        params->ipv4_mtu = _at.read_int();
        params->wlan_offload = _at.read_int();
        params->local_addr_ind = _at.read_int();
        params->non_ip_mtu = _at.read_int();
        params->serving_plmn_rate_control_value = _at.read_int();
    }
    _at.resp_stop();

    free(temp);
    free(ipv6_and_subnetmask);

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp)
{
    _at.lock();

    _at.cmd_start("AT+CESQ");
    _at.cmd_stop();

    _at.resp_start("+CESQ:");
    rxlev = _at.read_int();
    ber = _at.read_int();
    rscp = _at.read_int();
    ecno = _at.read_int();
    rsrq = _at.read_int();
    rsrp = _at.read_int();
    _at.resp_stop();
    if (rxlev < 0 || ber < 0 || rscp < 0 || ecno < 0 || rsrq < 0 || rsrp < 0) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_signal_quality(int &rssi, int &ber)
{
    _at.lock();

    _at.cmd_start("AT+CSQ");
    _at.cmd_stop();

    _at.resp_start("+CSQ:");
    rssi = _at.read_int();
    ber = _at.read_int();
    _at.resp_stop();
    if (rssi < 0 || ber < 0) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return _at.unlock_return_error();
}

/** Get the last 3GPP error code
 *  @return see 3GPP TS 27.007 error codes
 */
int AT_CellularNetwork::get_3gpp_error()
{
    return _at.get_3gpp_error();
}

nsapi_error_t AT_CellularNetwork::get_operator_params(int &format, operator_t &operator_params)
{
    _at.lock();

    _at.cmd_start("AT+COPS?");
    _at.cmd_stop();

    _at.resp_start("+COPS:");
    _at.read_int(); //ignore mode
    format = _at.read_int();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        switch (format) {
            case 0:
                _at.read_string(operator_params.op_long, sizeof(operator_params.op_long));
                break;
            case 1:
                _at.read_string(operator_params.op_short, sizeof(operator_params.op_short));
                break;
            default:
                _at.read_string(operator_params.op_num, sizeof(operator_params.op_num));
                break;
        }
        operator_params.op_rat = (RadioAccessTechnology)_at.read_int();
    }

    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_operator_names(operator_names_list &op_names)
{
    _at.lock();

    _at.cmd_start("AT+COPN?");
    _at.cmd_stop();

    _at.resp_start("+COPN:");
    operator_names_t *names = NULL;
    while (_at.info_resp()) {
        names = op_names.add_new();
        if (!names) {
            tr_warn("Could not allocate new operator_names_t");
            _at.resp_stop();
            _at.unlock();
            op_names.delete_all();
            return NSAPI_ERROR_NO_MEMORY;
        }
        _at.read_string(names->numeric, sizeof(names->numeric));
        _at.read_string(names->alpha, sizeof(names->alpha));
    }

    _at.resp_stop();
    return _at.unlock_return_error();
}
