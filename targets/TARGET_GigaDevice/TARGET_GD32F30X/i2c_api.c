/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#include "PeripheralPins.h"

#if DEVICE_I2C_ASYNCH
#define I2C_S(obj) (struct i2c_s *) (&((obj)->i2c))
#else
#define I2C_S(obj) (struct i2c_s *) (obj)
#endif

#define BUSY_TIMEOUT  ((SystemCoreClock / obj_s->freq) * 2 * 10)
#define FLAG_TIMEOUT  (0x1000U)

/** Reset I2C peripheral by hardware method. Most of the implementation enable RCU reset.
 *
 *  @param obj  The I2C object
 */
static void i2c_hw_reset(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    switch (obj_s->i2c) {
        case I2C_0:
            rcu_periph_reset_enable(RCU_I2C0RST);
            rcu_periph_reset_disable(RCU_I2C0RST);
            break;

        case I2C_1:
            rcu_periph_reset_enable(RCU_I2C1RST);
            rcu_periph_reset_disable(RCU_I2C1RST);
            break;
    }
}

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 *  @param obj  The I2C object
 *  @param sda  The sda pin
 *  @param scl  The scl pin
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    struct i2c_s *obj_s = I2C_S(obj);

    /* find the I2C by pins */
    uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);

    obj_s->sda = sda;
    obj_s->scl = scl;
    obj_s->i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj_s->i2c != (I2CName)NC);

    switch (obj_s->i2c) {
        case I2C_0:
            /* enable I2C0 clock and configure the pins of I2C0 */
            obj_s->index = 0;
            rcu_periph_clock_enable(RCU_I2C0);

            break;

        case I2C_1:
            /* enable I2C1 clock and configure the pins of I2C1 */
            obj_s->index = 1;
            rcu_periph_clock_enable(RCU_I2C1);

            break;

        default:
            break;
    }

    /* configure the pins of I2C */
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    /* 100 KHz as the default I2C frequence */
    i2c_frequency(obj, 100000);

    obj_s->state = (operation_state_enum)I2C_STATE_NONE;
    obj_s->previous_state_mode = I2C_STATE_NONE;
    obj_s->global_trans_option = I2C_FIRST_AND_LAST_FRAME;

#if DEVICE_I2CSLAVE
    /* I2C master by default */
    obj_s->slave = 0;
#endif
}

/** Configure the I2C frequency
 *
 *  @param obj The I2C object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz)
{
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);

    /* wait until I2C_FLAG_I2CBSY flag is reset */
    timeout = BUSY_TIMEOUT;
    while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_I2CBSY)) && (--timeout != 0));

    /* reset to clear pending flags */
    i2c_hw_reset(obj);

    /* disable I2C peripheral */
    i2c_disable(obj_s->i2c);

    /* configure I2C frequence */
    i2c_clock_config(obj_s->i2c, hz, I2C_DTCY_2);

    /* configure I2C address mode and slave address */
    i2c_mode_addr_config(obj_s->i2c, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0);

    /* enable I2C peripheral */
    i2c_enable(obj_s->i2c);
}

/** Reset I2C peripheral. TODO: The action here. Most of the implementation sends stop()
 *
 *  @param obj The I2C object
 */
void i2c_reset(i2c_t *obj)
{
    i2c_stop(obj);
}

/** Send START command
 *
 *  @param obj The I2C object
 */
int i2c_start(i2c_t *obj)
{
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);

    /* clear I2C_FLAG_AERR Flag */
    i2c_flag_clear(obj_s->i2c, I2C_FLAG_AERR);

    /* wait until I2C_FLAG_I2CBSY flag is reset */
    timeout = FLAG_TIMEOUT;
    while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_I2CBSY)) == SET) {
        if ((timeout--) == 0) {
            return (int)GD_BUSY;
        }
    }

    /* ensure the i2c has been stopped */
    timeout = FLAG_TIMEOUT;
    while ((I2C_CTL0(obj_s->i2c) & I2C_CTL0_STOP) == I2C_CTL0_STOP) {
        if ((timeout--) == 0) {
            return (int)GD_ERROR;
        }
    }

    /* generate a START condition */
    i2c_start_on_bus(obj_s->i2c);

    /* ensure the i2c has been started successfully */
    timeout = FLAG_TIMEOUT;
    while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_SBSEND)) == RESET) {
        if ((timeout--) == 0) {
            return (int)GD_ERROR;
        }
    }

    return (int)GD_OK;
}

/** Send STOP command
 *
 *  @param obj The I2C object
 */
int i2c_stop(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    /* generate a STOP condition */
    i2c_stop_on_bus(obj_s->i2c);

    /* wait for STOP bit reset */
    while ((I2C_CTL0(obj_s->i2c) & I2C_CTL0_STOP));

    return 0;
}

/** Read one byte
 *
 *  @param obj The I2C object
 *  @param last Acknoledge
 *  @return The read byte
 */
int i2c_byte_read(i2c_t *obj, int last)
{
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);

    if (last) {
        /* disable acknowledge */
        i2c_ack_config(obj_s->i2c, I2C_ACK_DISABLE);
    } else {
        /* enable acknowledge */
        i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);
    }

    /* wait until the byte is received */
    timeout = FLAG_TIMEOUT;
    while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_RBNE)) == RESET) {
        if ((timeout--) == 0) {
            return -1;
        }
    }

    return (int)I2C_DATA(obj_s->i2c);
}

/** Write one byte
 *
 *  @param obj The I2C object
 *  @param data Byte to be written
 *  @return 0 if NAK was received, 1 if ACK was received, 2 for timeout.
 */
int i2c_byte_write(i2c_t *obj, int data)
{
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);

    I2C_DATA(obj_s->i2c) = (uint8_t)data;

    /* wait until the byte is transmitted */
    timeout = FLAG_TIMEOUT;
    while (((i2c_flag_get(obj_s->i2c, I2C_FLAG_TBE)) == RESET) &&
            ((i2c_flag_get(obj_s->i2c, I2C_FLAG_BTC)) == RESET)) {
        if ((timeout--) == 0) {
            return 2;
        }
    }

    return 1;
}

/** Blocking reading data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of read bytes
 */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint32_t count = 0U;
    int timeout = 0;

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME ||
            obj_s->global_trans_option == I2C_LAST_FRAME) {
        if (stop) {
            obj_s->global_trans_option = I2C_FIRST_AND_LAST_FRAME;
        } else {
            obj_s->global_trans_option = I2C_FIRST_FRAME;
        }
    } else if (obj_s->global_trans_option == I2C_FIRST_FRAME ||
               obj_s->global_trans_option == I2C_NEXT_FRAME) {
        if (stop) {
            obj_s->global_trans_option = I2C_LAST_FRAME;
        } else {
            obj_s->global_trans_option = I2C_NEXT_FRAME;
        }
    }

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_FIRST_FRAME) {
        /* wait until I2C_FLAG_I2CBSY flag is reset */
        timeout = FLAG_TIMEOUT;
        while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_I2CBSY)) == SET) {
            if ((timeout--) == 0) {
                i2c_stop(obj);
                return I2C_ERROR_BUS_BUSY;
            }
        }
    }

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_FIRST_FRAME ||
            obj_s->previous_state_mode != I2C_STATE_MASTER_BUSY_RX) {
        /* generate a START condition */
        i2c_start_on_bus(obj_s->i2c);

        /* ensure the i2c has been started successfully */
        timeout = FLAG_TIMEOUT;
        while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_SBSEND)) == RESET) {
            if ((timeout--) == 0) {
                i2c_stop(obj);
                return I2C_ERROR_BUS_BUSY;
            }
        }

        /* send slave address */
        i2c_master_addressing(obj_s->i2c, address, I2C_RECEIVER);

        if (1 == length) {
            /* disable acknowledge */
            i2c_ack_config(obj_s->i2c, I2C_ACK_DISABLE);
            /* send a stop condition to I2C bus*/
        } else if (2 == length) {
            /* send a NACK for the next data byte which will be received into the shift register */
            i2c_ackpos_config(obj_s->i2c, I2C_ACKPOS_NEXT);
            /* disable acknowledge */
            i2c_ack_config(obj_s->i2c, I2C_ACK_DISABLE);
        } else {
            /* enable acknowledge */
            i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);
        }

        /* wait until I2C_FLAG_ADDSEND flag is set */
        while (!i2c_flag_get(obj_s->i2c, I2C_FLAG_ADDSEND)) {
            timeout++;
            if (timeout > 100000) {
                i2c_stop(obj);
                return I2C_ERROR_NO_SLAVE;
            }
        }

        /* clear ADDSEND */
        i2c_flag_clear(obj_s->i2c, I2C_FLAG_ADDSEND);
    }

    obj_s->state = (operation_state_enum)I2C_STATE_MASTER_BUSY_RX;

    for (count = 0; count < length; count++) {
        if (length > 2 && count == length - 3) {
            while (RESET == i2c_flag_get(obj_s->i2c, I2C_FLAG_BTC));
            i2c_ack_config(obj_s->i2c, I2C_ACK_DISABLE);
        } else if (2 == length && count == 0) {
            while (RESET == i2c_flag_get(obj_s->i2c, I2C_FLAG_BTC));
        }

        while (RESET == i2c_flag_get(obj_s->i2c, I2C_FLAG_RBNE));
        data[count] = i2c_data_receive(obj_s->i2c);
    }

    obj_s->previous_state_mode = obj_s->state;

    /* if not sequential read, then send stop */
    if (stop) {
        i2c_stop(obj);
    }

    return count;
}

/** Blocking sending data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @param stop    Stop to be generated after the transfer is done
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    struct i2c_s *obj_s = I2C_S(obj);
    gd_status_enum status = GD_OK;
    uint32_t count = 0;
    int timeout = 0;

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME ||
            obj_s->global_trans_option == I2C_LAST_FRAME) {
        if (stop) {
            obj_s->global_trans_option = I2C_FIRST_AND_LAST_FRAME;
        } else {
            obj_s->global_trans_option = I2C_FIRST_FRAME;
        }
    } else if (obj_s->global_trans_option == I2C_FIRST_FRAME ||
               obj_s->global_trans_option == I2C_NEXT_FRAME) {
        if (stop) {
            obj_s->global_trans_option = I2C_LAST_FRAME;
        } else {
            obj_s->global_trans_option = I2C_NEXT_FRAME;
        }
    }

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_FIRST_FRAME) {
        /* wait until I2C_FLAG_I2CBSY flag is reset */
        timeout = FLAG_TIMEOUT;
        while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_I2CBSY)) == SET) {
            if ((timeout--) == 0) {
                i2c_stop(obj);
                return I2C_ERROR_BUS_BUSY;
            }
        }
    }

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_FIRST_FRAME ||
            obj_s->previous_state_mode != I2C_STATE_MASTER_BUSY_TX) {
        /* generate a START condition */
        i2c_start_on_bus(obj_s->i2c);

        /* ensure the i2c has been started successfully */
        timeout = FLAG_TIMEOUT;
        while ((i2c_flag_get(obj_s->i2c, I2C_FLAG_SBSEND)) == RESET) {
            if ((timeout--) == 0) {
                i2c_stop(obj);
                return I2C_ERROR_BUS_BUSY;
            }
        }

        /* send slave address */
        i2c_master_addressing(obj_s->i2c, address, I2C_TRANSMITTER);

        /* wait until I2C_FLAG_ADDSEND flag is set */
        while (!i2c_flag_get(obj_s->i2c, I2C_FLAG_ADDSEND)) {
            timeout++;
            if (timeout > 100000) {
                i2c_stop(obj);
                return I2C_ERROR_NO_SLAVE;
            }
        }

        /* clear ADDSEND */
        i2c_flag_clear(obj_s->i2c, I2C_FLAG_ADDSEND);
    }

    obj_s->state = (operation_state_enum)I2C_STATE_MASTER_BUSY_TX;

    for (count = 0; count < length; count++) {
        status = (gd_status_enum)i2c_byte_write(obj, data[count]);
        if (status != 1) {
            i2c_stop(obj);
            return count;
        }
    }

    obj_s->previous_state_mode = obj_s->state;

    /* if not sequential write, then send stop */
    if (stop) {
        i2c_stop(obj);
    }

    return count;
}

const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

#if DEVICE_I2CSLAVE

/** Configure I2C address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    struct i2c_s *obj_s = I2C_S(obj);

    /* disable I2C peripheral */
    i2c_disable(obj_s->i2c);
    /* I2C clock configure */
    i2c_clock_config(obj_s->i2c, 100000, I2C_DTCY_2);
    /* I2C address configure */
    i2c_mode_addr_config(obj_s->i2c, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, address);
    /* enable I2C0 */
    i2c_enable(obj_s->i2c);
    /* enable acknowledge */
    i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);
}

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param enable_slave Enable i2c hardware so you can receive events with ::i2c_slave_receive
 *  @return non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    struct i2c_s *obj_s = I2C_S(obj);

    if (enable_slave) {
        obj_s->slave = 1;
    } else {
        obj_s->slave = 0;
    }
}

/* the same as the definition in I2CSlave.h class I2CSlave */
#define NoData         0 /* the slave has not been addressed */
#define ReadAddressed  1 /* the master has requested a read from this slave (slave as transmitter) */
#define WriteGeneral   2 /* the master is writing to all slave */
#define WriteAddressed 3 /* the master is writing to this slave (slave as receiver) */

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addresses, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int i2c_slave_receive(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    int ret = NoData;

    i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);

    if (i2c_flag_get(obj_s->i2c, I2C_FLAG_ADDSEND)) {
        i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);
        if (i2c_flag_get(obj_s->i2c, I2C_FLAG_RXGC)) {
            ret = WriteGeneral;
        }

        if (i2c_flag_get(obj_s->i2c, I2C_FLAG_TRS)) {
            ret = ReadAddressed;
        } else {
            ret = WriteAddressed;
        }
    }

    i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);

    return (ret);
}

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @return non-zero if a value is available
 */
int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    struct i2c_s *obj_s = I2C_S(obj);
    int count = 0;
    int timeout = 0;

    i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);

    /* wait until ADDSEND bit is set */
    while (!i2c_flag_get(obj_s->i2c, I2C_FLAG_ADDSEND)) {
        timeout++;
        if (timeout > 100000) {
            return -1;
        }
    }
    /* clear ADDSEND bit */
    i2c_flag_clear(obj_s->i2c, I2C_FLAG_ADDSEND);

    while (0 < length) {
        /* wait until the RBNE bit is set */
        timeout = 0;
        while (!i2c_flag_get(obj_s->i2c, I2C_FLAG_RBNE)) {
            timeout++;
            if (timeout > 100000) {
                return -1;
            }
        }
        *data = i2c_data_receive(obj_s->i2c);
        data++;
        length--;
        count++;
    }
    /* wait until the STPDET bit is set */
    timeout = 0;
    while (!i2c_flag_get(obj_s->i2c, I2C_FLAG_STPDET)) {
        timeout++;
        if (timeout > 100) {
            return count;
        }
    }
    /* clear the STPDET bit */
    i2c_enable(obj_s->i2c);

    i2c_ack_config(obj_s->i2c, I2C_ACK_DISABLE);

    return count;
}

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @return non-zero if a value is available
 */
int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    struct i2c_s *obj_s = I2C_S(obj);
    int count = 0;
    int timeout = 0;

    i2c_ack_config(obj_s->i2c, I2C_ACK_ENABLE);
    /* wait until ADDSEND bit is set */
    while (!i2c_flag_get(obj_s->i2c, I2C_FLAG_ADDSEND)) {
        timeout++;
        if (timeout > 100000) {
            return -1;
        }
    }
    /* clear ADDSEND bit */
    i2c_flag_clear(obj_s->i2c, I2C_FLAG_ADDSEND);
    while (length > 0) {
        /* wait until the TBE bit is set */
        timeout = 0;
        while (!i2c_flag_get(obj_s->i2c, I2C_FLAG_TBE)) {
            timeout++;
            if (timeout > 100000) {
                return -1;
            }
        }
        i2c_data_transmit(obj_s->i2c, *data);
        data++;
        length--;
        count++;
    }
    /* the master doesn't acknowledge for the last byte */
    timeout = 0;
    while (!i2c_flag_get(obj_s->i2c, I2C_FLAG_AERR)) {
        timeout++;
        if (timeout > 100000) {
            return -1;
        }
    }
    /* clear the bit of AERR */
    i2c_flag_clear(obj_s->i2c, I2C_FLAG_AERR);
    /* disable acknowledge */
    i2c_ack_config(obj_s->i2c, I2C_ACK_DISABLE);

    return count;
}
#endif /* DEVICE_I2CSLAVE */

#endif /* DEVICE_I2C */
