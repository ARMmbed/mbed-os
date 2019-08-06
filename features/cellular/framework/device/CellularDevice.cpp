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
#include "CellularUtil.h"
#include "CellularLog.h"
#include "events/EventQueue.h"

namespace mbed {

MBED_WEAK CellularDevice *CellularDevice::get_default_instance()
{
    return get_target_default_instance();
}

MBED_WEAK CellularDevice *CellularDevice::get_target_default_instance()
{
    return NULL;
}

CellularDevice::CellularDevice(FileHandle *fh) : _network_ref_count(0), _sms_ref_count(0),
    _info_ref_count(0), _fh(fh), _queue(8 * EVENTS_EVENT_SIZE), _state_machine(0), _nw(0), _status_cb(0)
{
    MBED_ASSERT(fh);
    set_sim_pin(NULL);
    set_plmn(NULL);
}

CellularDevice::~CellularDevice()
{
    tr_debug("CellularDevice destruct");
    delete _state_machine;
}

void CellularDevice::stop()
{
    MBED_ASSERT(_state_machine);
    _state_machine->stop();
}

FileHandle &CellularDevice::get_file_handle() const
{
    return *_fh;
}

events::EventQueue *CellularDevice::get_queue()
{
    return &_queue;
}

CellularContext *CellularDevice::get_context_list() const
{
    return NULL;
}

void CellularDevice::get_retry_timeout_array(uint16_t *timeout, int &array_len) const
{
    if (_state_machine && timeout) {
        _state_machine->get_retry_timeout_array(timeout, array_len);
    }
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
        _nw = open_network(_fh);
        // Attach to network so we can get update status from the network
        _nw->attach(callback(this, &CellularDevice::stm_callback));
        _state_machine = new CellularStateMachine(*this, *get_queue(), *_nw);
        _state_machine->set_cellular_callback(callback(this, &CellularDevice::stm_callback));
        if (strlen(_plmn)) {
            _state_machine->set_plmn(_plmn);
        }
        if (strlen(_sim_pin)) {
            _state_machine->set_sim_pin(_sim_pin);
        }
    }
    err = _state_machine->start_dispatch();
    if (err) {
        tr_error("Start state machine failed.");
        delete _state_machine;
        _state_machine = NULL;
        return err;
    }
    return err;
}

nsapi_error_t CellularDevice::start_state_machine(CellularStateMachine::CellularState target_state)
{
    _mutex.lock();
    nsapi_error_t err = create_state_machine();
    if (err) {
        _mutex.unlock();
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

void CellularDevice::stm_callback(nsapi_event_t ev, intptr_t ptr)
{
    cellular_callback(ev, ptr);
}

void CellularDevice::cellular_callback(nsapi_event_t ev, intptr_t ptr, CellularContext *ctx)
{
    if (ev >= NSAPI_EVENT_CELLULAR_STATUS_BASE && ev <= NSAPI_EVENT_CELLULAR_STATUS_END) {
        cellular_connection_status_t cell_ev = (cellular_connection_status_t)ev;
        cell_callback_data_t *ptr_data = (cell_callback_data_t *)ptr;
        (void)ptr_data; // avoid compile warning, used only for debugging
        if (cell_ev == CellularStateRetryEvent) {
            tr_debug("callback: CellularStateRetryEvent, err: %d, data: %d, retrycount: %d", ptr_data->error, ptr_data->status_data, *(const int *)ptr_data->data);
        } else {
            tr_debug("callback: %d, err: %d, data: %d", ev, ptr_data->error, ptr_data->status_data);
        }
        if (cell_ev == CellularRegistrationStatusChanged && _state_machine) {
            // broadcast only network registration changes to state machine
            _state_machine->cellular_event_changed(ev, ptr);
        }
    } else {
        tr_debug("callback: %d, ptr: %d", ev, ptr);
        if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE && ptr == NSAPI_STATUS_DISCONNECTED) {
            // we have been disconnected, reset state machine so that application can start connect sequence again
            if (_state_machine) {
                CellularStateMachine::CellularState current_state, targeted_state;
                bool is_running = _state_machine->get_current_status(current_state, targeted_state);
                if (!is_running) {
                    _state_machine->reset();
                }
            }
        }
    }

    // broadcast network and cellular changes to state machine and CellularContext.
    CellularContext *curr = get_context_list();
    while (curr) {
        if (ctx) {
            if (ctx == curr) {
                curr->cellular_callback(ev, ptr);
                break;
            }
        } else {
            curr->cellular_callback(ev, ptr);
        }
        curr = curr->_next;
    }

    // forward to callback function if set by attach(...).
    if (_status_cb) {
        _status_cb(ev, ptr);
    }
}

nsapi_error_t CellularDevice::shutdown()
{
    if (_state_machine) {
        _state_machine->stop();
    }
    CellularContext *curr = get_context_list();
    while (curr) {
        curr->cellular_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
        curr = (CellularContext *)curr->_next;
    }
    return NSAPI_ERROR_OK;
}

void CellularDevice::set_retry_timeout_array(const uint16_t timeout[], int array_len)
{
    if (create_state_machine() == NSAPI_ERROR_OK) {
        _state_machine->set_retry_timeout_array(timeout, array_len);
    }
}

} // namespace mbed
