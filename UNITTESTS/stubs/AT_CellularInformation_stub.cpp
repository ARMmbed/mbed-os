/*
 * Copyright (c) , Arm Limited and affiliates.
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
#include "nsapi_types.h"

using namespace mbed;

AT_CellularInformation::AT_CellularInformation(ATHandler &at, AT_CellularDevice &device) : _at(at), _device(device)
{
}

AT_CellularInformation::~AT_CellularInformation()
{
}

nsapi_error_t AT_CellularInformation::get_manufacturer(char *buf, size_t buf_size)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularInformation::get_model(char *buf, size_t buf_size)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularInformation::get_revision(char *buf, size_t buf_size)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularInformation::get_serial_number(char *buf, size_t buf_size, SerialNumberType type)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularInformation::get_imsi(char *imsi, size_t buf_size)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularInformation::get_iccid(char *buf, size_t buf_size)
{
    return NSAPI_ERROR_OK;
}
