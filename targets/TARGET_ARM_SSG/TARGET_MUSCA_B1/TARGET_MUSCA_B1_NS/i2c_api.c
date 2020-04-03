/* mbed Microcontroller Library
 * Copyright (c) 2017-2020 Arm Limited
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

#include "i2c_api.h"
#include "mbed_error.h"
#include "i2c_ip6510_drv.h"
#include "objects.h"
#include "pinmap.h"
#include "PeripheralNames.h"

/* Return error codes for the i2c_byte_write api */
#define BYTE_TRANSFER_ERR_NACK (0)
#define BYTE_TRANSFER_ERR_NONE (1)
#define BYTE_TRANSFER_ERR_TIMEOUT (2)

/* Macros for frequency configuration */
#define I2C_SPEED_100KHZ (100000)
#define I2C_SPEED_400KHZ (400000)

#if DEVICE_I2CSLAVE
/* Return values for slave addressing specified in mbed I2C slave driver */
#define I2C_SLAVE_READ_ADDRESS (1)
#define I2C_SLAVE_WRITE_ALL_ADDRESS (2)
#define I2C_SLAVE_WRITE_ADDRESS (3)
#define I2C_SLAVE_NOT_ADDRESSED (0)
#endif

extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];

/* Extend the return values defined in i2c_api.h */
enum {
    I2C_ERROR_NONE = 0,
    /* I2C_ERROR_NO_SLAVE = -1,
     * I2C_ERROR_BUS_BUSY = -2, */
    I2C_ERROR_GENERAL = -3
};

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    enum i2c_ip6510_error_t ret;

    /* Determine the I2C to use */
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    if (i2c == (I2CName)NC) {
        error("I2C pinout mapping failed, invalid pins.");
        return;
    }

    /* Initialize the parent object */
    switch (i2c) {
#ifdef I2C0_IP6510_DEV
        case I2C_0:
            obj->dev = &I2C0_IP6510_DEV;
            break;
#endif
#ifdef I2C1_IP6510_DEV
        case I2C_1:
            obj->dev = &I2C1_IP6510_DEV;
            break;
#endif
        default:
            error("Failed to initialize I2C, bad reference.");
            return;
    }
    obj->last_address = 0U;
    obj->sda = sda;
    obj->scl = scl;
    obj->byte_state = BYTE_TRANSFER_STATE_NONE;

    /* If already init, uninit */
    if (i2c_ip6510_get_state(obj->dev) == I2C_IP6510_INITIALIZED) {
        i2c_ip6510_uninit(obj->dev);
    }

    /* Set the GPIO pins */
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    /* Initialize peripheral */
    ret = i2c_ip6510_init(obj->dev, SystemCoreClock);
    i2c_ip6510_set_speed(obj->dev, I2C_IP6510_SPEED_100KHZ);
    i2c_ip6510_set_timeout(obj->dev, 0xFFU);

    if (ret != I2C_IP6510_ERR_NONE) {
        error("Failed to initialize I2C, error occured in native driver.");
        return;
    }
}

void i2c_frequency(i2c_t *obj, int hz)
{
    /* The peripheral only supports 100k and 400k clock in master mode */
    switch (hz) {
        case I2C_SPEED_100KHZ:
            i2c_ip6510_set_speed(obj->dev, I2C_IP6510_SPEED_100KHZ);
            break;

        case I2C_SPEED_400KHZ:
            i2c_ip6510_set_speed(obj->dev, I2C_IP6510_SPEED_400KHZ);
            break;

        default:
            error("Invalid I2C frequency.");
            return;
    }
}

void i2c_reset(i2c_t *obj)
{
    i2c_ip6510_uninit(obj->dev);
    i2c_init(obj, obj->sda, obj->scl);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    enum i2c_ip6510_error_t ret;

    /* Shifted 8 bit address is used in upper layer */
    address = ((uint32_t)address)>>1;

    ret = i2c_ip6510_master_read(obj->dev, (uint16_t)address, (uint8_t*)data,
            stop, (uint32_t*)(&length));

    if (ret != I2C_IP6510_ERR_NONE) {
        return I2C_ERROR_GENERAL;
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    enum i2c_ip6510_error_t ret;

    /* Shifted 8 bit address is used in upper layer */
    address = ((uint32_t)address)>>1;

    /* Parameter checking */
    if (data == NULL || length == 0) {
        ret = i2c_ip6510_monitor_slave(obj->dev, (uint16_t)address);
        if (ret != I2C_IP6510_ERR_NONE) {
            return I2C_ERROR_NO_SLAVE;
        }
        return length;
    }

    ret = i2c_ip6510_master_write(obj->dev, (uint16_t)address, (uint8_t*)data,
            stop, (uint32_t*)(&length));

    if (ret != I2C_IP6510_ERR_NONE) {
        return I2C_ERROR_GENERAL;
    }
    return length;
}

int i2c_start(i2c_t *obj)
{
    /** \note The peripheral does not support building up the transaction by
     *        instructions. The functionality is achieved with a software
     *        state machine.
     *        The i2c transaction is started later, when the address and the
     *        first data byte is written.
     */
    obj->byte_state = BYTE_TRANSFER_STATE_START;
    obj->last_address = 0U;
    i2c_ip6510_hold_enable(obj->dev);

    return I2C_ERROR_NONE;
}

int i2c_stop(i2c_t *obj)
{
    /* Both the master and slave api calls this function */
    if (i2c_ip6510_get_device_mode(obj->dev) == I2C_IP6510_MASTER_MODE) {

        /** \note The peripheral does not support building up the transaction by
         *        instructions. The functionality is achieved with a software
         *        state machine.
         *        STOP condition is generated on hold disable.
         */
        i2c_ip6510_hold_disable(obj->dev);

        /* If there was only one byte written (the slave address) before calling
         * stop, the state machine is in ADDRESS state. Writing just the address
         * is equal to a slave monitoring
         */
        if (obj->byte_state == BYTE_TRANSFER_STATE_ADDRESS) {
            /* Return value is not needed because signalling to the caller is
             * not defined
             */
            (void)i2c_ip6510_monitor_slave(obj->dev, obj->last_address);
        }

        obj->byte_state = BYTE_TRANSFER_STATE_NONE;
        obj->last_address = 0U;

    } else {
        /* In slave mode the the I2C controller only writes and reads data from
         * the I2C bus. The function just clears the interrupts for
         * further adressing detection.
         */
        i2c_ip6510_clear_irq(obj->dev, I2C_IP6510_ALL_INTR_MASK);
    }
    return I2C_ERROR_NONE;
}

int i2c_byte_read(i2c_t *obj, int last)
{
    uint8_t read_byte = 0U;
    uint32_t slave_read_cntr = 1U;

    /* Both the master and slave api calls this function */
    if (i2c_ip6510_get_device_mode(obj->dev) == I2C_IP6510_MASTER_MODE) {

        /** \note The peripheral does not support building up the transaction by
         *        instructions. The functionality is achieved with a software
         *        state machine.
         */
        switch (obj->byte_state) {
            case BYTE_TRANSFER_STATE_ADDRESS:
                obj->byte_state = BYTE_TRANSFER_STATE_DATA;
                i2c_ip6510_master_byte_read(
                        obj->dev, obj->last_address, last, true, &read_byte);
                return read_byte;

            case BYTE_TRANSFER_STATE_DATA:
                i2c_ip6510_master_byte_read(
                        obj->dev, obj->last_address, last, false, &read_byte);
                return read_byte;

            case BYTE_TRANSFER_STATE_NONE:
            case BYTE_TRANSFER_STATE_START:
            default:
                /* Reading is invalid in these states */
                return I2C_ERROR_GENERAL;
        }
    } else {
        /* In slave mode the driver only writes and reads data from the
         * I2C controller, no need to track states.
         */
        i2c_ip6510_slave_read(
                            obj->dev, &read_byte, &slave_read_cntr);
        return read_byte;
    }
}

int i2c_byte_write(i2c_t *obj, int data)
{
    enum i2c_ip6510_error_t ret = I2C_IP6510_ERR_NONE;
    uint32_t slave_write_cntr = 1U;

    /* Both the master and slave api calls this function */
    if (i2c_ip6510_get_device_mode(obj->dev) == I2C_IP6510_MASTER_MODE) {

        /** \note The peripheral does not support building up the transaction by
         *        instructions. The functionality is achieved with a software
         *        state machine.
         */
        switch (obj->byte_state) {
            case BYTE_TRANSFER_STATE_NONE:
                /* Writing is invalid before start symbol
                 * BYTE_TRANSFER_ERR_TIMEOUT is the only error code mbed defines,
                 * this is the only way to signal an error
                 */
                return BYTE_TRANSFER_ERR_TIMEOUT;

            case BYTE_TRANSFER_STATE_START:
                obj->byte_state = BYTE_TRANSFER_STATE_ADDRESS;
                obj->last_address = ((uint32_t)data)>>1;
                return BYTE_TRANSFER_ERR_NONE;

            case BYTE_TRANSFER_STATE_ADDRESS:
                obj->byte_state = BYTE_TRANSFER_STATE_DATA;
                ret = i2c_ip6510_master_byte_write(
                        obj->dev, obj->last_address, (uint8_t*)&data, true);
                break;

            case BYTE_TRANSFER_STATE_DATA:
                ret = i2c_ip6510_master_byte_write(
                        obj->dev, obj->last_address, (uint8_t*)&data, false);
                break;

            default:
                return BYTE_TRANSFER_ERR_TIMEOUT;
        }
    } else {
        /* In slave mode the driver only writes and reads data from the
         * I2C controller, no need to track states.
         */
        ret = i2c_ip6510_slave_write(
                    obj->dev, (uint8_t*)&data, &slave_write_cntr);
    }

    if (ret != I2C_IP6510_ERR_NONE) {
        /* No need to reset the state machine. The host might try to resend
         * the data byte. Also the start and stop functions reset the states.
         */
        if (ret == I2C_IP6510_ERR_NACK) {
            return BYTE_TRANSFER_ERR_NACK;
        }
        return BYTE_TRANSFER_ERR_TIMEOUT;
    }
    return BYTE_TRANSFER_ERR_NONE;
}

#if DEVICE_I2CSLAVE

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    if (!enable_slave) {
        /* Check if master mode is already set */
        if (i2c_ip6510_get_device_mode(obj->dev) != I2C_IP6510_MASTER_MODE) {
            /* Set Master Mode */
            i2c_ip6510_set_master_mode(obj->dev);
        }
    } else {
        /* Check if slave mode is already set */
        if (i2c_ip6510_get_device_mode(obj->dev) != I2C_IP6510_SLAVE_MODE) {
            /* Set Slave Mode */
            i2c_ip6510_set_slave_mode(obj->dev, obj->last_address);
        }
    }
}

int i2c_slave_receive(i2c_t *obj)
{
    uint32_t irq_status = i2c_ip6510_get_irq_status(obj->dev);
    enum i2c_ip6510_transf_dir_t dir = i2c_ip6510_get_slave_tranf_dir(obj->dev);
    uint32_t transfer_size = i2c_ip6510_get_transfer_size(obj->dev);

    if (irq_status & I2C_IP6510_INTR_DATA_MASK) {

        if (dir == I2C_IP6510_TRANSMITTER) {
            /* Slave is adressed for writing */
            return I2C_SLAVE_WRITE_ADDRESS;
        }
        if ((dir == I2C_IP6510_RECEIVER) && (transfer_size != 0U)) {
            /* Slave is adressed for reading */
            return I2C_SLAVE_READ_ADDRESS;
        }
    }

    if ((irq_status & I2C_IP6510_INTR_COMP_MASK)
                    && (dir == I2C_IP6510_RECEIVER) && (transfer_size != 0U))  {
        /* An I2C transfer is complete with less then FIFO_SIZE-2 bytes */
        return I2C_SLAVE_READ_ADDRESS;
    }

    return I2C_SLAVE_NOT_ADDRESSED;
}

int  i2c_slave_read(i2c_t *obj, char *data, int length)
{
    enum i2c_ip6510_error_t ret;

    ret = i2c_ip6510_slave_read(
            obj->dev, (uint8_t*)data, (uint32_t*)&length);
    i2c_ip6510_clear_irq(obj->dev, I2C_IP6510_ALL_INTR_MASK);

    if (ret != I2C_IP6510_ERR_NONE) {
        return 0;
    }

    return length;
}

int  i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    enum i2c_ip6510_error_t ret;

    ret = i2c_ip6510_slave_write(
            obj->dev, (uint8_t*)data, (uint32_t*)&length);
    i2c_ip6510_clear_irq(obj->dev, I2C_IP6510_ALL_INTR_MASK);

    if (ret != I2C_IP6510_ERR_NONE) {
        return 0;
    }

    return length;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    /* Shifted 8 bit address is used in upper layer */
    i2c_ip6510_set_slave_mode(obj->dev, ((uint16_t)address)>>1);
    obj->last_address = address>>1;
}

const PinMap *i2c_slave_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

#endif

const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_I2C_SCL;
}
