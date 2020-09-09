/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if DEVICE_I2C

#include "i2c_api.h"
#include "iom_api.h"
#include "PeripheralPins.h"
#include "mbed_assert.h"

#define DEFAULT_CLK_FREQ (AM_HAL_IOM_400KHZ)

static am_hal_iom_transfer_t xfer = {0};

I2CName i2c_get_peripheral_name(PinName sda, PinName scl)
{
    uint32_t iom_sda = pinmap_peripheral(sda, i2c_master_sda_pinmap());
    uint32_t iom_scl = pinmap_peripheral(scl, i2c_master_scl_pinmap());

    uint32_t iom = pinmap_merge(iom_sda, iom_scl);

    if ((int)iom == NC) {
        return IOM_NUM;
    }

    return (I2CName)iom;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    MBED_ASSERT(obj);

    // iom determination
    I2CName iom = i2c_get_peripheral_name(sda, scl);
    MBED_ASSERT((int)iom != IOM_NUM);

    // iom configuration
    obj->i2c.iom_obj.iom.inst = (uint32_t)iom;
    obj->i2c.iom_obj.iom.cfg.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    obj->i2c.iom_obj.iom.cfg.ui32ClockFreq = DEFAULT_CLK_FREQ;
    obj->i2c.iom_obj.iom.cfg.pNBTxnBuf = NULL;
    obj->i2c.iom_obj.iom.cfg.ui32NBTxnBufLength = 0;

    // pin configuration
    if ((int)sda != NC) {
        pinmap_config(sda, i2c_master_sda_pinmap());
    }
    if ((int)scl != NC) {
        pinmap_config(scl, i2c_master_scl_pinmap());
    }

    // invariant xfer settings
    xfer.ui32InstrLen = 0;
    xfer.ui32Instr = 0;
    xfer.ui8RepeatCount = 0;
    xfer.ui8Priority = 1;
    xfer.ui32PauseCondition = 0;
    xfer.ui32StatusSetClr = 0;

    // initialization
    iom_init(&obj->i2c.iom_obj);
}

void i2c_free(i2c_t *obj)
{
    MBED_ASSERT(obj);
    iom_deinit(&obj->i2c.iom_obj);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    MBED_ASSERT(obj);
    if (hz > AM_HAL_IOM_MAX_FREQ) {
        hz = AM_HAL_IOM_MAX_FREQ;
    }
    obj->i2c.iom_obj.iom.cfg.ui32ClockFreq = hz;
    iom_init(&obj->i2c.iom_obj);
}

int  i2c_start(i2c_t *obj)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
    return I2C_ERROR_NO_SLAVE;
}

int  i2c_stop(i2c_t *obj)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
    return I2C_ERROR_NO_SLAVE;
}

int i2c_read(i2c_t *obj, int address8bit, char *data, int length, int stop)
{
    MBED_ASSERT(obj);

    int handled_chars = 0;

    xfer.uPeerInfo.ui32I2CDevAddr = (address8bit >> 1);
    xfer.eDirection = AM_HAL_IOM_RX;
    xfer.ui32NumBytes = length;
    xfer.pui32RxBuffer = (uint32_t *)data;
    xfer.pui32TxBuffer = NULL;
    xfer.bContinue = (stop) ? false : true;
    uint32_t status = am_hal_iom_blocking_transfer(obj->i2c.iom_obj.iom.handle, &xfer);
    if (AM_HAL_STATUS_SUCCESS != status) {
        return I2C_ERROR_NO_SLAVE;
    }
    handled_chars += xfer.ui32NumBytes;

    return handled_chars;
}

int i2c_write(i2c_t *obj, int address8bit, const char *data, int length, int stop)
{
    MBED_ASSERT(obj);

    int handled_chars = 0;

    xfer.uPeerInfo.ui32I2CDevAddr = (address8bit >> 1);
    xfer.eDirection = AM_HAL_IOM_TX;
    xfer.ui32NumBytes = length;
    xfer.pui32TxBuffer = (uint32_t *)data;
    xfer.pui32RxBuffer = NULL;
    xfer.bContinue = (stop) ? false : true;
    uint32_t status = am_hal_iom_blocking_transfer(obj->i2c.iom_obj.iom.handle, &xfer);
    if (AM_HAL_STATUS_SUCCESS != status) {
        return I2C_ERROR_NO_SLAVE;
    }
    handled_chars += xfer.ui32NumBytes;

    return handled_chars;
}

void i2c_reset(i2c_t *obj)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
    return I2C_ERROR_NO_SLAVE;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(0);
    return I2C_ERROR_NO_SLAVE;
}

const PinMap *i2c_master_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap(void)
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap(void)
{
    return PinMap_I2C_SCL;
}

#endif // DEVICE_I2C
