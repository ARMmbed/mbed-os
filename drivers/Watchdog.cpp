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
#ifdef DEVICE_WATCHDOG

#include "Watchdog.h"

namespace mbed
{

Watchdog *Watchdog::first;

void Watchdog::add_to_list()
{
    this->next = first;
    first = this;
    _is_initialized =  true;
}

void Watchdog::start()
{
    MBED_ASSERT(!_is_initialized);
    core_util_critical_section_enter();
    add_to_list();
    core_util_critical_section_exit();
}


void Watchdog::kick()
{
    MBED_ASSERT(_is_initialized);
    core_util_critical_section_enter();
    _current_count = 0;
    core_util_critical_section_exit();
}

void Watchdog::stop()
{
    MBED_ASSERT(_is_initialized);
    core_util_critical_section_enter();
    remove_from_list();
    core_util_critical_section_exit();
}

void Watchdog::remove_from_list()
{
    Watchdog *cur_ptr = first,
              *prev_ptr = NULL;
    while(cur_ptr != NULL) {
        if(cur_ptr == this) {
            if(cur_ptr == first) {
                prev_ptr = first;
                first = cur_ptr->next;
                prev_ptr->next = NULL;
            } else {
                prev_ptr->next = cur_ptr->next;
                cur_ptr->next = NULL;
            }
            _is_initialized = false;
            break;
        } else {
            prev_ptr = cur_ptr;
            cur_ptr = cur_ptr->next;
        }
    }
}

void Watchdog::is_alive()
{
    Watchdog *cur_ptr =  first;
    while(cur_ptr != NULL) {
        if(cur_ptr->_current_count > cur_ptr->_max_timeout) {
            system_reset();
        } else {
            cur_ptr->_current_count++;
            cur_ptr = cur_ptr->next;
        }
    }
}

Watchdog::~Watchdog()
{
    if(_is_initialized)
        stop();
}

} // namespace mbed

#endif // DEVICE_WATCHDOG
