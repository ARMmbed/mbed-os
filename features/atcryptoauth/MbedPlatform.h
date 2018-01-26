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

#ifndef MBEDPLATFORM_H
#define MBEDPLATFORM_H

#include "mbed.h"
#include "ATCAPlatformInterface.h"

#define DEV_DELAY_TPU_MS    (100 + 10)
#define DEV_DELAY_TWLO_MS   (60 + 20)
#define DEV_DELAY_TWHI_MS   (1500 + 10)

/** Mbed implementation of abstract Platform interface.
 */
class MbedPlatform : public ATCAPlatformInterface {
private:
    /* I2C pin names */
    PinName     sda;
    PinName     scl;
    I2C *       i2c;
    int32_t     freq;
public:
    MbedPlatform(PinName i2c_sda, PinName i2c_scl, int32_t hz, uint8_t i2c_addr)
        : ATCAPlatformInterface(i2c_addr), sda(i2c_sda), scl(i2c_scl), i2c(NULL), freq(hz)
    { 
    }

    virtual ~MbedPlatform() {
        if (i2c != NULL)
            delete i2c;
    }
    virtual ATCAError   Init();
    virtual void        Deinit();
    virtual void        SDALow(uint32_t us);
    virtual void        WaitUs(uint32_t us);
    virtual ATCAError   Read(uint8_t * buf, size_t len);
    virtual ATCAError   Write(uint8_t * buf, size_t len);
private:
    void Dump(uint8_t * buf, size_t len, bool tx);
};

#endif /* MBEDPLATFORM_H */

