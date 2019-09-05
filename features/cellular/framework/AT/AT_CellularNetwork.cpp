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
    { CellularNetwork::C_EREG, "+CEREG", "+CEREG:"},
    { CellularNetwork::C_GREG, "+CGREG", "+CGREG:"},
    { CellularNetwork::C_REG,  "+CREG", "+CREG:"}
};

#if MBED_CONF_MBED_TRACE_ENABLE
static const char *const reg_type_str[(int)AT_CellularNetwork::RegistrationStatusMax] = {
    "NotRegistered",
    "RegisteredHomeNetwork",
    "SearchingNetwork",
    "RegistrationDenied",
    "RegistrationUnknown",
    "RegisteredRoaming",
    "RegisteredSMSOnlyHome",
    "RegisteredSMSOnlyRoaming",
    "AttachedEmergencyOnly",
    "RegisteredCSFBNotPreferredHome",
    "RegisteredCSFBNotPreferredRoaming",
    "AlreadyRegistered"
};

static const char *const rat_str[AT_CellularNetwork::RAT_MAX] = {
    "GSM",
    "GSM_COMPACT",
    "UTRAN",
    "EGPRS",
    "HSDPA",
    "HSUPA",
    "HSDPA_HSUPA",
    "E_UTRAN",
    "CATM1",
    "NB1",
    "RAT unknown",
};

#endif


AT_CellularNetwork::AT_CellularNetwork(ATHandler &atHandler) : AT_CellularBase(atHandler),
    _connection_status_cb(NULL), _ciotopt_network_support_cb(NULL), _op_act(RAT_UNKNOWN),
    _connect_status(NSAPI_STATUS_DISCONNECTED), _supported_network_opt(CIOT_OPT_MAX)
{

    _urc_funcs[C_EREG] = callback(this, &AT_CellularNetwork::urc_cereg);
    _urc_funcs[C_GREG] = callback(this, &AT_CellularNetwork::urc_cgreg);
    _urc_funcs[C_REG] = callback(this, &AT_CellularNetwork::urc_creg);

    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (get_property((AT_CellularBase::CellularProperty)type) != RegistrationModeDisable) {
            _at.set_urc_handler(at_reg[type].urc_prefix, _urc_funcs[type]);
        }
    }

    if (get_property(AT_CellularBase::PROPERTY_AT_CGEREP)) {
        // additional urc to get better disconnect info for application. Not critical.
        _at.set_urc_handler("+CGEV: NW DET", callback(this, &AT_CellularNetwork::urc_cgev));
        _at.set_urc_handler("+CGEV: ME DET", callback(this, &AT_CellularNetwork::urc_cgev));
    }


    _at.set_urc_handler("+CCIOTOPTI:", callback(this, &AT_CellularNetwork::urc_cciotopti));
}

AT_CellularNetwork::~AT_CellularNetwork()
{
    (void)set_packet_domain_event_reporting(false);
    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (get_property((AT_CellularBase::CellularProperty)type) != RegistrationModeDisable) {
            _at.set_urc_handler(at_reg[type].urc_prefix, 0);
        }
    }

    if (get_property(AT_CellularBase::PROPERTY_AT_CGEREP)) {
        _at.set_urc_handler("+CGEV: ME DET", 0);
        _at.set_urc_handler("+CGEV: NW DET", 0);
    }
    _at.set_urc_handler("+CCIOTOPTI:", 0);
}

void AT_CellularNetwork::urc_cgev()
{
    call_network_cb(NSAPI_STATUS_DISCONNECTED);
}

void AT_CellularNetwork::read_reg_params_and_compare(RegistrationType type)
{
    registration_params_t reg_params;
    read_reg_params(type, reg_params);

    if (_at.get_last_error() == NSAPI_ERROR_OK && _connection_status_cb) {
        _reg_params._type = type;
        cell_callback_data_t data;
        data.error = NSAPI_ERROR_OK;
        if (reg_params._act != _reg_params._act) {
            _reg_params._act = reg_params._act;
            data.status_data = reg_params._act;
            _connection_status_cb((nsapi_event_t)CellularRadioAccessTechnologyChanged, (intptr_t)&data);
        }
        if (reg_params._status != _reg_params._status) {
            RegistrationStatus previous_registration_status = _reg_params._status;
            _reg_params._status = reg_params._status;
            data.status_data = reg_params._status;
            _connection_status_cb((nsapi_event_t)CellularRegistrationStatusChanged, (intptr_t)&data);
            if (reg_params._status == NotRegistered) { // Other states means that we are trying to connect or connected
                if (previous_registration_status == RegisteredHomeNetwork ||
                        previous_registration_status == RegisteredRoaming) {
                    if (type != C_REG) {// we are interested only if we drop from packet network
                        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
                    }
                }
            }
        }
        if (reg_params._cell_id != -1 && reg_params._cell_id != _reg_params._cell_id) {
            _reg_params._cell_id = reg_params._cell_id;
            _reg_params._lac = reg_params._lac;
            data.status_data = reg_params._cell_id;
            _connection_status_cb((nsapi_event_t)CellularCellIDChanged, (intptr_t)&data);
        }
    }
}

void AT_CellularNetwork::urc_creg()
{
    read_reg_params_and_compare(C_REG);
}

void AT_CellularNetwork::urc_cereg()
{
    read_reg_params_and_compare(C_EREG);
}

void AT_CellularNetwork::urc_cgreg()
{
    read_reg_params_and_compare(C_GREG);
}

void AT_CellularNetwork::call_network_cb(nsapi_connection_status_t status)
{
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connect_status);
    }
}

void AT_CellularNetwork::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
}

nsapi_error_t AT_CellularNetwork::set_registration_urc(RegistrationType type, bool urc_on)
{
    int index = (int)type;
    MBED_ASSERT(index >= 0 && index < C_MAX);

    RegistrationMode mode = (RegistrationMode)get_property((AT_CellularBase::CellularProperty)type);
    if (mode == RegistrationModeDisable) {
        return NSAPI_ERROR_UNSUPPORTED;
    } else {
        if (urc_on) {
            return _at.at_cmd_discard(at_reg[index].cmd, "=", "%d", mode);
        } else {
            return _at.at_cmd_discard(at_reg[index].cmd, "=0");
        }
    }
}

nsapi_error_t AT_CellularNetwork::get_network_registering_mode(NWRegisteringMode &mode)
{
    int ret;
    nsapi_error_t error = _at.at_cmd_int("+COPS", "?", ret);
    mode = (NWRegisteringMode)ret;
    return error;
}

nsapi_error_t AT_CellularNetwork::set_registration(const char *plmn)
{

    if (!plmn) {
        tr_debug("Automatic network registration");
        NWRegisteringMode mode;
        if (get_network_registering_mode(mode) != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (mode != NWModeAutomatic) {
            return _at.at_cmd_discard("+COPS", "=0");
        }
        return NSAPI_ERROR_OK;
    } else {
        tr_debug("Manual network registration to %s", plmn);
        if (_op_act != RAT_UNKNOWN) {
            return _at.at_cmd_discard("+COPS", "=1,2,", "%s%d", plmn, _op_act);
        } else {
            return _at.at_cmd_discard("+COPS", "=1,2", "%s", plmn);
        }
    }
}

void AT_CellularNetwork::read_reg_params(RegistrationType type, registration_params_t &reg_params)
{
    const int MAX_STRING_LENGTH = 9;
    char string_param[MAX_STRING_LENGTH] = {0};

    reg_params._type = type;

    int int_param = _at.read_int();
    reg_params._status = (int_param >= 0 && int_param < RegistrationStatusMax) ? (RegistrationStatus)int_param : NotRegistered;

    int len = _at.read_string(string_param, TWO_BYTES_HEX + 1);
    if (len > 0) {
        reg_params._lac = hex_str_to_int(string_param, TWO_BYTES_HEX);
    } else {
        reg_params._lac = -1;
    }

    len = _at.read_string(string_param, FOUR_BYTES_HEX + 1);
    if (len > 0) {
        reg_params._cell_id = hex_str_to_int(string_param, FOUR_BYTES_HEX);
    } else {
        reg_params._cell_id = -1;
    }

    int_param = _at.read_int();
    reg_params._act = (int_param >= 0 && int_param < RAT_MAX) ? (RadioAccessTechnology)int_param : RAT_UNKNOWN ;

    // Skip [<cause_type>],[<reject_cause>]
    _at.skip_param(2);

    len = _at.read_string(string_param, ONE_BYTE_BINARY + 1);
    reg_params._active_time = calculate_active_time(string_param, len);

    len = _at.read_string(string_param, ONE_BYTE_BINARY + 1);
    reg_params._periodic_tau = calculate_periodic_tau(string_param, len);

#if MBED_CONF_MBED_TRACE_ENABLE
    tr_debug("%s %s, LAC %d, cell %d, %s", at_reg[(int)type].urc_prefix, reg_type_str[reg_params._status], reg_params._lac, reg_params._cell_id, rat_str[reg_params._act]);
#endif
}

nsapi_error_t AT_CellularNetwork::set_attach()
{
    _at.lock();
    AttachStatus status;
    get_attach(status);

    if (status == Detached) {
        tr_debug("Network attach");
        _at.at_cmd_discard("+CGATT", "=1");
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_attach(AttachStatus &status)
{
    int attach_status;
    nsapi_error_t err = _at.at_cmd_int("+CGATT", "?", attach_status);
    status = (attach_status == 1) ? Attached : Detached;

    return err;
}

nsapi_error_t AT_CellularNetwork::detach()
{
    _at.lock();
    tr_debug("Network detach");
    _at.at_cmd_discard("+CGATT", "=0");

    _at.at_cmd_discard("+COPS", "=2");

    call_network_cb(NSAPI_STATUS_DISCONNECTED);

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    _op_act = RAT_UNKNOWN;
    return NSAPI_ERROR_UNSUPPORTED;
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

    _at.cmd_start_stop("+COPS", "=?");

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

nsapi_error_t AT_CellularNetwork::set_ciot_optimization_config(CIoT_Supported_Opt supported_opt,
                                                               CIoT_Preferred_UE_Opt preferred_opt,
                                                               Callback<void(CIoT_Supported_Opt)> network_support_cb)
{
    _ciotopt_network_support_cb = network_support_cb;
    return _at.at_cmd_discard("+CCIOTOPT", "=1,", "%d%d", supported_opt, preferred_opt);
}

void AT_CellularNetwork::urc_cciotopti()
{
    _supported_network_opt = (CIoT_Supported_Opt)_at.read_int();

    if (_ciotopt_network_support_cb) {
        _ciotopt_network_support_cb(_supported_network_opt);
    }
}

nsapi_error_t AT_CellularNetwork::get_ciot_ue_optimization_config(CIoT_Supported_Opt &supported_opt,
                                                                  CIoT_Preferred_UE_Opt &preferred_opt)
{
    _at.lock();

    _at.cmd_start_stop("+CCIOTOPT", "?");

    _at.resp_start("+CCIOTOPT:");
    _at.read_int();
    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        supported_opt = (CIoT_Supported_Opt)_at.read_int();
        preferred_opt = (CIoT_Preferred_UE_Opt)_at.read_int();
    }

    _at.resp_stop();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularNetwork::get_ciot_network_optimization_config(CIoT_Supported_Opt &supported_network_opt)
{
    supported_network_opt = _supported_network_opt;
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_signal_quality(int &rssi, int *ber)
{
    _at.lock();

    _at.cmd_start_stop("+CSQ", "");

    _at.resp_start("+CSQ:");
    int t_rssi = _at.read_int();
    int t_ber = _at.read_int();
    _at.resp_stop();
    if (t_rssi < 0 || t_ber < 0) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    // RSSI value is returned in dBm with range from -51 to -113 dBm, see 3GPP TS 27.007
    if (t_rssi == 99) {
        rssi = SignalQualityUnknown;
    } else {
        rssi = -113 + 2 * t_rssi;
    }

    if (ber) {
        if (t_ber == 99) {
            *ber = SignalQualityUnknown;
        } else {
            *ber = t_ber;
        }
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

    _at.cmd_start_stop("+COPS", "?");

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

    _at.cmd_start_stop("+COPN", "");

    _at.resp_start("+COPN:");
    operator_names_t *names = NULL;
    while (_at.info_resp()) {
        names = op_names.add_new();
        _at.read_string(names->numeric, sizeof(names->numeric));
        _at.read_string(names->alpha, sizeof(names->alpha));
    }

    _at.resp_stop();
    return _at.unlock_return_error();
}

void AT_CellularNetwork::get_context_state_command()
{
    _at.cmd_start_stop("+CGACT", "?");
    _at.resp_start("+CGACT:");
}

bool AT_CellularNetwork::is_active_context(int *number_of_active_contexts, int cid)
{
    _at.lock();

    if (number_of_active_contexts) {
        *number_of_active_contexts = 0;
    }
    bool active_found = false;
    int context_id;
    // read active contexts
    get_context_state_command();

    while (_at.info_resp()) {
        context_id = _at.read_int(); // discard context id
        if (_at.read_int() == 1) { // check state
            tr_debug("Found active context");
            if (number_of_active_contexts) {
                (*number_of_active_contexts)++;
            }
            if (cid == -1) {
                active_found = true;
            } else if (context_id == cid) {
                active_found = true;
            }
            if (!number_of_active_contexts && active_found) {
                break;
            }
        }
    }
    _at.resp_stop();
    _at.unlock();

    return active_found;
}

nsapi_error_t AT_CellularNetwork::get_registration_params(registration_params_t &reg_params)
{
    reg_params = _reg_params;
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_registration_params(RegistrationType type, registration_params_t &reg_params)
{
    int i = (int)type;
    MBED_ASSERT(i >= 0 && i < C_MAX);

    if (!get_property((AT_CellularBase::CellularProperty)at_reg[i].type)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _at.lock();

    _at.cmd_start_stop(at_reg[i].cmd, "?");

    _at.resp_start(at_reg[i].urc_prefix);
    (void)_at.read_int(); // ignore urc mode subparam
    read_reg_params(type, reg_params);
    _at.resp_stop();

    _reg_params = reg_params;

    return _at.unlock_return_error();
}

int AT_CellularNetwork::calculate_active_time(const char *active_time_string, int active_time_length)
{
    if (active_time_length != ONE_BYTE_BINARY) {
        return -1;
    }

    uint32_t ie_unit = binary_str_to_uint(active_time_string, TIMER_UNIT_LENGTH);
    uint32_t ie_value = binary_str_to_uint(active_time_string + TIMER_UNIT_LENGTH, active_time_length - TIMER_UNIT_LENGTH);

    switch (ie_unit) {
        case 0: // multiples of 2 seconds
            return 2 * ie_value;
        case 1: // multiples of 1 minute
            return 60 * ie_value;
        case 2: // multiples of decihours
            return 6 * 60 * ie_value;
        case 7: // timer is deactivated
            return 0;
        default: // other values shall be interpreted as multiples of 1 minute
            return 60 * ie_value;
    }
}

int AT_CellularNetwork::calculate_periodic_tau(const char *periodic_tau_string, int periodic_tau_length)
{
    if (periodic_tau_length != ONE_BYTE_BINARY) {
        return -1;
    }

    uint32_t ie_unit = binary_str_to_uint(periodic_tau_string, TIMER_UNIT_LENGTH);
    uint32_t ie_value = binary_str_to_uint(periodic_tau_string + TIMER_UNIT_LENGTH, periodic_tau_length - TIMER_UNIT_LENGTH);

    switch (ie_unit) {
        case 0: // multiples of 10 minutes
            return 60 * 10 * ie_value;
        case 1: // multiples of 1 hour
            return 60 * 60 * ie_value;
        case 2: // multiples of 10 hours
            return 10 * 60 * 60 * ie_value;
        case 3: // multiples of 2 seconds
            return 2 * ie_value;
        case 4: // multiples of 30 seconds
            return 30 * ie_value;
        case 5: // multiples of 1 minute
            return 60 * ie_value;
        case 6: // multiples of 320 hours
            return 320 * 60 * 60 * ie_value;
        default: // timer is deactivated
            return 0;
    }
}

nsapi_error_t AT_CellularNetwork::set_receive_period(int mode, EDRXAccessTechnology act_type, uint8_t edrx_value)
{
    char edrx[5];
    uint_to_binary_str(edrx_value, edrx, 5, 4);
    edrx[4] = '\0';

    return _at.at_cmd_discard("+CEDRXS", "=", "%d%d%s", mode, act_type, edrx);
}

nsapi_error_t AT_CellularNetwork::set_packet_domain_event_reporting(bool on)
{
    if (!get_property(AT_CellularBase::PROPERTY_AT_CGEREP)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    return _at.at_cmd_discard("+CGEREP", "=", "%d", on ? 1 : 0);
}
