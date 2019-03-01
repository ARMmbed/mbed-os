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

#include "CellularDevice.h"
#include "CellularDevice_stub.h"
#include "EventQueue.h"
#include "CellularUtil.h"

using namespace mbed;

int CellularDevice_stub::connect_counter = -1;


MBED_WEAK CellularDevice *CellularDevice::get_default_instance()
{
    return NULL;
}

CellularDevice::CellularDevice(FileHandle *fh) :  _network_ref_count(0), _sms_ref_count(0),
    _info_ref_count(0), _fh(fh), _queue(5 * EVENTS_EVENT_SIZE), _state_machine(0), _nw(0)
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

CellularContext *CellularDevice::get_context_list() const
{
    return NULL;
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
