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

#include "AT_CellularNetwork.h"
#include "CellularNetwork.h"
#include "CellularUtil.h"
#include "CellularLog.h"
#include "FileHandle.h"
#include "nsapi_types.h"

using namespace mbed;
using namespace mbed_cellular_util;

AT_CellularNetwork::AT_CellularNetwork(ATHandler &atHandler) : AT_CellularBase(atHandler)
{
}

AT_CellularNetwork::~AT_CellularNetwork()
{
}

nsapi_error_t AT_CellularNetwork::init()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_credentials(const char *apn,
        const char *username, const char *password)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_credentials(const char *apn,
     AuthenticationType type, const char *username, const char *password)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::connect(const char *apn,
        const char *username, const char *password)
{
    return connect();
}

nsapi_error_t AT_CellularNetwork::connect()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::activate_context()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::open_data_channel()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::disconnect()
{
    return NSAPI_ERROR_OK;
}

void AT_CellularNetwork::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
}

nsapi_connection_status_t AT_CellularNetwork::get_connection_status() const
{
    return NSAPI_STATUS_LOCAL_UP;
}

nsapi_error_t AT_CellularNetwork::set_blocking(bool blocking)
{
    return NSAPI_ERROR_OK;;
}

nsapi_ip_stack_t AT_CellularNetwork::string_to_stack_type(const char* pdp_type)
{
    return IPV4_STACK;
}

nsapi_error_t AT_CellularNetwork::set_registration_urc(RegistrationType type, bool urc_on)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_network_registering_mode(NWRegisteringMode& mode)
{
    mode = NWModeAutomatic;
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_registration(const char *plmn)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_registration_status(RegistrationType type, RegistrationStatus &status)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_cell_id(int &cell_id)
{
    return NSAPI_ERROR_OK;
}

bool AT_CellularNetwork::has_registration(RegistrationType reg_type)
{
    return false;
}

nsapi_error_t AT_CellularNetwork::set_attach(int timeout)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_attach(AttachStatus &status)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::detach()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_apn_backoff_timer(int &backoffTime)
{
    return NSAPI_ERROR_OK;
}

NetworkStack *AT_CellularNetwork::get_stack()
{
    return NULL;
}

const char *AT_CellularNetwork::get_ip_address()
{
    return NULL;
}

nsapi_error_t AT_CellularNetwork::set_stack_type(nsapi_ip_stack_t stack_type)
{
    return NSAPI_ERROR_OK;
}

nsapi_ip_stack_t AT_CellularNetwork::get_stack_type()
{
    return IPV4_STACK;
}

bool AT_CellularNetwork::get_modem_stack_type(nsapi_ip_stack_t requested_stack)
{
    return false;
}

void AT_CellularNetwork::urc_no_carrier()
{

}

nsapi_error_t AT_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_access_technology(RadioAccessTechnology opAct)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_access_technology(RadioAccessTechnology& rat)
{
    rat = RAT_CATM1;
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::scan_plmn(operList_t &operators, int &opsCount)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_ciot_optimization_config(Supported_UE_Opt supported_opt,
                                                               Preferred_UE_Opt preferred_opt)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_ciot_optimization_config(Supported_UE_Opt& supported_opt,
                                                               Preferred_UE_Opt& preferred_opt)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_rate_control(
        CellularNetwork::RateControlExceptionReports &reports,
        CellularNetwork::RateControlUplinkTimeUnit &timeUnit, int &uplinkRate)
{
    return NSAPI_ERROR_OK;
}


nsapi_error_t AT_CellularNetwork::get_pdpcontext_params(pdpContextList_t& params_list)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_signal_quality(int &rssi, int &ber)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_operator_params(int &format, operator_t &operator_params)
{
    return NSAPI_ERROR_OK;
}

int AT_CellularNetwork::get_3gpp_error()
{
    return 0;
}

nsapi_error_t AT_CellularNetwork::get_operator_names(operator_names_list &op_names)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::do_user_authentication()
{
    return NSAPI_ERROR_OK;
}



