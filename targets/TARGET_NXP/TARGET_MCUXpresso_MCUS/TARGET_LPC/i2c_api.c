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
#include "fsl_i2c.h"
#include "PeripheralPins.h"

/* Array of I2C peripheral base address. */
static I2C_Type *const i2c_addrs[] = I2C_BASE_PTRS;

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->instance = pinmap_merge(i2c_sda, i2c_scl);
    obj->next_repeated_start = 0;
    MBED_ASSERT((int)obj->instance != NC);

    i2c_master_config_t master_config;

    switch (obj->instance) {
        case 0:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
            RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
            break;
        case 1:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);
            RESET_PeripheralReset(kFC1_RST_SHIFT_RSTn);
            break;
        case 2:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
            RESET_PeripheralReset(kFC2_RST_SHIFT_RSTn);
            break;
        case 3:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM3);
            RESET_PeripheralReset(kFC3_RST_SHIFT_RSTn);
            break;
        case 4:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM4);
            RESET_PeripheralReset(kFC4_RST_SHIFT_RSTn);
            break;
        case 5:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM5);
            RESET_PeripheralReset(kFC5_RST_SHIFT_RSTn);
            break;
        case 6:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM6);
            RESET_PeripheralReset(kFC6_RST_SHIFT_RSTn);
            break;
        case 7:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM7);
            RESET_PeripheralReset(kFC7_RST_SHIFT_RSTn);
            break;
#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 8U)
        case 8:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM8);
            RESET_PeripheralReset(kFC8_RST_SHIFT_RSTn);
            break;
#endif
#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 9U)
        case 9:
            CLOCK_AttachClk(kFRO12M_to_FLEXCOMM9);
            RESET_PeripheralReset(kFC9_RST_SHIFT_RSTn);
            break;
#endif
    }

    I2C_MasterGetDefaultConfig(&master_config);
    I2C_MasterInit(i2c_addrs[obj->instance], &master_config, 12000000);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
}

int i2c_start(i2c_t *obj)
{
    I2C_Type *base = i2c_addrs[obj->instance];
    uint32_t status;

    do {
        status = I2C_GetStatusFlags(base);
    } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);

    /* Clear controller state. */
    I2C_MasterClearStatusFlags(base, I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK);

    /* Start the transfer */
    base->MSTDAT = 0;
    base->MSTCTL = I2C_MSTCTL_MSTSTART_MASK;

    do {
        status = I2C_GetStatusFlags(base);
    } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);

    return 0;
}

int i2c_stop(i2c_t *obj)
{
    I2C_Type *base = i2c_addrs[obj->instance];
    uint32_t status;

    do {
        status = I2C_GetStatusFlags(base);
    } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);

    /* Clear controller state. */
    I2C_MasterClearStatusFlags(base, I2C_STAT_MSTARBLOSS_MASK | I2C_STAT_MSTSTSTPERR_MASK);

    base->MSTCTL = I2C_MSTCTL_MSTSTOP_MASK;

    do {
        status = I2C_GetStatusFlags(base);
    } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);

    return 0;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    I2C_MasterSetBaudRate(i2c_addrs[obj->instance], hz, 12000000);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    I2C_Type *base = i2c_addrs[obj->instance];
    i2c_master_transfer_t master_xfer;

    memset(&master_xfer, 0, sizeof(master_xfer));
    master_xfer.slaveAddress = address >> 1;
    master_xfer.direction = kI2C_Read;
    master_xfer.data = (uint8_t *)data;
    master_xfer.dataSize = length;
    if (obj->next_repeated_start) {
        master_xfer.flags |= kI2C_TransferRepeatedStartFlag;
    }
    if (!stop) {
        master_xfer.flags |= kI2C_TransferNoStopFlag;
    }
    obj->next_repeated_start = master_xfer.flags & kI2C_TransferNoStopFlag ? 1 : 0;

    if (I2C_MasterTransferBlocking(base, &master_xfer) != kStatus_Success) {
        return I2C_ERROR_NO_SLAVE;
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    I2C_Type *base = i2c_addrs[obj->instance];
    i2c_master_transfer_t master_xfer;

    if (length == 0) {
        if (I2C_MasterStart(base, address >> 1, kI2C_Write) != kStatus_Success) {
            return I2C_ERROR_NO_SLAVE;
        }

        if (((I2C_GetStatusFlags(base) & I2C_STAT_MSTSTATE_MASK) >> I2C_STAT_MSTSTATE_SHIFT) == I2C_STAT_MSTCODE_NACKADR) {
            i2c_stop(obj);
            return I2C_ERROR_NO_SLAVE;
        } else {
            i2c_stop(obj);
            return length;
        }
    }

    memset(&master_xfer, 0, sizeof(master_xfer));
    master_xfer.slaveAddress = address >> 1;
    master_xfer.direction = kI2C_Write;
    master_xfer.data = (uint8_t *)data;
    master_xfer.dataSize = length;
    if (obj->next_repeated_start) {
        master_xfer.flags |= kI2C_TransferRepeatedStartFlag;
    }
    if (!stop) {
        master_xfer.flags |= kI2C_TransferNoStopFlag;
    }
    obj->next_repeated_start = master_xfer.flags & kI2C_TransferNoStopFlag ? 1 : 0;

    if (I2C_MasterTransferBlocking(base, &master_xfer) != kStatus_Success) {
        return I2C_ERROR_NO_SLAVE;
    }

    return length;
}

void i2c_reset(i2c_t *obj)
{
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    uint8_t data;
    I2C_Type *base = i2c_addrs[obj->instance];
    uint32_t status;

    do {
        status = I2C_GetStatusFlags(base);
    } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);

    data = base->MSTDAT;

    if (!last) {
        base->MSTCTL = I2C_MSTCTL_MSTCONTINUE_MASK; //ACK and Continue
    }

    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    I2C_Type *base = i2c_addrs[obj->instance];
    uint32_t status;
    int ret_value = 1;

    // write the data
    base->MSTDAT = data;

    base->MSTCTL = I2C_MSTCTL_MSTCONTINUE_MASK;

    do {
        status = I2C_GetStatusFlags(base);
    } while ((status & I2C_STAT_MSTPENDING_MASK) == 0);


    /* Check if arbitration lost */
    if (status & I2C_STAT_MSTARBLOSS_MASK) {
        I2C_MasterClearStatusFlags(base, I2C_STAT_MSTARBLOSS_MASK);
        ret_value = 2;
    }

    /* Check if no acknowledgement (NAK) */
    if (((status & I2C_STAT_MSTSTATE_MASK) >> I2C_STAT_MSTSTATE_SHIFT) == I2C_STAT_MSTCODE_NACKDAT) {
        ret_value = 0;
    }

    return ret_value;
}


#if DEVICE_I2CSLAVE
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    i2c_slave_config_t slave_config;
    I2C_SlaveGetDefaultConfig(&slave_config);
    slave_config.enableSlave = (bool)enable_slave;

    I2C_SlaveInit(i2c_addrs[obj->instance], &slave_config, 12000000);
}

int i2c_slave_receive(i2c_t *obj)
{
    uint32_t status_flags = I2C_GetStatusFlags(i2c_addrs[obj->instance]);

    if (status_flags & kI2C_SlaveSelected) {
        if (((status_flags >> I2C_STAT_SLVSTATE_SHIFT) & I2C_STAT_SLVSTATE_MASK) == 0x1) {
            // read addressed
            return 1;
        } else {
            // write addressed
            return 3;
        }
    } else {
        // slave not addressed
        return 0;
    }
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    I2C_Type *base = i2c_addrs[obj->instance];

    I2C_SlaveReadBlocking(base, (uint8_t *)data, length);

    return length;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    I2C_Type *base = i2c_addrs[obj->instance];

    I2C_SlaveWriteBlocking(base, (uint8_t *)data, length);

    return length;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    if ((idx >= 0) && (idx <= 3)) {
        I2C_SlaveSetAddress(i2c_addrs[obj->instance], (i2c_slave_address_register_t)idx, address, false);
    }
}
#endif

#endif
