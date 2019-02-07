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

#ifndef UBLOX_N2XX_CELLULAR_SIM_H_
#define UBLOX_N2XX_CELLULAR_SIM_H_

#include "AT_CellularSIM.h"

namespace mbed {

class UBLOX_N2XX_CellularSIM : public AT_CellularSIM {

public:

    UBLOX_N2XX_CellularSIM(ATHandler &atHandler);
    virtual ~UBLOX_N2XX_CellularSIM();

public:

    virtual nsapi_error_t set_pin(const char *sim_pin);

    virtual nsapi_error_t change_pin(const char *sim_pin, const char *new_pin);

    virtual nsapi_error_t set_pin_query(const char *sim_pin, bool query_pin);

    virtual nsapi_error_t get_sim_state(SimState &state);

private:

    static const int MAX_SIM_RESPONSE_LENGTH = 25;

    // URC handlers
    void NPIN_URC();

    char simstr[MAX_SIM_RESPONSE_LENGTH];
};

} // namespace mbed

#endif // UBLOX_N2XX_CELLULAR_SIM_H_
