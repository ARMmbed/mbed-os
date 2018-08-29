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

#include "EasyCellularConnection.h"
#include "CellularSIM.h"
#include "CellularLog.h"
#include "mbed_wait_api.h"

#if USE_APN_LOOKUP
#include "APN_db.h"
#endif //USE_APN_LOOKUP

namespace mbed {

void EasyCellularConnection::network_callback(nsapi_event_t ev, intptr_t ptr)
{
    // forward to application
    if (_status_cb) {
        _status_cb(ev, ptr);
    }
}

EasyCellularConnection::EasyCellularConnection(CellularDevice *device) : _is_initialized(false),
        _serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE), _device(device),
        _credentials_err(NSAPI_ERROR_OK), _status_cb(0)
{
#if USE_APN_LOOKUP
    _credentials_set = false;
#endif // #if USE_APN_LOOKUP
}

EasyCellularConnection::~EasyCellularConnection()
{

}

nsapi_error_t EasyCellularConnection::init()
{
    nsapi_error_t err = NSAPI_ERROR_OK;
    _stm_error = false;
    if (!_is_initialized) {
#if defined (MDMRTS) && defined (MDMCTS)
        _serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
        err = _device->init_stm(&_serial);
        if (err != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_NO_MEMORY;
        }

        err = _device->start_dispatch();
        if (err != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_NO_MEMORY;
        }

        _network = _device->open_network(&_serial);
        _device->attach(callback(this, &EasyCellularConnection::network_callback));
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
        _credentials_err = _device->set_credentials(apn, uname, pwd);
#if USE_APN_LOOKUP
        if (_credentials_err == NSAPI_ERROR_OK) {
            _credentials_set = true;
        }
#endif // #if USE_APN_LOOKUP
    }
}

void EasyCellularConnection::set_sim_pin(const char *sim_pin)
{
    if (sim_pin && strlen(sim_pin) > 0) {
        _credentials_err = init();
        if (_credentials_err) {
            return;
        }
        _device->set_sim_pin(sim_pin);
    }
}

nsapi_error_t EasyCellularConnection::connect(const char *sim_pin, const char *apn, const char *uname, const char *pwd)
{
    if (_device->is_connected()) {
        return NSAPI_ERROR_IS_CONNECTED;
    }

    set_credentials(apn, uname, pwd);
    if (_credentials_err) {
        return _credentials_err;
    }

    if (sim_pin) {
        set_sim_pin(sim_pin);
    }

    return connect();
}

nsapi_error_t EasyCellularConnection::check_connect()
{
    if (_device->is_connected()) {
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
        err = _device->set_sim_ready();
        if (err == NSAPI_ERROR_OK) {
            char imsi[MAX_IMSI_LENGTH + 1];
            wait(1); // need to wait to access SIM in some modems
            CellularSIM* sim = _device->open_sim(&_serial);
            err = sim->get_imsi(imsi);
            if (err == NSAPI_ERROR_OK) {
                const char *apn_config = apnconfig(imsi);
                if (apn_config) {
                    const char *apn = _APN_GET(apn_config);
                    const char *uname = _APN_GET(apn_config);
                    const char *pwd = _APN_GET(apn_config);
                    tr_info("Looked up APN %s", apn);
                    err = _device->set_credentials(apn, uname, pwd);
                }
            }
            _device->close_sim();
        }
        if (err) {
            tr_error("APN lookup failed");
            return err;
        }
    }
#endif // USE_APN_LOOKUP

    err = _device->connect();
    if (err != NSAPI_ERROR_OK) {
        tr_info("No cellular connection");
        err = NSAPI_ERROR_NO_CONNECTION;
    }
    return err;
}

nsapi_error_t EasyCellularConnection::disconnect()
{
    _credentials_err = NSAPI_ERROR_OK;
    _is_initialized = false;
    _stm_error = false;
#if USE_APN_LOOKUP
    _credentials_set = false;
#endif // #if USE_APN_LOOKUP

    return _device->disconnect();
}

bool EasyCellularConnection::is_connected()
{
    return _device->is_connected();
}

const char *EasyCellularConnection::get_ip_address()
{
    if (!_network) {
        return NULL;
    }
    return _network->get_ip_address();
}

const char *EasyCellularConnection::get_netmask()
{
    if (!_network) {
        return NULL;
    }
    return _network->get_netmask();
}

const char *EasyCellularConnection::get_gateway()
{
    if (!_network) {
        return NULL;
    }
    return _network->get_gateway();
}

void EasyCellularConnection::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _status_cb = status_cb;
}

void EasyCellularConnection::modem_debug_on(bool on)
{
    _device->modem_debug_on(on);
}

void EasyCellularConnection::set_plmn(const char *plmn)
{
    if (plmn && strlen(plmn) > 0) {
        if (!_device) {
            _credentials_err = init();
            if (_credentials_err) {
                return;
            }
        }
        _device->set_plmn(plmn);
    }
}

NetworkStack *EasyCellularConnection::get_stack()
{
    return _device->get_stack();
}

CellularDevice *EasyCellularConnection::get_device() const
{
    return _device;
}

UARTSerial *EasyCellularConnection::get_serial()
{
    return &_serial;
}

} // namespace

#endif // CELLULAR_DEVICE
