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

#ifndef AT_CELLULAR_POWER_H_
#define AT_CELLULAR_POWER_H_

#include "CellularPower.h"
#include "AT_CellularBase.h"

namespace mbed {

/**
 *  Class AT_CellularPower
 *
 *  Class that provides power handling functions for modem/module.
 */
class AT_CellularPower : public CellularPower, public AT_CellularBase
{
public:
    AT_CellularPower(ATHandler &atHandler);
    virtual ~AT_CellularPower();

public:
    virtual nsapi_error_t on();

    virtual nsapi_error_t off();

    virtual nsapi_error_t set_at_mode();

    virtual nsapi_error_t set_power_level(int func_level);

    virtual nsapi_error_t reset();

    virtual nsapi_error_t opt_power_save_mode(int periodic_time, int active_time);

    virtual nsapi_error_t opt_receive_period(int mode, EDRXAccessTechnology act_type, uint8_t edrx_value);
};

} // namespace mbed

#endif /* AT_CELLULAR_POWER_H_ */
