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
    1,  // PROPERTY_IPV6_STACK
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

nsapi_error_t QUECTEL_BG96::soft_power_on()
{
    if (_pwr.is_connected()) {
        tr_info("QUECTEL_BG96::soft_power_on");
        // check if modem was powered on already
        if (!wake_up()) {
            if (!wake_up(true)) {
                tr_error("Modem not responding");
                soft_power_off();
                return NSAPI_ERROR_DEVICE_ERROR;
            }
        }
    }

#if defined (MBED_CONF_QUECTEL_BG96_RTS) && defined(MBED_CONF_QUECTEL_BG96_CTS)
    if (_at->at_cmd_discard("+IFC", "=", "%d%d", 2, 2) != NSAPI_ERROR_OK) {
        tr_warn("Set flow control failed");
        return NSAPI_ERROR_DEVICE_ERROR;
    }
#endif

    return NSAPI_ERROR_OK;
}

nsapi_error_t QUECTEL_BG96::soft_power_off()
{
    _at->lock();
    _at->cmd_start("AT+QPOWD");
    _at->cmd_stop_read_resp();
    if (_at->get_last_error() != NSAPI_ERROR_OK) {
        tr_warn("Force modem off");
        if (_pwr.is_connected()) {
            press_button(_pwr, 650); // BG96_Hardware_Design_V1.1: Power off signal at least 650 ms
        }
    }
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

void QUECTEL_BG96::press_button(DigitalOut &button, uint32_t timeout)
{
    if (!button.is_connected()) {
        return;
    }
    button = _active_high;
    ThisThread::sleep_for(timeout);
    button = !_active_high;
}

bool QUECTEL_BG96::wake_up(bool reset)
{
    // check if modem is already ready
    _at->lock();
    _at->flush();
    _at->set_at_timeout(30);
    _at->cmd_start("AT");
    _at->cmd_stop_read_resp();
    nsapi_error_t err = _at->get_last_error();
    _at->restore_at_timeout();
    _at->unlock();
    // modem is not responding, power it on
    if (err != NSAPI_ERROR_OK) {
        if (!reset) {
            // BG96_Hardware_Design_V1.1 requires VBAT to be stable over 30 ms, that's handled above
            tr_info("Power on modem");
            press_button(_pwr, 250); // BG96_Hardware_Design_V1.1 requires time 100 ms, but 250 ms seems to be more robust
        } else {
            tr_warn("Reset modem");
            press_button(_rst, 150); // BG96_Hardware_Design_V1.1 requires RESET_N timeout at least 150 ms
        }
        _at->lock();
        // According to BG96_Hardware_Design_V1.1 USB is active after 4.2s, but it seems to take over 5s
        _at->set_at_timeout(6000);
        _at->resp_start();
        _at->set_stop_tag("RDY");
        bool rdy = _at->consume_to_stop_tag();
        _at->set_stop_tag(OK);
        _at->restore_at_timeout();
        _at->unlock();
        if (!rdy) {
            return false;
        }
    }

    // sync to check that AT is really responsive and to clear garbage
    return _at->sync(500);
}
