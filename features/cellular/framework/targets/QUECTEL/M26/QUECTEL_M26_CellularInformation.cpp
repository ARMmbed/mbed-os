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

#include "QUECTEL_M26_CellularInformation.h"

using namespace mbed;

QUECTEL_M26_CellularInformation::QUECTEL_M26_CellularInformation(ATHandler &atHandler) : AT_CellularInformation(atHandler)
{

}

// According to M26_AT_Commands_Manual_V1.9
nsapi_error_t QUECTEL_M26_CellularInformation::get_iccid(char *buf, size_t buf_size)
{
    return _at.at_cmd_str("+CCID", "", buf, buf_size);
}
