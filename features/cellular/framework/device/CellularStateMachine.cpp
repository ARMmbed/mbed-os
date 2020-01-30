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
#include "CellularInformation.h"

#ifndef MBED_TRACE_MAX_LEVEL
#define MBED_TRACE_MAX_LEVEL TRACE_LEVEL_INFO
#endif

// timeout to wait for AT responses
#define TIMEOUT_POWER_ON     (1*1000)
#define TIMEOUT_SIM_PIN      (1*1000)
#define TIMEOUT_NETWORK      (10*1000)
/** CellularStateMachine does connecting up to packet service attach, and
 *  after that it's up to CellularContext::connect() to connect to PDN.
 *  If CellularContext or an application does not set timeout (via `CellularDevice::set_timeout`)
 *  then TIMEOUT_CONNECT is used also for connecting to PDN and also for socket operations.
 */
#define TIMEOUT_CONNECT      (60*1000)
#define TIMEOUT_REGISTRATION (180*1000)

// maximum time when retrying network register, attach and connect in seconds ( 20minutes )
#define TIMEOUT_NETWORK_MAX (20*60)

#define RETRY_COUNT_DEFAULT 3


const int STM_STOPPED = -99;
const int ACTIVE_PDP_CONTEXT = 0x01;
const int ATTACHED_TO_NETWORK = 0x02;
const int DEVICE_READY = 0x04;

namespace mbed {

CellularStateMachine::CellularStateMachine(CellularDevice &device, events::EventQueue &queue, CellularNetwork &nw) :
    _cellularDevice(device), _state(STATE_INIT), _next_state(_state), _target_state(_state),
    _event_status_cb(), _network(nw), _queue(queue), _sim_pin(0), _retry_count(0),
    _event_timeout(-1), _event_id(-1), _plmn(0), _command_success(false),
    _is_retry(false), _cb_data(), _current_event(CellularDeviceReady), _status(0)
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
    _retry_array_length = CELLULAR_RETRY_ARRAY_SIZE;

    _state_timeout_power_on = TIMEOUT_POWER_ON;
    _state_timeout_sim_pin = TIMEOUT_SIM_PIN;
    _state_timeout_registration = TIMEOUT_REGISTRATION;
    _state_timeout_network = TIMEOUT_NETWORK;
    _state_timeout_connect = TIMEOUT_CONNECT;
}

CellularStateMachine::~CellularStateMachine()
{
    tr_debug("CellularStateMachine destruct");
    stop();
}

void CellularStateMachine::reset()
{
    _state = STATE_INIT;
    _event_timeout = -1;
    _event_id = -1;
    _is_retry = false;
    _status = 0;
    _target_state = STATE_INIT;
    enter_to_state(STATE_INIT);
}

void CellularStateMachine::stop()
{
    tr_debug("CellularStateMachine stop");
    reset();
    _event_id = STM_STOPPED;
}

bool CellularStateMachine::power_on()
{
    _cb_data.error = _cellularDevice.hard_power_on();
    if (_cb_data.error != NSAPI_ERROR_OK) {
        tr_warn("Hard power on failed.");
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
    CellularDevice::SimState state = CellularDevice::SimStateUnknown;
    // wait until SIM is readable
    // here you could add wait(secs) if you know start delay of your SIM
    _cb_data.error = _cellularDevice.get_sim_state(state);
    if (_cb_data.error != NSAPI_ERROR_OK) {
        tr_info("Waiting for SIM (err while reading)...");
        return false;
    }

    // report current state so callback can set sim pin if needed
    _cb_data.status_data = state;
    send_event_cb(CellularSIMStatusChanged);

    if (state == CellularDevice::SimStatePinNeeded) {
        if (_sim_pin) {
            tr_info("Entering PIN to open SIM");
            _cb_data.error = _cellularDevice.set_pin(_sim_pin);
            if (_cb_data.error) {
                tr_error("Failed to set PIN: error %d", _cb_data.error);
            }
        } else {
            // No sim pin provided even it's needed, stop state machine
            tr_error("PIN required but no SIM pin provided.");
            _retry_count = CELLULAR_RETRY_ARRAY_SIZE;
            return false;
        }
    }

    bool sim_ready = state == CellularDevice::SimStateReady;

    if (sim_ready) {
#ifdef MBED_CONF_CELLULAR_CLEAR_ON_CONNECT
        if (_cellularDevice.clear() != NSAPI_ERROR_OK) {
            tr_warning("CellularDevice clear failed");
            return false;
        }
#endif
        _cb_data.error = _network.set_registration(_plmn);
        tr_debug("STM: set_registration: %d, plmn: %s", _cb_data.error, _plmn ? _plmn : "NULL");
        if (_cb_data.error) {
            return false;
        }
    }

    return sim_ready;
}

bool CellularStateMachine::is_registered()
{
    CellularNetwork::RegistrationStatus status;
    bool is_registered = false;

    // accept only CGREG/CEREG. CREG is for circuit switch network changed. If we accept CREG attach will fail if also
    // CGREG/CEREG is not registered.
    for (int type = 0; type < CellularNetwork::C_REG; type++) {
        if (get_network_registration((CellularNetwork::RegistrationType) type, status, is_registered)) {
            if (is_registered) {
                break;
            }
        }
    }

    _cb_data.status_data = status;
    // in manual registering we are forcing registration to certain network so we don't accept active context or attached
    // as indication that device is registered to correct network.
    if (_plmn && strlen(_plmn)) {
        return is_registered;
    }
    return is_registered || _status;
}

bool CellularStateMachine::get_network_registration(CellularNetwork::RegistrationType type,
                                                    CellularNetwork::RegistrationStatus &status, bool &is_registered)
{
    is_registered = false;
    bool is_roaming = false;
    CellularNetwork::registration_params_t reg_params;
    _cb_data.error = _network.get_registration_params(type, reg_params);

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

    if (is_registered || is_roaming) {
        tr_info("Roaming %u Registered %u", is_roaming, is_registered);
    }

    return true;
}

void CellularStateMachine::report_failure(const char *msg)
{
    tr_error("CellularStateMachine failure: %s", msg);

    _event_id = -1;
    _cb_data.final_try = true;
    send_event_cb(_current_event);

    tr_error("CellularStateMachine target state %s, current state %s", get_state_string(_target_state), get_state_string(_state));
}

const char *CellularStateMachine::get_state_string(CellularState state) const
{
#if MBED_CONF_MBED_TRACE_ENABLE
    static const char *strings[STATE_MAX_FSM_STATE] = { "Init", "Power", "Device ready", "SIM pin", "Signal quality", "Registering network", "Attaching network"};
    return strings[state];
#else
    return NULL;
#endif // #if MBED_CONF_MBED_TRACE_ENABLE
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
    if (_retry_count < _retry_array_length) {
        tr_debug("%s: retry %d/%d", get_state_string(_state), _retry_count, _retry_array_length);
        // send info to application/driver about error logic so it can implement proper error logic
        _cb_data.status_data = _current_event;
        _cb_data.data = &_retry_count;
        _cb_data.error = NSAPI_ERROR_OK;
        send_event_cb(CellularStateRetryEvent);

        _event_timeout = _retry_timeout_array[_retry_count];
        _is_retry = true;
        _cb_data.error = NSAPI_ERROR_OK;
        _retry_count++;
    } else {
        _cb_data.final_try = true;
        report_failure(get_state_string(_state));
    }
}

void CellularStateMachine::state_init()
{
    change_timeout(_state_timeout_power_on);
    tr_info("Start connecting (timeout %d ms)", _state_timeout_power_on);
    _cb_data.error = _cellularDevice.is_ready();
    _status = _cb_data.error ? 0 : DEVICE_READY;
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
    change_timeout(_state_timeout_power_on);
    tr_info("Modem power ON (timeout %d ms)", _state_timeout_power_on);
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

#ifdef MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY
    MBED_ASSERT(MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY >= CellularNetwork::RAT_GSM &&
                MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY < CellularNetwork::RAT_UNKNOWN);
    nsapi_error_t err = _network.set_access_technology((CellularNetwork::RadioAccessTechnology)MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY);
    if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
        tr_warning("Failed to set access technology to %d", MBED_CONF_CELLULAR_RADIO_ACCESS_TECHNOLOGY);
        return false;
    }
#endif // MBED_CONF_CELLULAR_DEBUG_AT

    send_event_cb(CellularDeviceReady);
    _cellularDevice.set_ready_cb(nullptr);

    return true;
}

void CellularStateMachine::state_device_ready()
{
    change_timeout(_state_timeout_power_on);
    if (!(_status & DEVICE_READY)) {
        tr_debug("Device was not ready, calling soft_power_on()");
        _cb_data.error = _cellularDevice.soft_power_on();
    }
    if (_cb_data.error == NSAPI_ERROR_OK) {
        _cb_data.error = _cellularDevice.init();
        if (_cb_data.error == NSAPI_ERROR_OK) {

#if MBED_CONF_MBED_TRACE_ENABLE
            CellularInformation *info = _cellularDevice.open_information();

            char *buf = new (std::nothrow) char[2048]; // size from 3GPP TS 27.007
            if (buf) {
                if (info->get_manufacturer(buf, 2048) == NSAPI_ERROR_OK) {
                    tr_info("Modem manufacturer: %s", buf);
                }
                if (info->get_model(buf, 2048) == NSAPI_ERROR_OK) {
                    tr_info("Modem model: %s", buf);
                }
                if (info->get_revision(buf, 2048) == NSAPI_ERROR_OK) {
                    tr_info("Modem revision: %s", buf);
                }
                delete[] buf;
            }
            _cellularDevice.close_information();
#endif // MBED_CONF_MBED_TRACE_ENABLE

            if (device_ready()) {
                _status = 0;
                enter_to_state(STATE_SIM_PIN);
            } else {
                tr_warning("Power cycle CellularDevice and restart connecting");
                (void) _cellularDevice.soft_power_off();
                (void) _cellularDevice.hard_power_off();
                _status = 0;
                _is_retry = true;
                enter_to_state(STATE_INIT);
            }
        }
    }

    if (_cb_data.error != NSAPI_ERROR_OK) {
        if (_retry_count == 0) {
            _cellularDevice.set_ready_cb(callback(this, &CellularStateMachine::device_ready_cb));
        }
        retry_state_or_fail();
    }
}

void CellularStateMachine::state_sim_pin()
{
    change_timeout(_state_timeout_sim_pin);
    tr_info("Setup SIM (timeout %d ms)", _state_timeout_sim_pin);
    if (open_sim()) {
        bool success = false;
        for (int type = 0; type < CellularNetwork::C_MAX; type++) {
            _cb_data.error = _network.set_registration_urc((CellularNetwork::RegistrationType)type, true);
            if (!_cb_data.error && (type == CellularNetwork::C_EREG || type == CellularNetwork::C_GREG)) {
                success = true;
            }
        }
        if (!success) {
            tr_error("Failed to set CEREG/CGREG URC's for registration");
            retry_state_or_fail();
            return;
        }
        if (_network.is_active_context()) { // check if context was already activated
            tr_debug("Active context found.");
            _status |= ACTIVE_PDP_CONTEXT;
        }
        CellularNetwork::AttachStatus status = CellularNetwork::Detached; // check if modem is already attached to a network
        if (_network.get_attach(status) == NSAPI_ERROR_OK && status == CellularNetwork::Attached) {
            _status |= ATTACHED_TO_NETWORK;
            tr_debug("Cellular already attached.");
        }

        // if packet domain event reporting is not set it's not a stopper. We might lack some events when we are
        // dropped from the network.
        _cb_data.error = _network.set_packet_domain_event_reporting(true);
        if (_cb_data.error == NSAPI_STATUS_ERROR_UNSUPPORTED) {
            tr_warning("Packet domain event reporting not supported!");
        } else if (_cb_data.error) {
            tr_warning("Packet domain event reporting set failed!");
        }
        enter_to_state(STATE_SIGNAL_QUALITY);
    } else {
        retry_state_or_fail();
    }
}

void CellularStateMachine::state_signal_quality()
{
    _cb_data.error = _network.get_signal_quality(_signal_quality.rssi, &_signal_quality.ber);

    if (_cb_data.error != NSAPI_ERROR_OK) {
        retry_state_or_fail();
    } else {
        _cb_data.data = &_signal_quality;
        send_event_cb(_current_event);
        enter_to_state(STATE_REGISTERING_NETWORK);
    }
}

void CellularStateMachine::state_registering()
{
    change_timeout(_state_timeout_network);
    if (is_registered()) {
        if (_cb_data.status_data != CellularNetwork::RegisteredHomeNetwork &&
                _cb_data.status_data != CellularNetwork::RegisteredRoaming && _status) {
            // there was already activated context or attached to network, and registration status is not registered, set to already registered.
            _cb_data.status_data = CellularNetwork::AlreadyRegistered;
        }
        _cb_data.error = NSAPI_ERROR_OK;
        send_event_cb(_current_event);
        // we are already registered, go to attach
        enter_to_state(STATE_ATTACHING_NETWORK);
    } else {
        tr_info("Network registration (timeout %d ms)", _state_timeout_registration);
        change_timeout(_state_timeout_registration);
        if (!_command_success && !_plmn) { // don't call set_registration twice for manual registration
            _cb_data.error = _network.set_registration(_plmn);
            _command_success = (_cb_data.error == NSAPI_ERROR_OK);
        }
        retry_state_or_fail();
    }
}

void CellularStateMachine::state_attaching()
{
    if (_status != ATTACHED_TO_NETWORK) {
        change_timeout(_state_timeout_connect);
        tr_info("Attaching network (timeout %d ms)", _state_timeout_connect);
        _cb_data.error = _network.set_attach();
    }
    if (_cb_data.error == NSAPI_ERROR_OK) {
        _cb_data.status_data = CellularNetwork::Attached;
        send_event_cb(_current_event);
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
        report_failure("Failed to call queue.");
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
    if (_event_id == -1 || _event_id == STM_STOPPED) {
        is_running = false;
    } else {
        is_running = true;
    }
    _mutex.unlock();
    return is_running;
}

void CellularStateMachine::event()
{
    // Don't send Signal quality when in signal quality state or it can confuse callback functions when running retry logic
    if (_state > STATE_SIGNAL_QUALITY) {
        _cb_data.error = _network.get_signal_quality(_signal_quality.rssi, &_signal_quality.ber);
        _cb_data.data = &_signal_quality;

        if (_cb_data.error == NSAPI_ERROR_OK) {
            send_event_cb(CellularSignalQuality);
            if (_signal_quality.rssi == CellularNetwork::SignalQualityUnknown) {
                tr_info("RSSI unknown");
            } else {
                tr_info("RSSI %d dBm", _signal_quality.rssi);
            }
        }
    }

    _event_timeout = -1;
    _is_retry = false;

    switch (_state) {
        case STATE_INIT:
            _current_event = CellularDeviceReady;
            state_init();
            break;
        case STATE_POWER_ON:
            _current_event = CellularDeviceReady;
            state_power_on();
            break;
        case STATE_DEVICE_READY:
            _current_event = CellularDeviceReady;
            state_device_ready();
            break;
        case STATE_SIM_PIN:
            _current_event = CellularSIMStatusChanged;
            state_sim_pin();
            break;
        case STATE_SIGNAL_QUALITY:
            _current_event = CellularSignalQuality;
            state_signal_quality();
            break;
        case STATE_REGISTERING_NETWORK:
            _current_event = CellularRegistrationStatusChanged;
            state_registering();
            break;
        case STATE_ATTACHING_NETWORK:
            _current_event = CellularAttachNetwork;
            state_attaching();
            break;
        default:
            MBED_ASSERT(0);
            break;
    }

    if (check_is_target_reached()) {
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
    if (_event_id != -1) {
        tr_warn("Canceling ongoing event (%d)", _event_id);
        _queue.cancel(_event_id);
    }
    _event_id = -1;
    return NSAPI_ERROR_OK;
}

void CellularStateMachine::set_cellular_callback(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _event_status_cb = status_cb;
}

void CellularStateMachine::send_event_cb(cellular_connection_status_t status)
{
    if (_event_status_cb) {
        _event_status_cb((nsapi_event_t)status, (intptr_t)&_cb_data);
    }
}

void CellularStateMachine::change_timeout(const int &timeout)
{
    _cb_data.status_data = _current_event;
    _cb_data.data = &timeout;
    _cb_data.error = NSAPI_ERROR_OK;
    // event callback is a preferred method to communicate to CellularDevice,
    // for example calling CellularDevice::set_timeout would call back to this class
    send_event_cb(CellularDeviceTimeout);
}

bool CellularStateMachine::check_is_target_reached()
{
    if (((_target_state == _state || _target_state < _next_state) && _cb_data.error == NSAPI_ERROR_OK && !_is_retry) ||
            _event_id == STM_STOPPED) {
        if (_target_state != _state && _target_state < _next_state) {
            // we are skipping the state, update _state to current state because we have reached it
            _state = _target_state;
        }
        _event_id = -1;
        return true;
    }
    return false;
}

void CellularStateMachine::cellular_event_changed(nsapi_event_t ev, intptr_t ptr)
{
    cell_callback_data_t *data = (cell_callback_data_t *)ptr;
    if ((cellular_connection_status_t)ev == CellularRegistrationStatusChanged && (
                _state == STATE_REGISTERING_NETWORK || _state == STATE_SIGNAL_QUALITY)) {
        // expect packet data so only these states are valid
        CellularNetwork::registration_params_t reg_params;
        nsapi_error_t err = _network.get_registration_params(reg_params);

        if (err == NSAPI_ERROR_OK && (reg_params._type == CellularNetwork::C_EREG || reg_params._type == CellularNetwork::C_GREG)) {
            if ((data->status_data == CellularNetwork::RegisteredHomeNetwork ||
                    data->status_data == CellularNetwork::RegisteredRoaming) && data->error == NSAPI_ERROR_OK) {
                _queue.cancel(_event_id);
                _is_retry = false;
                _event_id = -1;
                if (!check_is_target_reached()) {
                    continue_from_state(STATE_ATTACHING_NETWORK);
                }
            }
        } else {
            tr_debug("creg event, discard...");
        }
    }
}

void CellularStateMachine::device_ready_cb()
{
    tr_debug("Device ready callback");
    if (_state == STATE_DEVICE_READY && _cellularDevice.init() == NSAPI_ERROR_OK) {
        tr_debug("State was STATE_DEVICE_READY and at mode ready, cancel state and move to next");
        _queue.cancel(_event_id);
        _event_id = -1;
        if (device_ready()) {
            _is_retry = false;
            _status = 0;
            if (!check_is_target_reached()) {
                continue_from_state(STATE_SIM_PIN);
            }
        } else {
            continue_from_state(STATE_DEVICE_READY);
        }
    }
}

void CellularStateMachine::set_retry_timeout_array(const uint16_t timeout[], int array_len)
{
    if (!timeout || array_len <= 0) {
        _retry_array_length = 0;
        return;
    }
    _retry_array_length = array_len > CELLULAR_RETRY_ARRAY_SIZE ? CELLULAR_RETRY_ARRAY_SIZE : array_len;
    for (int i = 0; i < _retry_array_length; i++) {
        _retry_timeout_array[i] = timeout[i];
    }
}

void CellularStateMachine::get_retry_timeout_array(uint16_t *timeout, int &array_len) const
{
    for (int i = 0; i < _retry_array_length; i++) {
        timeout[i] = _retry_timeout_array[i];
    }
    array_len = _retry_array_length;
}

void CellularStateMachine::set_timeout(int timeout)
{
    _state_timeout_power_on = timeout;
    _state_timeout_sim_pin = timeout;
    _state_timeout_registration = timeout;
    _state_timeout_network = timeout;
    _state_timeout_connect = timeout;
}

} // namespace

