/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
 
#include "mbed_assert.h"
#include "i2c_api.h"


#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "mbed_wait_api.h"
#include "us_ticker_api.h"
#include "W7500x_i2c.h"

/* Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted. */
#define FLAG_TIMEOUT ((int)0x1000)
#define LONG_TIMEOUT ((int)0xFFFF)


void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    I2C_ConfigStruct conf;
    
    //Determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->I2Cx = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj->I2Cx != (I2CName)NC);
    
    obj->sda = sda;
    obj->scl = scl;
    obj->ADDRESS = 0x0;
    obj->is_setAddress = 0;
    
    conf.sda = (I2C_PinName)obj->sda;
    conf.scl = (I2C_PinName)obj->scl;
    
    I2C_Init(&conf);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    
}

inline int i2c_start(i2c_t *obj)
{
    I2C_ConfigStruct conf;
    conf.sda = (I2C_PinName)obj->sda;
    conf.scl = (I2C_PinName)obj->scl;
    
    I2C_Start(&conf);
    
    return 0;
}

inline int i2c_stop(i2c_t *obj)
{  
    I2C_ConfigStruct conf;
    conf.sda = (I2C_PinName)obj->sda;
    conf.scl = (I2C_PinName)obj->scl;
    
    I2C_Stop(&conf);
   
    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    I2C_ConfigStruct conf;

    conf.sda = (I2C_PinName)obj->sda;
    conf.scl = (I2C_PinName)obj->scl;
    
    
    if(stop)
    {  
        if(I2C_Read(&conf, address,  (uint8_t*)data, length) != 0)
            return -1;
    }
    else
    {
        if(I2C_ReadRepeated(&conf, address,  (uint8_t*)data, length) != 0)
            return -1;
    }
    
    return length;
    
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    I2C_ConfigStruct conf;
    
    conf.sda = (I2C_PinName)obj->sda;
    conf.scl = (I2C_PinName)obj->scl;
    
    
    if(stop)
    {  
        if(I2C_Write(&conf, address,  (uint8_t*)data, length) != 0)
           return -1;
    }
    else
    {
        if(I2C_WriteRepeated(&conf, address,  (uint8_t*)data, length) != 0)
           return -1;
    }
    
    return length;
}

int i2c_byte_read(i2c_t *obj, int last)
{
    uint8_t ret;
    I2C_ConfigStruct conf;
    
    conf.sda = (I2C_PinName)obj->sda;
    conf.scl = (I2C_PinName)obj->scl;
    
    ret = I2C_ReadByte(&conf);
    
    if(last)
        I2C_SendNACK(&conf);
    else
        I2C_SendACK(&conf);
    
    return (int)ret;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    I2C_ConfigStruct conf;
    
    conf.sda = (I2C_PinName)obj->sda;
    conf.scl = (I2C_PinName)obj->scl;
    
    if(I2C_WriteByte(&conf, (uint8_t)data)) // NACK
        return 0;
    else //ack
        return 1;
}

void i2c_reset(i2c_t *obj)
{
   
}


#endif // DEVICE_I2C
