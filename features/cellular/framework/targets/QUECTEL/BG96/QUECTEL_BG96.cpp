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

#include "rtos/ThisThread.h"
#include "QUECTEL_BG96.h"
#include "QUECTEL_BG96_CellularNetwork.h"
#include "QUECTEL_BG96_CellularStack.h"
#include "QUECTEL_BG96_CellularInformation.h"
#include "QUECTEL_BG96_CellularContext.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;
using namespace rtos;

#define CONNECT_DELIM         "\r\n"
#define CONNECT_BUFFER_SIZE   (1280 + 80 + 80) // AT response + sscanf format
#define CONNECT_TIMEOUT       8000

#define DEVICE_READY_URC "CPIN:"

#if !defined(MBED_CONF_QUECTEL_BG96_PWR)
#define MBED_CONF_QUECTEL_BG96_PWR    NC
#endif

#if !defined(MBED_CONF_QUECTEL_BG96_RST)
#define MBED_CONF_QUECTEL_BG96_RST    NC
#endif

#if !defined(MBED_CONF_QUECTEL_BG96_POLARITY)
#define MBED_CONF_QUECTEL_BG96_POLARITY    1 // active high
#endif

static const intptr_t cellular_properties[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,    // C_EREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,    // C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    1,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
};

QUECTEL_BG96::QUECTEL_BG96(FileHandle *fh, PinName pwr, bool active_high, PinName rst)
    : AT_CellularDevice(fh),
      _active_high(active_high),
      _pwr(pwr, !_active_high),
      _rst(rst, !_active_high)
{
    AT_CellularBase::set_cellular_properties(cellular_properties);
}

void QUECTEL_BG96::set_at_urcs_impl()
{
    _at->set_urc_handler("+QIURC: \"pdpde",  mbed::Callback<void()>(this, &QUECTEL_BG96::urc_pdpdeact));
}

AT_CellularNetwork *QUECTEL_BG96::open_network_impl(ATHandler &at)
{
    return new QUECTEL_BG96_CellularNetwork(at);
}

AT_CellularContext *QUECTEL_BG96::create_context_impl(ATHandler &at, const char *apn, bool cp_req, bool nonip_req)
{
    return new QUECTEL_BG96_CellularContext(at, this, apn, cp_req, nonip_req);
}

AT_CellularInformation *QUECTEL_BG96::open_information_impl(ATHandler &at)
{
    return new QUECTEL_BG96_CellularInformation(at);
}

void QUECTEL_BG96::set_ready_cb(Callback<void()> callback)
{
    _at->set_urc_handler(DEVICE_READY_URC, callback);
}

nsapi_error_t QUECTEL_BG96::hard_power_on()
{
    if (_pwr.is_connected()) {
        tr_info("Modem power on");
        ThisThread::sleep_for(250);
        _pwr = !_active_high;
        ThisThread::sleep_for(250); // BG96_Hardware_Design_V1.1 says 100 ms, but 250 ms seems to be more robust
        _pwr = _active_high;
        ThisThread::sleep_for(500);
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_BG96::soft_power_on()
{
    if (!_rst.is_connected()) {
        return NSAPI_ERROR_OK;
    }

    tr_info("Reset modem");
    _rst = !_active_high;
    ThisThread::sleep_for(100);
    _rst = _active_high;
    ThisThread::sleep_for(150 + 460); // RESET_N timeout from BG96_Hardware_Design_V1.1
    _rst = !_active_high;
    ThisThread::sleep_for(500);

    // wait for RDY
    _at->lock();
    _at->set_at_timeout(10 * 1000);
    _at->resp_start();
    _at->set_stop_tag("RDY");
    bool rdy = _at->consume_to_stop_tag();
    _at->set_stop_tag(OK);
    _at->restore_at_timeout();

    if (!rdy) {
        // check if modem was silently powered on
        _at->clear_error();
        _at->set_at_timeout(100);
        _at->at_cmd_discard("", ""); //Send AT
        _at->restore_at_timeout();
    }
    return _at->unlock_return_error();
}

nsapi_error_t QUECTEL_BG96::hard_power_off()
{
    if (_pwr.is_connected()) {
        tr_info("Modem power off");
        _pwr = _active_high;
        ThisThread::sleep_for(650); // from BG96_Hardware_Design_V1.1
        _pwr = !_active_high;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_BG96::init()
{
    setup_at_handler();

    int retry = 0;

    _at->lock();
    _at->flush();
    _at->at_cmd_discard("E0", ""); // echo off

    _at->at_cmd_discard("+CMEE", "=1"); // verbose responses

    if (_at->get_last_error() != NSAPI_ERROR_OK) {
        return _at->unlock_return_error();
    }

    do {
        _at->clear_error();
        _at->at_cmd_discard("+CFUN", "=1"); // set full functionality
        if (_at->get_last_error() == NSAPI_ERROR_OK) {
            break;
        }
        // wait some time that modem gets ready for CFUN command, and try again
        retry++;
        ThisThread::sleep_for(64); // experimental value
    } while (retry < 3);

    return _at->unlock_return_error();
}

#if MBED_CONF_QUECTEL_BG96_PROVIDE_DEFAULT
#include "UARTSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static UARTSerial serial(MBED_CONF_QUECTEL_BG96_TX, MBED_CONF_QUECTEL_BG96_RX, MBED_CONF_QUECTEL_BG96_BAUDRATE);
#if defined (MBED_CONF_QUECTEL_BG96_RTS) && defined(MBED_CONF_QUECTEL_BG96_CTS)
    tr_debug("QUECTEL_BG96 flow control: RTS %d CTS %d", MBED_CONF_QUECTEL_BG96_RTS, MBED_CONF_QUECTEL_BG96_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_QUECTEL_BG96_RTS, MBED_CONF_QUECTEL_BG96_CTS);
#endif
    static QUECTEL_BG96 device(&serial,
                               MBED_CONF_QUECTEL_BG96_PWR,
                               MBED_CONF_QUECTEL_BG96_POLARITY,
                               MBED_CONF_QUECTEL_BG96_RST);
    return &device;
}
#endif

void QUECTEL_BG96::urc_pdpdeact()
{
    _at->lock();
    _at->skip_param();
    int cid = _at->read_int();
    const nsapi_error_t err = _at->unlock_return_error();

    if (err != NSAPI_ERROR_OK) {
        return;
    }
    send_disconnect_to_context(cid);
}
