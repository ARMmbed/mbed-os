/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#include "ALT1250_PPP.h"
#include "ALT1250_PPP_CellularContext.h"
#include "AT_CellularNetwork.h"
#include "ALT1250_PPP_CellularNetwork.h"
#include "CellularLog.h"
#include "rtos/ThisThread.h"

using namespace rtos;
using namespace mbed;
using namespace events;

#define CONNECT_DELIM         "\r\n"
#define CONNECT_BUFFER_SIZE   (1280 + 80 + 80) // AT response + sscanf format
#define CONNECT_TIMEOUT       8000

#if !defined(MBED_CONF_ALT1250_PPP_RST)
#define MBED_CONF_ALT1250_PPP_RST    NC
#endif

static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeEnable,// C_EREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    1,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    0,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};

ALT1250_PPP::ALT1250_PPP(FileHandle *fh, PinName rst, PinDirection pin_dir, PinMode pin_mode, bool value)
    : AT_CellularDevice(fh),
      _rst(rst, pin_dir, pin_mode, value)
{
    set_cellular_properties(cellular_properties);
}

AT_CellularContext *ALT1250_PPP::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new ALT1250_PPP_CellularContext(at, this, apn, cp_req, nonip_req);
}

AT_CellularNetwork *ALT1250_PPP::open_network_impl(ATHandler &at)
{
    return new ALT1250_PPP_CellularNetwork(at, *this);
}

nsapi_error_t ALT1250_PPP::soft_power_on()
{
    // check if modem is already ready
    _at.lock();
    _at.flush();
    _at.set_at_timeout(30);
    _at.cmd_start("AT");
    _at.cmd_stop_read_resp();
    nsapi_error_t err = _at.get_last_error();
    _at.restore_at_timeout();
    _at.unlock();

    // modem is not responding to AT commands, power it on
    if (err != NSAPI_ERROR_OK) {
        tr_warn("Modem is not responding to AT commands, reset it");
        if (_rst.is_connected()) {
            _rst = 0;
            ThisThread::sleep_for(100);
            _rst = 1;
        }
        if (_at.sync(2000)) {
            tr_warn("Modem is AT ready");
            return NSAPI_ERROR_OK;
        }
        tr_warn("Modem is not AT ready following reset");
        return _at.get_last_error();
    }

    return NSAPI_ERROR_OK;
}

#if MBED_CONF_ALT1250_PPP_PROVIDE_DEFAULT

#if !NSAPI_PPP_AVAILABLE
#error Must define lwip.ppp-enabled
#endif

#include "BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_ALT1250_PPP_TX, MBED_CONF_ALT1250_PPP_RX, MBED_CONF_ALT1250_PPP_BAUDRATE);
#if defined (MBED_CONF_ALT1250_PPP_RTS) && defined (MBED_CONF_ALT1250_PPP_CTS)
    tr_debug("ALT1250_PPP flow control: RTS %d CTS %d", MBED_CONF_ALT1250_PPP_RTS, MBED_CONF_ALT1250_PPP_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_ALT1250_PPP_RTS, MBED_CONF_ALT1250_PPP_CTS);
#endif
    static ALT1250_PPP device(&serial, MBED_CONF_ALT1250_PPP_RST, PIN_OUTPUT, OpenDrainNoPull, 1);
    return &device;
}
#endif

