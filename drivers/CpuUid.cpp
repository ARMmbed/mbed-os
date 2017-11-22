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

#include <cstdio>
#include "hal/cpu_uid_api.h"
#include "drivers/CpuUid.h"

#if DEVICE_CPUUID

namespace mbed {

CpuUid::CpuUid() : _data(NULL)
{
    _size = cpu_uid_get_length();
    if (0 < _size)
    {
        _data = new uint8_t[_size];
        cpu_uid_get_uid(_data);
    }
}

CpuUid::~CpuUid()
{
    if (_data)
    {
        delete _data;
    }
}

CpuUid::operator std::string()
{
    std::string str;
    char buf[3];
    
    for (int i = 0; i < _size; ++i)
    {
        snprintf(buf, 3, "%.2X", _data[i]);
        str += buf;
    }

    return str;
}

CpuUid::operator CpuUidArray()
{
    CpuUidArray array;
    
    for (int i = 0; i < _size; ++i)
    {
        array.push_back(_data[i]);
    }
    
    return array;
}    

uint8_t CpuUid::operator[](int x)
{
    if (x >= 0 && x < _size)
    {
        return _data[x];
    }
    
    return 0x00;
}
    
} // namespace mbed

#endif
