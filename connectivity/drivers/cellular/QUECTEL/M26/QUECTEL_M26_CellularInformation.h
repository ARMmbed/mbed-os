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

#ifndef QUECTEL_M26_CELLULAR_INFORMATION_H_
#define QUECTEL_M26_CELLULAR_INFORMATION_H_

#include "AT_CellularInformation.h"

namespace mbed {

class QUECTEL_M26_CellularInformation : public AT_CellularInformation {
public:
    QUECTEL_M26_CellularInformation(ATHandler &atHandler, AT_CellularDevice &device);

public: //from CellularInformation
    virtual nsapi_error_t get_iccid(char *buf, size_t buf_size);
};

} // namespace mbed

#endif // QUECTEL_M26_CELLULAR_INFORMATION_H_
