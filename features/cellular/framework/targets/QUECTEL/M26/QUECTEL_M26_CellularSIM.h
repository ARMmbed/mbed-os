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

#ifndef QUECTEL_M26_CELLULAR_SIM_H_
#define QUECTEL_M26_CELLULAR_SIM_H_

#include "AT_CellularSIM.h"

namespace mbed {

class QUECTEL_M26_CellularSIM : public AT_CellularSIM {
public:
    QUECTEL_M26_CellularSIM(ATHandler &atHandler);
    virtual ~QUECTEL_M26_CellularSIM();

public: //from CellularSIM
    virtual nsapi_error_t get_sim_state(SimState &state);
    virtual nsapi_error_t get_iccid(char *buf, size_t buf_size);
    virtual nsapi_error_t change_pin(const char *sim_pin, const char *new_pin);
    virtual nsapi_error_t set_pin_query(const char *sim_pin, bool query_pin);
};

} // namespace mbed

#endif // QUECTEL_M26_CELLULAR_SIM_H_
