/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include <cstddef>
#include "rtos/EventFlags.h"
#include <list>

/** Store the next value to be returned by Event Flags member functions */
std::list<uint32_t> eventFlagsStubNextRetval;

rtos::EventFlags::EventFlags() {}
rtos::EventFlags::~EventFlags() {}
uint32_t rtos::EventFlags::set(uint32_t flags)
{
    return 0;
}
uint32_t rtos::EventFlags::clear(uint32_t flags)
{
    return 0;
}
uint32_t rtos::EventFlags::get() const
{
    return 0;
}
uint32_t rtos::EventFlags::wait_all(uint32_t flags, uint32_t timeout, bool clear)
{
    return 0;
}
uint32_t rtos::EventFlags::wait_any(uint32_t flags, uint32_t timeout, bool clear)
{
    if (!eventFlagsStubNextRetval.empty()) {
        uint32_t ret = eventFlagsStubNextRetval.front();
        eventFlagsStubNextRetval.pop_front();
        return ret;
    }
    return 0;
}
