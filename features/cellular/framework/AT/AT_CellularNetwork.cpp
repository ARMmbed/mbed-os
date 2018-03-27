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

using namespace std;
using namespace mbed_cellular_util;
using namespace mbed;

struct at_reg_t {
    const CellularNetwork::RegistrationType type;
    const char *const cmd;
};

static const at_reg_t at_reg[] = {
    { CellularNetwork::C_EREG, "AT+CEREG" },
    { CellularNetwork::C_GREG, "AT+CGREG" },
    { CellularNetwork::C_REG,  "AT+CREG"  },
};

AT_CellularNetwork::AT_CellularNetwork(ATHandler &atHandler) : AT_CellularBase(atHandler),
    _stack(NULL), _apn(NULL), _uname(NULL), _pwd(NULL), _ip_stack_type_requested(DEFAULT_STACK), _ip_stack_type(DEFAULT_STACK), _cid(-1),
    _connection_status_cb(NULL), _op_act(operator_t::RAT_UNKNOWN), _authentication_type(CHAP), _last_reg_type(C_REG),
    _connect_status(NSAPI_STATUS_DISCONNECTED), _new_context_set(false)
{

    _at.set_urc_handler("NO CARRIER", callback(this, &AT_CellularNetwork::urc_no_carrier));
}

AT_CellularNetwork::~AT_CellularNetwork()
{
    free_credentials();
}

void AT_CellularNetwork::free_credentials()
{
    if (_uname) {
        free(_uname);
    }

    if (_pwd) {
        free(_pwd);
    }

    if (_apn) {
        free(_apn);
    }
}

void AT_CellularNetwork::urc_no_carrier()
{
    _connect_status = NSAPI_STATUS_DISCONNECTED;
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
    }
}

nsapi_error_t AT_CellularNetwork::set_credentials(const char *apn,
        const char *username, const char *password)
{
    size_t len;
    if (apn && (len = strlen(apn)) > 0) {
        _apn = (char*)malloc(len*sizeof(char)+1);
        if (_apn) {
            memcpy(_apn, apn, len+1);
        } else {
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    if (username && (len = strlen(username)) > 0) {
        _uname = (char*)malloc(len*sizeof(char)+1);
        if (_uname) {
            memcpy(_uname, username, len+1);
        } else {
            return NSAPI_ERROR_NO_MEMORY;
        }
    }

    if (password && (len = strlen(password)) > 0) {
        _pwd = (char*)malloc(len*sizeof(char)+1);
        if (_pwd) {
            memcpy(_pwd, password, len+1);
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

nsapi_error_t AT_CellularNetwork::connect()
{
    _at.lock();

    _connect_status = NSAPI_STATUS_CONNECTING;
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_CONNECTING);
    }

    nsapi_error_t err = set_context_to_be_activated();
    if (err != NSAPI_ERROR_OK) {
        _at.unlock();
        tr_error("Failed to activate network context!");

        _connect_status = NSAPI_STATUS_DISCONNECTED;
        if (_connection_status_cb) {
            _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
        }

        return err;
    }

    err = open_data_channel();
    if (err != NSAPI_ERROR_OK) {

        // If new PDP context was created and failed to activate, delete it
        if (_new_context_set) {
            delete_current_context();
        }

        _at.unlock();

        tr_error("Failed to open data channel!");

        _connect_status = NSAPI_STATUS_DISCONNECTED;
        if (_connection_status_cb) {
            _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
        }

        return err;
    }

    _at.unlock();

#if !NSAPI_PPP_AVAILABLE
    _connect_status = NSAPI_STATUS_GLOBAL_UP;
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_GLOBAL_UP);
    }
#endif

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::open_data_channel()
{
    //old way: _at.send("ATD*99***%d#", _cid) && _at.recv("CONNECT");
    nsapi_error_t err = NSAPI_ERROR_NO_CONNECTION;
#if NSAPI_PPP_AVAILABLE
    tr_info("Open data channel in PPP mode");
    _at.cmd_start("AT+CGDATA=\"PPP\",");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("CONNECT", true);
    if (_at.get_last_error()) {
        tr_warn("Failed to CONNECT");
    }
    /* Initialize PPP
     * mbed_ppp_init() is a blocking call, it will block until
     * connected, or timeout after 30 seconds*/
    err = nsapi_ppp_connect(_at.get_file_handle(), callback(this, &AT_CellularNetwork::ppp_status_cb), _uname, _pwd, _ip_stack_type);
#else
    // do check for stack to validate that we have support for stack
    _stack = get_stack();
    if (!_stack) {
        return err;
    }

    bool is_context_active = false;
    _at.cmd_start("AT+CGACT?");
    _at.cmd_stop();
    _at.resp_start("+CGACT:");
    while (_at.info_resp()) {
        int context_id = _at.read_int();
        int context_activation_state = _at.read_int();
        if (context_id == _cid && context_activation_state == 1) {
            is_context_active = true;
            tr_debug("PDP context %d is active.", _cid);
            break;
        }
    }
    _at.resp_stop();

    if (!is_context_active) {
        tr_info("Activate PDP context %d", _cid);
        _at.cmd_start("AT+CGACT=1,");
        _at.write_int(_cid);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    err = (_at.get_last_error() == NSAPI_ERROR_OK) ? NSAPI_ERROR_OK : NSAPI_ERROR_NO_CONNECTION;
#endif
    return err;
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
    return nsapi_ppp_disconnect(_at.get_file_handle());
#else
    _at.lock();
    _at.cmd_start("AT+CGACT=0,");
    _at.write_int(_cid);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    _at.restore_at_timeout();

    _connect_status = NSAPI_STATUS_DISCONNECTED;
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
    }

    return _at.unlock_return_error();
#endif
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
    return NSAPI_ERROR_UNSUPPORTED;
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



nsapi_error_t AT_CellularNetwork::set_context_to_be_activated()
{
    // try to find or create context with suitable stack
    if (!get_context()) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

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

    return _at.get_last_error();
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

    char pdp_type[8+1] = {0};

    switch (tmp_stack) {
        case IPV4_STACK:
            strncpy(pdp_type, "IP", sizeof(pdp_type));
            break;
        case IPV6_STACK:
            strncpy(pdp_type, "IPV6", sizeof(pdp_type));
            break;
        case IPV4V6_STACK:
            strncpy(pdp_type, "IPV4V6", sizeof(pdp_type));
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
                if (_apn && (strcmp(apn, _apn) != 0) ) {
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
        if (!set_new_context(cid_max+1)) {
            return false;
        }
    }

    // save the apn
    if (apn_len > 0 && !_apn) {
        _apn = (char*)malloc(apn_len*sizeof(char)+1);
        if (_apn) {
            memcpy(_apn, apn, apn_len+1);
        } else {
            return false;
        }
    }

    tr_debug("Context id %d", _cid);
    return true;
}

nsapi_ip_stack_t AT_CellularNetwork::string_to_stack_type(const char* pdp_type)
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

nsapi_error_t AT_CellularNetwork::set_registration_urc(bool urc_on)
{
    for (unsigned int i = 0; i < sizeof(at_reg)/sizeof(at_reg[0]); i++) {
        if (has_registration(at_reg[i].type)) {
            _last_reg_type = at_reg[i].type;
            if (urc_on) {
                _at.cmd_start(at_reg[i].cmd);
                _at.write_string("=2", false);
                _at.cmd_stop();
            } else {
                _at.cmd_start(at_reg[i].cmd);
                _at.write_string("=0", false);
                _at.cmd_stop();
            }

            _at.resp_start();
            _at.resp_stop();
        }
    }
    return _at.get_last_error();
}

nsapi_error_t AT_CellularNetwork::set_registration(const char *plmn)
{
    _at.lock();

    nsapi_error_t ret = set_registration_urc(false);
    if (ret) {
        tr_error("Setting registration URC failed!");
        _at.clear_error(); // allow temporary failures here
    }

    if (!plmn) {
        tr_debug("Automatic network registration");
        _at.cmd_start("AT+COPS?");
        _at.cmd_stop();
        _at.resp_start("AT+COPS:");
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

nsapi_error_t AT_CellularNetwork::get_registration_status(RegistrationType type, RegistrationStatus &status)
{
    int i = (int)type;
    MBED_ASSERT(i >= 0 && i < C_MAX);

    const char *rsp[] =            { "+CEREG:",    "+CGREG:",     "+CREG:"};

    const int LAC_LENGTH = 5, CELL_ID_LENGTH = 9;
    char lac_string[LAC_LENGTH] = {0}, cell_id_string[CELL_ID_LENGTH] = {0};
    bool lac_read = false, cell_id_read = false;

    _cell_id = -1;
    _lac = -1;

    _at.lock();

    if (!has_registration(at_reg[i].type)) {
        _at.unlock();
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _at.cmd_start(at_reg[i].cmd);
    _at.write_string("=2", false);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start(at_reg[i].cmd);
    _at.write_string("?", false);
    _at.cmd_stop();

    _at.resp_start(rsp[i]);
    _at.read_int(); // ignore urc mode subparam
    status = (RegistrationStatus)_at.read_int();

    int len = _at.read_string(lac_string, LAC_LENGTH);
    if (memcmp(lac_string, "ffff", LAC_LENGTH-1) && len >= 0) {
        lac_read = true;
    }

    len = _at.read_string(cell_id_string, CELL_ID_LENGTH);
    if (memcmp(cell_id_string, "ffffffff", CELL_ID_LENGTH-1) && len >= 0) {
        cell_id_read = true;
    }

    _at.resp_stop();

    _at.cmd_start(at_reg[i].cmd);
    _at.write_string("=0", false);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    nsapi_error_t ret = _at.get_last_error();
    _at.unlock();

    if (lac_read) {
        _lac = hex_str_to_int(lac_string, LAC_LENGTH);
        tr_debug("lac %s %d", lac_string, _lac );
    }

    if (cell_id_read) {
        _cell_id = hex_str_to_int(cell_id_string, CELL_ID_LENGTH);
        tr_debug("cell_id %s %d", cell_id_string, _cell_id );
    }

    return ret;
}

nsapi_error_t AT_CellularNetwork::get_cell_id(int &cell_id)
{
    RegistrationStatus tmp;

    nsapi_error_t error = get_registration_status(_last_reg_type, tmp);

    cell_id = _cell_id;

    return error;
}

bool AT_CellularNetwork::has_registration(RegistrationType reg_type)
{
    (void)reg_type;
    return true;
}

nsapi_error_t AT_CellularNetwork::set_attach(int timeout)
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


nsapi_error_t AT_CellularNetwork::get_apn_backoff_timer(int &backoff_timer)
{
    _at.lock();

    // If apn is set
    if (_apn) {
        _at.cmd_start("AT+CABTRDP=");
        _at.write_string(_apn);
        _at.cmd_stop();
        _at.resp_start("+CABTRDP:");
        if (_at.info_resp()) {
            _at.skip_param();
            backoff_timer = _at.read_int();
        }
        _at.resp_stop();
    }

    return _at.unlock_return_error();
}

NetworkStack *AT_CellularNetwork::get_stack()
{
    // use lwIP/PPP if modem does not have IP stack
#if NSAPI_PPP_AVAILABLE
    _stack = nsapi_ppp_get_stack();
#else
    _stack = NULL;
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

nsapi_error_t AT_CellularNetwork::set_access_technology_impl(operator_t::RadioAccessTechnology opsAct)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularNetwork::set_access_technology(operator_t::RadioAccessTechnology opAct)
{
    if (opAct == operator_t::RAT_UNKNOWN) {
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

        op->op_status = (operator_t::Status)_at.read_int();
        _at.read_string(op->op_long, sizeof(op->op_long));
        _at.read_string(op->op_short, sizeof(op->op_short));
        _at.read_string(op->op_num, sizeof(op->op_num));

        // Optional - try read an int
        ret = _at.read_int();
        op->op_rat = (ret == error_code) ? operator_t::RAT_UNKNOWN:(operator_t::RadioAccessTechnology)ret;

        if ((_op_act == operator_t::RAT_UNKNOWN) ||
                ((op->op_rat != operator_t::RAT_UNKNOWN) && (op->op_rat == _op_act))) {
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

nsapi_error_t AT_CellularNetwork::get_ciot_optimization_config(Supported_UE_Opt& supported_opt,
        Preferred_UE_Opt& preferred_opt)
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
            tr_debug("reports %d",reports);
            next_element = _at.read_int();
        } else {
            comma_found = false;
        }

        if (comma_found && next_element >= 0) {
            timeUnit = (RateControlUplinkTimeUnit)next_element;
            tr_debug("time %d",timeUnit);
            next_element = _at.read_int();
        } else {
            comma_found = false;
        }

        if (comma_found && next_element >= 0) {
            uplinkRate = next_element;
            tr_debug("rate %d",uplinkRate);
        }
    }
    _at.resp_stop();
    nsapi_error_t ret = _at.get_last_error();
    _at.unlock();

    return (ret == NSAPI_ERROR_OK) ? NSAPI_ERROR_OK : NSAPI_ERROR_PARAMETER;
}

nsapi_error_t AT_CellularNetwork::get_pdpcontext_params(pdpContextList_t& params_list)
{
    const int ipv6_subnet_size = 128;
    const int max_ipv6_size = 64;
    char* ipv6_and_subnetmask = (char*)malloc(ipv6_subnet_size);
    if (!ipv6_and_subnetmask) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    char* temp = (char*)malloc(max_ipv6_size);
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
            params_list.delete_all();
            _at.resp_stop();
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

        operator_params.op_rat = (operator_t::RadioAccessTechnology)_at.read_int();
    }

    _at.resp_stop();

    return _at.unlock_return_error();
}
