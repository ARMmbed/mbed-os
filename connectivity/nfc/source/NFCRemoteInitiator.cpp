/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "NFCRemoteInitiator.h"

using namespace mbed;
using namespace mbed::nfc;

NFCRemoteInitiator::NFCRemoteInitiator(NFCController *controller, const Span<uint8_t> &buffer) :
    NFCRemoteEndpoint(controller), NFCNDEFCapable(buffer), _delegate(NULL)
{

}

NFCRemoteInitiator::~NFCRemoteInitiator()
{

}

void NFCRemoteInitiator::set_delegate(Delegate *delegate)
{
    _delegate = delegate;
}

void NFCRemoteInitiator::connected()
{
    if (_delegate != NULL) {
        _delegate->on_connected();
    }
}

void NFCRemoteInitiator::disconnected()
{
    if (_delegate != NULL) {
        _delegate->on_disconnected();
    }
}

NFCNDEFCapable::Delegate *NFCRemoteInitiator::ndef_capable_delegate()
{
    return _delegate;
}
