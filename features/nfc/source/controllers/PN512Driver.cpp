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

#include "PN512Driver.h"

#include "nfc/stack/platform/nfc_debug.h"

using namespace mbed;
using namespace mbed::nfc;

PN512Driver::PN512Driver(PN512TransportDriver *transport_driver) : NFCControllerDriver(), _transport_driver(transport_driver)
{
    _transport_driver->set_delegate(this);
}

nfc_transceiver_t *PN512Driver::initialize(nfc_scheduler_timer_t *scheduler_timer)
{
    // Initialize transport
    _transport_driver->initialize();

    nfc_err_t ret = pn512_init(&_pn512, _transport_driver->get_transport(), scheduler_timer);
    if (ret != NFC_OK) {
        NFC_ERR("PN512 init error (%d)", ret);
        return NULL;
    }
    NFC_DBG("PN512 Initialized");

    return pn512_get_transceiver(&_pn512);
}

void PN512Driver::get_supported_nfc_techs(nfc_tech_t *initiator, nfc_tech_t *target) const
{
    initiator->nfc_type1 = 0;
    initiator->nfc_type2 = 1;
    initiator->nfc_type3 = 1;
    initiator->nfc_iso_dep_a = 1;
    initiator->nfc_iso_dep_b = 0;
    initiator->nfc_nfc_dep_a = 1;
    initiator->nfc_nfc_dep_f_212 = 1;
    initiator->nfc_nfc_dep_f_424 = 1;

    target->nfc_type1 = 0;
    target->nfc_type2 = 0;
    target->nfc_type3 = 0;
    target->nfc_iso_dep_a = 1;
    target->nfc_iso_dep_b = 0;
    target->nfc_nfc_dep_a = 1;
    target->nfc_nfc_dep_f_212 = 1;
    target->nfc_nfc_dep_f_424 = 1;
}

void PN512Driver::on_hw_interrupt()
{
    hw_interrupt(); // Propagate interrupt signal
}
