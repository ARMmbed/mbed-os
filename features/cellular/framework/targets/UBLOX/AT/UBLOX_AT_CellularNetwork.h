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

#ifndef UBLOX_AT_CELLULAR_NETWORK_H_
#define UBLOX_AT_CELLULAR_NETWORK_H_

#include "CellularLog.h"
#include "drivers/Timer.h"
#include "mbed_wait_api.h"
#include "AT_CellularNetwork.h"
#include "AT_CellularContext.h"

namespace mbed {

class UBLOX_AT_CellularNetwork : public AT_CellularNetwork {
public:
    UBLOX_AT_CellularNetwork(ATHandler &atHandler);
    virtual ~UBLOX_AT_CellularNetwork();

    nsapi_error_t ubx_reboot();

protected:
    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology opRat);
};

} // namespace mbed

#endif // UBLOX_AT_CELLULAR_NETWORK_H_
