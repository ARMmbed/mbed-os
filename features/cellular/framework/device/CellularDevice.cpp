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
#include "CellularContext.h"
#include "CellularSIM.h"
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
    static UARTSerial serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
#if DEVICE_SERIAL_FC
    if (MDMRTS != NC && MDMCTS != NC) {
        tr_info("_USING flow control, MDMRTS: %d MDMCTS: %d", MDMRTS, MDMCTS);
        serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
    }
#endif
    static CELLULAR_DEVICE device(&serial);
    return &device;
}
#else
MBED_WEAK CellularDevice *CellularDevice::get_default_instance()
{
    return NULL;
}
#endif // CELLULAR_DEVICE

CellularDevice::CellularDevice(FileHandle *fh) : _network_ref_count(0), _sms_ref_count(0), _power_ref_count(0), _sim_ref_count(0),
    _info_ref_count(0), _fh(fh), _queue(5 * EVENTS_EVENT_SIZE), _state_machine(0), _nw(0), _status_cb(0)
{
    set_sim_pin(NULL);
    set_plmn(NULL);
}

CellularDevice::~CellularDevice()
{
}

void CellularDevice::stop()
{
    MBED_ASSERT(_state_machine);
    _state_machine->stop();
}

events::EventQueue *CellularDevice::get_queue()
{
    return &_queue;
}

CellularContext *CellularDevice::get_context_list() const
{
    return NULL;
}

void CellularDevice::set_sim_pin(const char *sim_pin)
{
    if (sim_pin) {
        strncpy(_sim_pin, sim_pin, sizeof(_sim_pin));
        _sim_pin[sizeof(_sim_pin) - 1] = '\0';
    } else {
        memset(_sim_pin, 0, sizeof(_sim_pin));
    }
}

void CellularDevice::set_plmn(const char *plmn)
{
    if (plmn) {
        strncpy(_plmn, plmn, sizeof(_plmn));
        _plmn[sizeof(_plmn) - 1] = '\0';
    } else {
        memset(_plmn, 0, sizeof(_plmn));
    }
}

nsapi_error_t CellularDevice::set_device_ready()
{
    return start_state_machine(CellularStateMachine::STATE_DEVICE_READY);
}

nsapi_error_t CellularDevice::set_sim_ready()
{
    return start_state_machine(CellularStateMachine::STATE_SIM_PIN);
}

nsapi_error_t CellularDevice::register_to_network()
{
    return start_state_machine(CellularStateMachine::STATE_REGISTERING_NETWORK);
}

nsapi_error_t CellularDevice::attach_to_network()
{
    return start_state_machine(CellularStateMachine::STATE_ATTACHING_NETWORK);
}

nsapi_error_t CellularDevice::create_state_machine()
{
    nsapi_error_t err = NSAPI_ERROR_OK;
    if (!_state_machine) {
        _state_machine = new CellularStateMachine(*this, *get_queue());
        _state_machine->set_cellular_callback(callback(this, &CellularDevice::cellular_callback));
        err = _state_machine->start_dispatch();
        if (err) {
            delete _state_machine;
            _state_machine = NULL;
        }
    }
    return err;
}

nsapi_error_t CellularDevice::start_state_machine(CellularStateMachine::CellularState target_state)
{
    _mutex.lock();
    nsapi_error_t err = create_state_machine();
    if (err) {
        return err;
    }

    CellularStateMachine::CellularState current_state, targeted_state;

    bool is_running = _state_machine->get_current_status(current_state, targeted_state);

    if (current_state >= target_state) { // can stm be in this state but failed?
        _mutex.unlock();
        return NSAPI_ERROR_ALREADY;
    } else if (is_running && targeted_state >= target_state) {
        _mutex.unlock();
        return NSAPI_ERROR_IN_PROGRESS;
    }

    err = _state_machine->run_to_state(target_state);
    _mutex.unlock();

    return err;
}

void CellularDevice::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _status_cb = status_cb;
}

void CellularDevice::cellular_callback(nsapi_event_t ev, intptr_t ptr)
{
    if (ev >= NSAPI_EVENT_CELLULAR_STATUS_BASE && ev <= NSAPI_EVENT_CELLULAR_STATUS_END) {
        cell_callback_data_t *ptr_data = (cell_callback_data_t *)ptr;
        tr_debug("callback: %d, err: %d, data: %d", ev, ptr_data->error, ptr_data->status_data);
        cellular_connection_status_t cell_ev = (cellular_connection_status_t)ev;
        if (cell_ev == CellularRegistrationStatusChanged && _state_machine) {
            // broadcast only network registration changes to state machine
            _state_machine->cellular_event_changed(ev, ptr);
        }

        if (cell_ev == CellularDeviceReady && ptr_data->error == NSAPI_ERROR_OK) {
            // Here we can create mux and give new filehandles as mux reserves the one what was in use.
            // if mux we would need to set new filehandle:_state_machine->set_filehandle( get fh from mux);
            _nw = open_network(_fh);
            // Attach to network so we can get update status from the network
            _nw->attach(callback(this, &CellularDevice::cellular_callback));
            if (strlen(_plmn)) {
                _state_machine->set_plmn(_plmn);
            }
        } else if (cell_ev == CellularSIMStatusChanged && ptr_data->error == NSAPI_ERROR_OK &&
                   ptr_data->status_data == CellularSIM::SimStatePinNeeded) {
            if (strlen(_sim_pin)) {
                _state_machine->set_sim_pin(_sim_pin);
            }
        }
    } else {
        tr_debug("callback: %d, ptr: %d", ev, ptr);
        if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE && ptr == NSAPI_STATUS_DISCONNECTED) {
            // we have been disconnected, reset state machine so that application can start connect sequence again
            if (_state_machine) {
                _state_machine->reset();
            }
        }
    }

    // broadcast network and cellular changes to state machine and CellularContext.
    CellularContext *curr = get_context_list();
    while (curr) {
        curr->cellular_callback(ev, ptr);
        curr = curr->_next;
    }

    // forward to callback function if set by attach(...)
    if (_status_cb) {
        _status_cb(ev, ptr);
    }
}

} // namespace mbed
