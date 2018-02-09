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

#include "UBLOX_C027_CellularNetwork.h"
#include "UBLOX_C027_CellularPower.h"
#include "UBLOX_C027.h"

using namespace mbed;
using namespace events;

UBLOX_C027::UBLOX_C027(EventQueue &queue) : AT_CellularDevice(queue)
{
}

UBLOX_C027::~UBLOX_C027()
{
}

CellularNetwork *UBLOX_C027::open_network(FileHandle *fh)
{
    if (!_network) {
        _network = new UBLOX_C027_CellularNetwork(*get_at_handler(fh));
    }
    return _network;
}

CellularPower *UBLOX_C027::open_power(FileHandle *fh)
{
    if (!_power) {
        _power = new UBLOX_C027_CellularPower(*get_at_handler(fh));
    }
    return _power;
}
