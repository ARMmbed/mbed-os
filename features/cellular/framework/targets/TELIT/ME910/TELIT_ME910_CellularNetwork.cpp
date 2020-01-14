/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "TELIT_ME910_CellularNetwork.h"

using namespace mbed;

TELIT_ME910_CellularNetwork::TELIT_ME910_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularNetwork(atHandler, device)
{
}

TELIT_ME910_CellularNetwork::~TELIT_ME910_CellularNetwork()
{
}

nsapi_error_t TELIT_ME910_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    switch (opsAct) {
        case RAT_GSM:
        case RAT_CATM1:
        case RAT_NB1:
            _op_act = opsAct;
            return NSAPI_ERROR_OK;

        default:
            _op_act = RAT_UNKNOWN;
            return NSAPI_ERROR_UNSUPPORTED;
    }
}

nsapi_error_t TELIT_ME910_CellularNetwork::do_user_authentication()
{
#if defined(MBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME) && defined(MBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD)
    if (!_device.get_property(AT_CellularDevice::PROPERTY_AT_CGAUTH)) {
        return NSAPI_ERROR_UNSUPPORTED;
    }
    const bool stored_debug_state = _at.get_debug();
    _at.set_debug(false);

    nsapi_error_t err = _at.at_cmd_discard("+CGAUTH", "=", "%d%d%s%s", 1, CellularContext::AuthenticationType::CHAP, MBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME, MBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD);

    _at.set_debug(stored_debug_state);

    if (err != NSAPI_ERROR_OK) {
        return NSAPI_ERROR_AUTH_FAILURE;
    }
#endif // #if defined(MBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME) && defined(MBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD)
    return NSAPI_ERROR_OK;
}


