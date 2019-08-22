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
#include "CellularContext.h"
#include "CellularLog.h"
#include "ThisThread.h"

MBED_WEAK CellularInterface *CellularInterface::get_target_default_instance()
{
    return mbed::CellularContext::get_default_instance();
}

namespace mbed {

MBED_WEAK CellularContext *CellularContext::get_default_instance()
{
    CellularDevice *dev = CellularDevice::get_default_instance();
    if (!dev) {
        return NULL;
    }

    static CellularContext *context = dev->create_context(NULL, NULL, MBED_CONF_CELLULAR_CONTROL_PLANE_OPT);
#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
#if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
    context->set_file_handle(static_cast<UARTSerial *>(&dev->get_file_handle()), MDMDCD, MDM_PIN_POLARITY);
#endif // #if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
#endif // #if DEVICE_SERIAL
    return context;
}

MBED_WEAK CellularContext *CellularContext::get_default_nonip_instance()
{
    // Uses default APN, uname, password from mbed_app.json
    CellularDevice *dev = CellularDevice::get_default_instance();
    if (!dev) {
        return NULL;
    }

    static CellularContext *context = dev->create_context(NULL, NULL, MBED_CONF_CELLULAR_CONTROL_PLANE_OPT, true);
#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
#if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
    context->set_file_handle(static_cast<UARTSerial *>(&dev->get_file_handle()), MDMDCD, MDM_PIN_POLARITY);
#endif // #if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
#endif // #if DEVICE_SERIAL
    return context;
}

CellularContext::CellularContext() : _next(0), _stack(0), _pdp_type(DEFAULT_PDP_TYPE),
    _authentication_type(CellularContext::CHAP), _connect_status(NSAPI_STATUS_DISCONNECTED), _status_cb(0),
    _cid(-1), _new_context_set(false), _is_context_active(false), _is_context_activated(false),
    _apn(0), _uname(0), _pwd(0), _dcd_pin(NC), _active_high(false), _cp_netif(0), _retry_array_length(0),
    _retry_count(0), _device(0), _nw(0), _is_blocking(true), _nonip_req(false), _cp_in_use(false)
{
    memset(_retry_timeout_array, 0, CELLULAR_RETRY_ARRAY_SIZE);
}

void CellularContext::cp_data_received()
{
    _cp_netif->data_received();
}

CellularDevice *CellularContext::get_device() const
{
    return _device;
}

int CellularContext::get_cid() const
{
    return _cid;
}

void CellularContext::set_authentication_type(AuthenticationType type)
{
    _authentication_type = type;
}

void CellularContext::validate_ip_address()
{
    const int IP_MAX_TRIES = 10; // maximum of 2 seconds as we wait 200ms between tries
    const int IP_WAIT_INTERVAL = 200; // 200 ms between retries
    const char *ip = NULL;
    int i = 0;

    while (1) {
        ip = get_ip_address();
        if (ip || i >= IP_MAX_TRIES) {
            if (ip == NULL) {
                tr_warning("Connected but no local ip address");
            } else {
                tr_info("Cellular local IP: %s", ip);
            }
            break;
        }
        rtos::ThisThread::sleep_for(IP_WAIT_INTERVAL);
        i++;
    }
}

void CellularContext::do_connect_with_retry()
{
    if (_cb_data.final_try) {
        _cb_data.final_try = false;
        _cb_data.error = NSAPI_ERROR_NO_CONNECTION;
        call_network_cb(NSAPI_STATUS_DISCONNECTED);
        return;
    }
    do_connect();
    if (_cb_data.error == NSAPI_ERROR_OK) {
        // Some modems don't get the ip address right after connect so we must
        // validate it but even if we don't get ip we still send NSAPI_STATUS_GLOBAL_UP
        if (!_nonip_req && !_cp_in_use) { // don't validate if non-ip case
            validate_ip_address();
        }
        call_network_cb(NSAPI_STATUS_GLOBAL_UP);
        return;
    }

    if (_retry_count == 0) {
        _device->get_retry_timeout_array(_retry_timeout_array, _retry_array_length);
    }

    if (_is_blocking) {
        while (_retry_count < _retry_array_length) {
            tr_debug("SYNC do_connect failed with %d, retry after %d seconds", _cb_data.error, _retry_timeout_array[_retry_count]);
            rtos::ThisThread::sleep_for(_retry_timeout_array[_retry_count] * 1000);
            do_connect();
            if (_cb_data.error == NSAPI_ERROR_OK) {
                return;
            }
            _retry_count++;
        }
    } else {
        if (_retry_count < _retry_array_length) {
            if (_retry_count == _retry_array_length - 1) {
                // set the flag that this is the last try for ppp connect / pdp context activate
                _cb_data.final_try = true;
            }
            tr_debug("ASYNC do_connect failed with %d, retry after %d seconds", _cb_data.error, _retry_timeout_array[_retry_count]);
            int id = _device->get_queue()->call_in(_retry_timeout_array[_retry_count] * 1000, this, &CellularContext::do_connect_with_retry);
            if (id == 0) {
                tr_error("Failed call via eventqueue in do_connect_with_retry()");
#if !NSAPI_PPP_AVAILABLE
                _cb_data.final_try = true;
                _cb_data.error = NSAPI_ERROR_NO_MEMORY;
                // in PPP mode we did not activate any context, just searched the correct _cid
                if (_status_cb) {
                    _status_cb((nsapi_event_t)CellularActivatePDPContext, (intptr_t)&_cb_data);
                }
                _cb_data.final_try = false;
                _cb_data.error = NSAPI_ERROR_OK;
#else
                call_network_cb(NSAPI_STATUS_DISCONNECTED);
#endif // !NSAPI_PPP_AVAILABLE
            }
            _retry_count++;
            return; // don't call NSAPI_STATUS_DISCONNECTED in every failure, only the last one.
        }
    }

#if NSAPI_PPP_AVAILABLE
    if (_cb_data.error != NSAPI_ERROR_OK) {
        tr_error("Failed to open data channel!");
        call_network_cb(NSAPI_STATUS_DISCONNECTED);
    }
#endif // #if NSAPI_PPP_AVAILABLE
}

void CellularContext::do_connect()
{
    _cb_data.error = NSAPI_ERROR_OK;
}

void CellularContext::call_network_cb(nsapi_connection_status_t status)
{
    if (_connect_status != status) {
        _connect_status = status;
        if (_status_cb) {
            _status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connect_status);
        }
        if (_nw && _connect_status == NSAPI_STATUS_DISCONNECTED) {
            tr_info("CellularContext disconnected");
        }
    }
}

} // namespace mbed
