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

#include "AT_CellularDevice.h"
#include "AT_CellularNetwork.h"
#include "ATHandler.h"

const int DEFAULT_AT_TIMEOUT = 1000;

using namespace mbed;

AT_CellularDevice::AT_CellularDevice(FileHandle *fh) : CellularDevice(fh), _network(0), _sms(0),
    _power(0), _information(0), _context_list(0), _default_timeout(DEFAULT_AT_TIMEOUT),
    _modem_debug_on(false)
{
}

AT_CellularDevice::~AT_CellularDevice()
{
}

ATHandler *AT_CellularDevice::get_at_handler(FileHandle *fileHandle)
{
    return ATHandler::get_instance(fileHandle, _queue, _default_timeout, "\r", get_send_delay(), _modem_debug_on);
}

ATHandler *AT_CellularDevice::get_at_handler()
{
    return get_at_handler(NULL);
}

nsapi_error_t AT_CellularDevice::release_at_handler(ATHandler *at_handler)
{
    if (at_handler) {
        return at_handler->close();
    } else {
        return NSAPI_ERROR_PARAMETER;
    }
}

CellularContext *create_context(FileHandle *fh = NULL, const char *apn = MBED_CONF_NSAPI_DEFAULT_CELLULAR_APN)
{
}

void delete_context(CellularContext *context)
{
}

CellularNetwork *AT_CellularDevice::open_network(FileHandle *fh)
{
    return new AT_CellularNetwork(*ATHandler::get_instance(fh, _queue, _default_timeout, "\r", get_send_delay(), _modem_debug_on));
}

CellularSMS *AT_CellularDevice::open_sms(FileHandle *fh)
{
    return NULL;
}

CellularPower *AT_CellularDevice::open_power(FileHandle *fh)
{
    return NULL;
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

CellularContext *AT_CellularDevice::create_context(FileHandle *fh, const char *apn)
{
    return NULL;
}

AT_CellularContext *AT_CellularDevice::create_context_impl(ATHandler &at, const char *apn)
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

nsapi_error_t AT_CellularDevice::set_power_save_mode(int periodic_time, int active_time)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::init_module()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::set_pin(const char *sim_pin)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularDevice::get_sim_state(SimState &state)
{
    return NSAPI_ERROR_OK;
}
