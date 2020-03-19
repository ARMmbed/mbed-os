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
#include "drivers/BufferedSerial.h"

using namespace mbed;
using namespace rtos;
using namespace events;

#if !defined(MBED_CONF_QUECTEL_EC2X_PWR)
#define MBED_CONF_QUECTEL_EC2X_PWR    NC
#endif

#if !defined(MBED_CONF_QUECTEL_EC2X_RST)
#define MBED_CONF_QUECTEL_EC2X_RST    NC
#endif

#if !defined(MBED_CONF_QUECTEL_EC2X_TX)
#define MBED_CONF_QUECTEL_EC2X_TX    NC
#endif

#if !defined(MBED_CONF_QUECTEL_EC2X_RX)
#define MBED_CONF_QUECTEL_EC2X_RX    NC
#endif

#if !defined(MBED_CONF_QUECTEL_EC2X_POLARITY)
#define MBED_CONF_QUECTEL_EC2X_POLARITY    1 // active high
#endif

#if !defined(MBED_CONF_QUECTEL_EC2X_START_TIMEOUT)
#define MBED_CONF_QUECTEL_EC2X_START_TIMEOUT    15000
#endif


static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
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
    1,  // PROPERTY_AT_CGEREP,
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    0,  // PROPERTY_SOCKET_COUNT
    0,  // PROPERTY_IP_TCP
    0,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};

QUECTEL_EC2X::QUECTEL_EC2X(FileHandle *fh, PinName pwr, bool active_high, PinName rst)
    : AT_CellularDevice(fh),
      _active_high(active_high),
      _pwr_key(pwr, !_active_high),
      _rst(rst, !_active_high)
{
    set_cellular_properties(cellular_properties);
}

#if MBED_CONF_QUECTEL_EC2X_PROVIDE_DEFAULT
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_QUECTEL_EC2X_TX,
                                 MBED_CONF_QUECTEL_EC2X_RX,
                                 MBED_CONF_QUECTEL_EC2X_BAUDRATE);
#if defined(MBED_CONF_QUECTEL_EC2X_RTS) && defined(MBED_CONF_QUECTEL_EC2X_CTS)
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_QUECTEL_EC2X_RTS, MBED_CONF_QUECTEL_EC2X_CTS);
#endif
    static QUECTEL_EC2X device(&serial,
                               MBED_CONF_QUECTEL_EC2X_PWR,
                               MBED_CONF_QUECTEL_EC2X_POLARITY,
                               MBED_CONF_QUECTEL_EC2X_RST);
    return &device;
}
#endif

nsapi_error_t QUECTEL_EC2X::press_power_button(uint32_t timeout)
{
    if (_pwr_key.is_connected()) {
        _pwr_key = _active_high;
        ThisThread::sleep_for(timeout);
        _pwr_key = !_active_high;
        ThisThread::sleep_for(100);
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_EC2X::hard_power_on()
{
    return press_power_button(600);
}

nsapi_error_t QUECTEL_EC2X::hard_power_off()

{
    return press_power_button(750);
}

nsapi_error_t QUECTEL_EC2X::soft_power_on()
{
    if (_rst.is_connected()) {
        _rst = _active_high;
        ThisThread::sleep_for(460);
        _rst = !_active_high;
        ThisThread::sleep_for(100);

        _at.lock();

        _at.set_at_timeout(MBED_CONF_QUECTEL_EC2X_START_TIMEOUT);
        _at.resp_start();
        _at.set_stop_tag("RDY");
        bool rdy = _at.consume_to_stop_tag();
        _at.set_stop_tag(OK);

        _at.unlock();

        if (!rdy) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_EC2X::soft_power_off()
{
    return hard_power_off();
}
