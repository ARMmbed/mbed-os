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

#include "AT_CellularDevice_stub.h"
#include "AT_CellularNetwork.h"
#include "AT_CellularContext.h"
#include "ATHandler.h"

const int DEFAULT_AT_TIMEOUT = 1000;

using namespace mbed;

int AT_CellularDevice_stub::failure_count = 0;
nsapi_error_t AT_CellularDevice_stub::nsapi_error_value = 0;
int AT_CellularDevice_stub::init_module_failure_count = 0;
int AT_CellularDevice_stub::set_pin_failure_count = 0;
int AT_CellularDevice_stub::get_sim_failure_count = 0;
bool AT_CellularDevice_stub::pin_needed = false;
bool AT_CellularDevice_stub::supported_bool = false;
int AT_CellularDevice_stub::max_sock_value = 1;

AT_CellularDevice::AT_CellularDevice(FileHandle *fh) :
    CellularDevice(),
    _at(fh, _queue, get_property(AT_CellularDevice::PROPERTY_AT_SEND_DELAY), "\r"),
#if MBED_CONF_CELLULAR_USE_SMS
    _sms(0),
#endif // MBED_CONF_CELLULAR_USE_SMS
    _network(0),
    _information(0), _context_list(0), _default_timeout(DEFAULT_AT_TIMEOUT), _modem_debug_on(false)
{
}

AT_CellularDevice::~AT_CellularDevice()
{
    close_network();
}

ATHandler *AT_CellularDevice::get_at_handler()
{
    return &_at;
}

CellularContext *create_context(const char *apn)
{
}

void delete_context(CellularContext *context)
{
}

CellularNetwork *AT_CellularDevice::open_network()
{
    if (_network) {
        return _network;
    }
    _network = new AT_CellularNetwork(_at, *this);
    return _network;
}
#if MBED_CONF_CELLULAR_USE_SMS
CellularSMS *AT_CellularDevice::open_sms()
{
    return NULL;
}

void AT_CellularDevice::close_sms()
{
}

AT_CellularSMS *AT_CellularDevice::open_sms_impl(ATHandler &at)
{
    return NULL;
}

#endif // MBED_CONF_CELLULAR_USE_SMS

CellularInformation *AT_CellularDevice::open_information()
{
    return NULL;
}

void AT_CellularDevice::close_network()
{
    if (_network) {
        delete _network;
        _network = NULL;
    }
}

void AT_CellularDevice::close_information()
{
}

CellularContext *AT_CellularDevice::get_context_list() const
{
    return NULL;
}

CellularContext *AT_CellularDevice::create_context(const char *apn, bool cp_req, bool nonip_req)
{
    return NULL;
}

AT_CellularContext *AT_CellularDevice::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return NULL;
}

void AT_CellularDevice::delete_context(CellularContext *context)
{
}

AT_CellularNetwork *AT_CellularDevice::open_network_impl(ATHandler &at)
{
    _network = new AT_CellularNetwork(at, *this);

    return _network;
}

AT_CellularInformation *AT_CellularDevice::open_information_impl(ATHandler &at)
{
    return NULL;
}

void AT_CellularDevice::set_timeout(int timeout)
{
    _default_timeout = timeout;
}

void AT_CellularDevice::modem_debug_on(bool on)
{
    _modem_debug_on = on;

}

nsapi_error_t AT_CellularDevice::is_ready()
{
    if (AT_CellularDevice_stub::init_module_failure_count) {
        AT_CellularDevice_stub::init_module_failure_count--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return AT_CellularDevice_stub::nsapi_error_value;
}

void AT_CellularDevice::set_ready_cb(mbed::Callback<void()> callback)
{
}

nsapi_error_t AT_CellularDevice::set_power_save_mode(int periodic_time, int active_time)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularDevice::init()
{
    if (AT_CellularDevice_stub::init_module_failure_count) {
        AT_CellularDevice_stub::init_module_failure_count--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return AT_CellularDevice_stub::nsapi_error_value;
}

nsapi_error_t AT_CellularDevice::shutdown()
{
    if (AT_CellularDevice_stub::init_module_failure_count) {
        AT_CellularDevice_stub::init_module_failure_count--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return AT_CellularDevice_stub::nsapi_error_value;
}

nsapi_error_t AT_CellularDevice::set_pin(const char *sim_pin)
{
    if (AT_CellularDevice_stub::set_pin_failure_count) {
        AT_CellularDevice_stub::set_pin_failure_count--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return AT_CellularDevice_stub::nsapi_error_value;
}

nsapi_error_t AT_CellularDevice::get_sim_state(SimState &state)
{
    if (AT_CellularDevice_stub::get_sim_failure_count) {
        AT_CellularDevice_stub::get_sim_failure_count--;
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (AT_CellularDevice_stub::pin_needed) {
        AT_CellularDevice_stub::pin_needed = false;
        state = SimStatePinNeeded;
    } else {
        state = SimStateReady;
    }

    return AT_CellularDevice_stub::nsapi_error_value;
}

nsapi_error_t AT_CellularDevice::hard_power_on()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::hard_power_off()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::soft_power_on()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::soft_power_off()
{
    return NSAPI_ERROR_OK;
}

void AT_CellularDevice::cellular_callback(nsapi_event_t ev, intptr_t ptr, CellularContext *ctx)
{
}

void AT_CellularDevice::set_at_urcs_impl()
{
}

nsapi_error_t AT_CellularDevice::clear()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::set_baud_rate(int baud_rate)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::set_baud_rate_impl(int baud_rate)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularDevice::set_cellular_properties(const intptr_t *property_array)
{
}

intptr_t AT_CellularDevice::get_property(CellularProperty key)
{
    if (key == PROPERTY_C_GREG) {
        return AT_CellularNetwork::RegistrationModeDisable;
    } else if (key == PROPERTY_C_REG || key == PROPERTY_C_EREG) {
        return AT_CellularNetwork::RegistrationModeEnable;
    } else if (key == PROPERTY_AT_CGAUTH) {
        return true;
    } else if (key == PROPERTY_IPV4_PDP_TYPE) {
        return true;
    } else if (key == PROPERTY_SOCKET_COUNT) {
        return AT_CellularDevice_stub::max_sock_value;
    }

    return AT_CellularDevice_stub::supported_bool;
}
