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

using namespace mbed;

AT_CellularContext::AT_CellularContext(ATHandler &at, CellularDevice *device, const char *apn,  bool cp_req, bool nonip_req) :
    AT_CellularBase(at), _is_connected(false),
    _current_op(OP_INVALID), _fh(0), _cp_req(cp_req)
{
    _stack = NULL;
    _pdp_type = DEFAULT_PDP_TYPE;
    _authentication_type = CellularContext::CHAP;
    _connect_status = NSAPI_STATUS_DISCONNECTED;
    _is_context_active = false;
    _is_context_activated = false;
    _apn = apn;
    _uname = MBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME;
    _pwd = MBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD;
    _status_cb = NULL;
    _cid = -1;
    _new_context_set = false;
    _next = NULL;
    _cp_netif = NULL;
    memset(_retry_timeout_array, 0, CELLULAR_RETRY_ARRAY_SIZE);
    _retry_array_length = 0;
    _retry_count = 0;
    _is_blocking = true;
    _device = device;
    _nw = NULL;
    _nonip_req = nonip_req;
    _cp_in_use = false;
}

AT_CellularContext::~AT_CellularContext()
{
}

void AT_CellularContext::set_file_handle(UARTSerial *serial, PinName dcd_pin, bool active_high)
{
}

void AT_CellularContext::enable_hup(bool enable)
{
}

void AT_CellularContext::set_file_handle(FileHandle *fh)
{
}

nsapi_error_t AT_CellularContext::connect()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularContext::set_device_ready()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularContext::set_sim_ready()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularContext::register_to_network()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularContext::attach_to_network()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularContext::check_operation(nsapi_error_t err, ContextOperation op)
{
    return NSAPI_ERROR_OK;
}

uint32_t AT_CellularContext::get_timeout_for_operation(ContextOperation op) const
{
    uint32_t timeout = 10 * 60 * 1000; // default timeout is 10 minutes as registration and attach may take time
    return timeout;
}



bool AT_CellularContext::is_connected()
{
    return true;
}

NetworkStack *AT_CellularContext::get_stack()
{
    return NULL;
}

const char *AT_CellularContext::get_ip_address()
{
    return NULL;
}

void AT_CellularContext::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
}

nsapi_error_t AT_CellularContext::set_blocking(bool blocking)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularContext::set_plmn(const char *plmn)
{
}

void AT_CellularContext::set_sim_pin(const char *sim_pin)
{
}

nsapi_error_t AT_CellularContext::connect(const char *sim_pin, const char *apn, const char *uname,
                                          const char *pwd)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularContext::set_credentials(const char *apn, const char *uname, const char *pwd)
{

}

const char *AT_CellularContext::get_netmask()
{
    return NULL;
}

const char *AT_CellularContext::get_gateway()
{
    return NULL;
}

AT_CellularBase::CellularProperty AT_CellularContext::pdp_type_t_to_cellular_property(pdp_type_t pdp_type)
{
    AT_CellularBase::CellularProperty prop = PROPERTY_IPV4_PDP_TYPE;
    if (pdp_type == IPV6_PDP_TYPE) {
        prop = PROPERTY_IPV6_PDP_TYPE;
    } else if (pdp_type == IPV4V6_PDP_TYPE) {
        prop = PROPERTY_IPV4V6_PDP_TYPE;
    } else if (pdp_type == NON_IP_PDP_TYPE) {
        prop = PROPERTY_NON_IP_PDP_TYPE;
    }

    return prop;
}

// PDP Context handling
void AT_CellularContext::delete_current_context()
{
}

nsapi_error_t AT_CellularContext::do_user_authentication()
{
    return NSAPI_ERROR_OK;
}

bool AT_CellularContext::get_context()
{
    return true;
}

bool AT_CellularContext::set_new_context(int cid)
{
    return true;
}

nsapi_error_t AT_CellularContext::do_activate_context()
{
    return NSAPI_ERROR_OK;
}

void AT_CellularContext::activate_context()
{

}

void AT_CellularContext::deactivate_context()
{

}

void AT_CellularContext::do_connect()
{
}

#if NSAPI_PPP_AVAILABLE
nsapi_error_t AT_CellularContext::open_data_channel()
{
    return NSAPI_ERROR_OK;
}

void AT_CellularContext::ppp_status_cb(nsapi_event_t ev, intptr_t ptr)
{
}

#endif //#if NSAPI_PPP_AVAILABLE

nsapi_error_t AT_CellularContext::disconnect()
{
    return NSAPI_ERROR_OK;
}


nsapi_connection_status_t AT_CellularContext::get_connection_status() const
{
    return NSAPI_STATUS_DISCONNECTED;
}

nsapi_error_t AT_CellularContext::get_apn_backoff_timer(int &backoff_timer)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularContext::get_rate_control(
    CellularContext::RateControlExceptionReports &reports,
    CellularContext::RateControlUplinkTimeUnit &timeUnit, int &uplinkRate)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularContext::get_pdpcontext_params(pdpContextList_t &params_list)
{
    return NSAPI_ERROR_OK;
}

// Called by CellularDevice for network and cellular device changes
void AT_CellularContext::cellular_callback(nsapi_event_t ev, intptr_t ptr)
{
}

ControlPlane_netif *AT_CellularContext::get_cp_netif()
{
    return NULL;
}

nsapi_error_t AT_CellularContext::activate_non_ip_context()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularContext::setup_control_plane_opt()
{
    return NSAPI_ERROR_OK;
}

void AT_CellularContext::deactivate_ip_context()
{
}

void AT_CellularContext::deactivate_non_ip_context()
{
}

void AT_CellularContext::set_disconnect()
{
}

void AT_CellularContext::set_cid(int cid)
{
}

void AT_CellularContext::do_connect_with_retry()
{

}

char *AT_CellularContext::get_interface_name(char *interface_name)
{
    return NULL;
}
