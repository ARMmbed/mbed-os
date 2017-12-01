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

#include "hal/cpu_uid_api.h"
#include "platform/mbed_assert.h"
#include "drivers/CpuUid.h"

#if DEVICE_CPUUID

namespace mbed {

uint8_t CpuUid::_uidbuf[MBED_CPU_UID_SIZE] = {0};
uint8_t* CpuUid::_uidptr = NULL;
char CpuUid::_strbuf[CPU_UID_STRING_BUFFER_SIZE] = {'\0'};
char* CpuUid::_strptr = NULL;
#ifndef MBED_CPU_UID_STR_SIZE_MAX
const char CpuUid::_hexChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
#endif

CpuUid::CpuUid()
{
    populate_uid_buf();
}

CpuUid::~CpuUid()
{
}

void CpuUid::populate_uid_buf()
{
    if (_uidptr == NULL) {
        cpu_uid_get_uid(_uidbuf);
        _uidptr = _uidbuf;
    }
}

void CpuUid::populate_str_buf()
{
    if (_strptr == NULL) {
#ifdef MBED_CPU_UID_STR_SIZE_MAX
        cpu_uid_get_str(_strbuf);
        _strptr = _strbuf;
#else
        int pos = 0;
        populate_uid_buf();
        for (int i = 0; i < MBED_CPU_UID_SIZE; ++i) {
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
