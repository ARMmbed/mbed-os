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

#include "AT_CellularInformation.h"

using namespace mbed;

AT_CellularInformation::AT_CellularInformation(ATHandler &at) : AT_CellularBase(at)
{
}

AT_CellularInformation::~AT_CellularInformation()
{
}

nsapi_error_t AT_CellularInformation::get_manufacturer(char *buf, size_t buf_size)
{
    return get_info("AT+CGMI", buf, buf_size);
}

nsapi_error_t AT_CellularInformation::get_model(char *buf, size_t buf_size)
{
    return get_info("AT+CGMM", buf, buf_size);
}

nsapi_error_t AT_CellularInformation::get_revision(char *buf, size_t buf_size)
{
    return get_info("AT+CGMR", buf, buf_size);
}

nsapi_error_t AT_CellularInformation::get_info(const char *cmd, char *buf, size_t buf_size)
{
    _at.lock();

    _at.cmd_start(cmd);
    _at.cmd_stop();
    _at.set_delimiter(0);
    _at.resp_start();
    _at.read_string(buf, buf_size-1);
    _at.resp_stop();
    _at.set_default_delimiter();

    return _at.unlock_return_error();
}
