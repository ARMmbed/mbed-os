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

#include "PN512TransportDriver.h"

using namespace mbed;
using namespace mbed::nfc;

PN512TransportDriver::PN512TransportDriver() : _delegate(NULL)
{

}

PN512TransportDriver::~PN512TransportDriver()
{

}

void PN512TransportDriver::set_delegate(Delegate *delegate)
{
    _delegate = delegate;
}

void PN512TransportDriver::hw_interrupt()
{
    if (_delegate != NULL) {
        _delegate->on_hw_interrupt();
    }
}
