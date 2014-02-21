/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
  

#ifndef __UUID_H__
#define __UUID_H__

#include "blecommon.h"

class UUID
{
private:
    
public:
    enum
    {
        UUID_TYPE_SHORT = 0,    // Short BLE UUID
        UUID_TYPE_LONG  = 1     // Full 128-bit UUID
    };
    
    UUID(void);
    UUID(uint8_t const[16]);
    UUID(uint16_t const);
    virtual ~UUID(void);
    
    uint8_t   type;        // UUID_TYPE_SHORT or UUID_TYPE_LONG
    uint8_t   base[16];    // in case of custom
    uint16_t  value;       // 16 bit uuid (byte 2-3 using with base)
    
    ble_error_t update(uint8_t const[16]);
    ble_error_t update(uint16_t const);
};

#endif
