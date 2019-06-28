/*
 * Copyright (c) 2018-2019 Arm Limited and affiliates.
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
#ifdef DEVICE_WATCHDOG

#include "Watchdog.h"

namespace mbed {

static uint32_t _timeout = 0;

Watchdog::Watchdog() : _running(false)
{
}

Watchdog::~Watchdog()
{
}

bool Watchdog::start(uint32_t timeout)
{
    _timeout = timeout;
    _running = true;
    return _running;
}

bool Watchdog::stop()
{
    _running = false;
    return _running;
}

void Watchdog::kick()
{
}

bool Watchdog::is_running() const
{
    return _running;
}

uint32_t Watchdog::get_timeout() const
{
    return _timeout;
}

uint32_t Watchdog::get_max_timeout() const
{
    return 0xFFFFFFFF;
}

} // namespace mbed
#endif // DEVICE_WATCHDOG
