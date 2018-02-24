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

#ifndef AT_CELLULAR_SIM_H_
#define AT_CELLULAR_SIM_H_

#include "CellularSIM.h"
#include "AT_CellularBase.h"

namespace mbed {

/**
 *  Class AT_CellularSIM
 *
 *  Class for SIM card handling.
 */
class AT_CellularSIM : public CellularSIM, public AT_CellularBase
{

public:
    AT_CellularSIM(ATHandler &atHandler);
    virtual ~AT_CellularSIM();

public:
    virtual nsapi_error_t set_pin(const char *sim_pin);

    virtual nsapi_error_t change_pin(const char *sim_pin, const char *new_pin);

    virtual nsapi_error_t set_pin_query(const char *sim_pin, bool query_pin);

    virtual nsapi_error_t get_sim_state(SimState &state);

    virtual nsapi_error_t get_imsi(char* imsi);
};

} // namespace mbed

#endif // AT_CELLULAR_SIM_H_
