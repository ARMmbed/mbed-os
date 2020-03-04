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

namespace mbed {

CellularContext::CellularContext() : _next(0), _stack(0), _pdp_type(DEFAULT_PDP_TYPE),
    _authentication_type(CellularContext::CHAP), _connect_status(NSAPI_STATUS_DISCONNECTED), _status_cb(0),
    _cid(-1), _new_context_set(false), _is_context_active(false), _is_context_activated(false),
    _apn(0), _uname(0), _pwd(0), _cp_netif(0), _retry_array_length(0),
    _retry_count(0), _device(0), _nw(0), _is_blocking(true)
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

void CellularContext::do_connect_with_retry()
{
    do_connect();
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
    }
}

CellularContext::pdp_type_t CellularContext::string_to_pdp_type(const char *pdp_type)
{
    return IPV4V6_PDP_TYPE;
}

}
