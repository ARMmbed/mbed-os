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

#include "Type4RemoteInitiator.h"
#include "NFCController.h"

#include "acore/ac_buffer.h"
#include "acore/ac_buffer_reader.h"
#include "acore/ac_buffer_builder.h"

#include "stack/transceiver/transceiver.h"
#include "stack/tech/iso7816/iso7816.h"
#include "stack/tech/iso7816/iso7816_app.h"
#include "stack/tech/type4/type4_target.h"

using namespace mbed;
using namespace mbed::nfc;

Type4RemoteInitiator::Type4RemoteInitiator(NFCController *controller, const Span<uint8_t> &buffer) :
    NFCRemoteInitiator(controller, buffer),
    _is_connected(false), _is_disconnected(false)
{
    // Init ISO7816
    nfc_tech_iso7816_init(&_iso7816, nfc_controller()->transceiver(), &Type4RemoteInitiator::s_disconnected_callback, this);

    // Init Type 4 app
    nfc_tech_type4_target_init(&_type4, &_iso7816, ndef_message());
}

Type4RemoteInitiator::~Type4RemoteInitiator()
{

}

nfc_err_t Type4RemoteInitiator::connect()
{
    if (_is_connected) {
        return NFC_ERR_BUSY;
    }

    if (_is_disconnected) {
        return NFC_ERR_DISCONNECTED;
    }

    // Connect ISO7816 stack
    nfc_tech_iso7816_connect(&_iso7816);

    // Call callback as it's a synchronous API
    connected();

    return NFC_OK;
}

nfc_err_t Type4RemoteInitiator::disconnect()
{
    if (!_is_connected) {
        return NFC_OK;
    }

    if (_is_disconnected) {
        return NFC_OK;
    }

    // Disconnect ISO7816 stack
    nfc_tech_iso7816_disconnect(&_iso7816);

    return NFC_OK;
}

bool Type4RemoteInitiator::is_connected() const
{
    return _is_connected;
}

bool Type4RemoteInitiator::is_disconnected() const
{
    return _is_disconnected;
}

nfc_rf_protocols_bitmask_t Type4RemoteInitiator::rf_protocols()
{
    nfc_rf_protocols_bitmask_t rf_protocols = {0};
    nfc_tech_t active_tech = transceiver_get_active_techs(nfc_controller()->transceiver());
    if (!transceiver_is_initiator_mode(nfc_controller()->transceiver())) {
        // We only support ISO-DEP
        rf_protocols.target_iso_dep = active_tech.nfc_iso_dep_a || active_tech.nfc_iso_dep_b;
    }

    return rf_protocols;
}

nfc_tag_type_t Type4RemoteInitiator::nfc_tag_type() const
{
    nfc_tech_t active_tech = transceiver_get_active_techs(nfc_controller()->transceiver());
    if (active_tech.nfc_iso_dep_a) {
        return nfc_tag_type_4a;
    } else { // if(active_tech.nfc_iso_dep_b)
        return nfc_tag_type_4b;
    }
}

bool Type4RemoteInitiator::is_iso7816_supported() const
{
    return true;
}

void Type4RemoteInitiator::add_iso7816_application(nfc_tech_iso7816_app_t *application)
{
    nfc_tech_iso7816_add_app(&_iso7816, application);
}

bool Type4RemoteInitiator::is_ndef_supported() const
{
    return true;
}

void Type4RemoteInitiator::disconnected_callback()
{
    // Call disconnected callback
    disconnected();
}

void Type4RemoteInitiator::s_disconnected_callback(nfc_tech_iso7816_t *pIso7816, void *pUserData)
{
    Type4RemoteInitiator *self = (Type4RemoteInitiator *) pUserData;
    self->disconnected_callback();
}
