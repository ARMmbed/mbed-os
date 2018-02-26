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

#ifndef MBEDPLATFORM_H
#define MBEDPLATFORM_H

#include "mbed.h"
#include "ATCAPlatformInterface.h"

#if !defined(DEVICE_I2C)
typedef void I2C;
#endif /* DEVICE_I2C */

/** Mbed implementation of abstract Platform interface.
 */
class MbedPlatform : public ATCAPlatformInterface {
private:
    /** I2C pin names */
    PinName     sda;
    PinName     scl;
    I2C *       i2c;    /** I2C driver object */
    int32_t     freq;   /** I2C frequency */
public:
    /** Instantiate platform object with I2C parameters required for
     *  accessing ATCAECC508A device.
     */
    MbedPlatform(PinName i2c_sda, PinName i2c_scl, int32_t hz, uint8_t i2c_addr)
        : ATCAPlatformInterface(i2c_addr), sda(i2c_sda), scl(i2c_scl), i2c(NULL), freq(hz)
    { 
    }

    virtual ~MbedPlatform() {
#if defined(DEVICE_I2C)
        if (i2c != NULL)
            delete i2c;
#endif
    }

    /** Initialize I2C.
     *
     *  @return         Error code from enum ATCAError.
     */
    virtual ATCAError Init();

    /** Deinit I2C. Particularly useful when ATCAECC508A watchdog timer is to be
     *  reset. That requires pulling SDA low. Hence requiring a re-init of I2C
     *  afterwards.
     */
    virtual void Deinit();

    /** Pull SDA pin low for given amount of time.
     *
     *  @param us   Delay in micro seconds
     */
    virtual void SDALow(uint32_t us);

    /** Wait for given amount of time.
     *
     *  @param us   Delay in micro seconds
     */
    virtual void WaitUs(uint32_t us);

    /** Read data from I2C.
     *
     *  @param buf  Buffer to receive data in.
     *  @param len  Buffer length.
     *  @return     Error code from enum ATCAError.
     */
    virtual ATCAError Read(uint8_t * buf, size_t len);

    /** Write data on I2C.
     *
     *  @param buf  Buffer containing send data.
     *  @param len  Buffer length.
     *  @return     Error code from enum ATCAError.
     */
    virtual ATCAError Write(uint8_t * buf, size_t len);
private:

    /** Dump Tx/Rx data.
     *
     *  @param buf  Data buffer.
     *  @param len  Buffer length.
     *  @param isTx True for Tx data, False for Rx data.
     */
    void Dump(uint8_t * buf, size_t len, bool isTx);
};

#endif /* MBEDPLATFORM_H */

