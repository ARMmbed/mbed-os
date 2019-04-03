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

#include "QUECTEL_EC2X.h"

#include "PinNames.h"
#include "AT_CellularNetwork.h"
#include "rtos/ThisThread.h"

using namespace mbed;
using namespace rtos;
using namespace events;

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,    // C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    1,  // PROPERTY_IPV6_STACK
    1,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};

QUECTEL_EC2X::QUECTEL_EC2X(FileHandle *fh, PinName pwr, PinName rst)
    : AT_CellularDevice(fh),
      _pwr_key(pwr, 0),
      _rst(rst, 0)

{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

#if MBED_CONF_QUECTEL_EC2X_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_QUECTEL_EC2X_TX,
                             MBED_CONF_QUECTEL_EC2X_RX,
                             MBED_CONF_QUECTEL_EC2X_BAUDRATE);
#if defined(MBED_CONF_QUECTEL_EC2X_RTS) && defined(MBED_CONF_QUECTEL_EC2X_CTS)
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_QUECTEL_EC2X_RTS, MBED_CONF_QUECTEL_EC2X_CTS);
#endif
    static QUECTEL_EC2X device(&serial, MBED_CONF_QUECTEL_EC2X_PWR, MBED_CONF_QUECTEL_EC2X_RST);
    return &device;
}

nsapi_error_t QUECTEL_EC2X::hard_power_on()
{
    if (_pwr_key.is_connected()) {
        _pwr_key = 1;
        ThisThread::sleep_for(600);
        _pwr_key = 0;
        ThisThread::sleep_for(100);
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_EC2X::hard_power_off()

{
    if (_pwr_key.is_connected()) {
        _pwr_key = 1;
        ThisThread::sleep_for(750);
        _pwr_key = 0;
        ThisThread::sleep_for(100);
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_EC2X::soft_power_on()
{
    if (_rst.is_connected()) {
        _rst = 1;
        ThisThread::sleep_for(460);
        _rst = 0;
        ThisThread::sleep_for(100);
    }

    _at->lock();

    _at->set_at_timeout(5000);
    _at->resp_start();
    _at->set_stop_tag("RDY");
    bool rdy = _at->consume_to_stop_tag();
    _at->set_stop_tag(OK);

    _at->unlock();

    if (!rdy) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_EC2X::soft_power_off()
{
    if (_pwr_key.is_connected()) {
        _pwr_key = 1;
        ThisThread::sleep_for(750);
        _pwr_key = 0;
        ThisThread::sleep_for(100);
    }

    return NSAPI_ERROR_OK;
}

#endif
