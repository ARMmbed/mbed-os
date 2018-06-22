/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "CellularConnectionFSM.h"

#ifdef CELLULAR_DEVICE

#ifndef MBED_TRACE_MAX_LEVEL
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_INFO
#endif
#include "CellularLog.h"
#include "CellularCommon.h"

// timeout to wait for AT responses
#define TIMEOUT_POWER_ON     (1*1000)
#define TIMEOUT_SIM_PIN      (1*1000)
#define TIMEOUT_NETWORK      (10*1000)
#define TIMEOUT_CONNECT      (60*1000)
#define TIMEOUT_REGISTRATION (180*1000)

// maximum time when retrying network register, attach and connect in seconds ( 20minutes )
#define TIMEOUT_NETWORK_MAX (20*60)

#define RETRY_COUNT_DEFAULT 3

namespace mbed
{

CellularConnectionFSM::CellularConnectionFSM() :
        _serial(0), _state(STATE_INIT), _next_state(_state), _status_callback(0), _event_status_cb(0), _network(0), _power(0), _sim(0),
        _queue(8 * EVENTS_EVENT_SIZE), _queue_thread(0), _cellularDevice(0), _retry_count(0), _event_timeout(-1),
        _at_queue(8 * EVENTS_EVENT_SIZE), _event_id(0), _plmn(0), _command_success(false), _plmn_network_found(false)
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
    _retry_array_length = MAX_RETRY_ARRAY_SIZE;
}

CellularConnectionFSM::~CellularConnectionFSM()
{
    stop();
}

void CellularConnectionFSM::stop()
{
    _queue.cancel(_event_id);
    _queue.break_dispatch();

    if (_queue_thread) {
        _queue_thread->terminate();
        delete _queue_thread;
        _queue_thread = NULL;
    }

    delete _cellularDevice;
    _cellularDevice = NULL;
    // _cellularDevice closes all interfaces in destructor
    _power = NULL;
    _network = NULL;
    _sim = NULL;

    _state = STATE_INIT;
    _next_state = _state;
}

nsapi_error_t CellularConnectionFSM::init()
{
    tr_info("CELLULAR_DEVICE: %s", CELLULAR_STRINGIFY(CELLULAR_DEVICE));
    _cellularDevice = new CELLULAR_DEVICE(_at_queue);
    if (!_cellularDevice) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    _power = _cellularDevice->open_power(_serial);
    if (!_power) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    _network = _cellularDevice->open_network(_serial);
    if (!_network) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    _sim = _cellularDevice->open_sim(_serial);
    if (!_sim) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    _at_queue.chain(&_queue);

    _retry_count = 0;
    _state = STATE_INIT;
    _next_state = STATE_INIT;

    return _network->init();
}

bool CellularConnectionFSM::power_on()
{
    nsapi_error_t err = _power->on();
    if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
        tr_warn("Cellular start failed. Power off/on.");
        err = _power->off();
        if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
            tr_error("Cellular power down failing after failed power up attempt!");
        }
        return false;
    }
    return true;
}

void CellularConnectionFSM::set_sim_pin(const char *sim_pin)
{
    strncpy(_sim_pin, sim_pin, sizeof(_sim_pin));
    _sim_pin[sizeof(_sim_pin)-1] = '\0';
}

void CellularConnectionFSM::set_plmn(const char *plmn)
{
    _plmn = plmn;
}

bool CellularConnectionFSM::open_sim()
{
    CellularSIM::SimState state = CellularSIM::SimStateUnknown;
    // wait until SIM is readable
    // here you could add wait(secs) if you know start delay of your SIM
    if (_sim->get_sim_state(state) != NSAPI_ERROR_OK) {
        tr_info("Waiting for SIM (err while reading)...");
        return false;
    }

    if (state == CellularSIM::SimStatePinNeeded) {
        if (strlen(_sim_pin)) {
            tr_info("SIM pin required, entering pin: %s", _sim_pin);
            nsapi_error_t err = _sim->set_pin(_sim_pin);
            if (err) {
                tr_error("SIM pin set failed with: %d, bailing out...", err);
            }
        } else {
            tr_warn("PIN required but No SIM pin provided.");
        }
    }

    if (_event_status_cb) {
        _event_status_cb((nsapi_event_t)CellularSIMStatusChanged, state);
    }

    return state == CellularSIM::SimStateReady;
}

bool CellularConnectionFSM::is_registered()
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

bool CellularConnectionFSM::get_network_registration(CellularNetwork::RegistrationType type,
        CellularNetwork::RegistrationStatus &status, bool &is_registered)
{
    is_registered = false;
    bool is_roaming = false;
    nsapi_error_t err = _network->get_registration_status(type, status);
    if (err != NSAPI_ERROR_OK) {
        if (err != NSAPI_ERROR_UNSUPPORTED) {
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

void CellularConnectionFSM::report_failure(const char *msg)
{
    tr_error("Cellular network failed: %s", msg);
    if (_status_callback) {
        _status_callback(_state, _next_state);
    }
}

const char *CellularConnectionFSM::get_state_string(CellularState state)
{
#if MBED_CONF_MBED_TRACE_ENABLE
    static const char *strings[] = { "Init", "Power", "Device ready", "SIM pin", "Registering network", "Manual registering", "Attaching network", "Activating PDP Context", "Connecting network", "Connected"};
    return strings[state];
#else
    return NULL;
#endif // #if MBED_CONF_MBED_TRACE_ENABLE
}

bool CellularConnectionFSM::is_registered_to_plmn()
{
    int format;
    CellularNetwork::operator_t op;

    nsapi_error_t err = _network->get_operator_params(format, op);
    if (err == NSAPI_ERROR_OK) {
        if (format == 2) {
            // great, numeric format we can do comparison for that
            if (strcmp(op.op_num, _plmn) == 0) {
                return true;
            }
            return false;
        }

        // format was alpha, get operator names to do the comparing
        CellularNetwork::operator_names_list names_list;
        nsapi_error_t err = _network->get_operator_names(names_list);
        if (err == NSAPI_ERROR_OK) {
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

nsapi_error_t CellularConnectionFSM::continue_from_state(CellularState state)
{
    tr_info("Continue state from %s to %s", get_state_string((CellularConnectionFSM::CellularState)_state),
            get_state_string((CellularConnectionFSM::CellularState)state));
    _state = state;
    _next_state = state;
    _retry_count = 0;
    if (!_queue.call_in(0, callback(this, &CellularConnectionFSM::event))) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t CellularConnectionFSM::continue_to_state(CellularState state)
{
    MBED_ASSERT(_cellularDevice);
    _retry_count = 0;
    if (state < _state) {
        _state = state;
    } else {
        // update next state so that we don't continue from previous state
        _state = _next_state;
    }
    if (!_queue.call_in(0, callback(this, &CellularConnectionFSM::event))) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    return NSAPI_ERROR_OK;
}

void CellularConnectionFSM::enter_to_state(CellularState state)
{
    _next_state = state;
    _retry_count = 0;
    _command_success = false;
}

void CellularConnectionFSM::retry_state_or_fail()
{
    if (++_retry_count < MAX_RETRY_ARRAY_SIZE) {
        tr_debug("Retry State %s, retry %d/%d", get_state_string(_state), _retry_count, MAX_RETRY_ARRAY_SIZE);
        _event_timeout = _retry_timeout_array[_retry_count];
    } else {
        report_failure(get_state_string(_state));
        return;
    }
}

void CellularConnectionFSM::state_init()
{
    // we should check that if power is already on then we can jump to device ready state
    _cellularDevice->set_timeout(TIMEOUT_POWER_ON);
    tr_info("Cellular state init (timeout %d ms)", TIMEOUT_POWER_ON);
    nsapi_error_t err = _power->is_device_ready();
    if (err != NSAPI_ERROR_OK) {
        _event_timeout = _start_time;
        tr_info("Init state, waiting %d ms before POWER state)", _start_time);
        enter_to_state(STATE_POWER_ON);
    } else {
        tr_info("Device was ready to accept commands, jump to device ready");
        enter_to_state(STATE_DEVICE_READY);
    }
}

void CellularConnectionFSM::state_power_on()
{
    _cellularDevice->set_timeout(TIMEOUT_POWER_ON);
    tr_info("Cellular power ON (timeout %d ms)", TIMEOUT_POWER_ON);
    if (power_on()) {
        enter_to_state(STATE_DEVICE_READY);
    } else {
        // retry to power on device
        retry_state_or_fail();
    }
}

void CellularConnectionFSM::device_ready()
{
    tr_info("Cellular device ready");
    if (_event_status_cb) {
        _event_status_cb((nsapi_event_t)CellularDeviceReady, 0);
    }
    _power->remove_device_ready_urc_cb(mbed::callback(this, &CellularConnectionFSM::ready_urc_cb));
    _cellularDevice->close_power();
    _power = NULL;
}

void CellularConnectionFSM::state_device_ready()
{
    _cellularDevice->set_timeout(TIMEOUT_POWER_ON);
    if (_power->set_at_mode() == NSAPI_ERROR_OK) {
        device_ready();
        enter_to_state(STATE_SIM_PIN);
    } else {
        if (_retry_count == 0) {
            (void)_power->set_device_ready_urc_cb(mbed::callback(this, &CellularConnectionFSM::ready_urc_cb));
        }
        retry_state_or_fail();
    }
}

void CellularConnectionFSM::state_sim_pin()
{
    _cellularDevice->set_timeout(TIMEOUT_SIM_PIN);
    tr_info("Sim state (timeout %d ms)", TIMEOUT_SIM_PIN);
    if (open_sim()) {
        bool success = false;
        for (int type = 0; type < CellularNetwork::C_MAX; type++) {
            if (!_network->set_registration_urc((CellularNetwork::RegistrationType)type, true)) {
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

void CellularConnectionFSM::state_registering()
{
    _cellularDevice->set_timeout(TIMEOUT_NETWORK);
    if (is_registered()) {
        // we are already registered, go to attach
        enter_to_state(STATE_ATTACHING_NETWORK);
    } else {
        _cellularDevice->set_timeout(TIMEOUT_REGISTRATION);
        if (!_command_success) {
            _command_success = (_network->set_registration() == NSAPI_ERROR_OK);
        }
        retry_state_or_fail();
    }
}

// only used when _plmn is set
void CellularConnectionFSM::state_manual_registering_network()
{
    _cellularDevice->set_timeout(TIMEOUT_REGISTRATION);
    tr_info("state_manual_registering_network");
    if (!_plmn_network_found) {
        if (is_registered() && is_registered_to_plmn()) {
            _plmn_network_found = true;
            enter_to_state(STATE_ATTACHING_NETWORK);
        } else {
            if (!_command_success) {
                _command_success = (_network->set_registration(_plmn) == NSAPI_ERROR_OK);
            }
            retry_state_or_fail();
        }
    }
}

void CellularConnectionFSM::state_attaching()
{
    _cellularDevice->set_timeout(TIMEOUT_CONNECT);
    if (_network->set_attach() == NSAPI_ERROR_OK) {
        _cellularDevice->close_sim();
        _sim = NULL;
        enter_to_state(STATE_ACTIVATING_PDP_CONTEXT);
    } else {
        retry_state_or_fail();
    }
}

void CellularConnectionFSM::state_activating_pdp_context()
{
    _cellularDevice->set_timeout(TIMEOUT_CONNECT);
    tr_info("Activate PDP Context (timeout %d ms)", TIMEOUT_CONNECT);
    if (_network->activate_context() == NSAPI_ERROR_OK) {
        // when using modems stack connect is synchronous
        _next_state = STATE_CONNECTING_NETWORK;
    } else {
        retry_state_or_fail();
    }
}

void CellularConnectionFSM::state_connect_to_network()
{
    _cellularDevice->set_timeout(TIMEOUT_CONNECT);
    tr_info("Connect to cellular network (timeout %d ms)", TIMEOUT_CONNECT);
    if (_network->connect() == NSAPI_ERROR_OK) {
        _cellularDevice->set_timeout(TIMEOUT_NETWORK);
        tr_debug("Connected to cellular network, set at timeout (timeout %d ms)", TIMEOUT_NETWORK);
        // when using modems stack connect is synchronous
        _next_state = STATE_CONNECTED;
    } else {
        retry_state_or_fail();
    }
}

void CellularConnectionFSM::state_connected()
{
    _cellularDevice->set_timeout(TIMEOUT_NETWORK);
    tr_debug("Cellular ready! (timeout %d ms)", TIMEOUT_NETWORK);
    if (_status_callback) {
        _status_callback(_state, _next_state);
    }
}

void CellularConnectionFSM::event()
{
    _event_timeout = -1;
    switch (_state) {
        case STATE_INIT:
            state_init();
            break;
        case STATE_POWER_ON:
            state_power_on();
            break;
        case STATE_DEVICE_READY:
            state_device_ready();
            break;
        case STATE_SIM_PIN:
            state_sim_pin();
            break;
        case STATE_REGISTERING_NETWORK:
            state_registering();
            break;
        case STATE_MANUAL_REGISTERING_NETWORK:
            state_manual_registering_network();
            break;
        case STATE_ATTACHING_NETWORK:
            state_attaching();
            break;
        case STATE_ACTIVATING_PDP_CONTEXT:
            state_activating_pdp_context();
            break;
        case STATE_CONNECTING_NETWORK:
            state_connect_to_network();
            break;
        case STATE_CONNECTED:
            state_connected();
            break;
        default:
            MBED_ASSERT(0);
            break;
    }

    if (_next_state != _state || _event_timeout >= 0) {
        if (_next_state != _state) { // state exit condition
            tr_info("Cellular state from %s to %s", get_state_string((CellularConnectionFSM::CellularState)_state),
                    get_state_string((CellularConnectionFSM::CellularState)_next_state));
            if (_status_callback) {
                if (!_status_callback(_state, _next_state)) {
                    return;
                }
            }
        } else {
            tr_info("Cellular event in %d seconds", _event_timeout);
        }
        _state = _next_state;
        if (_event_timeout == -1) {
            _event_timeout = 0;
        }
        _event_id = _queue.call_in(_event_timeout*1000, callback(this, &CellularConnectionFSM::event));
        if (!_event_id) {
            report_failure("Cellular event failure!");
            return;
        }
    }
}

nsapi_error_t CellularConnectionFSM::start_dispatch()
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

void CellularConnectionFSM::set_serial(UARTSerial *serial)
{
    _serial = serial;
}

void CellularConnectionFSM::set_callback(mbed::Callback<bool(int, int)> status_callback)
{
    _status_callback = status_callback;
}

void CellularConnectionFSM::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    MBED_ASSERT(_network);
    _event_status_cb = status_cb;
    if (status_cb) {
        _network->attach(callback(this, &CellularConnectionFSM::network_callback));
    } else {
        _network->attach(NULL);
    }
}

void CellularConnectionFSM::network_callback(nsapi_event_t ev, intptr_t ptr)
{
    tr_info("FSM: network_callback called with event: %d, intptr: %d, _state: %s", ev, ptr, get_state_string(_state));
    if ((cellular_connection_status_t)ev == CellularRegistrationStatusChanged &&
            (_state == STATE_REGISTERING_NETWORK || _state == STATE_MANUAL_REGISTERING_NETWORK)) {
        // expect packet data so only these states are valid
        if (ptr == CellularNetwork::RegisteredHomeNetwork || ptr == CellularNetwork::RegisteredRoaming) {
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
}

void CellularConnectionFSM::ready_urc_cb()
{
    tr_debug("Device ready URC func called");
    if (_state == STATE_DEVICE_READY && _power->set_at_mode() == NSAPI_ERROR_OK) {
        tr_debug("State was STATE_DEVICE_READY and at mode ready, cancel state and move to next");
        _queue.cancel(_event_id);
        device_ready();
        continue_from_state(STATE_SIM_PIN);
    }
}

events::EventQueue *CellularConnectionFSM::get_queue()
{
    return &_queue;
}

CellularNetwork *CellularConnectionFSM::get_network()
{
    return _network;
}

CellularDevice *CellularConnectionFSM::get_device()
{
    return _cellularDevice;
}

CellularSIM *CellularConnectionFSM::get_sim()
{
    return _sim;
}

NetworkStack *CellularConnectionFSM::get_stack()
{
    return _cellularDevice->get_stack();
}

void CellularConnectionFSM::set_retry_timeout_array(uint16_t timeout[], int array_len)
{
    _retry_array_length = array_len > MAX_RETRY_ARRAY_SIZE ? MAX_RETRY_ARRAY_SIZE : array_len;

    for (int i = 0; i < _retry_array_length; i++) {
        _retry_timeout_array[i] = timeout[i];
    }
}

} // namespace

#endif // CELLULAR_DEVICE
