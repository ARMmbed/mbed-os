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

#include "CellularStateMachine.h"
#include "CellularDevice.h"
#include "CellularPower.h"
#include "CellularSIM.h"
#include "CellularLog.h"
#include "Thread.h"
#include "UARTSerial.h"

#ifndef MBED_TRACE_MAX_LEVEL
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_INFO
#endif

// timeout to wait for AT responses
#define TIMEOUT_POWER_ON     (1*1000)
#define TIMEOUT_SIM_PIN      (1*1000)
#define TIMEOUT_NETWORK      (10*1000)
#define TIMEOUT_CONNECT      (60*1000)
#define TIMEOUT_REGISTRATION (180*1000)

// maximum time when retrying network register, attach and connect in seconds ( 20minutes )
#define TIMEOUT_NETWORK_MAX (20*60)

#define RETRY_COUNT_DEFAULT 3

const int STM_STOPPED = -99;
const int ACTIVE_PDP_CONTEXT = 0x01;
const int ATTACHED_TO_NETWORK = 0x02;

namespace mbed {

CellularStateMachine::CellularStateMachine(CellularDevice &device, events::EventQueue &queue) :
    _cellularDevice(device), _state(STATE_INIT), _next_state(_state), _target_state(_state),
    _event_status_cb(0), _network(0), _power(0), _sim(0), _queue(queue), _queue_thread(0), _sim_pin(0),
    _retry_count(0), _event_timeout(-1), _event_id(-1), _plmn(0), _command_success(false),
    _plmn_network_found(false), _is_retry(false), _cb_data(), _current_event(NSAPI_EVENT_CONNECTION_STATUS_CHANGE),
    _network_status(0)
{
#if MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY == 0
    _start_time = 0;
#else
    // so that not every device don't start at the exact same time (for example after power outage)
    _start_time = rand() % (MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY);
#endif // MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY

    // set initial retry values in seconds
    _retry_timeout_array[0] = 1; // double time on each retry in order to keep network happy
    _retry_timeout_array[1] = 2;
    _retry_timeout_array[2] = 4;
    _retry_timeout_array[3] = 8;
    _retry_timeout_array[4] = 16;
    _retry_timeout_array[5] = 32;
    _retry_timeout_array[6] = 64;
    _retry_timeout_array[7] = 128; // if around two minutes was not enough then let's wait much longer
    _retry_timeout_array[8] = 600;
    _retry_timeout_array[9] = TIMEOUT_NETWORK_MAX;
    _retry_array_length = RETRY_ARRAY_SIZE;
}

CellularStateMachine::~CellularStateMachine()
{
    stop();
}

void CellularStateMachine::reset()
{
    _state = STATE_INIT;
    _event_timeout = -1;
    _event_id = -1;
    _plmn_network_found = false;
    _is_retry = false;
    _network_status = 0;
    enter_to_state(STATE_INIT);
}

void CellularStateMachine::stop()
{
    tr_debug("CellularStateMachine stop");
    if (_queue_thread) {
        _queue.break_dispatch();
        _queue_thread->terminate();
        delete _queue_thread;
        _queue_thread = NULL;
    }

    reset();
    _event_id = STM_STOPPED;
    if (_power) {
        _cellularDevice.close_power();
        _power = NULL;
    }

    if (_sim) {
        _cellularDevice.close_sim();
        _sim = NULL;
    }

    if (_network) {
        _cellularDevice.close_network();
        _network = NULL;
    }
}

bool CellularStateMachine::power_on()
{
    _cb_data.error = _power->on();
    if (_cb_data.error != NSAPI_ERROR_OK && _cb_data.error != NSAPI_ERROR_UNSUPPORTED) {
        tr_warn("Power on failed. Try to power off/on.");
        _cb_data.error = _power->off();
        if (_cb_data.error != NSAPI_ERROR_OK && _cb_data.error != NSAPI_ERROR_UNSUPPORTED) {
            tr_error("Power off failed!");
        }
        return false;
    }
    return true;
}

void CellularStateMachine::set_sim_pin(const char *sim_pin)
{
    _sim_pin = sim_pin;
}

void CellularStateMachine::set_plmn(const char *plmn)
{
    _plmn = plmn;
}

bool CellularStateMachine::open_sim()
{
    if (!_sim) {
        // can only fail with allocation with new and then it's critical error
        _sim = _cellularDevice.open_sim();
    }

    CellularSIM::SimState state = CellularSIM::SimStateUnknown;
    // wait until SIM is readable
    // here you could add wait(secs) if you know start delay of your SIM
    _cb_data.error = _sim->get_sim_state(state);
    if (_cb_data.error != NSAPI_ERROR_OK) {
        tr_info("Waiting for SIM (err while reading)...");
        return false;
    }

    // report current state so callback can set sim pin if needed
    if (_event_status_cb) {
        _cb_data.status_data = state;
        _event_status_cb((nsapi_event_t)CellularSIMStatusChanged, (intptr_t)&_cb_data);
    }

    if (state == CellularSIM::SimStatePinNeeded) {
        if (strlen(_sim_pin)) {
            tr_info("Entering PIN to open SIM.");
            _cb_data.error = _sim->set_pin(_sim_pin);
            if (_cb_data.error) {
                tr_error("Failed to set PIN: error %d", _cb_data.error);
            }
        } else {
            // No sim pin provided even it's needed, stop state machine
            tr_error("PIN required but no SIM pin provided.");
            _retry_count = RETRY_ARRAY_SIZE;
            return false;
        }
    }

    return state == CellularSIM::SimStateReady;
}

bool CellularStateMachine::is_registered()
{
    CellularNetwork::RegistrationStatus status;
    bool is_registered = false;

    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (get_network_registration((CellularNetwork::RegistrationType) type, status, is_registered)) {
            if (is_registered) {
                break;
            }
        }
    }

    _cb_data.status_data = status;
    return is_registered || _network_status;
}

bool CellularStateMachine::get_network_registration(CellularNetwork::RegistrationType type,
                                                    CellularNetwork::RegistrationStatus &status, bool &is_registered)
{
    is_registered = false;
    bool is_roaming = false;
    CellularNetwork::registration_params_t reg_params;
    _cb_data.error = _network->get_registration_params(type, reg_params);

    if (_cb_data.error != NSAPI_ERROR_OK) {
        if (_cb_data.error != NSAPI_ERROR_UNSUPPORTED) {
            tr_warn("Get network registration failed (type %d)!", type);
        }
        return false;
    }
    status = reg_params._status;
    switch (status) {
        case CellularNetwork::RegisteredRoaming:
            is_roaming = true;// @suppress("No break at end of case")
        // fall-through
        case CellularNetwork::RegisteredHomeNetwork:
            is_registered = true;
            break;
        case CellularNetwork::RegisteredSMSOnlyRoaming:
            is_roaming = true;// @suppress("No break at end of case")
        // fall-through
        case CellularNetwork::RegisteredSMSOnlyHome:
            tr_warn("SMS only network registration!");
            break;
        case CellularNetwork::RegisteredCSFBNotPreferredRoaming:
            is_roaming = true; // @suppress("No break at end of case")
        // fall-through
        case CellularNetwork::RegisteredCSFBNotPreferredHome:
            tr_warn("Not preferred network registration!");
            break;
        case CellularNetwork::AttachedEmergencyOnly:
            tr_warn("Emergency only network registration!");
            break;
        case CellularNetwork::RegistrationDenied:
        case CellularNetwork::NotRegistered:
        case CellularNetwork::Unknown:
        case CellularNetwork::SearchingNetwork:
        default:
            break;
    }

    if (is_roaming) {
        tr_info("Roaming network.");
    }

    return true;
}

void CellularStateMachine::report_failure(const char *msg)
{
    tr_error("CellularStateMachine failure: %s", msg);

    _event_id = -1;
    if (_event_status_cb) {
        _cb_data.final_try = true;
        _event_status_cb(_current_event, (intptr_t)&_cb_data);
    }

    tr_error("CellularStateMachine target state %s, current state %s", get_state_string(_target_state), get_state_string(_state));
}

const char *CellularStateMachine::get_state_string(CellularState state) const
{
#if MBED_CONF_MBED_TRACE_ENABLE
    static const char *strings[STATE_MAX_FSM_STATE] = { "Init", "Power", "Device ready", "SIM pin", "Registering network", "Manual registering", "Attaching network"};
    return strings[state];
#else
    return NULL;
#endif // #if MBED_CONF_MBED_TRACE_ENABLE
}

bool CellularStateMachine::is_registered_to_plmn()
{
    int format;
    CellularNetwork::operator_t op;

    _cb_data.error = _network->get_operator_params(format, op);
    if (_cb_data.error == NSAPI_ERROR_OK) {
        if (format == 2) {
            // great, numeric format we can do comparison for that
            if (strcmp(op.op_num, _plmn) == 0) {
                return true;
            }
            return false;
        }

        // format was alpha, get operator names to do the comparing
        CellularNetwork::operator_names_list names_list;
        _cb_data.error = _network->get_operator_names(names_list);
        if (_cb_data.error == NSAPI_ERROR_OK) {
            CellularNetwork::operator_names_t *op_names = names_list.get_head();
            bool found_match = false;
            while (op_names) {
                if (format == 0) {
                    if (strcmp(op.op_long, op_names->alpha) == 0) {
                        found_match = true;
                    }
                } else if (format == 1) {
                    if (strcmp(op.op_short, op_names->alpha) == 0) {
                        found_match = true;
                    }
                }

                if (found_match) {
                    if (strcmp(_plmn, op_names->numeric)) {
                        names_list.delete_all();
                        return true;
                    }
                    names_list.delete_all();
                    return false;
                }
            }
        }
        names_list.delete_all();
    }

    return false;
}

void CellularStateMachine::enter_to_state(CellularState state)
{
    _next_state = state;
    _retry_count = 0;
    _command_success = false;
    _cb_data.error = NSAPI_ERROR_OK;
    _cb_data.status_data = -1;
    _cb_data.final_try = false;
}

void CellularStateMachine::retry_state_or_fail()
{
    if (++_retry_count < RETRY_ARRAY_SIZE) {
        tr_debug("%s: retry %d/%d", get_state_string(_state), _retry_count, RETRY_ARRAY_SIZE);
        _event_timeout = _retry_timeout_array[_retry_count];
        _is_retry = true;
    } else {
        report_failure(get_state_string(_state));
        return;
    }
}

void CellularStateMachine::state_init()
{
    _cellularDevice.set_timeout(TIMEOUT_POWER_ON);
    tr_info("Start connecting (timeout %d s)", TIMEOUT_POWER_ON / 1000);
    if (!_power) {
        _power = _cellularDevice.open_power();
    }
    _cb_data.error = _power->is_device_ready();
    if (_cb_data.error != NSAPI_ERROR_OK) {
        _event_timeout = _start_time;
        if (_start_time > 0) {
            tr_info("Startup delay %d ms", _start_time);
        }
        enter_to_state(STATE_POWER_ON);
    } else {
        enter_to_state(STATE_DEVICE_READY);
    }
}

void CellularStateMachine::state_power_on()
{
    _cellularDevice.set_timeout(TIMEOUT_POWER_ON);
    tr_info("Modem power ON (timeout %d s)", TIMEOUT_POWER_ON / 1000);
    if (power_on()) {
        enter_to_state(STATE_DEVICE_READY);
    } else {
        // retry to power on device
        retry_state_or_fail();
    }
}

bool CellularStateMachine::device_ready()
{
    tr_info("Modem ready");
    if (_cellularDevice.init_module() != NSAPI_ERROR_OK) {
        return false;
    }

    if (!_network) {
        _network = _cellularDevice.open_network();
    }

#ifdef MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY
    MBED_ASSERT(MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY >= CellularNetwork::RAT_GSM &&
                MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY < CellularNetwork::RAT_UNKNOWN);
    nsapi_error_t err = _network->set_access_technology((CellularNetwork::RadioAccessTechnology)MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY);
    if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
        tr_warning("Failed to set access technology to %d", MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY);
        return false;
    }
#endif // MBED_CONF_CELLULAR_DEBUG_AT

    if (_event_status_cb) {
        _event_status_cb((nsapi_event_t)CellularDeviceReady, (intptr_t)&_cb_data);
    }

    _power->remove_device_ready_urc_cb(mbed::callback(this, &CellularStateMachine::ready_urc_cb));
    _cellularDevice.close_power();
    _power = NULL;
    return true;
}

void CellularStateMachine::state_device_ready()
{
    _cellularDevice.set_timeout(TIMEOUT_POWER_ON);
    _cb_data.error = _power->set_at_mode();
    if (_cb_data.error == NSAPI_ERROR_OK) {
        if (device_ready()) {
            enter_to_state(STATE_SIM_PIN);
        } else {
            retry_state_or_fail();
        }
    } else {
        if (_retry_count == 0) {
            _power->set_device_ready_urc_cb(mbed::callback(this, &CellularStateMachine::ready_urc_cb));
        }
        retry_state_or_fail();
    }
}

void CellularStateMachine::state_sim_pin()
{
    _cellularDevice.set_timeout(TIMEOUT_SIM_PIN);
    tr_info("Setup SIM (timeout %d s)", TIMEOUT_SIM_PIN / 1000);
    if (open_sim()) {

        bool success = false;
        for (int type = 0; type < CellularNetwork::C_MAX; type++) {
            _cb_data.error = _network->set_registration_urc((CellularNetwork::RegistrationType)type, true);
            if (!_cb_data.error) {
                success = true;
            }
        }
        if (!success) {
            tr_warn("Failed to set any URC's for registration");
            retry_state_or_fail();
            return;
        }

        if (_network->is_active_context()) { // check if context was already activated
            tr_debug("ACTIVE CONTEXT FOUND, skip registering.");
            _network_status |= ACTIVE_PDP_CONTEXT;
        }
        CellularNetwork::AttachStatus status; // check if modem is already attached to a network
        if (_network->get_attach(status) == NSAPI_ERROR_OK && status == CellularNetwork::Attached) {
            _network_status |= ATTACHED_TO_NETWORK;
            tr_debug("DEVICE IS ALREADY ATTACHED TO NETWORK, skip registering and attach.");
        }
        if (_plmn) {
            enter_to_state(STATE_MANUAL_REGISTERING_NETWORK);
        } else {
            enter_to_state(STATE_REGISTERING_NETWORK);
        }
    } else {
        retry_state_or_fail();
    }
}

void CellularStateMachine::state_registering()
{
    _cellularDevice.set_timeout(TIMEOUT_NETWORK);
    tr_info("Network registration (timeout %d s)", TIMEOUT_REGISTRATION / 1000);
    if (is_registered()) {
        _cb_data.status_data = CellularNetwork::AlreadyRegistered;
        _cb_data.error = NSAPI_ERROR_OK;
        _event_status_cb(_current_event, (intptr_t)&_cb_data);
        // we are already registered, go to attach
        enter_to_state(STATE_ATTACHING_NETWORK);
    } else {
        _cellularDevice.set_timeout(TIMEOUT_REGISTRATION);
        if (!_command_success) {
            _cb_data.error = _network->set_registration();
            _command_success = (_cb_data.error == NSAPI_ERROR_OK);
        }
        retry_state_or_fail();
    }
}

// only used when _plmn is set
void CellularStateMachine::state_manual_registering_network()
{
    _cellularDevice.set_timeout(TIMEOUT_REGISTRATION);
    tr_info("Manual registration %s (timeout %d s)", _plmn, TIMEOUT_REGISTRATION / 1000);
    if (!_plmn_network_found) {
        if (is_registered() && is_registered_to_plmn()) {
            // we have to send registration changed event as network thinks that we are not registered even we have active PDP context
            _cb_data.status_data = CellularNetwork::AlreadyRegistered;
            _cb_data.error = NSAPI_ERROR_OK;
            _event_status_cb(_current_event, (intptr_t)&_cb_data);
            _plmn_network_found = true;
            enter_to_state(STATE_ATTACHING_NETWORK);
        } else {
            if (!_command_success) {
                _cb_data.error = _network->set_registration(_plmn);
                _command_success = (_cb_data.error == NSAPI_ERROR_OK);
            }
            retry_state_or_fail();
        }
    }
}

void CellularStateMachine::state_attaching()
{
    _cellularDevice.set_timeout(TIMEOUT_CONNECT);
    tr_info("Attaching network (timeout %d s)", TIMEOUT_CONNECT / 1000);
    if (_network_status != ATTACHED_TO_NETWORK) {
        _cb_data.error = _network->set_attach();
    }
    if (_cb_data.error == NSAPI_ERROR_OK) {
        _cellularDevice.close_sim();
        _sim = NULL;
        if (_event_status_cb) {
            _cb_data.status_data = CellularNetwork::Attached;
            _event_status_cb(_current_event, (intptr_t)&_cb_data);
        }
    } else {
        retry_state_or_fail();
    }
}

void CellularStateMachine::continue_from_state(CellularState state)
{
    _mutex.lock();
    tr_info("%s => %s", get_state_string((CellularStateMachine::CellularState)_state),
            get_state_string((CellularStateMachine::CellularState)state));
    _state = state;
    enter_to_state(state);
    _event_id = _queue.call_in(0, this, &CellularStateMachine::event);
    if (!_event_id) {
        _event_id = -1;
        _cb_data.error = NSAPI_ERROR_NO_MEMORY;
        report_failure("Failed to call queue.");
        stop();
    }
    _mutex.unlock();
}

nsapi_error_t CellularStateMachine::run_to_state(CellularStateMachine::CellularState state)
{
    _mutex.lock();
    // call pre_event via queue so that it's in same thread and it's safe to decisions
    int id = _queue.call_in(0, this, &CellularStateMachine::pre_event, state);
    if (!id) {
        stop();
        _mutex.unlock();
        return NSAPI_ERROR_NO_MEMORY;
    }
    _mutex.unlock();
    return NSAPI_ERROR_OK;
}

void CellularStateMachine::pre_event(CellularState state)
{
    if (_target_state < state) {
        // new wanted state will not be achieved with current _target_state so update it
        _target_state = state;
    } else {
        // wanted state is already / will be achieved, return without launching new event
        return;
    }
    // if _event_id is -1 it means that new event is not going to be launched so we must launch new event
    if (_event_id == -1) {
        if (!_cb_data.final_try) {
            // update next state so that we don't continue from previous state if state machine was paused and then started again.
            // but only if earlier try did not finish to failure, then we must continue from that state
            _state = _next_state;
        }
        enter_to_state(_next_state);
        _event_id = _queue.call_in(0, this, &CellularStateMachine::event);
        if (!_event_id) {
            _event_id = -1;
            report_failure("Failed to call queue.");
            stop();
        }
    }
}

bool CellularStateMachine::get_current_status(CellularStateMachine::CellularState &current_state, CellularStateMachine::CellularState &target_state)
{
    bool is_running;
    _mutex.lock();
    current_state = _state;
    target_state = _target_state;
    is_running = _event_id != -1;
    _mutex.unlock();
    return is_running;
}

void CellularStateMachine::event()
{
#if MBED_CONF_MBED_TRACE_ENABLE
    if (_network) {
        int rssi;
        int ber;
        if (_network->get_signal_quality(rssi, ber) == NSAPI_ERROR_OK) {
            if (rssi == 0) {
                tr_info("RSSI unknown");
            } else {
                tr_info("RSSI %d dBm", rssi);
            }
        }
    }
#endif

    _event_timeout = -1;
    _is_retry = false;

    switch (_state) {
        case STATE_INIT:
            _current_event = (nsapi_event_t)CellularDeviceReady;
            state_init();
            break;
        case STATE_POWER_ON:
            _current_event = (nsapi_event_t)CellularDeviceReady;
            state_power_on();
            break;
        case STATE_DEVICE_READY:
            _current_event = (nsapi_event_t)CellularDeviceReady;
            state_device_ready();
            break;
        case STATE_SIM_PIN:
            _current_event = (nsapi_event_t)CellularSIMStatusChanged;
            state_sim_pin();
            break;
        case STATE_REGISTERING_NETWORK:
            _current_event = (nsapi_event_t)CellularRegistrationStatusChanged;
            state_registering();
            break;
        case STATE_MANUAL_REGISTERING_NETWORK:
            _current_event = (nsapi_event_t)CellularRegistrationStatusChanged;
            state_manual_registering_network();
            break;
        case STATE_ATTACHING_NETWORK:
            _current_event = (nsapi_event_t)CellularAttachNetwork;
            state_attaching();
            break;
        default:
            MBED_ASSERT(0);
            break;
    }

    if ((_target_state == _state && _cb_data.error == NSAPI_ERROR_OK && !_is_retry) || _event_id == STM_STOPPED) {
        _event_id = -1;
        return;
    }

    if (_next_state != _state || _event_timeout >= 0) {
        if (_next_state != _state) { // state exit condition
            tr_debug("%s => %s", get_state_string((CellularStateMachine::CellularState)_state),
                     get_state_string((CellularStateMachine::CellularState)_next_state));
        } else {
            tr_info("Continue after %d seconds", _event_timeout);
        }
        _state = _next_state;
        if (_event_timeout == -1) {
            _event_timeout = 0;
        }
        _event_id = _queue.call_in(_event_timeout * 1000, callback(this, &CellularStateMachine::event));
        if (!_event_id) {
            _cb_data.error = NSAPI_ERROR_NO_MEMORY;
            report_failure("CellularStateMachine event failure!");
            return;
        }
    }
}

nsapi_error_t CellularStateMachine::start_dispatch()
{
    MBED_ASSERT(!_queue_thread);

    _queue_thread = new rtos::Thread(osPriorityNormal, 2048, NULL, "stm_queue");
    if (_queue_thread->start(callback(&_queue, &events::EventQueue::dispatch_forever)) != osOK) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    return NSAPI_ERROR_OK;
}

void CellularStateMachine::set_cellular_callback(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _event_status_cb = status_cb;
}

void CellularStateMachine::cellular_event_changed(nsapi_event_t ev, intptr_t ptr)
{
    cell_callback_data_t *data = (cell_callback_data_t *)ptr;
    if ((cellular_connection_status_t)ev == CellularRegistrationStatusChanged &&
            (_state == STATE_REGISTERING_NETWORK || _state == STATE_MANUAL_REGISTERING_NETWORK)) {
        // expect packet data so only these states are valid
        if ((data->status_data == CellularNetwork::RegisteredHomeNetwork || data->status_data == CellularNetwork::RegisteredRoaming) && data->error == NSAPI_ERROR_OK) {
            if (_plmn) {
                if (is_registered_to_plmn()) {
                    if (!_plmn_network_found) {
                        _plmn_network_found = true;
                        _queue.cancel(_event_id);
                        continue_from_state(STATE_ATTACHING_NETWORK);
                    }
                }
            } else {
                _queue.cancel(_event_id);
                continue_from_state(STATE_ATTACHING_NETWORK);
            }
        }
    }
}

void CellularStateMachine::ready_urc_cb()
{
    tr_debug("Device ready URC func called");
    if (_state == STATE_DEVICE_READY && _power->set_at_mode() == NSAPI_ERROR_OK) {
        tr_debug("State was STATE_DEVICE_READY and at mode ready, cancel state and move to next");
        _queue.cancel(_event_id);
        if (device_ready()) {
            continue_from_state(STATE_SIM_PIN);
        } else {
            continue_from_state(STATE_DEVICE_READY);
        }
    }
}

void CellularStateMachine::set_retry_timeout_array(uint16_t timeout[], int array_len)
{
    _retry_array_length = array_len > RETRY_ARRAY_SIZE ? RETRY_ARRAY_SIZE : array_len;

    for (int i = 0; i < _retry_array_length; i++) {
        _retry_timeout_array[i] = timeout[i];
    }
}

} // namespace

