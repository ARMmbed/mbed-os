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

#include "CellularDevice_stub.h"
#include "events/EventQueue.h"
#include "CellularUtil.h"
#include "myCellularDevice.h"

using namespace mbed;

int CellularDevice_stub::connect_counter = -1;
bool CellularDevice_stub::create_in_get_default = false;
uint16_t CellularDevice_stub::retry_timeout_array[CELLULAR_RETRY_ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int CellularDevice_stub::retry_array_length = 0;

MBED_WEAK CellularDevice *CellularDevice::get_default_instance()
{
    if (CellularDevice_stub::create_in_get_default) {
        static myCellularDevice dev(NULL);
        return &dev;
    } else {
        return NULL;
    }
}

CellularDevice::CellularDevice() :
    _network_ref_count(0),
#if MBED_CONF_CELLULAR_USE_SMS
    _sms_ref_count(0),
#endif //MBED_CONF_CELLULAR_USE_SMS
    _info_ref_count(0), _queue(10 * EVENTS_EVENT_SIZE), _state_machine(0),
    _nw(0), _status_cb(0)
{
}

CellularDevice::~CellularDevice()
{

}

events::EventQueue *CellularDevice::get_queue()
{
    return &_queue;
}

void CellularDevice::set_plmn(char const *)
{
}

void CellularDevice::set_sim_pin(char const *)
{
}

void CellularDevice::get_retry_timeout_array(uint16_t *timeout, int &array_len) const
{
    array_len = CellularDevice_stub::retry_array_length;

    if (CellularDevice_stub::retry_array_length == 0) {
        timeout = 0;
    } else {
        timeout = CellularDevice_stub::retry_timeout_array;
    }
}

nsapi_error_t CellularDevice::set_device_ready()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t CellularDevice::set_sim_ready()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t CellularDevice::register_to_network()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t CellularDevice::attach_to_network()
{
    if (CellularDevice_stub::connect_counter == 0) {
        return NSAPI_ERROR_ALREADY;
    } else  if (CellularDevice_stub::connect_counter == 1) {
        CellularDevice_stub::connect_counter--;
        return NSAPI_ERROR_IN_PROGRESS;
    } else if (CellularDevice_stub::connect_counter == 2) {
        CellularDevice_stub::connect_counter--;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t CellularDevice::set_pin(const char *sim_pin)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t CellularDevice::get_sim_state(SimState &state)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t CellularDevice::shutdown()
{
    return NSAPI_ERROR_OK;
}

void CellularDevice::cellular_callback(nsapi_event_t ev, intptr_t ptr, CellularContext *ctx)
{
}
