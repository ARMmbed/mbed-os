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

#include "UBLOX_PPP_CellularNetwork.h"

using namespace mbed;

UBLOX_PPP_CellularNetwork::UBLOX_PPP_CellularNetwork(ATHandler &atHandler) : AT_CellularNetwork(atHandler)
{
}

UBLOX_PPP_CellularNetwork::~UBLOX_PPP_CellularNetwork()
{
}

bool UBLOX_PPP_CellularNetwork::get_modem_stack_type(nsapi_ip_stack_t requested_stack)
{
    return requested_stack == IPV4_STACK ? true : false;
}

bool UBLOX_PPP_CellularNetwork::has_registration(RegistrationType reg_type)
{
    return (reg_type == C_REG || reg_type == C_GREG);
}

nsapi_error_t UBLOX_PPP_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opRat)
{
    _op_act = RAT_UNKNOWN;
    return NSAPI_ERROR_UNSUPPORTED;
}
