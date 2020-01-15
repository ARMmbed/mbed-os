/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "AT_CellularNetwork_stub.h"
#include "CellularNetwork.h"
#include "CellularUtil.h"
#include "CellularLog.h"
#include "FileHandle.h"
#include "nsapi_types.h"

using namespace mbed;
using namespace mbed_cellular_util;



nsapi_error_t AT_CellularNetwork_stub::nsapi_error_value = 0;
int AT_CellularNetwork_stub::fail_counter = 0;
int AT_CellularNetwork_stub::set_registration_urc_fail_counter = 0;
int AT_CellularNetwork_stub::get_registration_params_fail_counter = 0;

AT_CellularNetwork::AT_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device) : _at(atHandler), _device(device)
{
}

AT_CellularNetwork::~AT_CellularNetwork()
{
}

void AT_CellularNetwork::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
}

nsapi_error_t AT_CellularNetwork::set_registration_urc(RegistrationType type, bool urc_on)
{
    if (AT_CellularNetwork_stub::set_registration_urc_fail_counter) {
        AT_CellularNetwork_stub::set_registration_urc_fail_counter--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_network_registering_mode(NWRegisteringMode &mode)
{
    mode = NWModeAutomatic;
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_registration(const char *plmn)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_registration_params(RegistrationType type, registration_params_t &reg_params)
{
    if (AT_CellularNetwork_stub::get_registration_params_fail_counter) {
        AT_CellularNetwork_stub::get_registration_params_fail_counter--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    reg_params._status = CellularNetwork::RegisteredHomeNetwork;
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_registration_params(registration_params_t &reg_params)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_attach()
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

nsapi_error_t AT_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_access_technology(RadioAccessTechnology opAct)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::scan_plmn(operList_t &operators, int &opsCount)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::set_ciot_optimization_config(CIoT_Supported_Opt supported_opt,
                                                               CIoT_Preferred_UE_Opt preferred_opt,
                                                               Callback<void(CIoT_Supported_Opt)> network_support_cb)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_ciot_ue_optimization_config(CIoT_Supported_Opt &supported_opt,
                                                                  CIoT_Preferred_UE_Opt &preferred_opt)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_ciot_network_optimization_config(CIoT_Supported_Opt &supported_opt)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::get_signal_quality(int &rssi, int *ber)
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

bool AT_CellularNetwork::is_active_context(int *number_of_active_contexts, int cid)
{
    return false;
}

nsapi_error_t AT_CellularNetwork::set_receive_period(int mode, EDRXAccessTechnology act_type, uint8_t edrx_value)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularNetwork::get_context_state_command()
{
}

nsapi_error_t AT_CellularNetwork::set_packet_domain_event_reporting(bool on)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularNetwork::clear()
{
    return NSAPI_ERROR_OK;
}
