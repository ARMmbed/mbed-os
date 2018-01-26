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

#ifndef ATCAPLATFORMINTERFACE_H
#define ATCAPLATFORMINTERFACE_H

#include <stdint.h>
#include <stddef.h>
#include "ATCAError.h"

/** Abstract interface class for required platform interface.
 */
class ATCAPlatformInterface {
protected:
    uint8_t     addr;
public:
    ATCAPlatformInterface(uint8_t i2c_addr)
        : addr(i2c_addr) {
    }
    virtual ~ATCAPlatformInterface(){}

    virtual ATCAError   Init() = 0;
    virtual void        Deinit() = 0;
    virtual void        SDALow(uint32_t us) = 0;
    virtual void        WaitUs(uint32_t us) = 0;
    virtual ATCAError   Read(uint8_t * buf, size_t len) = 0;
    virtual ATCAError   Write(uint8_t * buf, size_t len) = 0;
};

#endif /* ATCAPLATFORMINTERFACE_H */
