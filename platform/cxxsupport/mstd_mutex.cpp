/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include <mstd_mutex>

#if MBED_CONF_RTOS_PRESENT

/* SharedPointer<Mutex> lazy-init kerfuffle will generate code - don't inline it */
namespace mstd {

// Lock and try lock use SingletonPtr::get() to lazy-init
void _Mutex_base::lock()
{
    _pm.get()->lock();
}

bool _Mutex_base::try_lock()
{
    return _pm.get()->trylock();
}

// Unlock knows it must have been initted, so optimise with get_no_init()
void _Mutex_base::unlock()
{
    _pm.get_no_init()->unlock();
}

// And don't forget to destroy - SingletonPtr doesn't do it automatically
_Mutex_base::~_Mutex_base()
{
    _pm.destroy();
}

} // namespace mstd

#endif
