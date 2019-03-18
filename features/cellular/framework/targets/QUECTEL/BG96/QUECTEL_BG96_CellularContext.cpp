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
#include "QUECTEL_BG96_CellularContext.h"
#include "QUECTEL_BG96_CellularStack.h"
#include "QUECTEL_BG96_ControlPlane_netif.h"
#include "CellularLog.h"

#include "Semaphore.h"

namespace mbed {

// Non-IP context supported only for context id 1
#define NIDD_PDP_CONTEXT_ID 1
// Timeout to wait for URC indicating NIDD connection opening
#define NIDD_OPEN_URC_TIMEOUT 3000

QUECTEL_BG96_CellularContext::QUECTEL_BG96_CellularContext(ATHandler &at, CellularDevice *device, const char *apn, bool cp_req, bool nonip_req) :
    AT_CellularContext(at, device, apn, cp_req, nonip_req)
{
    if (_nonip_req) {
        _at.set_urc_handler("+QIND:", mbed::Callback<void()>(this, &QUECTEL_BG96_CellularContext::urc_nidd));
    }
}

QUECTEL_BG96_CellularContext::~QUECTEL_BG96_CellularContext()
{
    if (_nonip_req) {
        _at.set_urc_handler("+QIND:", 0);
    }
}

#if !NSAPI_PPP_AVAILABLE
NetworkStack *QUECTEL_BG96_CellularContext::get_stack()
{
    if (_pdp_type == NON_IP_PDP_TYPE || (_nonip_req && _pdp_type != DEFAULT_PDP_TYPE)) {
        tr_error("Requesting stack for NON-IP context! Should request control plane netif: get_cp_netif()");
        return NULL;
    }

    if (!_stack) {
        _stack = new QUECTEL_BG96_CellularStack(_at, _cid, (nsapi_ip_stack_t)_pdp_type);
    }

    return _stack;
}
#endif // #if !NSAPI_PPP_AVAILABLE

ControlPlane_netif *QUECTEL_BG96_CellularContext::get_cp_netif()
{
    if (!_cp_netif) {
        _cp_netif = new QUECTEL_BG96_ControlPlane_netif(_at, _cid);
    }
    return _cp_netif;
}

nsapi_error_t QUECTEL_BG96_CellularContext::do_user_authentication()
{
    if (_pwd && _uname) {
        _at.cmd_start("AT+QICSGP=");
        _at.write_int(_cid);
        _at.write_int(1); // IPv4
        _at.write_string(_apn);
        _at.write_string(_uname);
        _at.write_string(_pwd);
        _at.write_int(_authentication_type);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_AUTH_FAILURE;
        }
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_BG96_CellularContext::activate_non_ip_context()
{
    _at.lock();

    // Open the NIDD connection
    _at.cmd_start("AT+QCFGEXT=\"nipd\",1");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    nsapi_size_or_error_t ret = _at.get_last_error();

    _at.unlock();

    if (ret == NSAPI_ERROR_OK) {
        _semaphore.try_acquire_for(NIDD_OPEN_URC_TIMEOUT);
        if (_cid == -1) {
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    return (ret == NSAPI_ERROR_OK) ? NSAPI_ERROR_OK : NSAPI_ERROR_NO_CONNECTION;
}

void QUECTEL_BG96_CellularContext::activate_context()
{
    tr_info("Activate PDP context %d", _cid);
    _at.cmd_start("AT+QIACT=");
    _at.write_int(_cid);
    _at.cmd_stop_read_resp();
    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        _is_context_activated = true;
    }
}

void QUECTEL_BG96_CellularContext::deactivate_context()
{
    _at.cmd_start("AT+QIDEACT=");
    _at.write_int(_cid);
    _at.cmd_stop_read_resp();
}

void QUECTEL_BG96_CellularContext::deactivate_non_ip_context()
{
    // Close the NIDD connection
    _at.cmd_start("AT+QCFGEXT=\"nipd\",0");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
}

void QUECTEL_BG96_CellularContext::urc_nidd()
{
    char nipd_string[6];

    // skip "nipd"
    _at.skip_param();
    _at.read_string(nipd_string, sizeof(nipd_string));

    if (!strcmp(nipd_string, "recv")) {
        cp_data_received();
    } else if (!strcmp(nipd_string, "open")) {
        urc_nidd_open();
    } else if (!strcmp(nipd_string, "close")) {
        urc_nidd_close();
    }
}

void QUECTEL_BG96_CellularContext::urc_nidd_open()
{
    int err = _at.read_int();
    if (!err) {
        _is_context_active = true;
        _is_context_activated = true;
        _cid = NIDD_PDP_CONTEXT_ID;
    } else {
        tr_error("NIDD connection open failed with error: %d", err);
    }
    _semaphore.release();
}

void QUECTEL_BG96_CellularContext::urc_nidd_close()
{
    set_disconnect();
}

nsapi_error_t QUECTEL_BG96_CellularContext::setup_control_plane_opt()
{
    _at.lock();

    _at.cmd_start("AT+QCFGEXT=\"pdp_type\",");
    _at.write_int(NIDD_PDP_CONTEXT_ID);
    _at.write_string("Non-IP");
    _at.write_string(_apn);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    _at.cmd_start("AT+CFUN=0");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    _at.cmd_start("AT+CFUN=1");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    // Configure Non-IP outgoing data type - 0 for no exception data
    _at.cmd_start("AT+QCFGEXT=\"nipdcfg\",0,");
    _at.write_string(_apn);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        _cp_in_use = true;
        if (_nonip_req) {
            _pdp_type = NON_IP_PDP_TYPE;
        }
    }

    return _at.unlock_return_error();
}

} /* namespace mbed */
