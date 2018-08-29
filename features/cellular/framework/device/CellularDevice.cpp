/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#include "CellularDevice.h"
#include "EventQueue.h"
#include "CellularUtil.h"

#ifdef CELLULAR_DEVICE
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)
#endif // CELLULAR_DEVICE

namespace mbed {

#ifdef CELLULAR_DEVICE
MBED_WEAK CellularDevice *CellularDevice::get_default_instance()
{
    static events::EventQueue event_queue(4 * EVENTS_EVENT_SIZE);
    static CELLULAR_DEVICE device(event_queue);
    return &device;
}
#else
MBED_WEAK CellularDevice *CellularDevice::get_default_instance()
{
    return NULL;
}
#endif // CELLULAR_DEVICE

CellularDevice::CellularDevice() : _network_ref_count(0), _sms_ref_count(0), _power_ref_count(0), _sim_ref_count(0),
        _info_ref_count(0)
{
}

events::EventQueue *CellularDevice::get_queue() const
{
    return NULL;
}

}
