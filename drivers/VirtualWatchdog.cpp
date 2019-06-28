/*
 * Copyright (c) 2019 Arm Limited and affiliates.
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

#include "drivers/VirtualWatchdog.h"
#include "drivers/Watchdog.h"

#define MS_TO_US(x) ((x) * 1000)
#define US_TO_MS(x) ((x) / 1000)

namespace mbed {

#if DEVICE_LPTICKER
    SingletonPtr<LowPowerTicker> _ticker;
#else
    SingletonPtr<Ticker> _ticker;
#endif

VirtualWatchdog *VirtualWatchdog::_first = NULL;
bool VirtualWatchdog::_is_hw_watchdog_running = false;
us_timestamp_t VirtualWatchdog::_ticker_timeout = 0;

VirtualWatchdog::VirtualWatchdog(uint32_t timeout, const char *const str): _name(str)
{
    _current_count = 0;
    _is_initialized = false;
    _next = NULL;
    _timeout = timeout;
    // start watchdog
    Watchdog &watchdog = Watchdog::get_instance();
    if (!_is_hw_watchdog_running) {
        if (watchdog.is_running() == true) {
            MBED_MAKE_ERROR(MBED_MODULE_DRIVER_WATCHDOG, MBED_ERROR_INITIALIZATION_FAILED);
        }
        // we use default hw timeout provided by config
        watchdog.start(Watchdog::watchdog_timeout);
        _ticker_timeout = MS_TO_US(Watchdog::watchdog_timeout / 2);
        if (_ticker_timeout == 0) {
            _ticker_timeout = 1;
        }
        _ticker->attach_us(callback(this, &VirtualWatchdog::process), _ticker_timeout);
        _is_hw_watchdog_running = true;
    }
}

VirtualWatchdog::~VirtualWatchdog()
{
    if (_is_initialized) {
        stop();
        // we do not need to stop hw watchdog, it's ticking by itself
    }
}

void VirtualWatchdog::start()
{
    MBED_ASSERT(!_is_initialized);
    core_util_critical_section_enter();
    add_to_list();
    core_util_critical_section_exit();
}


void VirtualWatchdog::kick()
{
    MBED_ASSERT(_is_initialized);
    core_util_critical_section_enter();
    _current_count = 0;
    core_util_critical_section_exit();
}

void VirtualWatchdog::stop()
{
    MBED_ASSERT(_is_initialized);
    core_util_critical_section_enter();
    remove_from_list();
    core_util_critical_section_exit();
}

void VirtualWatchdog::add_to_list()
{
    this->_next = _first;
    _first = this;
    _is_initialized =  true;
}

void VirtualWatchdog::remove_from_list()
{
    VirtualWatchdog *cur_ptr = _first, *prev_ptr = NULL;
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

void VirtualWatchdog::process()
{
    VirtualWatchdog *cur_ptr =  _first;
    while (cur_ptr != NULL) {
        if (cur_ptr->_current_count > cur_ptr->_timeout) {
            system_reset();
        } else {
            cur_ptr->_current_count += US_TO_MS(_ticker_timeout);
        }
        cur_ptr = cur_ptr->_next;
    }
}


} // namespace mbed

#endif // DEVICE_WATCHDOG
