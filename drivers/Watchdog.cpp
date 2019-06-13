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

namespace mbed {

Watchdog *Watchdog::_first = NULL;

Watchdog::Watchdog(uint32_t timeout, const char *const str): _name(str)
{
    _current_count = 0;
    _is_initialized = false;
    _next = NULL;
    _max_timeout = timeout;
}

void Watchdog::add_to_list()
{
    this->_next = _first;
    _first = this;
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
    Watchdog *cur_ptr = _first,
              *prev_ptr = NULL;
    while (cur_ptr != NULL) {
        if (cur_ptr == this) {
            if (cur_ptr == _first) {
                prev_ptr = _first;
                _first = cur_ptr->_next;
                prev_ptr->_next = NULL;
            } else {
                prev_ptr->_next = cur_ptr->_next;
                cur_ptr->_next = NULL;
            }
            _is_initialized = false;
            break;
        } else {
            prev_ptr = cur_ptr;
            cur_ptr = cur_ptr->_next;
        }
    }
}

void Watchdog::process(uint32_t elapsed_ms)
{
    Watchdog *cur_ptr =  _first;
    while (cur_ptr != NULL) {
        if (cur_ptr->_current_count > cur_ptr->_max_timeout) {
            system_reset();
        } else {
            cur_ptr->_current_count += elapsed_ms;
        }
        cur_ptr = cur_ptr->_next;
    }
}

Watchdog::~Watchdog()
{
    if (_is_initialized) {
        stop();
    }
}

} // namespace mbed

#endif // DEVICE_WATCHDOG
