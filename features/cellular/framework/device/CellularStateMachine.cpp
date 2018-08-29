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
#include "CellularLog.h"
#include "CellularUtil.h"
#include "CellularPower.h"
#include "CellularSIM.h"
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

namespace mbed {

CellularStateMachine::CellularStateMachine(CellularDevice &device, events::EventQueue &queue, CellularPower *power) :
        _cellularDevice(device), _state(STATE_INIT), _next_state(_state), _target_state(_state),
        _event_status_cb(0), _network(0), _power(power), _sim(0), _queue(queue), _queue_thread(0), _retry_count(0),
        _event_timeout(-1), _event_id(0), _plmn(0), _command_success(false), _plmn_network_found(false), _cb_data(),
        _current_event(NSAPI_EVENT_CONNECTION_STATUS_CHANGE), _automatic_reconnect(true), _blocking(true),
        _stm_semaphore(0)
{
    memset(_sim_pin, 0, sizeof(_sim_pin));
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

void CellularStateMachine::stop()
{
    _queue.cancel(_event_id);
    _queue.break_dispatch();

    if (_queue_thread) {
        _queue_thread->terminate();
        delete _queue_thread;
        _queue_thread = NULL;
    }

    _state = STATE_INIT;
    _next_state = _state;
    _target_state = _state;
    _cb_data.error = NSAPI_ERROR_OK;
    _cb_data.status_data = -1;

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

void CellularStateMachine::set_blocking(bool blocking)
{
    _blocking = blocking;
}

void CellularStateMachine::set_automatic_reconnect(bool do_reconnect)
{
    _automatic_reconnect = do_reconnect;
}

events::EventQueue *CellularStateMachine::get_queue() const
{
    return &_queue;
}

nsapi_error_t CellularStateMachine::set_credentials(const char *apn, const char *uname, const char *pwd)
{
    return _network->set_credentials(apn, uname, pwd);
}

void CellularStateMachine::set_sim(CellularSIM* sim)
{
    if (_sim) {
        // we own this so we'll close it before accepting new one
        _cellularDevice.close_sim();
    }
    _sim = sim;
}

void CellularStateMachine::set_network(CellularNetwork* nw)
{
    if (_network) {
        _cellularDevice.close_network();
    }
    _network = nw;
    _network->attach(callback(this, &CellularStateMachine::network_callback));
}

void CellularStateMachine::set_power(CellularPower* pwr)
{
    if (_power) {
        _cellularDevice.close_power();
    }
    _power = pwr;
}

bool CellularStateMachine::power_on()
{
    _cb_data.error = _power->on();
    if (_cb_data.error != NSAPI_ERROR_OK && _cb_data.error != NSAPI_ERROR_UNSUPPORTED) {
        tr_warn("Cellular start failed. Power off/on.");
        _cb_data.error = _power->off();
        if (_cb_data.error != NSAPI_ERROR_OK && _cb_data.error != NSAPI_ERROR_UNSUPPORTED) {
            tr_error("Cellular power down failing after failed power up attempt!");
        }
        return false;
    }
    return true;
}

void CellularStateMachine::set_sim_pin(const char *sim_pin)
{
    strncpy(_sim_pin, sim_pin, sizeof(_sim_pin));
    _sim_pin[sizeof(_sim_pin) - 1] = '\0';
}

void CellularStateMachine::set_plmn(const char *plmn)
{
    _plmn = plmn;
}

bool CellularStateMachine::open_sim()
{
    CellularSIM::SimState state = CellularSIM::SimStateUnknown;
    // wait until SIM is readable
    // here you could add wait(secs) if you know start delay of your SIM
    _cb_data.error = _sim->get_sim_state(state);
    if (_cb_data.error != NSAPI_ERROR_OK) {
        tr_info("Waiting for SIM (err while reading)...");
        if (_event_status_cb) {
            _cb_data.status_data = state;
            _event_status_cb((nsapi_event_t)CellularSIMStatusChanged, (intptr_t )&_cb_data);
        }
        return false;
    }

    // report current state so callback can set sim pin if needed
    if (_event_status_cb) {
        _cb_data.status_data = state;
        _event_status_cb((nsapi_event_t)CellularSIMStatusChanged, (intptr_t )&_cb_data);
    }

    if (state == CellularSIM::SimStatePinNeeded) {
        if (strlen(_sim_pin)) {
            tr_info("Entering PIN to open SIM");
            _cb_data.error = _sim->set_pin(_sim_pin);
            if (_cb_data.error) {
                tr_error("SIM pin set failed with: %d", _cb_data.error);
            }
        } else {
            // No sim pin provided even it's needed, stop state machine
            tr_error("PIN required but No SIM pin provided.");
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
            tr_debug("get_network_registration: type=%d, status=%d", type, status);
            if (is_registered) {
                break;
            }
        }
    }

    return is_registered;
}

bool CellularStateMachine::get_network_registration(CellularNetwork::RegistrationType type,
                                                     CellularNetwork::RegistrationStatus &status, bool &is_registered)
{
    is_registered = false;
    bool is_roaming = false;
    _cb_data.error = _network->get_registration_status(type, status);
    if (_cb_data.error != NSAPI_ERROR_OK) {
        if (_cb_data.error != NSAPI_ERROR_UNSUPPORTED) {
            tr_warn("Get network registration failed (type %d)!", type);
        }
        return false;
    }
    switch (status) {
        case CellularNetwork::RegisteredRoaming:
            is_roaming = true;
        // fall-through
        case CellularNetwork::RegisteredHomeNetwork:
            is_registered = true;
            break;
        case CellularNetwork::RegisteredSMSOnlyRoaming:
            is_roaming = true;
        // fall-through
        case CellularNetwork::RegisteredSMSOnlyHome:
            tr_warn("SMS only network registration!");
            break;
        case CellularNetwork::RegisteredCSFBNotPreferredRoaming:
            is_roaming = true;
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
        tr_warn("Roaming cellular network!");
    }

    return true;
}

void CellularStateMachine::report_failure(const char *msg)
{
    tr_error("Cellular stm failed with: %s", msg);

    if (_event_status_cb) {
        _event_status_cb(_current_event, (intptr_t )&_cb_data);
    }

    tr_error("Target state %s was not reached. Returning from state: %s", get_state_string(_target_state), get_state_string(_state));
    if (_blocking) {
        _stm_semaphore.release();
    }
}

const char *CellularStateMachine::get_state_string(CellularState state) const
{
#if MBED_CONF_MBED_TRACE_ENABLE
    static const char *strings[STATE_MAX_FSM_STATE] = { "Init", "Power", "Device ready", "SIM pin", "Registering network", "Manual registering", "Attaching network", "Activating PDP Context", "Connecting network", "Connected", "Disconnecting"};
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

void CellularStateMachine::continue_from_state(CellularState state)
{
    tr_info("Continue state from %s to %s", get_state_string((CellularStateMachine::CellularState)_state),
            get_state_string((CellularStateMachine::CellularState)state));
    _state = state;
    enter_to_state(state);
    _event_id = _queue.call_in(0, callback(this, &CellularStateMachine::event));
    if (!_event_id) {
        _cb_data.error = NSAPI_ERROR_NO_MEMORY;
        report_failure("Failed to call queue.");
        stop();
    }
}

nsapi_error_t CellularStateMachine::run_to_state(CellularStateMachine::CellularState state)
{
    // update next state so that we don't continue from previous state if state machine was paused and then started again.
    _state = _next_state;
    _target_state = state;
    enter_to_state(_next_state);

    _event_id = _queue.call_in(0, callback(this, &CellularStateMachine::event));

    if (!_event_id) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    if (_blocking) {
        // TODO, should we adjust semaphore wait time according to state we are trying to achieve?
        int ret_wait = _stm_semaphore.wait(10 * 60 * 1000); // cellular network searching may take several minutes
        if (ret_wait != 1) {
            tr_info("No cellular connection");
            return NSAPI_ERROR_NO_CONNECTION;
        }
    }

    return _cb_data.error;
}

void CellularStateMachine::enter_to_state(CellularState state)
{
    _next_state = state;
    _retry_count = 0;
    _command_success = false;
    _cb_data.error = NSAPI_ERROR_OK;
    _cb_data.status_data = -1;
}

void CellularStateMachine::retry_state_or_fail()
{
    if (++_retry_count < RETRY_ARRAY_SIZE) {
        tr_debug("Retry State %s, retry %d/%d", get_state_string(_state), _retry_count, RETRY_ARRAY_SIZE);
        _event_timeout = _retry_timeout_array[_retry_count];
    } else {
        report_failure(get_state_string(_state));
        return;
    }
}

void CellularStateMachine::state_init()
{
    // we should check that if power is already on then we can jump to device ready state
    _cellularDevice.set_timeout(TIMEOUT_POWER_ON);
    tr_info("Cellular state init (timeout %d ms)", TIMEOUT_POWER_ON);
    _cb_data.error = _power->is_device_ready();
    if (_cb_data.error != NSAPI_ERROR_OK) {
        _event_timeout = _start_time;
        tr_info("Init state, waiting %d ms before POWER state)", _start_time);
        enter_to_state(STATE_POWER_ON);
    } else {
        tr_info("Device was ready to accept commands, jump to device ready");
        enter_to_state(STATE_DEVICE_READY);
    }
}

void CellularStateMachine::state_power_on()
{
    _cellularDevice.set_timeout(TIMEOUT_POWER_ON);
    tr_info("Cellular power ON (timeout %d ms)", TIMEOUT_POWER_ON);
    if (power_on()) {
        enter_to_state(STATE_DEVICE_READY);
    } else {
        // retry to power on device
        retry_state_or_fail();
    }
}

void CellularStateMachine::device_ready()
{
    tr_info("Cellular device ready");
    if (_event_status_cb) {
        _event_status_cb((nsapi_event_t)CellularDeviceReady, (intptr_t )&_cb_data);
    }
    _power->remove_device_ready_urc_cb(mbed::callback(this, &CellularStateMachine::ready_urc_cb));
    _cellularDevice.close_power();
    _power = NULL;
}

void CellularStateMachine::state_device_ready()
{
    _cellularDevice.set_timeout(TIMEOUT_POWER_ON);
    _cb_data.error = _power->set_at_mode();
    if (_cb_data.error == NSAPI_ERROR_OK) {
        device_ready();
        enter_to_state(STATE_SIM_PIN);
    } else {
        if (_retry_count == 0) {
            (void)_power->set_device_ready_urc_cb(mbed::callback(this, &CellularStateMachine::ready_urc_cb));
        }
        retry_state_or_fail();
    }
}

void CellularStateMachine::state_sim_pin()
{
    _cellularDevice.set_timeout(TIMEOUT_SIM_PIN);
    tr_info("Sim state (timeout %d ms)", TIMEOUT_SIM_PIN);
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
    if (is_registered()) {
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
    tr_info("state_manual_registering_network");
    if (!_plmn_network_found) {
        if (is_registered() && is_registered_to_plmn()) {
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
    _cb_data.error = _network->set_attach();
    if (_cb_data.error == NSAPI_ERROR_OK) {
        _cellularDevice.close_sim();
        _sim = NULL;
        if (_event_status_cb) {
            _cb_data.status_data = CellularNetwork::Attached;
            _event_status_cb(_current_event, (intptr_t )&_cb_data);
        }
        enter_to_state(STATE_ACTIVATING_PDP_CONTEXT);
    } else {
        retry_state_or_fail();
    }
}

void CellularStateMachine::state_activating_pdp_context()
{
    _cellularDevice.set_timeout(TIMEOUT_CONNECT);
    tr_info("Activate PDP Context (timeout %d ms)", TIMEOUT_CONNECT);
    _cb_data.error = _network->activate_context();
    if (_cb_data.error == NSAPI_ERROR_OK) {
        if (_event_status_cb) {
            _event_status_cb(_current_event, (intptr_t )&_cb_data);
        }
        enter_to_state(STATE_CONNECTING_NETWORK);
    } else {
        retry_state_or_fail();
    }
}

void CellularStateMachine::state_connect_to_network()
{
    _cellularDevice.set_timeout(TIMEOUT_CONNECT);
    tr_info("Connect to cellular network (timeout %d ms)", TIMEOUT_CONNECT);
    _cb_data.error = _network->connect();
    if (_cb_data.error == NSAPI_ERROR_OK) {
        _cellularDevice.set_timeout(TIMEOUT_NETWORK);
        tr_debug("Connected to cellular network, set at timeout (timeout %d ms)", TIMEOUT_NETWORK);
        // when using modems stack connect is synchronous
        enter_to_state(STATE_CONNECTED);
    } else {
        retry_state_or_fail();
    }
}

void CellularStateMachine::event()
{
    _event_timeout = -1;
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
        case STATE_ACTIVATING_PDP_CONTEXT:
            _current_event = (nsapi_event_t)CellularActivatePDPContext;
            state_activating_pdp_context();
            break;
        case STATE_CONNECTING_NETWORK:
            _current_event = NSAPI_EVENT_CONNECTION_STATUS_CHANGE;
            state_connect_to_network();
            break;
        case STATE_CONNECTED:
            _current_event = NSAPI_EVENT_CONNECTION_STATUS_CHANGE;
            break;
        case STATE_DISCONNECTING:
        default:
            MBED_ASSERT(0);
            break;
    }

    if (_blocking && _target_state == _state && _cb_data.error == NSAPI_ERROR_OK) {
        tr_info("Target state reached: %s", get_state_string(_target_state));
        _stm_semaphore.release();
        return;
    }

    if (_next_state != _state || _event_timeout >= 0) {
        if (_next_state != _state) { // state exit condition
            tr_info("Cellular state from %s to %s", get_state_string((CellularStateMachine::CellularState)_state),
                    get_state_string((CellularStateMachine::CellularState)_next_state));
        } else {
            tr_info("Cellular event in %d seconds", _event_timeout);
        }
        _state = _next_state;
        if (_event_timeout == -1) {
            _event_timeout = 0;
        }
        _event_id = _queue.call_in(_event_timeout * 1000, callback(this, &CellularStateMachine::event));
        if (!_event_id) {
            _cb_data.error = NSAPI_ERROR_NO_MEMORY;
            report_failure("Cellular event failure!");
            return;
        }
    }
}

nsapi_error_t CellularStateMachine::start_dispatch()
{
    MBED_ASSERT(!_queue_thread);

    _queue_thread = new rtos::Thread(osPriorityNormal, 2048);
    if (!_queue_thread) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }
    if (_queue_thread->start(callback(&_queue, &events::EventQueue::dispatch_forever)) != osOK) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    return NSAPI_ERROR_OK;
}

void CellularStateMachine::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _event_status_cb = status_cb;
}

nsapi_error_t CellularStateMachine::disconnect()
{
    nsapi_error_t err = NSAPI_ERROR_OK;
    if (_network) {
        // set state to disconnecting
        _state = STATE_DISCONNECTING;
        err = _network->disconnect();
    }
    return err;
}

void CellularStateMachine::network_callback(nsapi_event_t ev, intptr_t ptr)
{
    cell_callback_data_t *data = (cell_callback_data_t*)ptr;
    if (ev >= NSAPI_EVENT_CELLULAR_STATUS_BASE && ev  <= NSAPI_EVENT_CELLULAR_STATUS_END) {
        tr_debug("FSM: network_callback called with event: %d, err: %d, data: %d _state: %s", ev, data->error, data->status_data, get_state_string(_state));
    } else {
        tr_debug("FSM: network_callback called with event: %d, ptr: %d _state: %s", ev, ptr, get_state_string(_state));
    }

    if ((cellular_connection_status_t)ev == CellularRegistrationStatusChanged &&
            (_state == STATE_REGISTERING_NETWORK || _state == STATE_MANUAL_REGISTERING_NETWORK)) {
        // expect packet data so only these states are valid
        if (data->status_data == CellularNetwork::RegisteredHomeNetwork || data->status_data == CellularNetwork::RegisteredRoaming) {
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

    if (_event_status_cb) {
        _event_status_cb(ev, ptr);
    }

    // try to reconnect if we think that we are connected, automatic reconnection is on and we get event disconnected
    if (_automatic_reconnect && ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE && ptr == NSAPI_STATUS_DISCONNECTED &&
            _state == STATE_CONNECTED) {

        tr_info("FSM: start automatic reconnect!");
        // call disconnect to set filehandle irq back to us, don't really care about return value.
        (void)_network->disconnect();

         // start from registering phase as we might have been deregistered if there is no network
        if (_plmn) {
            continue_from_state(STATE_MANUAL_REGISTERING_NETWORK);
        } else {
            continue_from_state(STATE_REGISTERING_NETWORK);
        }

        if (_event_status_cb) {
            _event_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_RECONNECTING);
        }
    }
}

void CellularStateMachine::ready_urc_cb()
{
    tr_debug("Device ready URC func called");
    if (_state == STATE_DEVICE_READY && _power->set_at_mode() == NSAPI_ERROR_OK) {
        tr_debug("State was STATE_DEVICE_READY and at mode ready, cancel state and move to next");
        _queue.cancel(_event_id);
        device_ready();
        continue_from_state(STATE_SIM_PIN);
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

