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

#include "SARA4_PPP.h"
#include "SARA4_PPP_CellularNetwork.h"
#include "CellularUtil.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;
using namespace mbed_cellular_util;

static const intptr_t cellular_properties[AT_CellularDevice::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC, // C_EREG
    AT_CellularNetwork::RegistrationModeLAC, // C_GREG
    AT_CellularNetwork::RegistrationModeDisable, // C_REG
    0,  // AT_CGSN_WITH_TYPE
    0,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // AT_CNMI
    1,  // AT_CSMP
    1,  // AT_CMGF
    1,  // AT_CSDH
    0,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    1,  // PROPERTY_IPV4V6_STACK
    0,  // PROPERTY_NON_IP_PDP_TYPE
    1,  // PROPERTY_AT_CGEREP
    1,  // PROPERTY_AT_COPS_FALLBACK_AUTO
    0,  // PROPERTY_SOCKET_COUNT
    0,  // PROPERTY_IP_TCP
    0,  // PROPERTY_IP_UDP
    0,  // PROPERTY_AT_SEND_DELAY
};

SARA4_PPP::SARA4_PPP(FileHandle *fh) : AT_CellularDevice(fh)
{
    set_cellular_properties(cellular_properties);
}

AT_CellularNetwork *SARA4_PPP::open_network_impl(ATHandler &at)
{
    return new SARA4_PPP_CellularNetwork(at, *this);
}

nsapi_error_t SARA4_PPP::set_power_save_mode(int periodic_time, int active_time)
{
    _at.lock();

    if (periodic_time == 0 && active_time == 0) {
        // disable PSM
        _at.at_cmd_discard("+CPSMS", "=0");
    } else {
        const int PSMTimerBits = 5;

        /**
            Table 10.5.163a/3GPP TS 24.008: GPRS Timer 3 information element
            Bits 5 to 1 represent the binary coded timer value.
            Bits 6 to 8 defines the timer value unit for the GPRS timer as follows:
            8 7 6
            0 0 0 value is incremented in multiples of 10 minutes
            0 0 1 value is incremented in multiples of 1 hour
            0 1 0 value is incremented in multiples of 10 hours
            0 1 1 value is incremented in multiples of 2 seconds
            1 0 0 value is incremented in multiples of 30 seconds
            1 0 1 value is incremented in multiples of 1 minute
            1 1 0 value is incremented in multiples of 320 hours (NOTE 1)
            1 1 1 value indicates that the timer is deactivated (NOTE 2).
         */
        char pt[8 + 1]; // timer value encoded as 3GPP IE
        const int ie_value_max = 0x1f;
        uint32_t periodic_timer = 0;
        if (periodic_time <= 2 * ie_value_max) { // multiples of 2 seconds
            periodic_timer = periodic_time / 2;
            strcpy(pt, "01100000");
        } else {
            if (periodic_time <= 30 * ie_value_max) { // multiples of 30 seconds
                periodic_timer = periodic_time / 30;
                strcpy(pt, "10000000");
            } else {
                if (periodic_time <= 60 * ie_value_max) { // multiples of 1 minute
                    periodic_timer = periodic_time / 60;
                    strcpy(pt, "10100000");
                } else {
                    if (periodic_time <= 10 * 60 * ie_value_max) { // multiples of 10 minutes
                        periodic_timer = periodic_time / (10 * 60);
                        strcpy(pt, "00000000");
                    } else {
                        if (periodic_time <= 60 * 60 * ie_value_max) { // multiples of 1 hour
                            periodic_timer = periodic_time / (60 * 60);
                            strcpy(pt, "00100000");
                        } else {
                            if (periodic_time <= 10 * 60 * 60 * ie_value_max) { // multiples of 10 hours
                                periodic_timer = periodic_time / (10 * 60 * 60);
                                strcpy(pt, "01000000");
                            } else { // multiples of 320 hours
                                int t = periodic_time / (320 * 60 * 60);
                                if (t > ie_value_max) {
                                    t = ie_value_max;
                                }
                                periodic_timer = t;
                                strcpy(pt, "11000000");
                            }
                        }
                    }
                }
            }
        }

        uint_to_binary_str(periodic_timer, &pt[3], sizeof(pt) - 3, PSMTimerBits);
        pt[8] = '\0';

        /**
            Table 10.5.172/3GPP TS 24.008: GPRS Timer information element
            Bits 5 to 1 represent the binary coded timer value.
            Bits 6 to 8 defines the timer value unit for the GPRS timer as follows:
            8 7 6
            0 0 0  value is incremented in multiples of 2 seconds
            0 0 1  value is incremented in multiples of 1 minute
            0 1 0  value is incremented in multiples of decihours
            1 1 1  value indicates that the timer is deactivated.

            Other values shall be interpreted as multiples of 1 minute in this version of the protocol.
        */
        char at[8 + 1];
        uint32_t active_timer; // timer value encoded as 3GPP IE
        if (active_time <= 2 * ie_value_max) { // multiples of 2 seconds
            active_timer = active_time / 2;
            strcpy(at, "00000000");
        } else {
            if (active_time <= 60 * ie_value_max) { // multiples of 1 minute
                active_timer = (1 << 5) | (active_time / 60);
                strcpy(at, "00100000");
            } else { // multiples of decihours
                int t = active_time / (6 * 60);
                if (t > ie_value_max) {
                    t = ie_value_max;
                }
                active_timer = t;
                strcpy(at, "01000000");
            }
        }

        uint_to_binary_str(active_timer, &at[3], sizeof(at) - 3, PSMTimerBits);
        at[8] = '\0';

        // request for both GPRS and LTE

        _at.at_cmd_discard("+CPSMS", "=1,,,", "%s%s", pt, at);

        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            tr_warn("Power save mode not enabled!");
        } else {
            // network may not agree with power save options but
            // that should be fine as timeout is not longer than requested
        }
    }

    return _at.unlock_return_error();
}

#if MBED_CONF_SARA4_PPP_PROVIDE_DEFAULT
#include "drivers/BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    static BufferedSerial serial(MBED_CONF_SARA4_PPP_TX, MBED_CONF_SARA4_PPP_RX, MBED_CONF_SARA4_PPP_BAUDRATE);
#if defined (MBED_CONF_SARA4_PPP_RTS) && defined (MBED_CONF_SARA4_PPP_CTS)
    tr_debug("SARA4_PPP flow control: RTS %d CTS %d", MBED_CONF_SARA4_PPP_RTS, MBED_CONF_SARA4_PPP_CTS);
    serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_SARA4_PPP_RTS, MBED_CONF_SARA4_PPP_CTS);
#endif
    static SARA4_PPP device(&serial);
    return &device;
}
#endif
