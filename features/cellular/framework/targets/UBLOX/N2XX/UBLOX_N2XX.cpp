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

#include "UBLOX_N2XX.h"
#include "UBLOX_N2XX_CellularNetwork.h"
#include "UBLOX_N2XX_CellularPower.h"
#include "UBLOX_N2XX_CellularContext.h"
#include "UBLOX_N2XX_CellularSIM.h"
#include "UBLOX_N2XX_CellularSMS.h"

using namespace mbed;
using namespace events;

static const AT_CellularBase::SupportedFeature unsupported_features[] =  {
    AT_CellularBase::AT_CellularSMSMmodeText,
    AT_CellularBase::AT_CMGF,
    AT_CellularBase::AT_CNMI,
    AT_CellularBase::AT_CSMP,
    AT_CellularBase::AT_CSDH,
    AT_CellularBase::SUPPORTED_FEATURE_END_MARK
};

UBLOX_N2XX::UBLOX_N2XX(FileHandle *fh): AT_CellularDevice(fh)
{
    AT_CellularBase::set_unsupported_features(unsupported_features);
}

UBLOX_N2XX::~UBLOX_N2XX()
{
}

AT_CellularNetwork *UBLOX_N2XX::open_network_impl(ATHandler &at)
{
    return new UBLOX_N2XX_CellularNetwork(at);
}

AT_CellularPower *UBLOX_N2XX::open_power_impl(ATHandler &at)
{
    return new UBLOX_N2XX_CellularPower(at);
}

AT_CellularContext *UBLOX_N2XX::create_context_impl(ATHandler &at, const char *apn)
{
    return new UBLOX_N2XX_CellularContext(at, this, apn);
}

AT_CellularSIM *UBLOX_N2XX::open_sim_impl(ATHandler &at)
{
    return new UBLOX_N2XX_CellularSIM(at);
}

AT_CellularSMS *UBLOX_N2XX::open_sms_impl(ATHandler &at)
{
    return new UBLOX_N2XX_CellularSMS(at);
}
