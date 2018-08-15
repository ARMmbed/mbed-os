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

#include "NFCRemoteInitiator.h"

#include "acore/buffer.h"
#include "acore/buffer_reader.h"
#include "acore/buffer_builder.h"

#include "stack/transceiver/transceiver.h"
#include "stack/tech/iso7816/iso7816.h"
#include "stack/tech/iso7816/iso7816_app.h"

using namespace mbed;
using namespace mbed::nfc;

NFCRemoteInitiator::NFCRemoteInitiator(NFCController* controller) : NFCRemoteEndpoint(controller) {

}

NFCRemoteInitiator::~NFCRemoteInitiator() {

}


void NFCRemoteInitiator::set_remote_initiator_delegate(Delegate* delegate)
{

}

nfc_tag_type_t NFCRemoteInitiator::nfc_tag_type() const
{

}

bool NFCRemoteInitiator::is_iso7816_supported() const
{

}

void NFCRemoteInitiator::add_iso7816_application(ISO7816App* application)
{

}
