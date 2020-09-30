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

#include "TELIT_GE866.h"
#include "AT_CellularNetwork.h"
#include "PinNames.h"
#include "rtos/ThisThread.h"

using namespace std::chrono_literals;
using namespace mbed;
using namespace rtos;
using namespace events;

#if !defined(MBED_CONF_TELIT_GE866_RESET)
#define MBED_CONF_TELIT_GE866_RESET    NC
#endif

#if !defined(MBED_CONF_TELIT_GE866_TX)
#define MBED_CONF_TELIT_GE866_TX    NC
#endif

#if !defined(MBED_CONF_TELIT_GE866_RX)
#define MBED_CONF_TELIT_GE866_RX    NC
#endif

#if !defined(MBED_CONF_TELIT_GE866_RTS)
#define MBED_CONF_TELIT_GE866_RTS    NC
#endif

#if !defined(MBED_CONF_TELIT_GE866_CTS)
#define MBED_CONF_TELIT_GE866_CTS    NC
#endif

#if !defined(MBED_CONF_TELIT_GE866_POLARITY)
#define MBED_CONF_TELIT_GE866_POLARITY    1 // active high
#endif


static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable,    // C_EREG
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
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    6,  // PROPERTY_SOCKET_COUNT
    1,  // PROPERTY_IP_TCP
    1,  // PROPERTY_IP_UDP
    20, // PROPERTY_AT_SEND_DELAY
};

TELIT_GE866::TELIT_GE866(FileHandle *fh, PinName reset, bool active_high)
    : AT_CellularDevice(fh),
      _active_high(active_high),
      _reset_key(reset, !_active_high)
{
    set_cellular_properties(cellular_properties);
}

nsapi_error_t TELIT_GE866::init()
{
    nsapi_error_t err = AT_CellularDevice::init();
    if (err != NSAPI_ERROR_OK) {
        return err;
    }
    _at.lock();

#if defined (MBED_CONF_TELIT_GE866_RTS) && defined (MBED_CONF_TELIT_GE866_CTS)
    _at.at_cmd_discard("&K3;&C1;&D0", "");
#else
    _at.at_cmd_discard("&K0;&C1;&D0", "");
#endif

    // AT#QSS=1
    // Enable the Query SIM Status unsolicited indication in the ME. The format of the
    // unsolicited indication is the following:
    // #QSS: <status>
    // The ME informs at
    // every SIM status change through the basic unsolicited indication where <status> range is 0...1
    // <status> values:
    // - 0: SIM not inserted
    // - 1: SIM inserted
    _at.at_cmd_discard("#QSS", "=1");

    // AT+CMER=2
    // Set command enables sending of unsolicited result codes from TA to TE in the case of
    // indicator state changes.
    // Current setting: buffer +CIEV Unsolicited Result Codes in the TA when TA-TE link is
    // reserved (e.g. on-line data mode) and flush them to the TE after
    // reservation; otherwise forward them directly to the TE
    _at.at_cmd_discard("+CMER", "=2");

    // AT+CMEE=2
    // Set command disables the use of result code +CME ERROR: <err> as an indication of an
    // error relating to the +Cxxx command issued. When enabled, device related errors cause the +CME
    // ERROR: <err> final result code instead of the default ERROR final result code. ERROR is returned
    // normally when the error message is related to syntax, invalid parameters or DTE functionality.
    // Current setting: enable and use verbose <err> values
    _at.at_cmd_discard("+CMEE", "=2");

    // AT#GAUTH=2
    // Set command sets the authentication type either for PPP-GPRS and PPP-GSM connections.
    // The value 2 corresponds to CHAP authentication which is the authentication type used by default 
    // in the MbedOS PPP implementation.
    _at.at_cmd_discard("#GAUTH", "=2");

    return _at.unlock_return_error();
}

#if MBED_CONF_TELIT_GE866_PROVIDE_DEFAULT
#include "drivers/BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_TELIT_GE866_TX, MBED_CONF_TELIT_GE866_RX, MBED_CONF_TELIT_GE866_BAUDRATE);
#if defined (MBED_CONF_TELIT_GE866_RTS) && defined (MBED_CONF_TELIT_GE866_CTS)
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_TELIT_GE866_RTS, MBED_CONF_TELIT_GE866_CTS);
#endif
    static TELIT_GE866 device(&serial,
                              MBED_CONF_TELIT_GE866_RESET,
                              MBED_CONF_TELIT_GE866_POLARITY);
    return &device;
}
#endif

nsapi_error_t TELIT_GE866::soft_power_on()
{
    _reset_key = !_active_high;
    ThisThread::sleep_for(300ms);
    _reset_key = _active_high;
    ThisThread::sleep_for(5s);

    return NSAPI_ERROR_OK;
}

nsapi_error_t TELIT_GE866::shutdown()
{
    // Ensure state machine is stopped and all contexts are disconnected
    CellularDevice::shutdown();

    _at.lock();

    // AT#SYSHALT=0,1
    // The module is turned off. It can be awaken by reset pin, alarm or DTR pin
    // transition to low.
    // Note that AT#SYSHALT=0,1 is used rather than AT+CFUN=0 because this is the 
    // recommended action in the hardware guide to be taken before hard power off.
    _at.at_cmd_discard("#SYSHALT", "=0,1");

    // It is necessary to wait 10 seconds after the issuing the #SYSHALT command before
    // the module can be considered fully off and safe to hard power off.
    ThisThread::sleep_for(10s);

    return _at.unlock_return_error();
}


nsapi_error_t TELIT_GE866::set_power_save_mode(int periodic_time, int active_time)
{
    // AT_CellularDevice base class implementation uses the +CPSMS AT command which is
    // not supported by the GE866
    return NSAPI_ERROR_UNSUPPORTED;
}

