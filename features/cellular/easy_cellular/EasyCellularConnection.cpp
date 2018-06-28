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

#include "CellularTargets.h"
#ifdef CELLULAR_DEVICE

#if NSAPI_PPP_AVAILABLE
#include "nsapi_ppp.h"
#endif

#include "CellularConnectionFSM.h"
#include "CellularUtil.h"

#include "EasyCellularConnection.h"
#include "CellularLog.h"
#include "mbed_wait_api.h"

#if USE_APN_LOOKUP
#include "APN_db.h"
#endif //USE_APN_LOOKUP

namespace mbed {

bool EasyCellularConnection::cellular_status(int state, int next_state)
{
    tr_info("cellular_status: %s ==> %s", _cellularConnectionFSM->get_state_string((CellularConnectionFSM::CellularState)state),
            _cellularConnectionFSM->get_state_string((CellularConnectionFSM::CellularState)next_state));

    if (_target_state == state) {
        tr_info("Target state reached: %s", _cellularConnectionFSM->get_state_string(_target_state));
        (void)_cellularSemaphore.release();
        return false; // return false -> state machine is halted
    }
    return true;
}

void EasyCellularConnection::network_callback(nsapi_event_t ev, intptr_t ptr)
{
    if (ev == NSAPI_EVENT_CONNECTION_STATUS_CHANGE) {
        if (ptr == NSAPI_STATUS_GLOBAL_UP) {
            _is_connected = true;
        } else {
            _is_connected = false;
        }
    }
    if (_status_cb) {
        _status_cb(ev, ptr);
    }
}

EasyCellularConnection::EasyCellularConnection(bool debug) :
        _is_connected(false), _is_initialized(false), _target_state(CellularConnectionFSM::STATE_POWER_ON), _cellularSerial(
                MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE), _cellularSemaphore(0), _cellularConnectionFSM(0), _credentials_err(
                NSAPI_ERROR_OK), _status_cb(0)
{
    tr_info("EasyCellularConnection()");
#if USE_APN_LOOKUP
    _credentials_set = false;
#endif // #if USE_APN_LOOKUP
    modem_debug_on(debug);
}

EasyCellularConnection::~EasyCellularConnection()
{
    if (_cellularConnectionFSM) {
        _cellularConnectionFSM->set_callback(NULL);
        _cellularConnectionFSM->attach(NULL);
        delete _cellularConnectionFSM;
    }
}

nsapi_error_t EasyCellularConnection::init()
{
    nsapi_error_t err = NSAPI_ERROR_OK;
    if (!_is_initialized) {
#if defined (MDMRTS) && defined (MDMCTS)
        _cellularSerial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
        _cellularConnectionFSM = new CellularConnectionFSM();
        _cellularConnectionFSM->set_serial(&_cellularSerial);
        _cellularConnectionFSM->set_callback(callback(this, &EasyCellularConnection::cellular_status));

        err = _cellularConnectionFSM->init();

        if (err == NSAPI_ERROR_OK) {
            err = _cellularConnectionFSM->start_dispatch();
            _cellularConnectionFSM->attach(callback(this, &EasyCellularConnection::network_callback));
        }
        _is_initialized = true;
    }

    return err;
}

void EasyCellularConnection::set_credentials(const char *apn, const char *uname, const char *pwd)
{
    if (apn && strlen(apn) > 0) {
        _credentials_err = init();

        if (_credentials_err) {
            return;
        }
        CellularNetwork *network = _cellularConnectionFSM->get_network();
        if (network) {
            _credentials_err = network->set_credentials(apn, uname, pwd);
#if USE_APN_LOOKUP
            if (_credentials_err == NSAPI_ERROR_OK) {
                _credentials_set = true;
            }
#endif // #if USE_APN_LOOKUP
        } else {
            //if get_network() returns NULL it means there was not enough memory for
            //an AT_CellularNetwork element during CellularConnectionFSM initialization
            tr_error("There was not enough memory during CellularConnectionFSM initialization");
        }
    }
}

void EasyCellularConnection::set_sim_pin(const char *sim_pin)
{
    if (sim_pin && strlen(sim_pin) > 0) {
        if (!_cellularConnectionFSM) {
            _credentials_err = init();

            if (_credentials_err) {
                return;
            }
        }
        _cellularConnectionFSM->set_sim_pin(sim_pin);
    }
}

nsapi_error_t EasyCellularConnection::connect(const char *sim_pin, const char *apn, const char *uname, const char *pwd)
{
    if (_is_connected) {
        return NSAPI_ERROR_IS_CONNECTED;
    }

    set_credentials(apn, uname, pwd);
    if (_credentials_err) {
        return _credentials_err;
    }

    if (sim_pin) {
        this->set_sim_pin(sim_pin);
    }

    return connect();
}

nsapi_error_t EasyCellularConnection::check_connect()
{
    if (_is_connected) {
        return NSAPI_ERROR_IS_CONNECTED;
    }

    // there was an error while setting credentials but it's a void function so check error here...
    if (_credentials_err) {
        return _credentials_err;
    }

    nsapi_error_t err = init();
    if (err) {
        return err;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t EasyCellularConnection::connect()
{
    nsapi_error_t err = check_connect();
    if (err) {
        return err;
    }
#if USE_APN_LOOKUP
    if (!_credentials_set) {
        _target_state = CellularConnectionFSM::STATE_SIM_PIN;
        err = _cellularConnectionFSM->continue_to_state(_target_state);
        if (err == NSAPI_ERROR_OK) {
            int sim_wait = _cellularSemaphore.wait(60 * 1000); // reserve 60 seconds to access to SIM
            if (sim_wait != 1) {
                tr_error("NO SIM ACCESS");
                err = NSAPI_ERROR_NO_CONNECTION;
            } else {
                char imsi[MAX_IMSI_LENGTH + 1];
                wait(1); // need to wait to access SIM in some modems
                err = _cellularConnectionFSM->get_sim()->get_imsi(imsi);
                if (err == NSAPI_ERROR_OK) {
                    const char *apn_config = apnconfig(imsi);
                    if (apn_config) {
                        const char* apn = _APN_GET(apn_config);
                        const char* uname = _APN_GET(apn_config);
                        const char* pwd = _APN_GET(apn_config);
                        tr_info("Looked up APN %s", apn);
                        err = _cellularConnectionFSM->get_network()->set_credentials(apn, uname, pwd);
                    }
                }
            }
        }
        if (err) {
            tr_error("APN lookup failed");
            return err;
        }
    }
#endif // USE_APN_LOOKUP

    _target_state = CellularConnectionFSM::STATE_CONNECTED;
    err = _cellularConnectionFSM->continue_to_state(_target_state);
    if (err == NSAPI_ERROR_OK) {
        int ret_wait = _cellularSemaphore.wait(10 * 60 * 1000); // cellular network searching may take several minutes
        if (ret_wait != 1) {
            tr_info("No cellular connection");
            err = NSAPI_ERROR_NO_CONNECTION;
        }
    }

    return err;
}

nsapi_error_t EasyCellularConnection::disconnect()
{
    _credentials_err = NSAPI_ERROR_OK;
    _is_connected = false;
    _is_initialized = false;
#if USE_APN_LOOKUP
    _credentials_set = false;
#endif // #if USE_APN_LOOKUP

    nsapi_error_t err = NSAPI_ERROR_OK;
    if (_cellularConnectionFSM && _cellularConnectionFSM->get_network()) {
        err = _cellularConnectionFSM->get_network()->disconnect();
    }

    if (err == NSAPI_ERROR_OK) {
        delete _cellularConnectionFSM;
        _cellularConnectionFSM = NULL;
    }

    return err;
}

bool EasyCellularConnection::is_connected()
{
    return _is_connected;
}

const char *EasyCellularConnection::get_ip_address()
{
    if (_cellularConnectionFSM) {
        CellularNetwork *network = _cellularConnectionFSM->get_network();
        if (!network) {
            return NULL;
        }
        return _cellularConnectionFSM->get_network()->get_ip_address();
    } else {
        return NULL;
    }
}

const char *EasyCellularConnection::get_netmask()
{
    if (_cellularConnectionFSM) {
        CellularNetwork *network = _cellularConnectionFSM->get_network();
        if (!network) {
            return NULL;
        }
        return network->get_netmask();
    } else {
        return NULL;
    }
}

const char *EasyCellularConnection::get_gateway()
{
    if (_cellularConnectionFSM) {
        CellularNetwork *network = _cellularConnectionFSM->get_network();
        if (!network) {
            return NULL;
        }
        return network->get_gateway();
    } else {
        return NULL;
    }
}

void EasyCellularConnection::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _status_cb = status_cb;
}

void EasyCellularConnection::modem_debug_on(bool on)
{
    if (_cellularConnectionFSM) {
        CellularDevice *dev = _cellularConnectionFSM->get_device();
        if (dev) {
            dev->modem_debug_on(on);
        }
    }
}

void EasyCellularConnection::set_plmn(const char *plmn)
{
    if (plmn && strlen(plmn) > 0) {
        if (!_cellularConnectionFSM) {
            _credentials_err = init();

            if (_credentials_err) {
                return;
            }
        }
        _cellularConnectionFSM->set_plmn(plmn);
    }
}

NetworkStack *EasyCellularConnection::get_stack()
{
    if (_cellularConnectionFSM) {
        return _cellularConnectionFSM->get_stack();
    } else {
        return NULL;
    }
}

} // namespace

#endif // CELLULAR_DEVICE
