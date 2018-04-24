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

#include "AT_CellularDevice.h"

using namespace events;
using namespace mbed;

#define DEFAULT_AT_TIMEOUT 1000 // at default timeout in milliseconds

AT_CellularDevice::AT_CellularDevice(EventQueue &queue) :
    _atHandlers(0), _network(0), _sms(0), _sim(0), _power(0), _information(0), _queue(queue),
    _default_timeout(DEFAULT_AT_TIMEOUT), _modem_debug_on(false)
{
}

AT_CellularDevice::~AT_CellularDevice()
{
    close_network();
    close_sms();
    close_power();
    close_sim();
    close_information();

    ATHandler *atHandler = _atHandlers;
    while (atHandler) {
        ATHandler *old = atHandler;
        atHandler = atHandler->_nextATHandler;
        delete old;
        old = NULL;
    }
}

// each parser is associated with one filehandle (that is UART)
ATHandler* AT_CellularDevice::get_at_handler(FileHandle *fileHandle)
{
    if (!fileHandle) {
        return NULL;
    }
    ATHandler *atHandler = _atHandlers;
    while (atHandler) {
        if (atHandler->get_file_handle() == fileHandle) {
            atHandler->inc_ref_count();
            return atHandler;
        }
        atHandler = atHandler->_nextATHandler;
    }

    atHandler = new ATHandler(fileHandle, _queue, _default_timeout, "\r", get_send_delay());
    if (atHandler) {
        if (_modem_debug_on) {
            atHandler->enable_debug(_modem_debug_on);
        }
        atHandler->_nextATHandler = _atHandlers;
        _atHandlers = atHandler;
    }

    return atHandler;
}

void AT_CellularDevice::release_at_handler(ATHandler* at_handler)
{
    if (!at_handler) {
        return;
    }
    at_handler->dec_ref_count();
    if (at_handler->get_ref_count() == 0) {
        // we can delete this at_handler
        ATHandler *atHandler = _atHandlers;
        ATHandler *prev = NULL;
        while (atHandler) {
            if (atHandler == at_handler) {
                if (prev == NULL) {
                    _atHandlers = _atHandlers->_nextATHandler;
                } else {
                    prev->_nextATHandler = atHandler->_nextATHandler;
                }
                delete atHandler;
                atHandler = NULL;
                break;
            } else {
                prev = atHandler;
                atHandler =atHandler->_nextATHandler;
            }
        }
    }
}

CellularNetwork *AT_CellularDevice::open_network(FileHandle *fh)
{
    if (!_network) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _network = new AT_CellularNetwork(*atHandler);
            if (!_network) {
                release_at_handler(atHandler);
            }
        }
    }
    return _network;
}

CellularSMS *AT_CellularDevice::open_sms(FileHandle *fh)
{
    if (!_sms) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _sms = new AT_CellularSMS(*atHandler);
            if (!_sms) {
                release_at_handler(atHandler);
            }
        }
    }
    return _sms;
}

CellularSIM *AT_CellularDevice::open_sim(FileHandle *fh)
{
    if (!_sim) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _sim = new AT_CellularSIM(*atHandler);
            if (!_sim) {
                release_at_handler(atHandler);
            }
        }
    }
    return _sim;
}

CellularPower *AT_CellularDevice::open_power(FileHandle *fh)
{
    if (!_power) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _power = new AT_CellularPower(*atHandler);
            if (!_power) {
                release_at_handler(atHandler);
            }
        }
    }
    return _power;
}

CellularInformation *AT_CellularDevice::open_information(FileHandle *fh)
{
    if (!_information) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _information = new AT_CellularInformation(*atHandler);
            if (!_information) {
                release_at_handler(atHandler);
            }
        }
    }
    return _information;
}

void AT_CellularDevice::close_network()
{
    if (_network) {
        release_at_handler(&_network->get_at_handler());
        delete _network;
        _network = NULL;
    }
}

void AT_CellularDevice::close_sms()
{
    if (_sms) {
        release_at_handler(&_sms->get_at_handler());
        delete _sms;
        _sms = NULL;
    }
}
void AT_CellularDevice::close_power()
{
    if (_power) {
        release_at_handler(&_power->get_at_handler());
        delete _power;
        _power = NULL;
    }
}

void AT_CellularDevice::close_sim()
{
    if (_sim) {
        release_at_handler(&_sim->get_at_handler());
        delete _sim;
        _sim = NULL;
    }
}

void AT_CellularDevice::close_information()
{
    if (_information) {
        release_at_handler(&_information->get_at_handler());
        delete _information;
        _information = NULL;
    }
}

void AT_CellularDevice::set_timeout(int timeout)
{
    _default_timeout = timeout;

    ATHandler *atHandler = _atHandlers;
    while (atHandler) {
        atHandler->set_at_timeout(_default_timeout, true); // set as default timeout
        atHandler = atHandler->_nextATHandler;
    }
}

uint16_t AT_CellularDevice::get_send_delay()
{
    return 0;
}

void AT_CellularDevice::modem_debug_on(bool on)
{
    _modem_debug_on = on;

    ATHandler *atHandler = _atHandlers;
    while (atHandler) {
        atHandler->enable_debug(_modem_debug_on);
        atHandler = atHandler->_nextATHandler;
    }
}

NetworkStack *AT_CellularDevice::get_stack()
{
    if (!_network) {
        return NULL;
    }
    return _network->get_stack();
}
