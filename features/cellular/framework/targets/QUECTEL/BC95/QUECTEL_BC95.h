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

#ifndef QUECTEL_BC95_H_
#define QUECTEL_BC95_H_

#include "AT_CellularDevice.h"

namespace mbed {

class QUECTEL_BC95 : public AT_CellularDevice
{
public:

    QUECTEL_BC95(events::EventQueue &queue);
    virtual ~QUECTEL_BC95();

public: // CellularDevice
    virtual CellularNetwork *open_network(FileHandle *fh);
    virtual CellularPower *open_power(FileHandle *fh);
    virtual CellularSIM *open_sim(FileHandle *fh);

public: // NetworkInterface
    void handle_urc(FileHandle *fh);
};
} // namespace mbed
#endif // QUECTEL_BC95_H_
