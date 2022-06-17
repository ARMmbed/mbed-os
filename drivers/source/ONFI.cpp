/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
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

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "ONFI.h"
#include "platform/mbed_assert.h"

#if DEVICE_ONFI

namespace mbed {

SingletonPtr<PlatformMutex> ONFI::_mutex;

ONFI::ONFI(PinName d0, PinName d1, PinName d2, PinName d3, PinName d4, PinName d5, PinName d6, PinName d7,
           PinName add09, PinName add11, PinName add12, PinName ba1, PinName rdy, PinName csx): _onfi()
{
    _onfi_d0 = d0;
    _onfi_d1 = d1;
    _onfi_d2 = d2;
    _onfi_d3 = d3;
    _onfi_d4 = d4;
    _onfi_d5 = d5;
    _onfi_d6 = d6;
    _onfi_d7 = d7;
    _onfi_add09 = add09;
    _onfi_add11 = add11;
    _onfi_add12 = add12;
    _onfi_ba1 = ba1;
    _onfi_rdy = rdy;
    _onfi_csx = csx;
    _initialized = false;
}

int ONFI::init()
{
    int ret = 0;
    lock();

    ret = onfi_init(&_onfi, _onfi_d0, _onfi_d1, _onfi_d2, _onfi_d3, _onfi_d4, _onfi_d5, _onfi_d6, _onfi_d7,
                    _onfi_add09, _onfi_add11, _onfi_add12, _onfi_ba1, _onfi_rdy, _onfi_csx);

    if (!ret) {
        _initialized = true;
    }
    unlock();
    return ret;
}

int ONFI::deinit()
{
    int ret = 0;
    lock();

    ret = onfi_free(&_onfi);
    _initialized = false;

    unlock();

    return ret;
}

int ONFI::read(uint32_t addr, uint8_t *buffer, uint32_t size)
{
    int ret = 0;

    if (_initialized) {
        lock();
        ret = onfi_read(&_onfi, addr, buffer, size);
        unlock();
    }

    return ret;
}

int ONFI::write(uint32_t addr, const void *buffer, uint32_t size)
{
    int ret = 0;

    if (_initialized) {
        lock();
        ret = onfi_write(&_onfi, addr, buffer, size);
        unlock();
    }

    return ret;
}

int ONFI::erase(uint32_t addr, uint32_t size)
{
    int ret = 0;

    if (_initialized)   {
        lock();
        ret = onfi_erase_block(&_onfi, addr, size);
        unlock();
    }

    return ret;
}

void ONFI::lock()
{
    _mutex->lock();
}

void ONFI::unlock()
{
    _mutex->unlock();
}

}

#endif
