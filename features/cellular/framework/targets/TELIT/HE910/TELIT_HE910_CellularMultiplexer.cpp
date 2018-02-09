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

#include "TELIT_HE910_CellularMultiplexer.h"

using namespace mbed;

TELIT_HE910_CellularMultiplexer::TELIT_HE910_CellularMultiplexer(ATHandler &atHandler)
: AT_CellularMultiplexer(atHandler)
{
}

TELIT_HE910_CellularMultiplexer::~TELIT_HE910_CellularMultiplexer()
{
}

nsapi_error_t TELIT_HE910_CellularMultiplexer::multiplexer_mode_start()
{
    _at.lock();

    _at.cmd_start("AT#SELINT=2");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start("ATE0V1&K3&D2");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start("AT+IPR=115200");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start("AT&W0&P0");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start("AT#CMUXMODE=5");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start("AT+CMUX=0");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    return _at.unlock_return_error();
}
