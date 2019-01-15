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

#include "GEMALTO_CINTERION_Module.h"
#include "GEMALTO_CINTERION_CellularContext.h"
#include "GEMALTO_CINTERION.h"
#include "AT_CellularInformation.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;

const uint16_t RESPONSE_TO_SEND_DELAY = 100; // response-to-send delay in milliseconds at bit-rate over 9600

GEMALTO_CINTERION::GEMALTO_CINTERION(FileHandle *fh) : AT_CellularDevice(fh)
{
}

AT_CellularContext *GEMALTO_CINTERION::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new GEMALTO_CINTERION_CellularContext(at, this, apn, cp_req, nonip_req);
}

nsapi_error_t GEMALTO_CINTERION::init()
{
    nsapi_error_t err = AT_CellularDevice::init();
    if (err != NSAPI_ERROR_OK) {
        return err;
    }

    CellularInformation *information = open_information();
    if (!information) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    char model[sizeof("ELS61") + 1]; // sizeof need to be long enough to hold just the model text
    nsapi_error_t ret = information->get_model(model, sizeof(model));
    close_information();
    if (ret != NSAPI_ERROR_OK) {
        tr_error("Cellular model not found!");
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return GEMALTO_CINTERION_Module::detect_model(model);
}

uint16_t GEMALTO_CINTERION::get_send_delay() const
{
    return RESPONSE_TO_SEND_DELAY;
}

#if MBED_CONF_GEMALTO_CINTERION_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_GEMALTO_CINTERION_TX, MBED_CONF_GEMALTO_CINTERION_RX, MBED_CONF_GEMALTO_CINTERION_BAUDRATE);
#if defined (MBED_CONF_UBLOX_AT_RTS) && defined(MBED_CONF_UBLOX_AT_CTS)
    tr_debug("GEMALTO_CINTERION flow control: RTS %d CTS %d", MBED_CONF_GEMALTO_CINTERION_RTS, MBED_CONF_GEMALTO_CINTERION_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_GEMALTO_CINTERION_RTS, MBED_CONF_GEMALTO_CINTERION_CTS);
#endif
    static GEMALTO_CINTERION device(&serial);
    return &device;
}
#endif
