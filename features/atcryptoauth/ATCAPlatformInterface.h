/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

/** Platform interface required for serving I2C based ATCA device.
 */
class ATCAPlatformInterface {
protected:
    uint8_t     addr; /** I2C device address */
public:
    ATCAPlatformInterface(uint8_t i2c_addr)
        : addr(i2c_addr) {
    }
    virtual ~ATCAPlatformInterface(){}

    /** Initialize I2C.
     *
     *  @return         Error code from enum ATCAError.
     */
    virtual ATCAError   Init() = 0;

    /** Deinit I2C. Particularly useful when ATCAECC508A watchdog timer is to
     *  be reset. That requires pulling SDA low. Hence requiring a re-init
     *  of I2C afterwords.
     *
     *  @return     Error code from enum ATCAError.
     */
    virtual void        Deinit() = 0;

    /** Pull SDA pin low for given amount of time.
     *
     *  @param us   Delay in micro seconds
     */
    virtual void        SDALow(uint32_t us) = 0;

    /** Wait for given amount of time.
     *
     *  @param us   Delay in micro seconds
     */
    virtual void        WaitUs(uint32_t us) = 0;

    /** Read data from I2C.
     *
     *  @param buf  Buffer to receive data in.
     *  @param len  Buffer length.
     *  @return     Error code from enum ATCAError.
     */
    virtual ATCAError   Read(uint8_t * buf, size_t len) = 0;

    /** Write data on I2C.
     *
     *  @param buf  Buffer containing send data.
     *  @param len  Buffer length.
     *  @return     Error code from enum ATCAError.
     */
    virtual ATCAError   Write(uint8_t * buf, size_t len) = 0;
};

#endif /* ATCAPLATFORMINTERFACE_H */
