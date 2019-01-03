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

uint32_t Watchdog::_is_initialized = 0;
uint32_t Watchdog::_re_initialize = 0;
uint32_t Watchdog::_bitmask = 0;
uint32_t Watchdog::_kick_bitmask = 0;
Watchdog::watchdog_usr_info Watchdog::_usr_info[MAX_THREAD_WATCHDOG_SUPPORT] = {0};
rtos::Mutex Watchdog::_ThreadSafeLockMutex;
mbed_error_status_t Watchdog::wd_register(const osThreadId_t tid, const uint32_t timeout)
{
    int store_index = -1;
    watchdog_status_t sts;
    int i;
    _ThreadSafeLockMutex.lock();
    uint32_t conf_timeout = hal_watchdog_get_reload_value();
    if(_is_initialized) {
        if(timeout > conf_timeout) {
            _re_initialize = 1;
            hal_watchdog_stop();
         }

        for(i =0 ; i < MAX_THREAD_WATCHDOG_SUPPORT; i++) {
            if(_bitmask & (1 << i))
            {
                if(_usr_info[i].tid == tid) {
                    if(_re_initialize) {
                        store_index = i;
                        break;
                    }
                    else {
                        _ThreadSafeLockMutex.unlock();
                        return MBED_ERROR_INITIALIZATION_FAILED;
                   }
                }
             }
            else if(store_index < 0) {
               store_index = i;
           }
        }
    }
    else {
        store_index = 0;
    }

    if(store_index >= 0) {
        _usr_info[store_index].tid = tid;
        _usr_info[store_index].bit_idx = store_index;
        _bitmask |= (1 << store_index);
        store_index = -1;

        if(!_is_initialized || _re_initialize ) {
            sts = start(timeout);
            if(sts != WATCHDOG_STATUS_OK ) {
                _is_initialized = 0;
                _ThreadSafeLockMutex.unlock();
                return MBED_ERROR_INITIALIZATION_FAILED;
             }
             _is_initialized = 1;
             _re_initialize = 0;
        }
    } else {
        _ThreadSafeLockMutex.unlock();
        return MBED_ERROR_OVERFLOW;
    }
    _ThreadSafeLockMutex.unlock();
    return MBED_SUCCESS;

}
watchdog_status_t Watchdog::start(const uint32_t timeout)
{
    if (timeout == 0) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    if (timeout > max_timeout()) {
        return WATCHDOG_STATUS_INVALID_ARGUMENT;
    }

    watchdog_config_t config;
    config.timeout_ms = timeout;
    return hal_watchdog_init(&config);
}


void Watchdog::kick(const osThreadId_t tid)
{
    int i;

    _ThreadSafeLockMutex.lock();
    if(_is_initialized ) {
        for(i =0 ; i < MAX_THREAD_WATCHDOG_SUPPORT; i++) {
            if(_usr_info[i].tid == tid)
            {
                _kick_bitmask |= (1 << _usr_info[i].bit_idx);
                break;
            }
        }
        if(!_re_initialize && _bitmask == _kick_bitmask) {
            hal_watchdog_kick();
            _kick_bitmask = 0;
        }
    }
    _ThreadSafeLockMutex.unlock();
}


mbed_error_status_t Watchdog::stop()
{
    watchdog_status_t sts;

    if(_is_initialized ) {
        sts = hal_watchdog_stop();
        if(sts != WATCHDOG_STATUS_OK ) {
            return MBED_ERROR_ALREADY_IN_USE;
        }
        else
        {
            _is_initialized = 0;
            _kick_bitmask = 0;
            return MBED_SUCCESS;
        }
    }
    else
         return MBED_ERROR_OPERATION_PROHIBITED;
}

mbed_error_status_t Watchdog::wd_unregister(const osThreadId_t tid)
{
    int i;
    uint32_t tbit = _bitmask;
    _ThreadSafeLockMutex.lock();
    for(i =0 ; i < MAX_THREAD_WATCHDOG_SUPPORT; i++) {
        if(_usr_info[i].tid == tid) {
        _bitmask &= (~(1 << _usr_info[i].bit_idx));
            _kick_bitmask &= (~(1 << _usr_info[i].bit_idx));
            _usr_info[i].tid = 0;
            _usr_info[i].bit_idx = 0;
        }
    }
    if(_bitmask == tbit) {
        _ThreadSafeLockMutex.unlock();
        return MBED_ERROR_OPERATION_PROHIBITED;
    }
    if(_bitmask == 0) {
        stop();
    }
    _ThreadSafeLockMutex.unlock();
    return MBED_SUCCESS;
}

uint32_t Watchdog::reload_value() const
{
    return hal_watchdog_get_reload_value();
}


uint32_t Watchdog::max_timeout()
{
    const watchdog_features_t features = hal_watchdog_get_platform_features();

    return features.max_timeout;
}

} // namespace mbed

#endif // DEVICE_WATCHDOG
