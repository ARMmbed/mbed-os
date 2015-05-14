/* mbed Microcontroller Library
 * CMSIS-style functionality to support dynamic vectors
 *******************************************************************************
 * Copyright (c) 2011 ARM Limited. All rights reserved.
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

/* Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted. */
#define FLAG_TIMEOUT ((int)0x1000)
#define LONG_TIMEOUT ((int)0xFFFF)

I2C_TypeDef * I2cHandle;

int i2c0_inited = 0;
int i2c1_inited = 0;

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    // Determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);

    obj->i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj->i2c != (I2CName)NC);

    // Enable I2C1 clock and pinout if not done
    if ((obj->i2c == I2C_0) && !i2c0_inited) {
        i2c0_inited = 1;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, OpenDrain);
        pin_mode(scl, OpenDrain);
    }

    // Enable I2C2 clock and pinout if not done
    if ((obj->i2c == I2C_1) && !i2c1_inited) {
        i2c1_inited = 1;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, OpenDrain);
        pin_mode(scl, OpenDrain);
    }

    // Reset to clear pending flags if any
    i2c_reset(obj);

    // I2C configuration
    i2c_frequency(obj, 100000); // 100 kHz per default
    obj->is_setAddress = 0;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    MBED_ASSERT((hz == 100000) || (hz == 400000) || (hz == 1000000));
    I2cHandle = (I2C_TypeDef *)(obj->i2c);

    // wait before init
    I2C_ConfigStruct conf;

    conf.mode = I2C_Master;
    conf.master.timeout = LONG_TIMEOUT;

    // Common settings: I2C clock = 48 MHz, Analog filter = ON, Digital filter coefficient = 0
    switch (hz) {
        case 100000:
            conf.master.prescale  = 0x61;      // Standard mode with Rise Time = 400ns and Fall Time = 100ns
            break;
        case 400000:
            break;
        case 1000000:
            break;
        default:
            break;
    }

    // I2C configuration
    I2C_Init(I2cHandle, conf);
}

inline int i2c_start(i2c_t *obj)
{
    obj->is_setAddress = 0;

    return 0;
}

inline int i2c_stop(i2c_t *obj)
{
    I2cHandle = (I2C_TypeDef *)(obj->i2c);

    // Generate the STOP condition
    I2C_Stop(I2cHandle);
    I2C_Reset(I2cHandle);
    obj->is_setAddress = 0;

    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    I2cHandle = (I2C_TypeDef *)(obj->i2c);
    int count;
    int value;

    if(!obj->is_setAddress)
    {
       if( I2C_Start(I2cHandle, address, I2C_READ_SA7) == ERROR )
        {
            return -1;
        }
       obj->is_setAddress = 1;
       obj->ADDRESS = address;
    }
    else
    {
        I2C_Restart_Structure(I2cHandle, address, I2C_READ_SA7);
        obj->ADDRESS = address;
    }

    // Read all bytes
    for (count = 0; count < (length-1); count++) {
        if( (value = i2c_byte_read(obj, 0)) == -1) return value;
        data[count] = (char)value;
    }

    if(stop){
        if( (value = i2c_byte_read(obj, 1)) == -1) return value;
        data[count] = (char)value;
    }
    else{
        if( (value = i2c_byte_read(obj, 0)) == -1) return value;
        data[count] = (char)value;
    }

    return count;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    I2cHandle = (I2C_TypeDef *)(obj->i2c);
    int count;

    if(!obj->is_setAddress)
    {
       if( I2C_Start(I2cHandle, address, I2C_WRITE_SA7) == ERROR )
        {
            return -1;
        }
       obj->is_setAddress = 1;
       obj->ADDRESS = address;
    }
    else
    {
        I2C_Restart_Structure(I2cHandle, address, I2C_WRITE_SA7);
        obj->ADDRESS = address;
    }

    for (count = 0; count < length; count++) {
        i2c_byte_write(obj, data[count]);
    }

    // If not repeated start, send stop
    if (stop) {
        i2c_stop(obj);
    }

    return count;
}

int i2c_byte_read(i2c_t *obj, int last)
{
    I2cHandle = (I2C_TypeDef *)(obj->i2c);

    return I2C_ReceiveData(I2cHandle, last);
}

int i2c_byte_write(i2c_t *obj, int data)
{
    I2cHandle = (I2C_TypeDef *)(obj->i2c);
    return I2C_SendDataAck(I2cHandle,(uint8_t)data);
}

void i2c_reset(i2c_t *obj)
{
    I2cHandle = (I2C_TypeDef *)(obj->i2c);

    I2C_Reset(I2cHandle);
}

//#if DEVICE_I2CSLAVE
//
//void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
//{
//    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
//    uint16_t tmpreg = 0;
//
//    // disable
//    i2c->OAR1 &= (uint32_t)(~I2C_OAR1_OA1EN);
//    // Get the old register value
//    tmpreg = i2c->OAR1;
//    // Reset address bits
//    tmpreg &= 0xFC00;
//    // Set new address
//    tmpreg |= (uint16_t)((uint16_t)address & (uint16_t)0x00FE); // 7-bits
//    // Store the new register value
//    i2c->OAR1 = tmpreg;
//    // enable
//    i2c->OAR1 |= I2C_OAR1_OA1EN;
//}
//
//void i2c_slave_mode(i2c_t *obj, int enable_slave)
//{
//    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
//    uint16_t tmpreg;
//
//    // Get the old register value
//    tmpreg = i2c->OAR1;
//
//    // Enable / disable slave
//    if (enable_slave == 1) {
//        tmpreg |= I2C_OAR1_OA1EN;
//    } else {
//        tmpreg &= (uint32_t)(~I2C_OAR1_OA1EN);
//    }
//
//    // Set new mode
//    i2c->OAR1 = tmpreg;
//}
//
//// See I2CSlave.h
//#define NoData         0 // the slave has not been addressed
//#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
//#define WriteGeneral   2 // the master is writing to all slave
//#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)
//
//int i2c_slave_receive(i2c_t *obj)
//{
//    I2cHandle.Instance = (I2C_TypeDef *)(obj->i2c);
//    int retValue = NoData;
//
//    if (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_BUSY) == 1) {
//        if (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_ADDR) == 1) {
//            if (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_DIR) == 1)
//                retValue = ReadAddressed;
//            else
//                retValue = WriteAddressed;
//
//            __HAL_I2C_CLEAR_FLAG(&I2cHandle, I2C_FLAG_ADDR);
//        }
//    }
//
//    return (retValue);
//}
//
//int i2c_slave_read(i2c_t *obj, char *data, int length)
//{
//    char size = 0;
//
//    while (size < length) data[size++] = (char)i2c_byte_read(obj, 0);
//
//    return size;
//}
//
//int i2c_slave_write(i2c_t *obj, const char *data, int length)
//{
//    char size = 0;
//    I2cHandle.Instance = (I2C_TypeDef *)(obj->i2c);
//
//    do {
//        i2c_byte_write(obj, data[size]);
//        size++;
//    } while (size < length);
//
//    return size;
//}
//
//
//#endif // DEVICE_I2CSLAVE

#endif // DEVICE_I2C
