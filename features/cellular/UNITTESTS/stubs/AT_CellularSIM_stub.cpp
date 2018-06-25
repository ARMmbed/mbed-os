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

#include "AT_CellularSIM.h"
#include "CellularLog.h"

using namespace mbed;

AT_CellularSIM::AT_CellularSIM(ATHandler &at) : AT_CellularBase(at)
{
}

AT_CellularSIM::~AT_CellularSIM()
{
}

nsapi_error_t AT_CellularSIM::get_sim_state(SimState &state)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSIM::set_pin(const char *sim_pin)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSIM::change_pin(const char *sim_pin, const char *new_pin)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSIM::set_pin_query(const char *sim_pin, bool query_pin)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSIM::get_imsi(char* imsi)
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularSIM::get_iccid(char *buf, size_t buf_size)
{
    return NSAPI_ERROR_OK;
}
