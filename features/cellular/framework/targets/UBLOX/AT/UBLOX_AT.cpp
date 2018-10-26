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

#include "UBLOX_AT.h"
#include "UBLOX_AT_CellularNetwork.h"
#include "UBLOX_AT_CellularPower.h"

using namespace mbed;
using namespace events;

#ifdef TARGET_UBLOX_C030_R410M
static const AT_CellularBase::SupportedFeature unsupported_features[] =  {
    AT_CellularBase::AT_CGSN_WITH_TYPE,
    AT_CellularBase::SUPPORTED_FEATURE_END_MARK
};
#endif

UBLOX_AT::UBLOX_AT(EventQueue &queue) : AT_CellularDevice(queue)
{
#ifdef TARGET_UBLOX_C030_R410M
    AT_CellularBase::set_unsupported_features(unsupported_features);
#endif
}

UBLOX_AT::~UBLOX_AT()
{
}

CellularNetwork *UBLOX_AT::open_network(FileHandle *fh)
{
    if (!_network) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _network = new UBLOX_AT_CellularNetwork(*atHandler);
            if (!_network) {
                release_at_handler(atHandler);
            }
        }
    }
    return _network;
}

CellularPower *UBLOX_AT::open_power(FileHandle *fh)
{
    if (!_power) {
        ATHandler *atHandler = get_at_handler(fh);
        if (atHandler) {
            _power = new UBLOX_AT_CellularPower(*atHandler);
            if (!_power) {
                release_at_handler(atHandler);
            }
        }
    }
    return _power;
}
