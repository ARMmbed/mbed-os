/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef NRF5X_DUMMYSIGNINGEVENTMONITOR_H
#define NRF5X_DUMMYSIGNINGEVENTMONITOR_H

#include "ble/pal/SigningEventMonitor.h"

namespace ble {
namespace vendor {
namespace nordic {

template<class Handler>
struct DummySigningEventMonitor :
    public ble::pal::SigningEventMonitor<DummySigningEventMonitor<Handler>, Handler >
{
    void set_signing_event_handler_(Handler *signing_event_handler)
    {
    }
};

} // nordic
} // vendor
} // ble


#endif //NRF5X_DUMMYSIGNINGEVENTMONITOR_H
