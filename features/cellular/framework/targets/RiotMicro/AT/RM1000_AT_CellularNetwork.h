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

#ifndef RM1000_AT_CELLULAR_NETWORK_H_
#define RM1000_AT_CELLULAR_NETWORK_H_

#include "AT_CellularNetwork.h"

namespace mbed {

class RM1000_AT_CellularNetwork : public AT_CellularNetwork {
public:
    RM1000_AT_CellularNetwork(ATHandler &atHandler);
    virtual ~RM1000_AT_CellularNetwork();

    virtual nsapi_error_t set_registration_urc(RegistrationType type, bool on);

protected:
    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology opRat);
private:
    // URC handlers
    void MODEM_FAULT_URC();
};

} // namespace mbed

#endif // RM1000_AT_CELLULAR_NETWORK_H_
