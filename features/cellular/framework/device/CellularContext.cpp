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

void CellularContext::cp_data_received()
{
    _cp_netif->data_received();
}

CellularDevice *CellularContext::get_device() const
{
    return _device;
}

void CellularContext::do_connect_with_retry()
{
    if (_cb_data.final_try) {
        _cb_data.final_try = false;
        _cb_data.error == NSAPI_ERROR_NO_CONNECTION;
        call_network_cb(NSAPI_STATUS_DISCONNECTED);
        return;
    }
    do_connect();
    if (_cb_data.error == NSAPI_ERROR_OK) {
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
