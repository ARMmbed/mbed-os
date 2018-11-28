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
#include "CellularTargets.h"
#include "nsapi_types.h"

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
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularPower::off()
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularPower::set_at_mode()
{
    _at.lock();
    _at.flush();
    _at.cmd_start("ATE0"); // echo off
    _at.cmd_stop_read_resp();

    _at.cmd_start("AT+CMEE=1"); // verbose responses
    _at.cmd_stop_read_resp();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularPower::set_power_level(int func_level, int do_reset)
{
    _at.lock();
    _at.cmd_start("AT+CFUN=");
    _at.write_int(func_level);
    _at.write_int(do_reset);
    _at.cmd_stop_read_resp();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularPower::reset()
{
    _at.lock();
    _at.cmd_start("AT+CFUN=");// reset to full power levels
    _at.write_int(1);
    _at.write_int(1);
    _at.cmd_stop_read_resp();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularPower::is_device_ready()
{
    _at.lock();
    _at.cmd_start("AT");
    _at.cmd_stop_read_resp();

    // we need to do this twice because for example after data mode the first 'AT' command will give modem a
    // stimulus that we are back to command mode.
    _at.clear_error();
    _at.cmd_start("AT");
    _at.cmd_stop_read_resp();

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularPower::set_device_ready_urc_cb(mbed::Callback<void()> callback)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

void AT_CellularPower::remove_device_ready_urc_cb(mbed::Callback<void()> callback)
{
}
