/*******************************************************************************
 * Copyright (c) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include "mbed_assert.h"
#include "i2c_api.h"

#if DEVICE_I2C

#include "i2c_regs.h"
#include "mxc_i2c.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#ifndef MXC_I2CM_RX_TIMEOUT
#define MXC_I2CM_RX_TIMEOUT     0x5000
#endif

#ifndef I2C_ERROR
#define I2C_ERROR   (MXC_F_I2C_INT_FL0_ARB_ER | MXC_F_I2C_INT_FL0_TO_ER | MXC_F_I2C_INT_FL0_ADDR_NACK_ER | \
                    MXC_F_I2C_INT_FL0_DATA_ER | MXC_F_I2C_INT_FL0_DO_NOT_RESP_ER | MXC_F_I2C_INT_FL0_START_ER | \
                    MXC_F_I2C_INT_FL0_STOP_ER)
#endif

#define MBED_NAK     0
#define MBED_ACK     1
#define MBED_TIMEOUT 2

//******************************************************************************
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    // SDA and SCL must map to same peripheral instance
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    mxc_i2c_regs_t *i2c = (mxc_i2c_regs_t*)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)i2c != NC);

    obj->i2c = i2c;
    obj->start_pending = 0;

    MXC_I2C_Shutdown(obj->i2c);
    MXC_I2C_Init(obj->i2c, 1, 0);// configure as master
}

//******************************************************************************
void i2c_frequency(i2c_t *obj, int hz)
{
    MXC_I2C_SetFrequency(obj->i2c, hz);
}

//******************************************************************************
int i2c_start(i2c_t *obj)
{ 
    int ret;
    ret = MXC_I2C_Start(obj->i2c);
    if (ret != 0) {
        //...
    }
    return ret;
}

//******************************************************************************
int i2c_stop(i2c_t *obj)
{ 
    int ret;
    ret = MXC_I2C_Stop(obj->i2c);
    if (ret != 0) {
        //...
    }
    return ret;
}

//******************************************************************************
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int ret = 0;
    mxc_i2c_req_t reqMaster;

    reqMaster.i2c = obj->i2c;
    reqMaster.addr = address;
    reqMaster.tx_buf = NULL;
    reqMaster.tx_len = 0;
    reqMaster.rx_buf = (unsigned char *)data;
    reqMaster.rx_len = length;
    reqMaster.restart = !stop;
    reqMaster.callback = NULL;

    ret = MXC_I2C_MasterTransaction(&reqMaster);
    if (ret == 0) {
        ret = length;
    }

    return ret;
}

//******************************************************************************
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int ret = 0;
    mxc_i2c_req_t reqMaster;

    reqMaster.i2c = obj->i2c;
    reqMaster.addr = address;
    reqMaster.tx_buf = (unsigned char *)data;
    reqMaster.tx_len = length;
    reqMaster.rx_buf = NULL;
    reqMaster.rx_len = 0;
    reqMaster.restart = !stop;
    reqMaster.callback = NULL;

    ret = MXC_I2C_MasterTransaction(&reqMaster);
    if (ret == 0) {
        ret = length;
    }

    return ret;
}

//******************************************************************************
void i2c_reset(i2c_t *obj)
{
    MXC_I2C_Recover(obj->i2c, 5);
}

//******************************************************************************
int i2c_byte_read(i2c_t *obj, int ack)
{
    unsigned char byt = 0; 

    MXC_I2C_ReadByte(obj->i2c, &byt, ack);

    return byt;
}

//******************************************************************************
int i2c_byte_write(i2c_t *obj, int data)
{
    int ret;

    ret = MXC_I2C_WriteByte(obj->i2c, (unsigned char)data);

    /*  
        Functions returns:
          =0 if byte is acknowledged, 
          =1 if not acknowledged, 
          <0 if error
    */
    // Convert return value
    if (ret == 0) {
        ret = MBED_ACK; // ACK Receviced
    } else if (ret == 1) {
        ret = MBED_NAK; // NAK Received
    } else {
        //... Error
    }

    return ret;
}

//******************************************************************************
const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

//******************************************************************************
const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

//******************************************************************************
const PinMap *i2c_slave_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

//******************************************************************************
const PinMap *i2c_slave_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

#if 0 // DEVICE_I2CSLAVE
//******************************************************************************
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{

}
//******************************************************************************
int i2c_slave_receive(i2c_t *obj)
{


}

//******************************************************************************
int i2c_slave_read(i2c_t *obj, char *data, int length)
{

}

//******************************************************************************
int i2c_slave_write(i2c_t *obj, const char *data, int length)
{

}

//******************************************************************************
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{

}

#endif

#endif  // #if DEVICE_I2C
