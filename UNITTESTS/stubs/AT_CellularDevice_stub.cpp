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
#include "AT_CellularPower.h"
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

AT_CellularDevice::AT_CellularDevice(FileHandle *fh) : CellularDevice(fh), _network(0), _sms(0),
    _power(0), _information(0), _context_list(0), _default_timeout(DEFAULT_AT_TIMEOUT),
    _modem_debug_on(false)
{
    _atHandlers = new ATHandler(fh, _queue, 0, ",");
    _at = _atHandlers;
}

AT_CellularDevice::~AT_CellularDevice()
{
    delete _network;
    delete _power;
    delete _atHandlers;
}

ATHandler *AT_CellularDevice::get_at_handler(FileHandle *fileHandle)
{
    return _atHandlers;
}

void AT_CellularDevice::release_at_handler(ATHandler *at_handler)
{
}

CellularContext *AT_CellularDevice::create_context(UARTSerial *serial, const char *const apn, PinName dcd_pin,
                                                   bool active_high, bool cp_req, bool nonip_req)
{
}

CellularContext *create_context(FileHandle *fh, const char *apn)
{
}

void delete_context(CellularContext *context)
{
}

CellularNetwork *AT_CellularDevice::open_network(FileHandle *fh)
{
    _network = new AT_CellularNetwork(*_atHandlers);
    return _network;
}

CellularSMS *AT_CellularDevice::open_sms(FileHandle *fh)
{
    return NULL;
}

CellularPower *AT_CellularDevice::open_power(FileHandle *fh)
{
    _power = new AT_CellularPower(*_atHandlers);
    return _power;
}

CellularInformation *AT_CellularDevice::open_information(FileHandle *fh)
{
    return NULL;
}

void AT_CellularDevice::close_network()
{
}

void AT_CellularDevice::close_sms()
{
}

void AT_CellularDevice::close_power()
{
}

void AT_CellularDevice::close_information()
{
}

CellularContext *AT_CellularDevice::get_context_list() const
{
    return NULL;
}

CellularContext *AT_CellularDevice::create_context(FileHandle *fh, const char *apn, bool cp_req, bool nonip_req)
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
    return new AT_CellularNetwork(at);
}

AT_CellularSMS *AT_CellularDevice::open_sms_impl(ATHandler &at)
{
    return NULL;
}

AT_CellularPower *AT_CellularDevice::open_power_impl(ATHandler &at)
{
    return NULL;
}

AT_CellularInformation *AT_CellularDevice::open_information_impl(ATHandler &at)
{
    return NULL;
}

void AT_CellularDevice::set_timeout(int timeout)
{
    _default_timeout = timeout;
}

uint16_t AT_CellularDevice::get_send_delay() const
{
    return 0;
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

nsapi_error_t AT_CellularDevice::reset()
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
