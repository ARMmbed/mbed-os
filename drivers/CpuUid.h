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
#ifndef MBED_CPUUID_H
#define MBED_CPUUID_H

#include <string>
#include <vector>
#include "platform/platform.h"

#if defined(DEVICE_CPUUID) || defined(DOXYGEN_ONLY)


namespace mbed {
/** \addtogroup drivers */

/** CPU UID reader class
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class CpuUid {
public:
    /** CPU UID array typedef
     */
    typedef std::vector<uint8_t> cpu_uid_array_t;
    
    /** CpuUid constructor
     */
    CpuUid();
    
    /** CpuUid destructor
     */
    virtual ~CpuUid();    

    /** Get size of CPU UID in bytes
     * 
     * @return Size of device's CPU UID in bytes
     */
    int size()
    {
        return _size;
    }
    
    /** Get CPU UID data pointer
     * 
     * @return Pointer to uid data buffer
     */
    const uint8_t *data()
    {
        return _data;
    }
    
    /** Overload operator for std::string
     * 
     * @return string object containing the CPU UID in uppercase hex letters in ascii format
     */
    operator std::string();
    
    /** Overload operator for cpu_uid_array_t
     * 
     * @return cpu_uid_array_t object containing the CPU UID
     */
    operator cpu_uid_array_t();
    
    /** Overload operator for byte pointer
     * 
     * @return Pointer to uid data buffer
     */
    operator const uint8_t*()
    {
        return _data;
    }

    /** Overload operator for array subscript
     * 
     * @param x CPU UID Byte index
     * 
     * @return Byte located at index x
     */
    uint8_t operator[](int x);
    
private:
    uint8_t *_data;
    int _size;
    
    static const char _hexChars[16];
};

} // namespace mbed

#endif

#endif
