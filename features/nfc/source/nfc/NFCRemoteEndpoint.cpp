/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "NFCRemoteEndpoint.h"
#include "NFCController.h"

#include "acore/ac_buffer.h"
#include "acore/ac_buffer_reader.h"
#include "acore/ac_buffer_builder.h"

#include "stack/transceiver/transceiver.h"

using namespace mbed;
using namespace mbed::nfc;

NFCRemoteEndpoint::NFCRemoteEndpoint(NFCController *controller) : _controller(controller)
{

}

NFCRemoteEndpoint::~NFCRemoteEndpoint()
{

}

nfc_rf_protocols_bitmask_t NFCRemoteEndpoint::rf_protocols()
{
    nfc_rf_protocols_bitmask_t rf_protocols = {0};
    nfc_tech_t active_tech = transceiver_get_active_techs(_controller->transceiver());
    if (!transceiver_is_initiator_mode(_controller->transceiver())) {
        // Note: We only support ISO-DEP for now
        rf_protocols.target_iso_dep = active_tech.nfc_iso_dep_a || active_tech.nfc_iso_dep_b;
    }

    return rf_protocols;
}

NFCController *NFCRemoteEndpoint::nfc_controller()
{
    return _controller;
}

const NFCController *NFCRemoteEndpoint::nfc_controller() const
{
    return _controller;
}

