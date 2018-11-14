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

#include "QUECTEL_UG96.h"
#include "QUECTEL_UG96_CellularNetwork.h"
#include "QUECTEL_UG96_CellularPower.h"
#include "QUECTEL_UG96_CellularContext.h"

using namespace mbed;
using namespace events;

#define CONNECT_DELIM         "\r\n"
#define CONNECT_BUFFER_SIZE   (1280 + 80 + 80) // AT response + sscanf format
#define CONNECT_TIMEOUT       8000

#define MAX_STARTUP_TRIALS 5
#define MAX_RESET_TRIALS 5

QUECTEL_UG96::QUECTEL_UG96(FileHandle *fh) : AT_CellularDevice(fh)
{
}

QUECTEL_UG96::~QUECTEL_UG96()
{
}

AT_CellularNetwork *QUECTEL_UG96::open_network_impl(ATHandler &at)
{
    return new QUECTEL_UG96_CellularNetwork(at);
}

AT_CellularPower *QUECTEL_UG96::open_power_impl(ATHandler &at)
{
    return new QUECTEL_UG96_CellularPower(at);
}

AT_CellularContext *QUECTEL_UG96::create_context_impl(ATHandler &at, const char *apn)
{
    return new QUECTEL_UG96_CellularContext(at, this, apn);
}
