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

#ifndef GEMALTO_CINTERION_H_
#define GEMALTO_CINTERION_H_

#include "AT_CellularDevice.h"

namespace mbed {

class GEMALTO_CINTERION : public AT_CellularDevice {
public:

    GEMALTO_CINTERION(events::EventQueue &queue);
    virtual ~GEMALTO_CINTERION();

public: // CellularDevice
    virtual CellularNetwork *open_network(FileHandle *fh);
    virtual nsapi_error_t init_module(FileHandle *fh);
};

} // namespace mbed

#endif // GEMALTO_CINTERION_H_
