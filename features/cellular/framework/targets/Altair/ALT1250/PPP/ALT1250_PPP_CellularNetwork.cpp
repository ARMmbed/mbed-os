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

#include "ALT1250_PPP_CellularNetwork.h"
#include "CellularLog.h"

using namespace mbed;

ALT1250_PPP_CellularNetwork::ALT1250_PPP_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularNetwork(atHandler, device)
{
}

ALT1250_PPP_CellularNetwork::~ALT1250_PPP_CellularNetwork()
{
}

nsapi_error_t ALT1250_PPP_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    _at.lock();

    _at.set_at_timeout(10000);
    char resp[20];
    _at.at_cmd_str("%RATACT", "?", resp, 20);
    tr_debug("ALT1250_PPP RAT: %s", resp);

    switch (opsAct) {
        case RAT_CATM1:
            if (memcmp(resp, "CATM", 4)) {
                _at.at_cmd_discard("%RATACT", "=\"CATM\"");
            }
            break;
        case RAT_NB1:
            if (memcmp(resp, "NBIOT", 5)) {
                _at.at_cmd_discard("%RATACT", "=\"NBIOT\"");
            }
            break;
        case RAT_GSM:
        case RAT_GSM_COMPACT:
        case RAT_UTRAN:
        case RAT_EGPRS:
            break;
        default:
            if (memcmp(resp, "DEFAULT", 7)) {
                _at.at_cmd_discard("%RATACT", "=\"DEFAULT\"");
            }
            _at.unlock();
            _op_act = RAT_UNKNOWN;
            return NSAPI_ERROR_UNSUPPORTED;
    }

    _at.restore_at_timeout();

    return _at.unlock_return_error();
}
