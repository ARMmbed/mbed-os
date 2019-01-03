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

#include "QUECTEL_M26_CellularPower.h"

using namespace mbed;

QUECTEL_M26_CellularPower::QUECTEL_M26_CellularPower(ATHandler &atHandler) : AT_CellularPower(atHandler)
{

}

QUECTEL_M26_CellularPower::~QUECTEL_M26_CellularPower()
{

}

nsapi_error_t QUECTEL_M26_CellularPower::set_at_mode()
{
    _at.lock();
    _at.cmd_start("AT");
    _at.cmd_stop_read_resp();

    _at.cmd_start("AT+CMEE="); // verbose responses
    _at.write_int(1);
    _at.cmd_stop_read_resp();

    return _at.unlock_return_error();
}

nsapi_error_t QUECTEL_M26_CellularPower::on()
{

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_M26_CellularPower::off()
{
    _at.lock();
    _at.cmd_start("AT+QPOWD=0");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    return _at.unlock_return_error();;
}
