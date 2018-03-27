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

#ifndef UBLOX_PPP_H_
#define UBLOX_PPP_H_

#include "AT_CellularDevice.h"

namespace mbed {

class UBLOX_PPP : public AT_CellularDevice
{

public:
    UBLOX_PPP(events::EventQueue &queue);
    virtual ~UBLOX_PPP();

public: // CellularDevice
    virtual CellularNetwork *open_network(FileHandle *fh);
    virtual CellularPower *open_power(FileHandle *fh);
};

MBED_DEPRECATED_SINCE("mbed-os-5.9", "This API will be deprecated, Use UBLOX_PPP instead of UBLOX_LISA_U.")
class UBLOX_LISA_U : public UBLOX_PPP
{
};

} // namespace mbed

#endif // UBLOX_PPP_H_
