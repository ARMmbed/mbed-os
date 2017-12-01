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

#include "platform/platform.h"

#if defined(DEVICE_CPUUID) || defined(DOXYGEN_ONLY)

#ifdef MBED_CPU_UID_STR_SIZE_MAX
#define CPU_UID_STRING_BUFFER_SIZE  MBED_CPU_UID_STR_SIZE_MAX
#else
#define CPU_UID_STRING_BUFFER_SIZE (MBED_CPU_UID_SIZE * 2 + 1)
#endif

namespace mbed {
/** \addtogroup drivers */

/** CPU UID reader class
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class CpuUid {
public:
    /** CpuUid constructor
     */
    CpuUid();
    
    /** Get size of CPU UID in bytes
     * 
     * @return Size of device's CPU UID in bytes
     */
    int size()
    {
        return MBED_CPU_UID_SIZE;
    }
    
    /** Get CPU UID data pointer
     * 
     * @return Pointer to uid data buffer
     */
    const uint8_t *data()
    {
        populate_uid_buf();
        return _uidptr;
    }
    
    /** Get CPU UID vendor string
     * 
     * @return Pointer to zero terminated uid vendor string
     * 
     * @note
     * If vendor did not define MBED_CPU_UID_STR_SIZE_MAX, CpuUid driver will
     * assume the HAL interface function cpu_uid_get_str() is not implemented
     * on the target, and instead the driver will construct just an ASCII
     * string out of the uid byte buffer contents.
     * 
     */
    const char *c_str()
    {
        populate_str_buf();
        return _strptr;
    }
    
    /** Overload operator for byte pointer
     * 
     * @return Pointer to uid data buffer
     */
    operator const uint8_t*()
    {
        populate_uid_buf();
        return _uidptr;
    }
    
    operator const char*()
    {
        populate_str_buf();
        return _strptr;
    }

    /** Overload operator for array subscript
     * 
     * @param x CPU UID Byte index
     * 
     * @return Byte located at index x
     */
    uint8_t operator[](int x)
    {
        if (x >= 0 && x < MBED_CPU_UID_SIZE) {
            populate_uid_buf();
            return _uidptr[x];
        }

        return 0x00;
    }
    
private:
    void populate_uid_buf();
    void populate_str_buf();
    
    static uint8_t _uidbuf[MBED_CPU_UID_SIZE];
    static uint8_t* _uidptr;
    static char _strbuf[CPU_UID_STRING_BUFFER_SIZE];
    static char* _strptr;
#ifndef MBED_CPU_UID_STR_SIZE_MAX
    static const char _hexChars[16];
#endif
};

} // namespace mbed

#endif

#endif
