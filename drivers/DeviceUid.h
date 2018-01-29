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
#ifndef MBED_DEVICEUID_H
#define MBED_DEVICEUID_H

#include "platform/platform.h"

#if defined(DEVICE_DEVICEUID) || defined(DOXYGEN_ONLY)

#ifdef MBED_DEVICEUID_STR_SIZE_MAX
#define DEVICEUID_STRING_BUFFER_SIZE  MBED_DEVICEUID_STR_SIZE_MAX
#else
#define DEVICEUID_STRING_BUFFER_SIZE (MBED_DEVICEUID_SIZE * 2 + 1)
#endif

namespace mbed {
/** \addtogroup drivers */

/** DEVICE UID reader class
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class DeviceUid {
public:
    /** DeviceUid constructor
     */
    DeviceUid();
    
    /** Get size of DEVICE UID in bytes
     * 
     * @return Size of device's DEVICE UID in bytes
     */
    int size()
    {
        return MBED_DEVICEUID_SIZE;
    }
    
    /** Get DEVICE UID data pointer
     * 
     * @return Pointer to uid data buffer
     */
    const uint8_t *data()
    {
        populate_uid_buf();
        return _uidptr;
    }
    
    /** Get DEVICE UID vendor string
     * 
     * @return Pointer to zero terminated uid vendor string
     * 
     * @note
     * If vendor did not define MBED_DEVICEUID_STR_SIZE_MAX, DeviceUid driver will
     * assume the HAL interface function device_uid_get_str() is not implemented
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
     * @param x DEVICE UID Byte index
     * 
     * @return Byte located at index x
     */
    uint8_t operator[](int x)
    {
        if (x >= 0 && x < MBED_DEVICEUID_SIZE) {
            populate_uid_buf();
            return _uidptr[x];
        }

        return 0x00;
    }
    
private:
    void populate_uid_buf();
    void populate_str_buf();
    
    static uint8_t _uidbuf[MBED_DEVICEUID_SIZE];
    static uint8_t* _uidptr;
    static char _strbuf[DEVICEUID_STRING_BUFFER_SIZE];
    static char* _strptr;
#ifndef MBED_DEVICEUID_STR_SIZE_MAX
    static const char _hexChars[16];
#endif
};

} // namespace mbed

#endif

#endif
