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

#include "AT_CellularContext.h"
#include "AT_CellularNetwork.h"
#include "AT_CellularStack.h"
#include "CellularLog.h"
#include "CellularUtil.h"
#include "CellularSIM.h"
#include "UARTSerial.h"
#include "mbed_wait_api.h"

#define NETWORK_TIMEOUT 30 * 60 * 1000 // 30 minutes
#define DEVICE_TIMEOUT 5 * 60 * 1000 // 5 minutes

#if NSAPI_PPP_AVAILABLE
#define AT_SYNC_TIMEOUT 1000 // 1 second timeout
#include "nsapi_ppp.h"
#endif

#define USE_APN_LOOKUP (MBED_CONF_CELLULAR_USE_APN_LOOKUP || (NSAPI_PPP_AVAILABLE && MBED_CONF_PPP_CELL_IFACE_APN_LOOKUP))

#if USE_APN_LOOKUP
#include "APN_db.h"
#endif //USE_APN_LOOKUP

using namespace mbed_cellular_util;
using namespace mbed;

AT_CellularContext::AT_CellularContext(ATHandler &at, CellularDevice *device, const char *apn) :
    AT_CellularBase(at), _ip_stack_type_requested(DEFAULT_STACK), _is_connected(false), _is_blocking(true),
    _current_op(OP_INVALID), _device(device), _nw(0), _fh(0)
{
    tr_info("New CellularContext %s (%p)", apn ? apn : "", this);
    _stack = NULL;
    _ip_stack_type = DEFAULT_STACK;
    _authentication_type = CellularContext::CHAP;
    _connect_status = NSAPI_STATUS_DISCONNECTED;
    _is_context_active = false;
    _is_context_activated = false;
    _apn = apn;
    _uname = NULL;
    _pwd = NULL;
    _status_cb = NULL;
    _cid = -1;
    _new_context_set = false;
    _next = NULL;
}

AT_CellularContext::~AT_CellularContext()
{
    tr_info("Delete CellularContext %s (%p)", _apn ? _apn : "", this);

    (void)disconnect();

    if (_nw) {
        _device->close_network();
    }
}

void AT_CellularContext::set_file_handle(FileHandle *fh)
{
    tr_info("CellularContext filehandle %p", fh);
    _fh = fh;
    _at.set_file_handle(_fh);
}

nsapi_error_t AT_CellularContext::connect()
{
    tr_info("CellularContext connect");
    if (_is_connected) {
        return NSAPI_ERROR_IS_CONNECTED;
    }
    nsapi_error_t err = _device->attach_to_network();
    _cb_data.error = check_operation(err, OP_CONNECT);

    if (_is_blocking) {
        if (_cb_data.error == NSAPI_ERROR_OK || _cb_data.error == NSAPI_ERROR_ALREADY) {
            do_connect();
        }
    } else {
        if (_cb_data.error == NSAPI_ERROR_ALREADY) {
            // device is already attached, to be async we must use queue to connect and give proper callbacks
            int id = _device->get_queue()->call_in(0, this, &AT_CellularContext::do_connect);
            if (id == 0) {
                return NSAPI_ERROR_NO_MEMORY;
            }
            return NSAPI_ERROR_OK;
        }
    }

    return _cb_data.error;
}

nsapi_error_t AT_CellularContext::set_device_ready()
{
    nsapi_error_t err = _device->set_device_ready();
    return check_operation(err, OP_DEVICE_READY);
}

nsapi_error_t AT_CellularContext::set_sim_ready()
{
    nsapi_error_t err = _device->set_sim_ready();
    return check_operation(err, OP_SIM_READY);
}

nsapi_error_t AT_CellularContext::register_to_network()
{
    nsapi_error_t err = _device->register_to_network();
    return check_operation(err, OP_REGISTER);
}

nsapi_error_t AT_CellularContext::attach_to_network()
{
    nsapi_error_t err = _device->attach_to_network();
    return check_operation(err, OP_ATTACH);
}

nsapi_error_t AT_CellularContext::check_operation(nsapi_error_t err, ContextOperation op)
{
    _current_op = op;
    if (err == NSAPI_ERROR_IN_PROGRESS || err == NSAPI_ERROR_OK) {
        if (_is_blocking) {
            int sema_err = _semaphore.wait(get_timeout_for_operation(op)); // cellular network searching may take several minutes
            if (sema_err != 1) {
                tr_warning("No cellular connection");
                return NSAPI_ERROR_TIMEOUT;
            }
            return NSAPI_ERROR_OK;
        }
    }

    return err;
}

uint32_t AT_CellularContext::get_timeout_for_operation(ContextOperation op) const
{
    uint32_t timeout = NETWORK_TIMEOUT; // default timeout is 30 minutes as registration and attach may take time
    if (op == OP_SIM_READY || op == OP_DEVICE_READY) {
        timeout = DEVICE_TIMEOUT; // use 5 minutes for device ready and sim
    }
    return timeout;
}

bool AT_CellularContext::is_connected()
{
    return _is_connected;
}

NetworkStack *AT_CellularContext::get_stack()
{
#if NSAPI_PPP_AVAILABLE
    // use lwIP/PPP if modem does not have IP stack
    if (!_stack) {
        _stack = nsapi_ppp_get_stack();
    }
#endif
    return _stack;
}

const char *AT_CellularContext::get_netmask()
{
    return NULL;
}

const char *AT_CellularContext::get_gateway()
{
    return NULL;
}

const char *AT_CellularContext::get_ip_address()
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

void AT_CellularContext::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _status_cb = status_cb;
}

nsapi_error_t AT_CellularContext::set_blocking(bool blocking)
{
    nsapi_error_t err = NSAPI_ERROR_OK;
    tr_info("CellularContext set blocking %d", blocking);
#if NSAPI_PPP_AVAILABLE
    err = nsapi_ppp_set_blocking(blocking);
#endif
    _is_blocking = blocking;
    return err;
}

void AT_CellularContext::set_plmn(const char *plmn)
{
    tr_info("CellularContext plmn %s", plmn);
    _device->set_plmn(plmn);
}

void AT_CellularContext::set_sim_pin(const char *sim_pin)
{
    _device->set_sim_pin(sim_pin);
}

nsapi_error_t AT_CellularContext::connect(const char *sim_pin, const char *apn, const char *uname,
                                          const char *pwd)
{
    set_sim_pin(sim_pin);
    set_credentials(apn, uname, pwd);
    return connect();
}

void AT_CellularContext::set_credentials(const char *apn, const char *uname, const char *pwd)
{
    _apn = apn;
    _uname = uname;
    _pwd = pwd;
}

bool AT_CellularContext::stack_type_supported(nsapi_ip_stack_t stack_type)
{
    if (stack_type == _ip_stack_type) {
        return true;
    } else {
        return false;
    }
}

nsapi_ip_stack_t AT_CellularContext::get_stack_type()
{
    return _ip_stack_type;
}

nsapi_ip_stack_t AT_CellularContext::string_to_stack_type(const char *pdp_type)
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

// PDP Context handling
nsapi_error_t AT_CellularContext::delete_current_context()
{
    tr_info("Delete context %d", _cid);
    _at.clear_error();
    _at.cmd_start("AT+CGDCONT=");
    _at.write_int(_cid);
    _at.cmd_stop_read_resp();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        _cid = -1;
        _new_context_set = false;
    }

    return _at.get_last_error();
}

nsapi_error_t AT_CellularContext::do_user_authentication()
{
    // if user has defined user name and password we need to call CGAUTH before activating or modifying context
    if (_pwd && _uname) {
        if (!is_supported(AT_CGAUTH)) {
            return NSAPI_ERROR_UNSUPPORTED;
        }
        _at.cmd_start("AT+CGAUTH=");
        _at.write_int(_cid);
        _at.write_int(_authentication_type);
        _at.write_string(_uname);
        _at.write_string(_pwd);
        _at.cmd_stop_read_resp();
        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_AUTH_FAILURE;
        }
    }

    return NSAPI_ERROR_OK;
}

bool AT_CellularContext::get_context()
{
    _at.cmd_start("AT+CGDCONT?");
    _at.cmd_stop();
    _at.resp_start("+CGDCONT:");
    _cid = -1;
    int cid_max = 0; // needed when creating new context
    char apn[MAX_ACCESSPOINT_NAME_LENGTH];
    int apn_len = 0;

    bool modem_supports_ipv6 = stack_type_supported(IPV6_STACK);
    bool modem_supports_ipv4 = stack_type_supported(IPV4_STACK);

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
                if (pdp_stack != DEFAULT_STACK && (stack_type_supported(pdp_stack) || pdp_stack == IPV4V6_STACK)) {
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
        memcpy(_found_apn, apn, apn_len + 1);
    }

    tr_info("Found PDP context %d", _cid);

    return true;
}

bool AT_CellularContext::set_new_context(int cid)
{
    nsapi_ip_stack_t tmp_stack = _ip_stack_type_requested;

    if (tmp_stack == DEFAULT_STACK) {
        bool modem_supports_ipv6 = stack_type_supported(IPV6_STACK);
        bool modem_supports_ipv4 = stack_type_supported(IPV4_STACK);

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
    _at.cmd_stop_read_resp();
    success = (_at.get_last_error() == NSAPI_ERROR_OK);

    // Fall back to ipv4
    if (!success && tmp_stack == IPV4V6_STACK) {
        _at.clear_error();
        tmp_stack = IPV4_STACK;
        _at.cmd_start("AT+FCLASS=0;+CGDCONT=");
        _at.write_int(cid);
        _at.write_string("IP");
        _at.write_string(_apn);
        _at.cmd_stop_read_resp();
        success = (_at.get_last_error() == NSAPI_ERROR_OK);
    }

    if (success) {
        _ip_stack_type = tmp_stack;
        _cid = cid;
        _new_context_set = true;
        tr_info("New PDP context %d, stack %s", _cid, pdp_type);
    }

    return success;
}

nsapi_error_t AT_CellularContext::do_activate_context()
{
    _at.lock();

    nsapi_error_t err = NSAPI_ERROR_OK;

    // try to find or create context with suitable stack
    if (get_context()) {
#if NSAPI_PPP_AVAILABLE
        _at.unlock();
        // in PPP we don't activate any context but leave it to PPP stack
        return err;
#else
        // try to authenticate user before activating or modifying context
        err = do_user_authentication();
#endif // NSAPI_PPP_AVAILABLE
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
    if (!get_stack()) {
        _at.unlock();
        tr_error("No cellular stack!");
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _is_context_active = false;
    _is_context_activated = false;
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
        _at.cmd_stop_read_resp();
        if (_at.get_last_error() == NSAPI_ERROR_OK) {
            _is_context_activated = true;
        }
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

void AT_CellularContext::do_connect()
{
    call_network_cb(NSAPI_STATUS_CONNECTING);

    if (!_is_context_active) {
        _cb_data.error = do_activate_context();
#if !NSAPI_PPP_AVAILABLE
        // in PPP mode we did not activate any context, just searched the correct _cid
        if (_status_cb) {
            _status_cb((nsapi_event_t)CellularActivatePDPContext, (intptr_t)&_cb_data);
        }
#endif // !NSAPI_PPP_AVAILABLE
    }

    if (_cb_data.error != NSAPI_ERROR_OK) {
        call_network_cb(NSAPI_STATUS_DISCONNECTED);
        _is_connected = false;
        return;
    }
#if NSAPI_PPP_AVAILABLE
    if (_cb_data.error == NSAPI_ERROR_OK) {
        _at.lock();
        _cb_data.error = open_data_channel();
        _at.unlock();
        if (_cb_data.error != NSAPI_ERROR_OK) {
            tr_error("Failed to open data channel!");
            call_network_cb(NSAPI_STATUS_DISCONNECTED);
            _is_connected = false;
        } else {
            _is_context_activated = true;
        }
    }
#else
    _is_connected = true;
    call_network_cb(NSAPI_STATUS_GLOBAL_UP);
#endif
}

#if NSAPI_PPP_AVAILABLE
nsapi_error_t AT_CellularContext::open_data_channel()
{
    tr_info("CellularContext PPP connect");
    if (is_supported(AT_CGDATA)) {
        _at.cmd_start("AT+CGDATA=\"PPP\",");
        _at.write_int(_cid);
    } else {
        MBED_ASSERT(_cid >= 0 && _cid <= 99);
        _at.cmd_start("ATD*99***");
        _at.use_delimiter(false);
        _at.write_int(_cid);
        _at.write_string("#", false);
        _at.use_delimiter(true);
    }
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
    return nsapi_ppp_connect(_at.get_file_handle(), callback(this, &AT_CellularContext::ppp_status_cb), _uname, _pwd, _ip_stack_type);
}

void AT_CellularContext::ppp_status_cb(nsapi_event_t ev, intptr_t ptr)
{
    tr_debug("ppp_status_cb: event %d, ptr %d", ev, ptr);
    if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE && ptr == NSAPI_STATUS_GLOBAL_UP) {
        _is_connected = true;
    } else {
        _is_connected = false;
    }

    _connect_status = (nsapi_connection_status_t)ptr;

    // call device's callback, it will broadcast this to here (cellular_callback)
    _device->cellular_callback(ev, ptr);
}

#endif //#if NSAPI_PPP_AVAILABLE

nsapi_error_t AT_CellularContext::disconnect()
{
    tr_info("CellularContext disconnect");
    if (!_nw || !_is_connected) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
#if NSAPI_PPP_AVAILABLE
    nsapi_error_t err = nsapi_ppp_disconnect(_at.get_file_handle());
    if (err != NSAPI_ERROR_OK) {
        tr_error("CellularContext disconnect failed!");
        // continue even in failure due to ppp disconnect in any case releases filehandle
    }
    // after ppp disconnect if we wan't to use same at handler we need to set filehandle again to athandler so it
    // will set the correct sigio and nonblocking
    _at.lock();
    _at.set_file_handle(_at.get_file_handle());
    _at.set_is_filehandle_usable(true);
    if (!_at.sync(AT_SYNC_TIMEOUT)) { // consume extra characters after ppp disconnect, also it may take a while until modem listens AT commands
        tr_error("AT sync failed after PPP Disconnect");
    }
    _at.unlock();
#endif // NSAPI_PPP_AVAILABLE
    _at.lock();

    // deactivate a context only if we have activated
    if (_is_context_activated) {
        // CGACT and CGATT commands might take up to 3 minutes to respond.
        _at.set_at_timeout(180 * 1000);
        _is_context_active = false;
        size_t active_contexts_count = 0;
        _at.cmd_start("AT+CGACT?");
        _at.cmd_stop();
        _at.resp_start("+CGACT:");
        while (_at.info_resp()) {
            int context_id = _at.read_int();
            int context_activation_state = _at.read_int();
            if (context_activation_state == 1) {
                active_contexts_count++;
                if (context_id == _cid) {
                    _is_context_active = true;
                }
            }
        }
        _at.resp_stop();

        CellularNetwork::RadioAccessTechnology rat = CellularNetwork::RAT_GSM;
        // always return NSAPI_ERROR_OK
        CellularNetwork::registration_params_t reg_params;
        _nw->get_registration_params(reg_params);
        rat = reg_params._act;
        // 3GPP TS 27.007:
        // For EPS, if an attempt is made to disconnect the last PDN connection, then the MT responds with ERROR
        if (_is_context_active && (rat < CellularNetwork::RAT_E_UTRAN || active_contexts_count > 1)) {
            _at.clear_error();
            _at.cmd_start("AT+CGACT=0,");
            _at.write_int(_cid);
            _at.cmd_stop_read_resp();
        }

        if (_new_context_set) {
            _at.clear_error();
            _at.cmd_start("AT+CGDCONT=");
            _at.write_int(_cid);
            _at.cmd_stop_read_resp();
        }
        _at.clear_error();
        _at.cmd_start("AT+CGATT=0");
        _at.cmd_stop_read_resp();
        _at.restore_at_timeout();
    }

    _is_connected = false;
    call_network_cb(NSAPI_STATUS_DISCONNECTED);

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularContext::get_apn_backoff_timer(int &backoff_timer)
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

nsapi_error_t AT_CellularContext::get_rate_control(
    CellularContext::RateControlExceptionReports &reports,
    CellularContext::RateControlUplinkTimeUnit &timeUnit, int &uplinkRate)
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
            next_element = _at.read_int();
        } else {
            comma_found = false;
        }

        if (comma_found && next_element >= 0) {
            timeUnit = (RateControlUplinkTimeUnit)next_element;
            next_element = _at.read_int();
        } else {
            comma_found = false;
        }

        if (comma_found && next_element >= 0) {
            uplinkRate = next_element;
        }
        if (_at.get_last_error() == NSAPI_ERROR_OK) {
            tr_debug("CGAPNRC: reports %d, time %d, rate %d", reports, timeUnit, uplinkRate);
        }
    }
    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularContext::get_pdpcontext_params(pdpContextList_t &params_list)
{
    const int ipv6_subnet_size = 128;
    const int max_ipv6_size = 64;
    char *ipv6_and_subnetmask = new char[ipv6_subnet_size];
    char *temp = new char[max_ipv6_size];

    _at.lock();

    _at.cmd_start("AT+CGCONTRDP=");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("+CGCONTRDP:");
    pdpcontext_params_t *params = NULL;
    while (_at.info_resp()) { // response can be zero or many +CGDCONT lines
        params = params_list.add_new();
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

    delete [] temp;
    delete [] ipv6_and_subnetmask;

    return _at.unlock_return_error();
}

// Called by CellularDevice for network and cellular device changes
void AT_CellularContext::cellular_callback(nsapi_event_t ev, intptr_t ptr)
{
    if (ev >= NSAPI_EVENT_CELLULAR_STATUS_BASE && ev <= NSAPI_EVENT_CELLULAR_STATUS_END) {
        cell_callback_data_t *data = (cell_callback_data_t *)ptr;
        cellular_connection_status_t st = (cellular_connection_status_t)ev;
        _cb_data.error = data->error;
        tr_debug("CellularContext: event %d, err %d, data %d", ev, data->error, data->status_data);
#if USE_APN_LOOKUP
        if (st == CellularSIMStatusChanged && data->status_data == CellularSIM::SimStateReady &&
                _cb_data.error == NSAPI_ERROR_OK) {
            if (!_apn) {
                char imsi[MAX_IMSI_LENGTH + 1];
                wait(1); // need to wait to access SIM in some modems
                _cb_data.error = _device->open_sim()->get_imsi(imsi);
                if (_cb_data.error == NSAPI_ERROR_OK) {
                    const char *apn_config = apnconfig(imsi);
                    if (apn_config) {
                        const char *apn = _APN_GET(apn_config);
                        const char *uname = _APN_GET(apn_config);
                        const char *pwd = _APN_GET(apn_config);
                        tr_info("Looked up APN %s", apn);
                        set_credentials(apn, uname, pwd);
                    }
                } else {
                    tr_error("APN lookup failed");
                    _device->stop();
                    if (_is_blocking) {
                        // operation failed, release semaphore
                        _semaphore.release();
                    }
                }
                _device->close_sim();
            }
        }
#endif // USE_APN_LOOKUP

        if (!_nw && st == CellularDeviceReady && data->error == NSAPI_ERROR_OK) {
            _nw = _device->open_network(_fh);
        }

        if (_is_blocking) {
            if (data->error != NSAPI_ERROR_OK) {
                // operation failed, release semaphore
                _semaphore.release();
            } else {
                if ((st == CellularDeviceReady && _current_op == OP_DEVICE_READY) ||
                        (st == CellularSIMStatusChanged && _current_op == OP_SIM_READY &&
                         data->status_data == CellularSIM::SimStateReady)) {
                    // target reached, release semaphore
                    _semaphore.release();
                } else if (st == CellularRegistrationStatusChanged && (data->status_data == CellularNetwork::RegisteredHomeNetwork ||
                                                                       data->status_data == CellularNetwork::RegisteredRoaming || data->status_data == CellularNetwork::AlreadyRegistered) && _current_op == OP_REGISTER) {
                    // target reached, release semaphore
                    _semaphore.release();
                } else if (st == CellularAttachNetwork && (_current_op == OP_ATTACH || _current_op == OP_CONNECT) &&
                           data->status_data == CellularNetwork::Attached) {
                    // target reached, release semaphore
                    _semaphore.release();
                }
            }
        } else {
            // non blocking
            if (st == CellularAttachNetwork && _current_op == OP_CONNECT && data->error == NSAPI_ERROR_OK &&
                    data->status_data == CellularNetwork::Attached) {
                // forward to application
                if (_status_cb) {
                    _status_cb(ev, ptr);
                }
                do_connect();
                return;
            }
        }
    } else {
        tr_debug("CellularContext: event %d, ptr %d", ev, ptr);
#if NSAPI_PPP_AVAILABLE
        if (_is_blocking) {
            if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE && ptr == NSAPI_STATUS_GLOBAL_UP) {
                tr_info("CellularContext IP %s", get_ip_address());
                _cb_data.error = NSAPI_ERROR_OK;
                _semaphore.release();
            } else if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE && ptr == NSAPI_STATUS_DISCONNECTED) {
                tr_info("PPP disconnected");
                _cb_data.error = NSAPI_ERROR_NO_CONNECTION;
                _semaphore.release();
            }
        }
#endif
    }

    // forward to application
    if (_status_cb) {
        _status_cb(ev, ptr);
    }
}

void AT_CellularContext::call_network_cb(nsapi_connection_status_t status)
{
    if (_connect_status != status) {
        _connect_status = status;
        if (_status_cb) {
            _status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connect_status);
        }
        if (_nw && _connect_status == NSAPI_STATUS_DISCONNECTED) {
            tr_info("CellularContext disconnected");
        }
    }
}
