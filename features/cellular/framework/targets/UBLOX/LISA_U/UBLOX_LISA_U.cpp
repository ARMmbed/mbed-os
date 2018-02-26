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

#include "UBLOX_LISA_U.h"
#include "UBLOX_LISA_U_CellularNetwork.h"
#include "UBLOX_LISA_U_CellularPower.h"

using namespace mbed;
using namespace events;

UBLOX_LISA_U::UBLOX_LISA_U(EventQueue &queue) : AT_CellularDevice(queue)
{
}

UBLOX_LISA_U::~UBLOX_LISA_U()
{
}

CellularNetwork *UBLOX_LISA_U::open_network(FileHandle *fh)
{
    if (!_network) {
        _network = new UBLOX_LISA_U_CellularNetwork(*get_at_handler(fh));
    }
    return _network;
}

CellularPower *UBLOX_LISA_U::open_power(FileHandle *fh)
{
    if (!_power) {
        _power = new UBLOX_LISA_U_CellularPower(*get_at_handler(fh));
    }
    return _power;
}
