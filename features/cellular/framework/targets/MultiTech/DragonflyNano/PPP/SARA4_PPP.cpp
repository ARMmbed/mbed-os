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

#include "SARA4_PPP.h"
#include "SARA4_PPP_CellularNetwork.h"
#include "SARA4_PPP_CellularPower.h"
#include "SARA4_PPP_CellularContext.h"

using namespace mbed;
using namespace events;

static const AT_CellularBase::SupportedFeature unsupported_features[] =  {
    AT_CellularBase::AT_CGSN_WITH_TYPE,
    AT_CellularBase::AT_CGDATA,
    AT_CellularBase::SUPPORTED_FEATURE_END_MARK
};

SARA4_PPP::SARA4_PPP(FileHandle *fh) : AT_CellularDevice(fh)
{
    AT_CellularBase::set_unsupported_features(unsupported_features);
}

SARA4_PPP::~SARA4_PPP()
{
}

AT_CellularNetwork *SARA4_PPP::open_network_impl(ATHandler &at)
{
    return new SARA4_PPP_CellularNetwork(at);
}

AT_CellularPower *SARA4_PPP::open_power_impl(ATHandler &at)
{
    return new SARA4_PPP_CellularPower(at);
}

AT_CellularContext *SARA4_PPP::create_context_impl(ATHandler &at, const char *apn)
{
    return new SARA4_PPP_CellularContext(at, this, apn);
}
