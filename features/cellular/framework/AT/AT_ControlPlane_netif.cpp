/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "ThisThread.h"
#include "AT_ControlPlane_netif.h"
#include "CellularLog.h"

using namespace mbed_cellular_util;

namespace mbed {

AT_ControlPlane_netif::AT_ControlPlane_netif(ATHandler &at, int cid, AT_CellularDevice &device) :
    _cid(cid), _cb(NULL), _data(NULL), _at(at), _device(device)
{
    _at.set_urc_handler("+CRTDCP:", mbed::Callback<void()>(this, &AT_ControlPlane_netif::urc_cp_recv));
}

AT_ControlPlane_netif::~AT_ControlPlane_netif()
{}

void AT_ControlPlane_netif::urc_cp_recv()
{
    //+CRTDCP: <cid>,<cpdata_length>,<cpdata>
    _at.lock();
    int cid = _at.read_int();
    int cpdata_length = _at.read_int();
    if (cpdata_length < 0) {
        return;
    }
    uint8_t *cpdata = new uint8_t[cpdata_length];
    ssize_t read_len = _at.read_hex_string((char *)cpdata, cpdata_length);

    // cid not expected to be different because: one context - one file handle
    // so this file handle cannot get urc from different context
    if (read_len > 0 && read_len == cpdata_length && cid == _cid) {
        packet_t *packet = _packet_list.add_new();
        packet->data = cpdata;
        packet->data_len = cpdata_length;
        data_received();
    } else {
        delete[] cpdata;
    }
    _at.unlock();
}

nsapi_size_or_error_t AT_ControlPlane_netif::send(const void *cpdata, nsapi_size_t cpdata_length)
{
    if (cpdata_length > MBED_CONF_CELLULAR_MAX_CP_DATA_RECV_LEN) {
        return NSAPI_ERROR_PARAMETER;
    }

    _at.lock();
    _at.cmd_start("AT+CSODCP=");
    _at.write_int(_cid);
    _at.write_int(cpdata_length);
    _at.write_hex_string((char *)cpdata, cpdata_length);
    _at.cmd_stop_read_resp();
    nsapi_size_or_error_t err = _at.unlock_return_error();

    return err ? err : cpdata_length;
}

nsapi_size_or_error_t AT_ControlPlane_netif::recv(void *cpdata, nsapi_size_t cpdata_length)
{
    _at.lock();
    if (_packet_list.count() <= 0) {
        (void) send("", 0); // poll for missing +CRTDCP indications
        if (_packet_list.count() <= 0) {
            return NSAPI_ERROR_WOULD_BLOCK;
        }
    }
    packet_t *packet = _packet_list.dequeue();
    int data_len = (cpdata_length >= packet->data_len) ? packet->data_len : cpdata_length;
    memcpy(cpdata, packet->data, data_len);
    delete[] packet->data;
    delete (packet);
    _at.unlock();
    return data_len;
}

void AT_ControlPlane_netif::attach(void (*callback)(void *), void *data)
{
    _cb = callback;
    _data = data;
}

void AT_ControlPlane_netif::data_received()
{
    // call socket event
    if (!_cb) {
        return;
    }
    _cb(_data);
}

} //mbed namespace
