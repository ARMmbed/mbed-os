/*******************************************************************************
 * Copyright (c) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
#include "i2cm_regs.h"
#include "i2cm.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#ifndef MXC_I2CM_RX_TIMEOUT
#define MXC_I2CM_RX_TIMEOUT     0x5000
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
    mxc_i2cm_regs_t *i2c = (mxc_i2cm_regs_t*)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)i2c != NC);

    obj->i2c = i2c;
    obj->fifo = MXC_I2CM_GET_FIFO(MXC_I2CM_GET_IDX(i2c));
    obj->start_pending = 0;

    // Merge pin function requests for use with CMSIS init func
    ioman_req_t io_req;
    pin_function_t *pin_func;
    pin_func = (pin_function_t *)pinmap_find_function(sda, PinMap_I2C_SDA);
    io_req.value  = pin_func->req_val;
    pin_func = (pin_function_t *)pinmap_find_function(scl, PinMap_I2C_SCL);
    io_req.value |= pin_func->req_val;

    obj->sys_cfg.io_cfg.req_reg = pin_func->reg_req;
    obj->sys_cfg.io_cfg.ack_reg = pin_func->reg_ack;
    obj->sys_cfg.io_cfg.req_val = io_req;
    obj->sys_cfg.clk_scale = CLKMAN_SCALE_DIV_1;

    I2CM_Init(obj->i2c, &obj->sys_cfg, I2CM_SPEED_400KHZ);
}

//******************************************************************************
void i2c_frequency(i2c_t *obj, int hz)
{
    I2CM_Init(obj->i2c, &obj->sys_cfg, hz);
}

//******************************************************************************
int i2c_start(i2c_t *obj)
{
    obj->start_pending = 1;
    return 0;
}

//******************************************************************************
int i2c_stop(i2c_t *obj)
{
    obj->start_pending = 0;
    I2CM_WriteTxFifo(obj->i2c, obj->fifo, MXC_S_I2CM_TRANS_TAG_STOP);
    I2CM_TxInProgress(obj->i2c);
    return 0;
}

//******************************************************************************
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    MBED_ASSERT(stop != 0);
    return I2CM_Read(obj->i2c, address >> 1, NULL, 0, (uint8_t *)data, length);
}

//******************************************************************************
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    mxc_i2cm_regs_t *i2cm = obj->i2c;
    mxc_i2cm_fifo_regs_t *fifo = obj->fifo;

    if (stop) {
        return I2CM_Write(i2cm, address >> 1, NULL, 0, (uint8_t *)data, length);
    }

    i2cm->inten = 0;
    i2cm->intfl = i2cm->intfl;
    if (I2CM_Tx(i2cm, fifo, address >> 1, (uint8_t *)data, length, 0) == E_NO_ERROR) {
        return length;
    } else {
        return -1;
    }
}

//******************************************************************************
void i2c_reset(i2c_t *obj)
{
    I2CM_Recover(obj->i2c);
}

//******************************************************************************
int i2c_byte_read(i2c_t *obj, int last)
{
    mxc_i2cm_regs_t *i2cm = obj->i2c;
    mxc_i2cm_fifo_regs_t *fifo = obj->fifo;
    int tmp;

    // Start the transaction if it is not currently ongoing
    if (!(i2cm->trans & MXC_F_I2CM_TRANS_TX_IN_PROGRESS)) {
        i2cm->trans |= MXC_F_I2CM_TRANS_TX_START;
    }

    if (last) {
        // NACK the last read byte
        if (I2CM_WriteTxFifo(i2cm, fifo, MXC_S_I2CM_TRANS_TAG_RXDATA_NACK) != E_NO_ERROR) {
            goto byte_read_err;
        }

        // Send the stop condition
        if (I2CM_WriteTxFifo(i2cm, fifo, MXC_S_I2CM_TRANS_TAG_STOP) != E_NO_ERROR) {
            goto byte_read_err;
        }
    } else {
        if (I2CM_WriteTxFifo(i2cm, fifo, MXC_S_I2CM_TRANS_TAG_RXDATA_COUNT) != E_NO_ERROR) {
            goto byte_read_err;
        }
    }

    do {
        // Wait for data in RX FIFO
        int timeout = MXC_I2CM_RX_TIMEOUT;
        while (!(i2cm->intfl & MXC_F_I2CM_INTFL_RX_FIFO_NOT_EMPTY) &&
                ((i2cm->bb & MXC_F_I2CM_BB_RX_FIFO_CNT) == 0)) {

            if((timeout-- < 0) || (i2cm->trans & MXC_F_I2CM_TRANS_TX_TIMEOUT)) {
                goto byte_read_err;
            }

            if (i2cm->trans & (MXC_F_I2CM_TRANS_TX_LOST_ARBITR | MXC_F_I2CM_TRANS_TX_NACKED)) {
                goto byte_read_err;
            }
        }
        i2cm->intfl = MXC_F_I2CM_INTFL_RX_FIFO_NOT_EMPTY;

    } while ((tmp = fifo->rx) & MXC_S_I2CM_RSTLS_TAG_EMPTY);

    return (uint8_t)tmp;

byte_read_err:
    i2c_reset(obj);
    return -1;
}

//******************************************************************************
int i2c_byte_write(i2c_t *obj, int data)
{
    mxc_i2cm_regs_t *i2cm = obj->i2c;
    mxc_i2cm_fifo_regs_t *fifo = obj->fifo;
    int result;

    if (obj->start_pending) {
        obj->start_pending = 0;
        data |= MXC_S_I2CM_TRANS_TAG_START;
    } else {
        data |= MXC_S_I2CM_TRANS_TAG_TXDATA_ACK;
    }

    if ((result = I2CM_WriteTxFifo(i2cm, fifo, data)) != E_NO_ERROR) {
        i2c_reset(obj);
        if (result == E_COMM_ERR) {
            return MBED_NAK;
        }
        return MBED_TIMEOUT;
    }

    // Start the transaction if it is not currently ongoing
    if (!(i2cm->trans & MXC_F_I2CM_TRANS_TX_IN_PROGRESS)) {
        i2cm->trans |= MXC_F_I2CM_TRANS_TX_START;
    }

    return MBED_ACK;
}
