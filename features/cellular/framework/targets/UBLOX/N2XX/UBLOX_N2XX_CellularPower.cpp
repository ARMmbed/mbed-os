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

#include "UBLOX_N2XX_CellularPower.h"
#include "onboard_modem_api.h"
#include "platform/mbed_wait_api.h"

using namespace mbed;

const int MAX_SIM_RESPONSE_LENGTH = 25;

UBLOX_N2XX_CellularPower::UBLOX_N2XX_CellularPower(ATHandler &atHandler) : AT_CellularPower(atHandler)
{
}

UBLOX_N2XX_CellularPower::~UBLOX_N2XX_CellularPower()
{
}

nsapi_error_t UBLOX_N2XX_CellularPower::on()
{
#if MODEM_ON_BOARD
    ::onboard_modem_init();
    ::onboard_modem_power_up();
#endif
    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_N2XX_CellularPower::off()
{
#if MODEM_ON_BOARD
    ::onboard_modem_power_down();
#endif
    return NSAPI_ERROR_OK;
}

nsapi_error_t UBLOX_N2XX_CellularPower::set_at_mode()
{
    _at.lock();
    _at.flush();
    _at.cmd_start("AT");
    _at.cmd_stop_read_resp();

    _at.cmd_start("AT+CMEE=1"); // verbose responses
    _at.cmd_stop_read_resp();
    return _at.unlock_return_error();
}


nsapi_error_t UBLOX_N2XX_CellularPower::set_power_level(int func_level, int do_reset, const char *sim_pin)
{
    char simstr[MAX_SIM_RESPONSE_LENGTH];

    _at.lock();
    if(func_level == 1) {
        _at.cmd_start("AT+CFUN=1");
        _at.cmd_stop();
        _at.resp_start();
        _at.read_string(simstr, sizeof(simstr));
        _at.resp_stop();
        _at.get_last_error();
        _at.clear_error();

        if (memcmp(simstr, "ENTER PIN", 9) == 0) {
            _at.cmd_start("AT+NPIN=0,");
            _at.write_string(sim_pin);
            _at.cmd_stop_read_resp();
            _at.get_last_error();
            _at.clear_error();
            wait_ms(1000);
        }
    }
    _at.cmd_start("AT+CFUN=");
    _at.write_int(func_level);
    _at.write_int(do_reset);
    _at.cmd_stop_read_resp();
    return _at.unlock_return_error();
}
