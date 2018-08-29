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

#include "CellularDevice.h"
#include "CellularUtil.h"
#include "CellularLog.h"
#include "CellularTargets.h"
#include "EventQueue.h"
#include "UARTSerial.h"

#ifdef CELLULAR_DEVICE
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)
#endif // CELLULAR_DEVICE

namespace mbed {

#ifdef CELLULAR_DEVICE
MBED_WEAK CellularDevice *CellularDevice::get_default_instance()
{
    static events::EventQueue event_queue(4 * EVENTS_EVENT_SIZE);
    static CELLULAR_DEVICE device(event_queue);
    return &device;
}
#else
MBED_WEAK CellularDevice *CellularDevice::get_default_instance()
{
    return NULL;
}
#endif // CELLULAR_DEVICE

CellularDevice::CellularDevice() : _error(NSAPI_ERROR_OK), _network_ref_count(0), _sms_ref_count(0),
        _power_ref_count(0), _sim_ref_count(0), _info_ref_count(0), _is_connected(false),
         _state_machine(0), _fh(0)
{
}

CellularDevice::~CellularDevice()
{
     delete _state_machine;
}

void CellularDevice::stop()
{
    MBED_ASSERT(_state_machine);
    _state_machine->stop();
}

bool CellularDevice::is_connected() const
{
    return _is_connected;
}

events::EventQueue *CellularDevice::get_queue() const
{
    return NULL;
}

nsapi_error_t CellularDevice::set_credentials(const char *apn, const char *uname, const char *pwd)
{
    MBED_ASSERT(_state_machine);
    return _state_machine->set_credentials(apn, uname, pwd);
}

void CellularDevice::set_sim_pin(const char *sim_pin)
{
    MBED_ASSERT(_state_machine);
    _state_machine->set_sim_pin(sim_pin);
}

nsapi_error_t CellularDevice::init_stm(FileHandle *fh)
{
    MBED_ASSERT(!_state_machine);

    if (fh == NULL) {
        return NSAPI_ERROR_PARAMETER;
    }
    _fh = fh;

    _state_machine = new CellularStateMachine(*this, *get_queue(), open_power(_fh));
    _state_machine->attach(callback(this, &CellularDevice::network_callback));
    _state_machine->set_sim(open_sim(_fh));
    CellularNetwork *nw = open_network(_fh);
    _state_machine->set_network(nw);

    nsapi_error_t err = nw->init();
    if (err != NSAPI_ERROR_OK) {
        delete _state_machine;
        _state_machine = NULL;
    }

    return err;
}

nsapi_error_t CellularDevice::start_dispatch() {
    MBED_ASSERT(_state_machine);
    return _state_machine->start_dispatch();
}

nsapi_error_t CellularDevice::set_device_ready()
{
    return _state_machine->run_to_state(CellularStateMachine::STATE_DEVICE_READY);
}

nsapi_error_t CellularDevice::set_sim_ready()
{
    return _state_machine->run_to_state(CellularStateMachine::STATE_SIM_PIN);
}

nsapi_error_t CellularDevice::register_to_network()
{
    return _state_machine->run_to_state(CellularStateMachine::STATE_REGISTERING_NETWORK);
}

nsapi_error_t CellularDevice::attach_to_network()
{
    return _state_machine->run_to_state(CellularStateMachine::STATE_ATTACHING_NETWORK);
}

nsapi_error_t CellularDevice::activate_context()
{
    return _state_machine->run_to_state(CellularStateMachine::STATE_ACTIVATING_PDP_CONTEXT);
}

nsapi_error_t CellularDevice::connect(const char *sim_pin, const char *apn,
                                  const char *uname, const char *pwd)
{
    set_sim_pin(sim_pin);
    set_credentials(apn, uname, pwd);
    return connect();
}

nsapi_error_t CellularDevice::connect()
{
    return _state_machine->run_to_state(CellularStateMachine::STATE_CONNECTED);
}

nsapi_error_t CellularDevice::disconnect()
{
    MBED_ASSERT(_state_machine);
    return _state_machine->disconnect();
}

void CellularDevice::set_plmn(const char* plmn)
{
    MBED_ASSERT(_state_machine);
    _state_machine->set_plmn(plmn);
}

void CellularDevice::set_blocking(bool blocking)
{
    MBED_ASSERT(_state_machine);
    _state_machine->set_blocking(blocking);
}

void CellularDevice::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _nw_status_cb = status_cb;
}

void CellularDevice::network_callback(nsapi_event_t ev, intptr_t ptr)
{
    if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE) {
        if (ptr == NSAPI_STATUS_GLOBAL_UP) {
            _is_connected = true;
        } else {
            _is_connected = false;
        }
    }

    if (ev >= NSAPI_EVENT_CELLULAR_STATUS_BASE && ev <= NSAPI_EVENT_CELLULAR_STATUS_END) {
        tr_debug("Device: network_callback called with event: %d, err: %d, data: %d", ev, ((cell_callback_data_t*)ptr)->error, ((cell_callback_data_t*)ptr)->status_data);
    } else {
        tr_debug("Device: network_callback called with event: %d, ptr: %d", ev, ptr);
    }
      // forward network callback to application is it has registered with attach
    if (_nw_status_cb) {
        _nw_status_cb(ev, ptr);
    }
}


} // namespae mbed
