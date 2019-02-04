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

#include "QUECTEL_M26_CellularNetwork.h"
#include "QUECTEL_M26_CellularPower.h"
#include "QUECTEL_M26_CellularSIM.h"
#include "QUECTEL_M26_CellularContext.h"
#include "QUECTEL_M26.h"

using namespace events;
using namespace mbed;

#define CONNECT_DELIM         "\r\n"
#define CONNECT_BUFFER_SIZE   (1280 + 80 + 80) // AT response + sscanf format
#define CONNECT_TIMEOUT       8000

#define MAX_STARTUP_TRIALS 5
#define MAX_RESET_TRIALS 5

static const AT_CellularBase::SupportedFeature unsupported_features[] =  {
    AT_CellularBase::AT_CGSN_WITH_TYPE,
    AT_CellularBase::AT_CGAUTH,
    AT_CellularBase::SUPPORTED_FEATURE_END_MARK
};

QUECTEL_M26::QUECTEL_M26(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_unsupported_features(unsupported_features);
}

QUECTEL_M26::~QUECTEL_M26()
{
}

AT_CellularNetwork *QUECTEL_M26::open_network_impl(ATHandler &at)
{
    return new QUECTEL_M26_CellularNetwork(at);
}

AT_CellularPower *QUECTEL_M26::open_power_impl(ATHandler &at)
{
    return new QUECTEL_M26_CellularPower(at);
}

AT_CellularSIM *QUECTEL_M26::open_sim_impl(ATHandler &at)
{
    return new QUECTEL_M26_CellularSIM(at);
}

AT_CellularContext *QUECTEL_M26::create_context_impl(ATHandler &at, const char *apn)
{
    return new QUECTEL_M26_CellularContext(at, this, apn);
}
