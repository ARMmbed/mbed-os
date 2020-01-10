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
    _at(at), _cid(cid), _cb(NULL), _data(NULL), _device(device)
{
}

AT_ControlPlane_netif::~AT_ControlPlane_netif()
{}

void AT_ControlPlane_netif::urc_cp_recv()
{
}

nsapi_size_or_error_t AT_ControlPlane_netif::send(const void *cpdata, nsapi_size_t cpdata_length)
{
    return cpdata_length;
}

nsapi_size_or_error_t AT_ControlPlane_netif::recv(void *cpdata, nsapi_size_t cpdata_length)
{
    return cpdata_length;
}

void AT_ControlPlane_netif::attach(void (*callback)(void *), void *data)
{
}

void AT_ControlPlane_netif::data_received()
{
}

} //mbed namespace
