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

#include <stdlib.h>
#include "rtos.h"
#include "CellularCommon.h"
#include "RM1000_AT_CellularNetwork.h"
#include "platform/mbed_wait_api.h"
#include "AT_CellularDevice.h"

#include "mbed-trace/mbed_trace.h"
#ifndef TRACE_GROUP
#define TRACE_GROUP  "RIOT"
#endif // TRACE_GROUP
using namespace mbed;

// Callback for MODEM faults URC.
void RM1000_AT_CellularNetwork::MODEM_FAULT_URC()
{
    tr_debug("RM1000_AT_CellularNetwork::ASSERTED_URC");
    _connect_status = NSAPI_STATUS_DISCONNECTED;
    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);
    }
}

RM1000_AT_CellularNetwork::RM1000_AT_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularNetwork(atHandler, device)
{
    tr_debug("RM1000_AT_CellularNetwork::RM1000_B0_CellularNetwork");
    _op_act = RAT_UNKNOWN;
    _at.set_urc_handler("ASSERTED!", callback(this, &RM1000_AT_CellularNetwork::MODEM_FAULT_URC));
    _at.set_urc_handler("ERRCODE:", callback(this, &RM1000_AT_CellularNetwork::MODEM_FAULT_URC));
    _at.set_urc_handler("Halt the processor", callback(this, &RM1000_AT_CellularNetwork::MODEM_FAULT_URC));
}

RM1000_AT_CellularNetwork::~RM1000_AT_CellularNetwork()
{
    tr_debug("RM1000_AT_CellularNetwork::~RM1000_B0_CellularNetwork");

    _at.set_urc_handler("ASSERTED!", NULL);
    _at.set_urc_handler("ERRCODE:", NULL);
    _at.set_urc_handler("Halt the processor", NULL);
}

nsapi_error_t RM1000_AT_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opRat)
{
    nsapi_error_t ret = NSAPI_ERROR_OK;

    tr_debug("RM1000_AT_CellularNetwork::set_access_technology_impl %d", opRat);

    switch (opRat) {
        case RAT_NB1:
            break;
        default: {
            _op_act = RAT_UNKNOWN;
            ret = NSAPI_ERROR_UNSUPPORTED;
        }
    }

    return (ret);
}

nsapi_error_t RM1000_AT_CellularNetwork::set_registration_urc(RegistrationType type, bool urc_on)
{
    tr_debug("RM1000_AT_CellularNetwork::set_registration_urc");

    int index = (int)type;
    MBED_ASSERT(index >= 0 && index < C_MAX);

    RegistrationMode mode = (RegistrationMode)_device.get_property((AT_CellularDevice::CellularProperty)type);
    if (mode == RegistrationModeDisable) {
        return NSAPI_ERROR_UNSUPPORTED;
    } else {
        return NSAPI_ERROR_OK; /* FIXME use at commands */
    }
}
