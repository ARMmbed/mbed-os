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

#include "NFCEEPROMDriver.h"

using namespace mbed;
using namespace mbed::nfc;

NFCEEPROMDriver::NFCEEPROMDriver() : _delegate(NULL), _event_queue(NULL)
{

}

NFCEEPROMDriver::~NFCEEPROMDriver()
{

}

void NFCEEPROMDriver::set_delegate(Delegate *delegate)
{
    _delegate = delegate;
}

void NFCEEPROMDriver::set_event_queue(events::EventQueue *queue)
{
    _event_queue = queue;
}

NFCEEPROMDriver::Delegate *NFCEEPROMDriver::delegate()
{
    return _delegate;
}

events::EventQueue *NFCEEPROMDriver::event_queue()
{
    return _event_queue;
}
