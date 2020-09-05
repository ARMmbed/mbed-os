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

#include "QUECTEL_BG96_CellularNetwork.h"
#include "QUECTEL_BG96_CellularStack.h"
#include "CellularLog.h"

using namespace mbed;

QUECTEL_BG96_CellularNetwork::QUECTEL_BG96_CellularNetwork(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularNetwork(atHandler, device)
{
}

QUECTEL_BG96_CellularNetwork::~QUECTEL_BG96_CellularNetwork()
{
}

nsapi_error_t QUECTEL_BG96_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    _at.lock();

    switch (opsAct) {
        case RAT_CATM1:
            _at.at_cmd_discard("+QCFG", "=\"nwscanseq\",020301");
            _at.at_cmd_discard("+QCFG", "=\"nwscanmode\",3,1");
            _at.at_cmd_discard("+QCFG", "=\"iotopmode\",0,1");
            break;
        case RAT_NB1:
            _at.at_cmd_discard("+QCFG", "=\"nwscanseq\",030201");
            _at.at_cmd_discard("+QCFG", "=\"nwscanmode\",3,1");
            _at.at_cmd_discard("+QCFG", "=\"iotopmode\",1,1");
#ifdef MBED_CONF_CELLULAR_OFFLOAD_DNS_QUERIES
            // enable ePCO IE for DNS address
            _at.cmd_start("AT+QNVFW=");
            _at.write_string("/nv/item_files/modem/nas/nas_ciot_capability_config");
            _at.write_string("0101", false);
#endif
            break;
        case RAT_GSM:
        case RAT_GSM_COMPACT:
        case RAT_UTRAN:
        case RAT_EGPRS:
            _at.at_cmd_discard("+QCFG", "=\"nwscanseq\",010203");
            _at.at_cmd_discard("+QCFG", "=\"nwscanmode\",1,1");
            break;
        default:
            _at.at_cmd_discard("+QCFG", "=\"nwscanseq\",020301");
            _at.at_cmd_discard("+QCFG", "=\"nwscanmode\",0,1");
            _at.at_cmd_discard("+QCFG", "=\"iotopmode\",2,1");

            _at.unlock();
            _op_act = RAT_UNKNOWN;
            return NSAPI_ERROR_UNSUPPORTED;
    }

    return _at.unlock_return_error();
}

void QUECTEL_BG96_CellularNetwork::get_context_state_command()
{
    // read active contexts
    _at.cmd_start_stop("+QIACT", "?");
    _at.resp_start("+QIACT:");
}

