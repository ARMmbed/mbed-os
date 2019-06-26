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

Watchdog *Watchdog::_first;

Watchdog::Watchdog(uint32_t timeout, const char *const str): _name(str)
{

}

void Watchdog::start()
{

}

void Watchdog::kick()
{

}

void Watchdog::stop()
{

}

void Watchdog::process(uint32_t elapsed_ms)
{

}

Watchdog::~Watchdog()
{

}

} // namespace mbed

#endif // DEVICE_WATCHDOG
