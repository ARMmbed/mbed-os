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

#include "CellularConnectionUtil.h"
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

namespace mbed {

static events::EventQueue at_queue(8 * EVENTS_EVENT_SIZE);
static CELLULAR_DEVICE cellularDevice(at_queue);

CellularConnectionUtil::CellularConnectionUtil() : _serial(0), _state(STATE_POWER_ON), _next_state(_state),
        _status_callback(0), _network(0), _power(0), _queue(8 * EVENTS_EVENT_SIZE),
        _queue_thread(0), _cellularDevice(&cellularDevice)
{
    memset(_sim_pin, 0, sizeof(_sim_pin));
}

CellularConnectionUtil::~CellularConnectionUtil()
{
    stop();
}

nsapi_error_t CellularConnectionUtil::init()
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

    at_queue.chain(&_queue);

    log_info("init done...");
    return NSAPI_ERROR_OK;
}

bool CellularConnectionUtil::open_power(FileHandle *fh)
{
    if (!_power) {
        _power = _cellularDevice->open_power(fh);
        if (!_power) {
            return false;
        }
    }

    nsapi_error_t err = _power->on();
    if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
        log_warn("Cellular start failed. Power off/on.");
        err = _power->off();
        if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
            log_error("Cellular power down failed!");
        }
        return false;
    }
    return true;
}

void CellularConnectionUtil::set_sim_pin(const char * sim_pin)
{
    strncpy(_sim_pin, sim_pin, sizeof(_sim_pin));
}

bool CellularConnectionUtil::open_sim()
{
    if (strlen(_sim_pin)) {
        nsapi_error_t err;
        static CellularSIM *sim;
        if (!sim) {
            sim = _cellularDevice->open_sim(_serial);
        }
        if (!sim) {
            return false;
        }
        CellularSIM::SimState state = CellularSIM::SimStateUnknown;
        // wait until SIM is readable
        // here you could add wait(secs) if you know start delay of your SIM
        while (sim->get_sim_state(state) != NSAPI_ERROR_OK || state == CellularSIM::SimStateUnknown) {
            log_debug("Waiting for SIM (state %d)...", state);
            return false;
        }
        if (state == CellularSIM::SimStatePinNeeded) {
            log_info("SIM pin required, entering pin: %s", _sim_pin);
            err = sim->set_pin(_sim_pin);
            if (err) {
                log_error("SIM pin set failed with: %d, bailing out...", err);
                return false;
            }
            // here you could add wait(secs) if you know delay of changing PIN on your SIM
            for (int i = 0; i < MAX_SIM_READY_WAITING_TIME; i++) {
                if (sim->get_sim_state(state) == NSAPI_ERROR_OK && state == CellularSIM::SimStateReady) {
                    break;
                }
                log_debug("SIM state: %d", state);
                return false;
            }
        }
        return state == CellularSIM::SimStateReady;
    } else {
        log_info("Continue without SIM.");
        return true;
    }
}

void CellularConnectionUtil::device_ready()
{
    CellularInformation *info = _cellularDevice->open_information(_serial);

    char buf[2048];
    if (info->get_manufacturer(buf, sizeof(buf)) == NSAPI_ERROR_OK) {
        log_info("Cellular device manufacturer: %s", buf);
    }
    if (info->get_model(buf, sizeof(buf)) == NSAPI_ERROR_OK) {
        log_info("Cellular device model: %s", buf);
    }
    if (info->get_revision(buf, sizeof(buf)) == NSAPI_ERROR_OK) {
        log_info("Cellular device revision: %s", buf);
    }
}

bool CellularConnectionUtil::open_network()
{
    if (!_network) {
        _network = _cellularDevice->open_network(_serial);
    }
    return (_network != NULL);
}

bool CellularConnectionUtil::set_network_registration(char *plmn)
{
    if (_network->set_registration(plmn) != NSAPI_ERROR_OK) {
        log_error("Failed to set network registration.");
        return false;
    }
    return true;
}

bool CellularConnectionUtil::get_network_registration(CellularNetwork::RegistrationType type,
        CellularNetwork::RegistrationStatus &status, bool &is_registered)
{
    is_registered = false;
    bool is_roaming = false;
    nsapi_error_t err = _network->get_registration_status(type, status);
    if (err !=  NSAPI_ERROR_OK) {
        if (err != NSAPI_ERROR_UNSUPPORTED) {
            log_warn("Get network registration failed (type %d)!", type);
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
            log_warn("SMS only network registration!");
            break;
        case CellularNetwork::RegisteredCSFBNotPreferredRoaming:
            is_roaming = true;
        // fall-through
        case CellularNetwork::RegisteredCSFBNotPreferredHome:
            log_warn("Not preferred network registration!");
            break;
        case CellularNetwork::AttachedEmergencyOnly:
            log_warn("Emergency only network registration!");
            break;
        case CellularNetwork::RegistrationDenied:
        case CellularNetwork::NotRegistered:
        case CellularNetwork::Unknown:
        case CellularNetwork::SearchingNetwork:
        default:
            break;
    }

    if (is_roaming) {
        log_warn("Roaming cellular network!");
    }

    return true;
}

bool CellularConnectionUtil::get_attach_network(CellularNetwork::AttachStatus &status)
{
    nsapi_error_t err = _network->get_attach(status);
    if (err != NSAPI_ERROR_OK) {
        return false;
    }
    return true;
}

bool CellularConnectionUtil::set_attach_network()
{
    nsapi_error_t attach_err = _network->set_attach();
    if (attach_err != NSAPI_ERROR_OK) {
        return false;
    }
    return true;
}

void CellularConnectionUtil::report_failure(const char* msg)
{
    log_error("Cellular network failed: %s", msg);
    if (_status_callback) {
        _status_callback(_state, _next_state);
    }
}

nsapi_error_t CellularConnectionUtil::continue_to_state(CellularState state)
{
    if (state < _state) {
        _state = state;
    }
    if (!_queue.call_in(0, callback(this, &CellularConnectionUtil::event))) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    return NSAPI_ERROR_OK;
}

void CellularConnectionUtil::event()
{
    nsapi_error_t err;
    int event_timeout = -1;

    switch (_state) {
        case STATE_POWER_ON:
            cellularDevice.set_timeout(TIMEOUT_POWER_ON);
            log_info("Cellular power ON (timeout %d ms)", TIMEOUT_POWER_ON);
            if (open_power(_serial)) {
                _next_state = STATE_DEVICE_READY;
            } else {
                static int retry_count;
                if (++retry_count <= 10) {
                    log_warn("Power ON retry %d", retry_count);
                    event_timeout = 1000;
                } else {
                    report_failure("Power");
                    return;
                }
            }
            break;
        case STATE_DEVICE_READY:
            cellularDevice.set_timeout(TIMEOUT_POWER_ON);
            if (_power->set_at_mode() == NSAPI_ERROR_OK) {
                log_info("Cellular device ready");
                _next_state = STATE_START_CELLULAR;
                device_ready();
            } else {
                static int retry_count = 0;
                log_info("Waiting for cellular device (retry %d/10, timeout %d ms)", retry_count, TIMEOUT_POWER_ON);
                if (++retry_count <= 10) {
                    event_timeout = 1000;
                } else {
                    report_failure("Power");
                    return;
                }
            }
            break;
        case STATE_START_CELLULAR:
            cellularDevice.set_timeout(TIMEOUT_NETWORK);
            log_info("Start cellular (timeout %d ms)", TIMEOUT_NETWORK);
            open_network();
            _next_state = STATE_SIM_PIN;
            break;
        case STATE_SIM_PIN:
            cellularDevice.set_timeout(TIMEOUT_SIM_PIN);
            log_info("Start cellular (timeout %d ms)", TIMEOUT_SIM_PIN);
            if (open_sim()) {
                _next_state = STATE_REGISTERING_NETWORK;
                log_info("Check for network registration");
            } else {
                static int retry_count;
                if (++retry_count <= 10) {
                    log_warn("Waiting for SIM %d", retry_count);
                    event_timeout = 1000;
                } else {
                    report_failure("Entering SIM PIN");
                    return;
                }
            }
            break;
        case STATE_REGISTERING_NETWORK:
            cellularDevice.set_timeout(TIMEOUT_NETWORK);
            CellularNetwork::RegistrationStatus status;
            bool is_registered;
            _next_state = STATE_REGISTER_NETWORK;
            for (int type = 0; type < CellularNetwork::C_MAX; type++) {
                if (get_network_registration((CellularNetwork::RegistrationType)type, status, is_registered)) {
                    log_debug("get_network_registration: type=%d, status=%d", type, status);
                    if (is_registered) {
                        log_info("Registered to cellular network (type %d, status %d)", type, status);
                        _next_state = STATE_ATTACH_NETWORK;
                        log_info("Check cellular network attach state");
                        break;
                    } else {
                        if (status == CellularNetwork::RegistrationDenied) {
                            static int backoff_timeout = 1;
                            log_warn("Network registration denied (type %d)! Retry after %d seconds.", type, backoff_timeout);
                            event_timeout = backoff_timeout * 1000;
                            backoff_timeout *= 2;
                            break;
                        } else if (status != CellularNetwork::NotRegistered) {
                            static int retry_count;
                            if (++retry_count <= 180) {
                                log_info("Waiting for registration %d/180 (type %d, status %d)", retry_count, type, status);
                                event_timeout = 1*1000;
                                _next_state = STATE_REGISTERING_NETWORK;
                            } else {
                                log_info("Start cellular registration");
                                _next_state = STATE_REGISTER_NETWORK;
                                retry_count = 0;
                                break;
                            }
                        } else {
                            // modem is not yet searching network for this technology
                        }
                    }
                }
            }
            break;
        case STATE_REGISTER_NETWORK:
            cellularDevice.set_timeout(TIMEOUT_REGISTRATION);
            log_info("Register to cellular network (timeout %d ms)", TIMEOUT_REGISTRATION);
            if (set_network_registration()) {
                _next_state = STATE_REGISTERING_NETWORK;
            } else {
                static int retry_count;
                if (++retry_count <= 3) {
                    event_timeout = 1000;
                } else {
                    report_failure("Registration");
                    return;
                }
            }
            break;
        case STATE_ATTACHING_NETWORK:
            cellularDevice.set_timeout(TIMEOUT_NETWORK);
            CellularNetwork::AttachStatus attach_status;
            if (get_attach_network(attach_status)) {
                if (attach_status == CellularNetwork::Attached) {
                    _next_state = STATE_CONNECT_NETWORK;
                } else {
                    _next_state = STATE_ATTACH_NETWORK;
                }
            } else {
                event_timeout = 0;
            }
            break;
        case STATE_ATTACH_NETWORK:
            cellularDevice.set_timeout(TIMEOUT_NETWORK);
            log_info("Attach to cellular network (timeout %d ms)", TIMEOUT_NETWORK);
            if (set_attach_network()) {
                _next_state = STATE_ATTACHING_NETWORK;
                log_info("Cellular network attaching");
            } else {
                event_timeout = 0;
            }
            break;
        case STATE_CONNECT_NETWORK:
            cellularDevice.set_timeout(TIMEOUT_NETWORK);
            log_info("Connect to cellular network (timeout %d ms)", TIMEOUT_NETWORK);
            err = _network->connect();
            if (!err) {
                _next_state = STATE_CONNECTED;
            } else {
                report_failure("Network connect");
                return;
            }
            break;
        case STATE_CONNECTED:
            cellularDevice.set_timeout(TIMEOUT_NETWORK);
            log_debug("Cellular ready! (timeout %d ms)", TIMEOUT_NETWORK);
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
        if (_next_state != _state) {
            log_info("Cellular state from %d to %d", _state, _next_state);
            if (_status_callback) {
                if (!_status_callback(_state, _next_state)) {
                    return;
                }
            }
        } else {
            if (event_timeout == 0) {
                static int retry_count = 0;
                if (++retry_count <= 3) {
                    log_info("Cellular event retry %d", retry_count);
                } else {
                    report_failure("Cellular connection failed!");
                    return;
                }
            } else {
                log_info("Cellular event in %d milliseconds", event_timeout);
            }
        }
        _state = _next_state;
        if (event_timeout == -1) {
            event_timeout = 0;
        }
        if (!_queue.call_in(event_timeout, callback(this, &CellularConnectionUtil::event))) {
            report_failure("Cellular event failure!");
            return;
        }
    }
}

nsapi_error_t CellularConnectionUtil::start_dispatch()
{
    log_info("CellularConnectionUtil::start");
    log_info("Create cellular thread");

    MBED_ASSERT(!_queue_thread);

    _queue_thread = new rtos::Thread(osPriorityNormal, 1024);
    if (!_queue_thread) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }
    if (_queue_thread->start(callback(&_queue, &events::EventQueue::dispatch_forever)) != osOK) {
        stop();
        return NSAPI_ERROR_NO_MEMORY;
    }

    log_info("CellularConnectionUtil::started");
    return NSAPI_ERROR_OK;
}

void CellularConnectionUtil::stop()
{
    log_info("CellularConnectionUtil::stop");
    _cellularDevice->close_power();
    _cellularDevice->close_network();
    if (_queue_thread) {
        _queue_thread->terminate();
        _queue_thread = NULL;
    }
}

void CellularConnectionUtil::set_serial(UARTSerial *serial)
{
    _serial = serial;
}

void CellularConnectionUtil::set_callback(mbed::Callback<bool(int, int)> status_callback)
{
    _status_callback = status_callback;
}

events::EventQueue *CellularConnectionUtil::get_queue()
{
    return &_queue;
}

CellularNetwork* CellularConnectionUtil::get_network()
{
    if (_state != STATE_CONNECTED) {
        return NULL;
    }
    return _network;
}

CellularDevice* CellularConnectionUtil::get_device()
{
    if (_cellularDevice) {
        return _cellularDevice;
    } else {
        return NULL;
    }
}

NetworkStack *CellularConnectionUtil::get_stack()
{
    return _cellularDevice->get_stack();
}

} // namespace

#endif // CELLULAR_DEVICE
