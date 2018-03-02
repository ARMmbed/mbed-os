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

// timeout to wait for AT responses
#define TIMEOUT_POWER_ON     (1*1000)
#define TIMEOUT_SIM_PIN      (1*1000)
#define TIMEOUT_NETWORK      (10*1000)
#define TIMEOUT_REGISTRATION (180*1000)

// maximum time when retrying network register, attach and connect in seconds ( 20minutes )
#define TIMEOUT_NETWORK_MAX (20*60)

#define RETRY_COUNT_DEFAULT 3

namespace mbed {

CellularConnectionFSM::CellularConnectionFSM() :
        _serial(0), _state(STATE_INIT), _next_state(_state), _status_callback(0), _network(0), _power(0), _sim(0),
        _queue(8 * EVENTS_EVENT_SIZE), _queue_thread(0), _retry_count(0), _state_retry_count(0), _at_queue(8 * EVENTS_EVENT_SIZE)
{
    memset(_sim_pin, 0, sizeof(_sim_pin));
#if MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY == 0
    _start_time = 0;
#else
    _start_time = rand() % (MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY);
#endif // MBED_CONF_CELLULAR_RANDOM_MAX_START_DELAY

    // set initial retry values in seconds
    _retry_timeout_array[0] = 1;
    _retry_timeout_array[1] = 2;
    _retry_timeout_array[2] = 4;
    _retry_timeout_array[3] = 16;
    _retry_timeout_array[4] = 32;
    _retry_timeout_array[5] = 60;
    _retry_timeout_array[6] = 120;
    _retry_timeout_array[7] = 360;
    _retry_timeout_array[8] = 600;
    _retry_timeout_array[9] = TIMEOUT_NETWORK_MAX;
    _retry_array_length = MAX_RETRY_ARRAY_SIZE;
    
    _cellularDevice = new CELLULAR_DEVICE(_at_queue);
}

CellularConnectionFSM::~CellularConnectionFSM()
{
    stop();
}

nsapi_error_t CellularConnectionFSM::init()
{
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

    tr_info("init done...");
    return NSAPI_ERROR_OK;
}

bool CellularConnectionFSM::open_power(FileHandle *fh)
{
    if (!_power) {
        _power = _cellularDevice->open_power(fh);
        if (!_power) {
            return false;
        }
    }
    nsapi_error_t err = _power->on();
    if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
        tr_warn("Cellular start failed. Power off/on.");
        err = _power->off();
        if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
            tr_error("Cellular power down failed!");
        }
        return false;
    }
    return true;
}

void CellularConnectionFSM::set_sim_pin(const char * sim_pin)
{
    strncpy(_sim_pin, sim_pin, sizeof(_sim_pin));
}

bool CellularConnectionFSM::open_sim()
{
    CellularSIM::SimState state = CellularSIM::SimStateUnknown;
    // wait until SIM is readable
    // here you could add wait(secs) if you know start delay of your SIM
    while (_sim->get_sim_state(state) != NSAPI_ERROR_OK || state == CellularSIM::SimStateUnknown) {
        tr_info("Waiting for SIM (state %d)...", state);
        return false;
    }
    tr_info("Initial SIM state: %d", state);

    if (strlen(_sim_pin)) {
        nsapi_error_t err;
        if (state == CellularSIM::SimStatePinNeeded) {
            tr_info("SIM pin required, entering pin: %s", _sim_pin);
            err = _sim->set_pin(_sim_pin);
            if (err) {
                tr_error("SIM pin set failed with: %d, bailing out...", err);
                return false;
            }
            // here you could add wait(secs) if you know delay of changing PIN on your SIM
            for (int i = 0; i < MAX_SIM_READY_WAITING_TIME; i++) {
                if (_sim->get_sim_state(state) == NSAPI_ERROR_OK && state == CellularSIM::SimStateReady) {
                    break;
                }
                tr_info("SIM state: %d", state);
                return false;
            }
        }
    } else {
        tr_info("No SIM pin provided.");
    }

    return state == CellularSIM::SimStateReady;
}

void CellularConnectionFSM::device_ready()
{
    CellularInformation *info = _cellularDevice->open_information(_serial);
    char device_info_buf[2048]; // may be up to 2048 according to 3GPP

    if (info->get_manufacturer(device_info_buf, sizeof(device_info_buf)) == NSAPI_ERROR_OK) {
        tr_info("Cellular device manufacturer: %s", device_info_buf);
    }
    if (info->get_model(device_info_buf, sizeof(device_info_buf)) == NSAPI_ERROR_OK) {
        tr_info("Cellular device model: %s", device_info_buf);
    }
    if (info->get_revision(device_info_buf, sizeof(device_info_buf)) == NSAPI_ERROR_OK) {
        tr_info("Cellular device revision: %s", device_info_buf);
    }
}

bool CellularConnectionFSM::set_network_registration(char *plmn)
{
    if (_network->set_registration(plmn) != NSAPI_ERROR_OK) {
        tr_error("Failed to set network registration.");
        return false;
    }
    return true;
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

bool CellularConnectionFSM::get_attach_network(CellularNetwork::AttachStatus &status)
{
    nsapi_error_t err = _network->get_attach(status);
    if (err != NSAPI_ERROR_OK) {
        return false;
    }
    return true;
}

bool CellularConnectionFSM::set_attach_network()
{
    nsapi_error_t attach_err = _network->set_attach();
    if (attach_err != NSAPI_ERROR_OK) {
        return false;
    }
    return true;
}

void CellularConnectionFSM::report_failure(const char* msg)
{
    tr_error("Cellular network failed: %s", msg);
    if (_status_callback) {
        _status_callback(_state, _next_state);
    }
}

nsapi_error_t CellularConnectionFSM::continue_to_state(CellularState state)
{
    if (state < _state) {
        _state = state;
    }
    if (!_queue.call_in(0, callback(this, &CellularConnectionFSM::event))) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    return NSAPI_ERROR_OK;
}

void CellularConnectionFSM::event()
{
    nsapi_error_t err;
    int event_timeout = -1;

    switch (_state) {
        case STATE_INIT:
            event_timeout = _start_time;
            tr_info("INIT state, waiting %d ms before POWER state)", _start_time);
            _next_state = STATE_POWER_ON;
            break;
        case STATE_POWER_ON:
            _cellularDevice->set_timeout(TIMEOUT_POWER_ON);
            tr_info("Cellular power ON (timeout %d ms)", TIMEOUT_POWER_ON);
            if (open_power(_serial)) {
                _next_state = STATE_DEVICE_READY;
                _retry_count = 0;
            } else {
                if (++_retry_count <= RETRY_COUNT_DEFAULT) {
                    tr_warn("Power ON retry %d", _retry_count);
                    event_timeout = 3 * 1000;
                } else {
                    report_failure("Power");
                    return;
                }
            }
            break;
        case STATE_DEVICE_READY:
            _cellularDevice->set_timeout(TIMEOUT_POWER_ON);
            if (_power->set_at_mode() == NSAPI_ERROR_OK) {
                tr_info("Cellular device ready");
                _next_state = STATE_SIM_PIN;
                _retry_count = 0;
                device_ready();
            } else {
                tr_info("Waiting for cellular device (retry %d/%d, timeout %d ms)", _retry_count, RETRY_COUNT_DEFAULT,
                        TIMEOUT_POWER_ON);
                if (_retry_count++ <= RETRY_COUNT_DEFAULT) {
                    event_timeout = 3 * 1000;
                } else {
                    report_failure("Power");
                    return;
                }
            }
            break;
        case STATE_SIM_PIN:
            _cellularDevice->set_timeout(TIMEOUT_SIM_PIN);
            tr_info("Start cellular (timeout %d ms)", TIMEOUT_SIM_PIN);
            if (open_sim()) {
                _next_state = STATE_REGISTERING_NETWORK;
                _retry_count = 0;
                _state_retry_count = 0;
                tr_info("Check for network registration");
            } else {
                if (_retry_count++ <= RETRY_COUNT_DEFAULT) {
                    tr_warn("Waiting for SIM %d/%d", _retry_count, RETRY_COUNT_DEFAULT);
                    event_timeout = 3 * 1000;
                } else {
                    report_failure("Entering SIM PIN");
                    return;
                }
            }
            break;
        case STATE_REGISTERING_NETWORK:
            _cellularDevice->set_timeout(TIMEOUT_NETWORK);
            CellularNetwork::RegistrationStatus status;
            bool is_registered;
            _next_state = STATE_REGISTER_NETWORK;
            for (int type = 0; type < CellularNetwork::C_MAX; type++) {
                if (get_network_registration((CellularNetwork::RegistrationType) type, status, is_registered)) {
                    tr_debug("get_network_registration: type=%d, status=%d", type, status);
                    if (is_registered) {
                        tr_info("Registered to cellular network (type %d, status %d)", type, status);
                        _next_state = STATE_ATTACHING_NETWORK;
                        _retry_count = 0;
                        _state_retry_count = 0;
                        event_timeout = 0;
                        tr_info("Check cellular network attach state");
                        break;
                    } else {
                        if (_retry_count < 180) {
                            event_timeout = 1000;
                            _next_state = STATE_REGISTERING_NETWORK;
                            tr_info("Waiting for registration %d/180 (type %d, status %d)", _retry_count, type, status);
                        } else {
                            tr_info("Start cellular registration");
                            _next_state = STATE_REGISTER_NETWORK;
                            _retry_count = 0;
                            break;
                        }
                    }
                }
            }

            if (_next_state == STATE_REGISTERING_NETWORK) {
                _retry_count++;
            }
            break;
        case STATE_REGISTER_NETWORK:
            _cellularDevice->set_timeout(TIMEOUT_REGISTRATION);
            tr_info("Register to cellular network (timeout %d ms)", TIMEOUT_REGISTRATION);
            if (set_network_registration()) {
                _next_state = STATE_REGISTERING_NETWORK;
                _retry_count = 0;
                if (_state_retry_count > RETRY_COUNT_DEFAULT) {
                    report_failure("Registration retry");
                    return;
                }
                _state_retry_count++;
            } else {
                if (_retry_count < _retry_array_length) {
                    event_timeout = _retry_timeout_array[_retry_count] * 1000;
                    _retry_count++;
                } else {
                    report_failure("Registration");
                    return;
                }
            }
            break;
        case STATE_ATTACHING_NETWORK:
            _cellularDevice->set_timeout(TIMEOUT_NETWORK);
            CellularNetwork::AttachStatus attach_status;
            if (get_attach_network(attach_status)) {
                if (attach_status == CellularNetwork::Attached) {
                    _next_state = STATE_CONNECT_NETWORK;
                    _retry_count = 0;
                } else {
                    _next_state = STATE_ATTACH_NETWORK;
                    _retry_count = 0;
                }
            } else {
                if (_retry_count++ <= RETRY_COUNT_DEFAULT) {
                    event_timeout = 1 * 1000;
                } else {
                    report_failure("Attaching");
                    return;
                }
            }
            break;
        case STATE_ATTACH_NETWORK:
            _cellularDevice->set_timeout(TIMEOUT_NETWORK);
            tr_info("Attach to cellular network (timeout %d ms)", TIMEOUT_NETWORK);
            if (set_attach_network()) {
                _next_state = STATE_ATTACHING_NETWORK;
                _retry_count = 0;
                if (_state_retry_count >= RETRY_COUNT_DEFAULT) {
                    report_failure("Attach retry");
                    return;
                }
                _state_retry_count++;
                tr_info("Cellular network attaching");
            } else {
                if (_retry_count < _retry_array_length) {
                    event_timeout = _retry_timeout_array[_retry_count] * 1000;
                    _retry_count++;
                } else {
                    report_failure("Attach");
                    return;
                }
            }
            break;
        case STATE_CONNECT_NETWORK:
            _cellularDevice->set_timeout(TIMEOUT_NETWORK);
            tr_info("Connect to cellular network (timeout %d ms)", TIMEOUT_NETWORK);
            err = _network->connect();
            if (!err) {
                _next_state = STATE_CONNECTED;
            } else {
                if (_retry_count < _retry_array_length) {
                    event_timeout = _retry_timeout_array[_retry_count] * 1000;
                    _retry_count++;
                } else {
                    report_failure("Network Connect");
                    return;
                }
            }
            break;
        case STATE_CONNECTED:
            _cellularDevice->set_timeout(TIMEOUT_NETWORK);
            tr_debug("Cellular ready! (timeout %d ms)", TIMEOUT_NETWORK);
            if (_status_callback) {
                if (!_status_callback(_state, _next_state)) {
                    return;
                }
            }
            break;
        default:
            MBED_ASSERT(0);
            break;
    }

    if (_next_state != _state || event_timeout >= 0) {
        if (_next_state != _state) { // state exit condition
            tr_info("Cellular state from %d to %d", _state, _next_state);
            if (_status_callback) {
                if (!_status_callback(_state, _next_state)) {
                    return;
                }
            }
        } else {
            if (event_timeout == 0) {
                static int retry_count = 0;
                if (++retry_count <= 3) {
                    tr_info("Cellular event retry %d", retry_count);
                } else {
                    report_failure("Cellular connection failed!");
                    return;
                }
            } else {
                tr_info("Cellular event in %d milliseconds", event_timeout);
            }
        }
        _state = _next_state;
        if (event_timeout == -1) {
            event_timeout = 0;
        }
        if (!_queue.call_in(event_timeout, callback(this, &CellularConnectionFSM::event))) {
            report_failure("Cellular event failure!");
            return;
        }
    }
}

nsapi_error_t CellularConnectionFSM::start_dispatch()
{
    tr_info("CellularConnectionUtil::start");
    tr_info("Create cellular thread");

    MBED_ASSERT(!_queue_thread);

    _queue_thread = new rtos::Thread;
    if (!_queue_thread) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }
    if (_queue_thread->start(callback(&_queue, &events::EventQueue::dispatch_forever)) != osOK) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    tr_info("CellularConnectionUtil::started");
    return NSAPI_ERROR_OK;
}

void CellularConnectionFSM::stop()
{
    tr_info("CellularConnectionUtil::stop");
    _cellularDevice->close_power();
    _cellularDevice->close_network();
    if (_queue_thread) {
        _queue_thread->terminate();
        _queue_thread = NULL;
    }
}

void CellularConnectionFSM::set_serial(UARTSerial *serial)
{
    _serial = serial;
}

void CellularConnectionFSM::set_callback(mbed::Callback<bool(int, int)> status_callback)
{
    _status_callback = status_callback;
}

events::EventQueue *CellularConnectionFSM::get_queue()
{
    return &_queue;
}

CellularNetwork* CellularConnectionFSM::get_network()
{
    return _network;
}

CellularDevice* CellularConnectionFSM::get_device()
{
    return _cellularDevice;
}

CellularSIM* CellularConnectionFSM::get_sim()
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
