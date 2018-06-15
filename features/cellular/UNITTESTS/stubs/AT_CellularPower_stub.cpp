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

#include "AT_CellularPower.h"
#include "CellularUtil.h"
#include "CellularLog.h"

using namespace mbed_cellular_util;
using namespace mbed;

AT_CellularPower::AT_CellularPower(ATHandler &at) : AT_CellularBase(at)
{
}

AT_CellularPower::~AT_CellularPower()
{
}

nsapi_error_t AT_CellularPower::on()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularPower::off()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularPower::set_at_mode()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularPower::set_power_level(int func_level, int do_reset)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularPower::reset()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularPower::opt_power_save_mode(int periodic_time, int active_time)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularPower::opt_receive_period(int mode, EDRXAccessTechnology act_type, uint8_t edrx_value)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularPower::set_device_ready_urc_cb(mbed::Callback<void()> callback)
{
    return NSAPI_ERROR_OK;
}

void AT_CellularPower::remove_device_ready_urc_cb(mbed::Callback<void()> callback){

}

nsapi_error_t AT_CellularPower::is_device_ready()
{
    return NSAPI_ERROR_OK;
}
