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

#include "QUECTEL_BC95_CellularNetwork.h"
#include "QUECTEL_BC95_CellularPower.h"
#include "QUECTEL_BC95_CellularSIM.h"

#include "QUECTEL_BC95.h"

#define CONNECT_DELIM         "\r\n"
#define CONNECT_BUFFER_SIZE   (1280 + 80 + 80) // AT response + sscanf format
#define CONNECT_TIMEOUT       8000

#define MAX_STARTUP_TRIALS 5
#define MAX_RESET_TRIALS 5

using namespace events;
using namespace mbed;

QUECTEL_BC95::QUECTEL_BC95(EventQueue &queue) : AT_CellularDevice(queue)
{
}

QUECTEL_BC95::~QUECTEL_BC95()
{
}

CellularNetwork *QUECTEL_BC95::open_network(FileHandle *fh)
{
    if (!_network) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _network = new QUECTEL_BC95_CellularNetwork(*atHandler);
            if (!_network) {
                release_at_handler(atHandler);
            }
        }
    }
    return _network;
}

CellularPower *QUECTEL_BC95::open_power(FileHandle *fh)
{
    if (!_power) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _power = new QUECTEL_BC95_CellularPower(*atHandler);
            if (!_power) {
                release_at_handler(atHandler);
            }
        }
    }
    return _power;
}

CellularSIM *QUECTEL_BC95::open_sim(FileHandle *fh)
{
    if (!_sim) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _sim = new QUECTEL_BC95_CellularSIM(*atHandler);
            if (!_sim) {
                release_at_handler(atHandler);
            }
        }
    }
    return _sim;
}
