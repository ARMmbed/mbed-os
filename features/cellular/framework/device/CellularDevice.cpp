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

#ifdef CELLULAR_DEVICE
#include "EventQueue.h"
#include "CellularUtil.h"
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)

namespace mbed {

PlatformMutex CellularDevice::_device_mutex;
CellularDevice *CellularDevice::_device;
events::EventQueue *CellularDevice::_event_queue;

CellularDevice *CellularDevice::get_default_instance()
{
    _device_mutex.lock();
    if (!_device) {
        _event_queue = new events::EventQueue(4 * EVENTS_EVENT_SIZE);
        _device = new CELLULAR_DEVICE(*_event_queue);
    }
    _device_mutex.unlock();

    return _device;
}

CellularDevice::CellularDevice() : _network_ref_count(0), _sms_ref_count(0), _power_ref_count(0), _sim_ref_count(0),
        _info_ref_count(0)
{
}

events::EventQueue &CellularDevice::get_queue()
{
    return *_event_queue;
}

}

#endif // CELLULAR_DEVICE
