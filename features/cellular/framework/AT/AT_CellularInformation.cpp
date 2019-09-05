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

#include <stdio.h>

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

nsapi_error_t AT_CellularInformation::get_serial_number(char *buf, size_t buf_size, SerialNumberType type)
{
    if (buf == NULL || buf_size == 0) {
        return NSAPI_ERROR_PARAMETER;
    }

    if (type == SN) {
        return get_info("AT+CGSN", buf, buf_size);
    }

    if (!get_property(PROPERTY_AT_CGSN_WITH_TYPE)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    return _at.at_cmd_str("+CGSN", "=", buf, buf_size, "%d", type);
}

nsapi_error_t AT_CellularInformation::get_info(const char *cmd, char *buf, size_t buf_size)
{
    if (buf == NULL || buf_size == 0) {
        return NSAPI_ERROR_PARAMETER;
    }
    _at.lock();
    _at.cmd_start(cmd);
    _at.cmd_stop();
    _at.set_delimiter(0);
    _at.resp_start();
    _at.read_string(buf, buf_size);
    _at.resp_stop();
    _at.set_default_delimiter();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularInformation::get_imsi(char *imsi, size_t buf_size)
{
    if (imsi == NULL || buf_size == 0 || buf_size < MAX_IMSI_LENGTH + 1) {
        return NSAPI_ERROR_PARAMETER;
    }
    _at.lock();
    _at.cmd_start_stop("+CIMI", "");
    _at.resp_start();
    _at.read_string(imsi, MAX_IMSI_LENGTH + 1);
    _at.resp_stop();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularInformation::get_iccid(char *buf, size_t buf_size)
{
    if (buf == NULL || buf_size == 0) {
        return NSAPI_ERROR_PARAMETER;
    }
    return _at.at_cmd_str("+CCID", "?", buf, buf_size);
}
