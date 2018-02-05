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
#include "fsl_lpi2c.h"
#include "PeripheralPins.h"

/* 7 bit IIC addr - R/W flag not included */
static int i2c_address = 0;
/* Array of I2C peripheral base address. */
static LPI2C_Type *const i2c_addrs[] = LPI2C_BASE_PTRS;

extern void i2c_setup_clock();
extern uint32_t i2c_get_clock();
void pin_mode_opendrain(PinName pin, bool enable);

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->instance = pinmap_merge(i2c_sda, i2c_scl);
    obj->next_repeated_start = 0;
    MBED_ASSERT((int)obj->instance != NC);

    lpi2c_master_config_t master_config;

    i2c_setup_clock();

    LPI2C_MasterGetDefaultConfig(&master_config);
    LPI2C_MasterInit(i2c_addrs[obj->instance], &master_config, i2c_get_clock());

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    pin_mode_opendrain(sda, true);
    pin_mode_opendrain(scl, true);
}

int i2c_start(i2c_t *obj)
{
    if (LPI2C_MasterStart(i2c_addrs[obj->instance], 0, kLPI2C_Write) != kStatus_Success) {
        return 1;
    }

    return 0;
}

int i2c_stop(i2c_t *obj)
{
    obj->next_repeated_start = 0;
    if (LPI2C_MasterStop(i2c_addrs[obj->instance]) != kStatus_Success) {
        return 1;
    }

    return 0;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    uint32_t busClock;

    busClock = i2c_get_clock();
    LPI2C_MasterSetBaudRate(i2c_addrs[obj->instance], hz, busClock);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    LPI2C_Type *base = i2c_addrs[obj->instance];
    lpi2c_master_transfer_t master_xfer;

    i2c_address = address >> 1;
    memset(&master_xfer, 0, sizeof(master_xfer));
    master_xfer.slaveAddress = address >> 1;
    master_xfer.direction = kLPI2C_Read;
    master_xfer.data = (uint8_t *)data;
    master_xfer.dataSize = length;
    if (obj->next_repeated_start) {
        master_xfer.flags |= kLPI2C_TransferRepeatedStartFlag;
    }
    if (!stop) {
        master_xfer.flags |= kLPI2C_TransferNoStopFlag;
    }
    obj->next_repeated_start = master_xfer.flags & kLPI2C_TransferNoStopFlag ? 1 : 0;

    /* The below function will issue a STOP signal at the end of the transfer.
     * This is required by the hardware in order to receive the last byte
     */
    if (LPI2C_MasterTransferBlocking(base, &master_xfer) != kStatus_Success) {
        return I2C_ERROR_NO_SLAVE;
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    LPI2C_Type *base = i2c_addrs[obj->instance];
    lpi2c_master_transfer_t master_xfer;

    if (length == 0) {
        if (LPI2C_MasterStart(base, address >> 1, kLPI2C_Write) != kStatus_Success) {
            return I2C_ERROR_NO_SLAVE;
        }

        if (base->MSR & kLPI2C_MasterNackDetectFlag) {
            i2c_stop(obj);
            return I2C_ERROR_NO_SLAVE;
        } else {
            i2c_stop(obj);
            return length;
        }
    }

    memset(&master_xfer, 0, sizeof(master_xfer));
    master_xfer.slaveAddress = address >> 1;
    master_xfer.direction = kLPI2C_Write;
    master_xfer.data = (uint8_t *)data;
    master_xfer.dataSize = length;
    if (obj->next_repeated_start) {
        master_xfer.flags |= kLPI2C_TransferRepeatedStartFlag;
    }
    if (!stop) {
        master_xfer.flags |= kLPI2C_TransferNoStopFlag;
    }
    obj->next_repeated_start = master_xfer.flags & kLPI2C_TransferNoStopFlag ? 1 : 0;

    if (LPI2C_MasterTransferBlocking(base, &master_xfer) != kStatus_Success) {
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
    LPI2C_Type *base = i2c_addrs[obj->instance];
    lpi2c_master_transfer_t master_xfer;

    memset(&master_xfer, 0, sizeof(master_xfer));
    master_xfer.slaveAddress = i2c_address;
    master_xfer.direction = kLPI2C_Read;
    master_xfer.data = &data;
    master_xfer.dataSize = 1;
    master_xfer.flags = kLPI2C_TransferNoStopFlag;

    if (LPI2C_MasterTransferBlocking(base, &master_xfer) != kStatus_Success) {
        return I2C_ERROR_NO_SLAVE;
    }
    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    LPI2C_Type *base = i2c_addrs[obj->instance];
    lpi2c_master_transfer_t master_xfer;
    status_t ret_value;

    memset(&master_xfer, 0, sizeof(master_xfer));
    master_xfer.slaveAddress = i2c_address;
    master_xfer.direction = kLPI2C_Write;
    master_xfer.data = &data;
    master_xfer.dataSize = 1;
    master_xfer.flags = kLPI2C_TransferNoStopFlag;

    ret_value = LPI2C_MasterTransferBlocking(base, &master_xfer);

    if (ret_value == kStatus_Success) {
        return 1;
    } else if (ret_value == kStatus_LPI2C_Nak) {
        return 0;
    } else {
        return 2;
    }
}


#if DEVICE_I2CSLAVE
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    lpi2c_slave_config_t slave_config;
    LPI2C_SlaveGetDefaultConfig(&slave_config);
    slave_config.enableSlave = (bool)enable_slave;

    LPI2C_SlaveInit(i2c_addrs[obj->instance], &slave_config, i2c_get_clock());
}

int i2c_slave_receive(i2c_t *obj)
{
    LPI2C_Type *base = i2c_addrs[obj->instance];
    uint32_t status_flags = LPI2C_SlaveGetStatusFlags(base);

    if (status_flags & kLPI2C_SlaveAddressValidFlag) {
        if (base->SASR & kLPI2C_Read) {
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
    LPI2C_Type *base = i2c_addrs[obj->instance];
    int actual_rx;

    LPI2C_SlaveReceive(base, (uint8_t *)data, length, (size_t *)&actual_rx);

    return actual_rx;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    LPI2C_Type *base = i2c_addrs[obj->instance];
    int actual_rx;

    LPI2C_SlaveSend(base, (uint8_t *)data, length, (size_t *)&actual_rx);

    return actual_rx;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    i2c_addrs[obj->instance]->SAMR = LPI2C_SAMR_ADDR0(address & 0xfe);
}
#endif

#endif
