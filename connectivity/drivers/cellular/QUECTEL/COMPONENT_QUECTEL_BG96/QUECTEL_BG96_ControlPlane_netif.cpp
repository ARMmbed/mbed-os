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

#include "CellularUtil.h"
#include "QUECTEL_BG96_ControlPlane_netif.h"
#include "CellularLog.h"

using namespace mbed_cellular_util;

namespace mbed {

QUECTEL_BG96_ControlPlane_netif::QUECTEL_BG96_ControlPlane_netif(ATHandler &at, int cid, AT_CellularDevice &device) : AT_ControlPlane_netif(at, cid, device)
{}

nsapi_size_or_error_t QUECTEL_BG96_ControlPlane_netif::send(const void *data, nsapi_size_t size)
{
    if (size > 100) { // from BG96_NIDD_AT_Commands_Manual_V1.0
        return NSAPI_ERROR_PARAMETER;
    }

    _at.lock();
    _at.cmd_start("AT+QCFGEXT=\"nipds\",1,");
    _at.write_hex_string((const char *)data, size);
    _at.write_int(2 * size);
    _at.cmd_stop_read_resp();
    nsapi_error_t err = _at.unlock_return_error();

    return (err == NSAPI_ERROR_OK) ? size : err;
}

nsapi_size_or_error_t QUECTEL_BG96_ControlPlane_netif::recv(void *buffer, nsapi_size_t size)
{
    _at.lock();

    _at.cmd_start_stop("+QCFGEXT", "=", "%s%d", "nipdr", 0);
    _at.resp_start("+QCFGEXT:");
    // skip 3 params: "nipdr",<total_receive_length>,<have_read_length>
    _at.skip_param(3);
    // get to <unread_length>
    int unread_length = _at.read_int();
    _at.resp_stop();

    if (!unread_length || unread_length == -1) {
        _at.unlock();
        return NSAPI_ERROR_WOULD_BLOCK;
    }
    if ((nsapi_size_t)unread_length > size) {
        tr_warn("recv %d/%d", size, unread_length);
        unread_length = size;
    }

    _at.cmd_start_stop("+QCFGEXT", "=", "%s%d%d", "nipdr", unread_length, 1);
    _at.resp_start("+QCFGEXT:");
    _at.skip_param(); // skip "nipdr"
    nsapi_size_t read_length = _at.read_int();
    ssize_t read_len = _at.read_hex_string((char *)buffer, read_length);
    _at.resp_stop();

    nsapi_error_t err = _at.unlock_return_error();

    if (err == NSAPI_ERROR_OK && read_len) {
        return read_len;
    }

    return NSAPI_ERROR_WOULD_BLOCK;
}

} // mbed namespace
