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

#ifndef TELIT_ME910_CELLULAR_NETWORK_H_
#define TELIT_ME910_CELLULAR_NETWORK_H_

#include "AT_CellularNetwork.h"

namespace mbed {

class TELIT_ME910_CellularNetwork : public AT_CellularNetwork {
public:
    TELIT_ME910_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device);
    virtual ~ TELIT_ME910_CellularNetwork();

protected:
    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology opRat);

};

} // namespace mbed

#endif // TELIT_ME910_CELLULAR_NETWORK_H_
