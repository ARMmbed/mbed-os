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

#include "MbedPlatform.h"

ATCAError MbedPlatform::Init()
{
#if defined(DEVICE_I2C)
    if (i2c != NULL)
        return ATCA_ERR_DEVICE_ALREADY_INITIALIZED;

    i2c = new I2C(sda, scl);
    if (i2c == NULL)
        return ATCA_ERR_MEM_ALLOC_FAILURE;
    i2c->frequency(freq);
    return ATCA_SUCCESS;
#else
    i2c = NULL;
    return ATCA_ERR_NO_I2C;
#endif
}

void MbedPlatform::Deinit()
{
    if (i2c != NULL)
        delete i2c;
    i2c = NULL;
}

void MbedPlatform::SDALow(uint32_t us)
{
    Deinit();
    DigitalOut sdao(sda);
    sdao = 0;
    WaitUs(us);
    sdao = 1;
}

void MbedPlatform::WaitUs(uint32_t us)
{
    wait_us(us);
}

ATCAError MbedPlatform::Read(uint8_t * buf, size_t len)
{
    int status =
#if defined(DEVICE_I2C)
        i2c->read(addr, (char *)buf, len);
#else
        1;
#endif
    //Dump(buf, len, false);
    return (status != 0)?ATCA_ERR_I2C_READ_ERROR:ATCA_SUCCESS;
}

ATCAError MbedPlatform::Write(uint8_t * buf, size_t len)
{
    //Dump(buf, len, true);
    int status =
#if defined(DEVICE_I2C)
        i2c->write(addr, (const char *)buf, len);
#else
        1;
#endif
    return (status != 0)?ATCA_ERR_I2C_WRITE_ERROR:ATCA_SUCCESS;
}

void MbedPlatform::Dump(uint8_t * buf, size_t len, bool tx)
{
    printf ("%s: ", (tx)?"Tx":"Rx");
    for (size_t i = 0; i < len; i++)
    {
        printf ("0x%02x ", buf[i]);
    }
    printf ("\r\n");
}
