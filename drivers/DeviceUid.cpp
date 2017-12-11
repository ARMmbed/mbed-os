/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "hal/device_uid_api.h"
#include "platform/mbed_assert.h"
#include "platform/CriticalSectionLock.h"
#include "drivers/DeviceUid.h"

#if DEVICE_DEVICEUID

namespace mbed {

uint8_t DeviceUid::_uidbuf[MBED_DEVICEUID_SIZE] = {0};
uint8_t* DeviceUid::_uidptr = NULL;
char DeviceUid::_strbuf[DEVICEUID_STRING_BUFFER_SIZE] = {'\0'};
char* DeviceUid::_strptr = NULL;
#ifndef MBED_DEVICEUID_STR_SIZE_MAX
const char DeviceUid::_hexChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
#endif

DeviceUid::DeviceUid()
{
    populate_uid_buf();
}

void DeviceUid::populate_uid_buf()
{
    if (_uidptr == NULL) {
        CriticalSectionLock lock;
        device_uid_get_uid(_uidbuf);
        _uidptr = _uidbuf;
    }
}

void DeviceUid::populate_str_buf()
{
    if (_strptr == NULL) {
        CriticalSectionLock lock;
#ifdef MBED_DEVICEUID_STR_SIZE_MAX
        device_uid_get_str(_strbuf);
        _strptr = _strbuf;
#else
        int pos = 0;
        populate_uid_buf();
        for (int i = 0; i < MBED_DEVICEUID_SIZE; ++i) {
            _strbuf[pos++] = _hexChars[_uidptr[i] >> 4];
            _strbuf[pos++] = _hexChars[_uidptr[i] & 0x0F];
        }
        _strbuf[pos] = '\0';
        _strptr = _strbuf;
#endif
    }
}

} // namespace mbed

#endif
