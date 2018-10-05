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

#include "TELIT_HE910_CellularPower.h"
// for modem powering up&down
#include "onboard_modem_api.h"

using namespace mbed;

TELIT_HE910_CellularPower::TELIT_HE910_CellularPower(ATHandler &atHandler) : AT_CellularPower(atHandler)
{

}

TELIT_HE910_CellularPower::~TELIT_HE910_CellularPower()
{

}

nsapi_error_t TELIT_HE910_CellularPower::on()
{
#if MODEM_ON_BOARD
    ::onboard_modem_init();
    ::onboard_modem_power_up();
#endif
    return NSAPI_ERROR_OK;
}

nsapi_error_t TELIT_HE910_CellularPower::off()
{
#if MODEM_ON_BOARD
    ::onboard_modem_power_down();
#endif
    return NSAPI_ERROR_OK;
}

/**
 * Set AT command mode.
 * @remark must be called after power on to prepare correct AT mode
 * @return blocking until success or failure
 */
nsapi_error_t TELIT_HE910_CellularPower::set_at_mode()
{
    nsapi_error_t err = AT_CellularPower::set_at_mode();
    if (err != NSAPI_ERROR_OK) {
        return err;
    }
    _at.lock();
    _at.cmd_start("AT&K0;&C1;&D0");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    return _at.unlock_return_error();
}
