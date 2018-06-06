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

#include "TELIT_HE910.h"
#include "TELIT_HE910_CellularPower.h"
#include "TELIT_HE910_CellularNetwork.h"

using namespace mbed;
using namespace events;

TELIT_HE910::TELIT_HE910(EventQueue &queue) : AT_CellularDevice(queue)
{

}

TELIT_HE910::~TELIT_HE910()
{
}

CellularPower *TELIT_HE910::open_power(FileHandle *fh)
{
    if (!_power) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _power = new TELIT_HE910_CellularPower(*atHandler);
            if (!_power) {
                release_at_handler(atHandler);
            }
        }
    }
    return _power;
}

CellularNetwork *TELIT_HE910::open_network(FileHandle *fh)
{
    if (!_network) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _network = new TELIT_HE910_CellularNetwork(*atHandler);
            if (!_network) {
                release_at_handler(atHandler);
            }
        }
    }
    return _network;
}

uint16_t TELIT_HE910::get_send_delay()
{
    return DEFAULT_DELAY_BETWEEN_AT_COMMANDS;
}

