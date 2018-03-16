/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "mbed_assert.h"
#include "i2c_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "drivers/i2c/adi_i2c.h"



#if defined(BUILD_I2C_MI_DYNAMIC)
#if defined(ADI_DEBUG)
#warning "BUILD_I2C_MI_DYNAMIC is defined.  Memory allocation for I2C will be dynamic"
int adi_i2c_memtype = 0;
#endif
#else
/*******************************************************************************
   ADI_I2C_DEV_DATA_TYPE Instance memory containing memory pointer should
   guarantee 4 byte alignmnet.
 *******************************************************************************/
static uint32_t         i2c_Mem[(ADI_I2C_MEMORY_SIZE + 3)/4];
static ADI_I2C_HANDLE   i2c_Handle;
#if defined(ADI_DEBUG)
#warning "BUILD_I2C_MI_DYNAMIC is NOT defined.  Memory allocation for I2C will be static"
int adi_i2c_memtype = 1;
#endif
#endif



void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    uint32_t        i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t        i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);
    ADI_I2C_HANDLE  *pI2C_Handle;
    uint32_t        *I2C_Mem;
    ADI_I2C_RESULT  I2C_Return = ADI_I2C_SUCCESS;
    uint32_t        I2C_DevNum = I2C_0;     /* ADuCM4050 only has 1 I2C port */


#if defined(BUILD_I2C_MI_DYNAMIC)
    I2C_DevNum = I2C_0;
    pI2C_Handle = &obj->I2C_Handle;
    obj->pI2C_Handle = pI2C_Handle;
    I2C_Mem = obj->I2C_Mem;
#else
    I2C_DevNum = I2C_0;
    pI2C_Handle = &i2c_Handle;
    obj->pI2C_Handle = pI2C_Handle;
    I2C_Mem = &i2c_Mem[0];
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
    /* The Hardware does not support this feature. */
    return -1;
}


int i2c_stop(i2c_t *obj)
{
    /* The Hardware does not support this feature. */
    return -1;
}


void i2c_frequency(i2c_t *obj, int hz)
{
    ADI_I2C_HANDLE  I2C_Handle;
    ADI_I2C_RESULT  I2C_Return = ADI_I2C_SUCCESS;


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
    ADI_I2C_RESULT      I2C_Return = ADI_I2C_SUCCESS;
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
    ADI_I2C_RESULT      I2C_Return;
    ADI_I2C_HANDLE      I2C_Handle = *obj->pI2C_Handle;

    I2C_Return = adi_i2c_Reset(I2C_Handle);
    if (I2C_Return) {
        obj->error = I2C_EVENT_ERROR;
        return;
    }
}


int i2c_byte_read(i2c_t *obj, int last)
{
    /* The Hardware does not support this feature. */
    return -1;
}


int i2c_byte_write(i2c_t *obj, int data)
{
    /* The Hardware does not support this feature. */
    return -1;
}

#endif  // #if DEVICE_I2C
