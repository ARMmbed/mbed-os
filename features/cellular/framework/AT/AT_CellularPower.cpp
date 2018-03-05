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

#include "AT_CellularPower.h"
#include "CellularUtil.h"
#include "CellularLog.h"
#include "nsapi_types.h"

static const int PSMTimerBits = 5;

using namespace mbed_cellular_util;
using namespace mbed;

AT_CellularPower::AT_CellularPower(ATHandler &at) : AT_CellularBase(at)
{
}

AT_CellularPower::~AT_CellularPower()
{
}

nsapi_error_t AT_CellularPower::on()
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularPower::off()
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularPower::set_at_mode()
{
    _at.lock();
    _at.flush();
    _at.cmd_start("ATE0"); // echo off
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start("AT+CMEE=1"); // verbose responses
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularPower::set_power_level(int func_level)
{
    _at.lock();
    _at.cmd_start("AT+CFUN=");
    _at.write_int(func_level);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularPower::reset()
{
    _at.lock();
    _at.cmd_start("AT+CFUN=");// reset to full power levels
    _at.write_int(1);
    _at.write_int(1);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularPower::opt_power_save_mode(int periodic_time, int active_time)
{
    _at.lock();

    if (periodic_time == 0 && active_time == 0) {
        // disable PSM
        _at.cmd_start("AT+CPSMS=");
        _at.write_int(0);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    } else {
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
        char pt[8+1];// timer value encoded as 3GPP IE
        const int ie_value_max = 0x1f;
        uint32_t periodic_timer = 0;
        if (periodic_time <= 2*ie_value_max) { // multiples of 2 seconds
            periodic_timer = periodic_time/2;
            strcpy(pt, "01100000");
        } else {
            if (periodic_time <= 30*ie_value_max) { // multiples of 30 seconds
                periodic_timer = periodic_time/30;
                strcpy(pt, "10000000");
            } else {
                if (periodic_time <= 60*ie_value_max) { // multiples of 1 minute
                    periodic_timer = periodic_time/60;
                    strcpy(pt, "10100000");
                } else {
                    if (periodic_time <= 10*60*ie_value_max) { // multiples of 10 minutes
                        periodic_timer = periodic_time/(10*60);
                        strcpy(pt, "00000000");
                    } else {
                        if (periodic_time <= 60*60*ie_value_max) { // multiples of 1 hour
                            periodic_timer = periodic_time/(60*60);
                            strcpy(pt, "00100000");
                        } else {
                            if (periodic_time <= 10*60*60*ie_value_max) { // multiples of 10 hours
                                periodic_timer = periodic_time/(10*60*60);
                                strcpy(pt, "01000000");
                            } else { // multiples of 320 hours
                                int t = periodic_time / (320*60*60);
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

        uint_to_binary_str(periodic_timer, &pt[3], sizeof(pt)-3, PSMTimerBits);
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
        char at[8+1];
        uint32_t active_timer; // timer value encoded as 3GPP IE
        if (active_time <= 2*ie_value_max) { // multiples of 2 seconds
            active_timer = active_time/2;
            strcpy(at, "00000000");
        } else {
            if (active_time <= 60*ie_value_max) { // multiples of 1 minute
                active_timer = (1<<5) | (active_time/60);
                strcpy(at, "00100000");
            } else { // multiples of decihours
                int t = active_time / (6*60);
                if (t > ie_value_max) {
                    t = ie_value_max;
                }
                active_timer = t;
                strcpy(at, "01000000");
            }
        }

        uint_to_binary_str(active_timer, &at[3], sizeof(at)-3, PSMTimerBits);
        pt[8] = '\0';

        // request for both GPRS and LTE
        _at.cmd_start("AT+CPSMS=");
        _at.write_int(1);
        _at.write_string(pt);
        _at.write_string(at);
        _at.write_string(pt);
        _at.write_string(at);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();


        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            tr_warn("Power save mode not enabled!");
        } else {
            // network may not agree with power save options but
            // that should be fine as timeout is not longer than requested
        }
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularPower::opt_receive_period(int mode, EDRXAccessTechnology act_type, uint8_t edrx_value)
{
    char edrx[5];
    uint_to_binary_str(edrx_value, edrx, 5, 4);
    edrx[4] = '\0';

    _at.lock();

    _at.cmd_start("AT+CEDRXS=");
    _at.write_int(mode);
    _at.write_int(act_type);
    _at.write_string(edrx);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    return _at.unlock_return_error();
}
