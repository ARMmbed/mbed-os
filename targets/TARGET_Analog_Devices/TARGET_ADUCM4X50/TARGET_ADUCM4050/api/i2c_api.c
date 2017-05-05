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
#include "mbed_assert.h"
#include "i2c_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "drivers/i2c/adi_i2c.h"

#define BUILD_I2C_MI_STATIC__

#if defined(BUILD_I2C_MI_STATIC)
static uint8_t          i2c_Mem[ADI_I2C_MEMORY_SIZE];
static ADI_I2C_HANDLE   i2c_Handle;
#if defined(ADI_DEBUG)
#warning "BUILD_I2C_MI_STATIC is defined.  Memory allocation for I2C will be static"
int                 adi_i2c_memtype = 1;
#endif
#else
#if defined(ADI_DEBUG)
#warning "BUILD_I2C_MI_STATIC is NOT defined.  Memory allocation for I2C will be dynamic"
int                 adi_i2c_memtype = 0;
#endif
#endif




void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    uint32_t        i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t        i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);
    ADI_I2C_HANDLE  I2C_Handle;
    ADI_I2C_HANDLE  *pI2C_Handle;
    uint8_t         *I2C_Mem;
    ADI_I2C_RESULT  I2C_Return = 0;
    uint32_t        I2C_DevNum = I2C_0;     /* ADuCM4050 only has 1 I2C port */


#if defined(BUILD_I2C_MI_STATIC)
    I2C_DevNum = I2C_0;
    pI2C_Handle = &i2c_Handle;
    obj->pI2C_Handle = pI2C_Handle;
    I2C_Handle = *obj->pI2C_Handle;
    obj->I2C_Handle = I2C_Handle;
    I2C_Mem = &i2c_Mem[0];
#else
    I2C_DevNum = I2C_0;
    pI2C_Handle = &obj->I2C_Handle;
    obj->pI2C_Handle = pI2C_Handle;
    I2C_Handle = *obj->pI2C_Handle;
    obj->I2C_Handle = I2C_Handle;
    I2C_Mem = obj->I2C_Mem;
#endif


    obj->instance = pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)obj->instance != NC);
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
    SystemCoreClockUpdate();
    I2C_Return = adi_i2c_Open(I2C_DevNum, I2C_Mem, ADI_I2C_MEMORY_SIZE, pI2C_Handle);
    if (I2C_Return) {
        obj->error = I2C_EVENT_ERROR;
        return;
    }
    I2C_Return = adi_i2c_Reset(*pI2C_Handle);
    if (I2C_Return) {
        obj->error = I2C_EVENT_ERROR;
        return;
    }
}


int i2c_start(i2c_t *obj)
{
    return 0;
}


int i2c_stop(i2c_t *obj)
{
    return 0;
}


void i2c_frequency(i2c_t *obj, int hz)
{
    ADI_I2C_HANDLE  I2C_Handle;
    ADI_I2C_RESULT  I2C_Return = 0;


    I2C_Handle = *obj->pI2C_Handle;
    I2C_Return = adi_i2c_SetBitRate(I2C_Handle, (uint32_t) hz);
    if (I2C_Return) {
        obj->error = I2C_EVENT_ERROR;
        return;
    }
}


int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    ADI_I2C_RESULT      I2C_Return;
    ADI_I2C_TRANSACTION I2C_inst;
    uint8_t             I2C_PrologueData = 0x00;
    uint32_t            I2C_Errors;                 /* HW Error result */
    ADI_I2C_HANDLE      I2C_Handle;


    I2C_Handle = *obj->pI2C_Handle;
    I2C_Return = adi_i2c_SetSlaveAddress(I2C_Handle, (address & 0x0000FFFF));
    I2C_inst.pPrologue     = &I2C_PrologueData;
    I2C_inst.nPrologueSize = 0;
    I2C_inst.pData         = (uint8_t*) data;
    I2C_inst.nDataSize     = length;
    I2C_inst.bReadNotWrite = true;
    I2C_inst.bRepeatStart  = stop;
    I2C_Return = adi_i2c_ReadWrite(I2C_Handle, &I2C_inst, &I2C_Errors);
    if (I2C_Return) {
        obj->error = I2C_EVENT_ERROR;
        return -1;
    } else {
        return length;
    }
}


int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    ADI_I2C_RESULT      I2C_Return = 0;
    ADI_I2C_TRANSACTION I2C_inst;
    uint8_t             I2C_PrologueData = 0x00;
    uint32_t            I2C_Errors;                 /* HW Error result */
    ADI_I2C_HANDLE      I2C_Handle;


    I2C_Handle = *obj->pI2C_Handle;
    I2C_Return = adi_i2c_SetSlaveAddress(I2C_Handle, (address & 0x0000FFFF));
    I2C_inst.pPrologue      = &I2C_PrologueData;
    I2C_inst.nPrologueSize  = 0;
    I2C_inst.pData          = (uint8_t*) data;
    I2C_inst.nDataSize      = length;
    I2C_inst.bReadNotWrite  = false;
    I2C_inst.bRepeatStart   = stop;
    I2C_Return = adi_i2c_ReadWrite(I2C_Handle, &I2C_inst, &I2C_Errors);
    if (I2C_Return) {
        obj->error = I2C_EVENT_ERROR;
        return -1;
    } else {
        return length;
    }
}


void i2c_reset(i2c_t *obj)
{
    return;
}


int i2c_byte_read(i2c_t *obj, int last)
{
    return 0;
}


int i2c_byte_write(i2c_t *obj, int data)
{
    return 0;
}



#if DEVICE_I2CSLAVE
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{

}

int i2c_slave_receive(i2c_t *obj)
{

}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    return length;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    return length;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{

}
#endif  // #if DEVICE_I2CSLAVE

#endif  // #if DEVICE_I2C
