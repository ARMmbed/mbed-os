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
#include "fsl_port.h"
#include "peripheral_clock_defines.h"
#include "PeripheralPins.h"

/* 7 bit IIC addr - R/W flag not included */
static int i2c_address = 0;
/* Array of I2C peripheral base address. */
static I2C_Type *const i2c_addrs[] = I2C_BASE_PTRS;
/* Array of I2C bus clock frequencies */
static clock_name_t const i2c_clocks[] = I2C_CLOCK_FREQS;

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);
    PORT_Type *port_addrs[] = PORT_BASE_PTRS;
    PORT_Type *base = port_addrs[sda >> GPIO_PORT_SHIFT];

    obj->instance = pinmap_merge(i2c_sda, i2c_scl);
    obj->next_repeated_start = 0;
    MBED_ASSERT((int)obj->instance != NC);

    i2c_master_config_t master_config;

    I2C_MasterGetDefaultConfig(&master_config);
    I2C_MasterInit(i2c_addrs[obj->instance], &master_config, CLOCK_GetFreq(i2c_clocks[obj->instance]));
    I2C_EnableInterrupts(i2c_addrs[obj->instance], kI2C_GlobalInterruptEnable);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    /* Enable internal pullup resistor */
    base->PCR[sda & 0xFF] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
    base->PCR[scl & 0xFF] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);

#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    base->PCR[sda & 0xFF] |= PORT_PCR_ODE_MASK;
    base->PCR[scl & 0xFF] |= PORT_PCR_ODE_MASK;
#endif
}

int i2c_start(i2c_t *obj)
{
    I2C_Type *base = i2c_addrs[obj->instance];
    uint32_t statusFlags = I2C_MasterGetStatusFlags(base);

    /* Check if the bus is already in use. */
    if (statusFlags & kI2C_BusBusyFlag) {
        /* Send a repeat START signal. */
        base->C1 |= I2C_C1_RSTA_MASK;
    } else {
        /* Send the START signal. */
        base->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;
    }

#if defined(FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING) && FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING
    while (!(base->S2 & I2C_S2_EMPTY_MASK))
    {
    }
#endif /* FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING */

    return 0;
}

int i2c_stop(i2c_t *obj)
{
    if (I2C_MasterStop(i2c_addrs[obj->instance]) != kStatus_Success) {
        return 1;
    }

    return 0;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    uint32_t busClock;

    busClock = CLOCK_GetFreq(i2c_clocks[obj->instance]);
    I2C_MasterSetBaudRate(i2c_addrs[obj->instance], hz, busClock);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    I2C_Type *base = i2c_addrs[obj->instance];
    i2c_master_transfer_t master_xfer;

    i2c_address = address >> 1;
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

    /* The below function will issue a STOP signal at the end of the transfer.
     * This is required by the hardware in order to receive the last byte
     */
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

        while (!(base->S & kI2C_IntPendingFlag)) {
        }

        base->S = kI2C_IntPendingFlag;

        if (base->S & kI2C_ReceiveNakFlag) {
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

    /* Setup the I2C peripheral to receive data. */
    base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);

    if (last) {
        base->C1 |= I2C_C1_TXAK_MASK; // NACK
    }

    data = (base->D & 0xFF);

    /* Change direction to Tx to avoid extra clocks. */
    base->C1 |= I2C_C1_TX_MASK;

    /* Wait until data transfer complete. */
    while (!(base->S & kI2C_IntPendingFlag))
    {
    }

    /* Clear the IICIF flag. */
    base->S = kI2C_IntPendingFlag;

    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    int ret_value = 1;
    uint8_t statusFlags = 0;
    I2C_Type *base = i2c_addrs[obj->instance];

    /* Setup the I2C peripheral to transmit data. */
    base->C1 |= I2C_C1_TX_MASK;

    /* Send a byte of data. */
    base->D = data;

    /* Wait until data transfer complete. */
    while (!(base->S & kI2C_IntPendingFlag)) {
    }

    statusFlags = base->S;

    /* Clear the IICIF flag. */
    base->S = kI2C_IntPendingFlag;

    /* Check if arbitration lost */
    if (statusFlags & kI2C_ArbitrationLostFlag) {
        base->S = kI2C_ArbitrationLostFlag;
        ret_value = 2;
    }

    /* Check if no acknowledgement (NAK) */
    if (statusFlags & kI2C_ReceiveNakFlag) {
        base->S = kI2C_ReceiveNakFlag;
        ret_value = 0;
    }

    return ret_value;
}


#if DEVICE_I2CSLAVE
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    i2c_slave_config_t slave_config;
    I2C_SlaveGetDefaultConfig(&slave_config);
    slave_config.slaveAddress = 0;
    slave_config.enableSlave = (bool)enable_slave;
#if FSL_I2C_DRIVER_VERSION > MAKE_VERSION(2, 0, 1)
    I2C_SlaveInit(i2c_addrs[obj->instance], &slave_config, CLOCK_GetFreq(i2c_clocks[obj->instance]));
#else
    I2C_SlaveInit(i2c_addrs[obj->instance], &slave_config);
#endif
}

int i2c_slave_receive(i2c_t *obj)
{
    uint32_t status_flags = I2C_SlaveGetStatusFlags(i2c_addrs[obj->instance]);

    if (status_flags & kI2C_AddressMatchFlag) {
        if (status_flags & kI2C_TransferDirectionFlag) {
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

    if (base->S & kI2C_AddressMatchFlag) {
        /* Slave receive, master writing to slave. */
        base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
        /* Read dummy to release the bus. */
        base->D;
    }

    I2C_SlaveReadBlocking(base, (uint8_t *)data, length);

    return length;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    I2C_Type *base = i2c_addrs[obj->instance];

    I2C_SlaveWriteBlocking(base, (uint8_t *)data, length);

    /* Switch to receive mode. */
    base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
    /* Read dummy to release bus. */
    base->D;

    return length;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    i2c_addrs[obj->instance]->A1 = address & 0xfe;
}
#endif

#endif
