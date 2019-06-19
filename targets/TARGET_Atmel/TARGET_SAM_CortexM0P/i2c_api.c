/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#include <math.h>

#include "cmsis.h"
#include "pinmap.h"
#include "sercom.h"
#include "i2c_master.h"
#include "i2c_slave.h"

#include "pinmap_function.h"

#if DEVICE_I2C_ASYNCH
#include "i2c_master_interrupt.h"
#endif


#if DEVICE_I2C_ASYNCH
#define pI2C_S(obj)			(&obj->i2c)
#else
#define pI2C_S(obj)			obj
#endif

#define I2C_MASTER_DEFAULT_BAUD     100000

/**
 * \brief I2C modes enum
 *
 * I2C mode selection.
 */
enum i2c_mode {
    /** Master mode. */
    I2C_MODE_MASTER         = 0x5,
    /** Slave mode. */
    I2C_MODE_SLAVE          = 0x4,
};

/* Extern Variables */
extern uint8_t g_sys_init;

typedef void (*I2CHandler)(void);

#if DEVICE_I2C_ASYNCH
#define _SERCOM_INTERRUPT_HANDLERS(n, unused) (uint32_t)SERCOM##n##_Handler,

/* To save the i2c objects */
static uint32_t i2c_instances[SERCOM_INST_NUM] = {0};
const uint32_t sercom_irq_handlers[SERCOM_INST_NUM] = {
    MREPEAT(SERCOM_INST_NUM, _SERCOM_INTERRUPT_HANDLERS, ~)
};

#endif

/* Forward declaration */
enum status_code _i2c_master_wait_for_bus(
    struct i2c_master_module *const module);

enum status_code _i2c_master_address_response(
    struct i2c_master_module *const module);

enum status_code _i2c_master_send_hs_master_code(
    struct i2c_master_module *const module,
    uint8_t hs_master_code);

/* Adding function from ASF for compatibility */
static enum status_code _i2c_slave_wait_for_bus(
    struct i2c_slave_module *const module)
{
    /* Sanity check arguments. */
    MBED_ASSERT(module);
    MBED_ASSERT(module->hw);

    SercomI2cs *const i2c_hw = &(module->hw->I2CS);

    /* Wait for reply. */
    uint16_t timeout_counter = 0;
    while ((!(i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_DRDY)) &&
            (!(i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_PREC)) &&
            (!(i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH))) {

        /* Check timeout condition. */
        if (++timeout_counter >= module->buffer_timeout) {
            return STATUS_ERR_TIMEOUT;
        }
    }
    return STATUS_OK;
}

/** Initialize the I2C peripheral
 *
 * Configures the pins used by I2C, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The I2C object to initialize
 * @param[in]  sda  The pin to use for SDA
 * @param[in]  scl  The pin to use for SCL
 * @return          void
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    Sercom* hw;
    uint32_t mux_func;
    struct i2c_master_config config_i2c_master;

    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(sda != NC);
    MBED_ASSERT(scl != NC);

    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }

    pI2C_S(obj)->pins[0] = sda;
    pI2C_S(obj)->pins[1] = scl;

    /* Calculate SERCOM instance from pins */
    uint32_t sercom_index = pinmap_merge_sercom(sda, scl);
    if (sercom_index == (uint32_t)NC) {
        return;
    }
    hw = (Sercom*)pinmap_peripheral_sercom(NC, sercom_index);

    i2c_master_get_config_defaults(&config_i2c_master);

    /* SERCOM PAD0 - SDA */
    mux_func = pinmap_function_sercom(sda, sercom_index);
    if (mux_func == (uint32_t)NC) return;
    config_i2c_master.pinmux_pad0 = (sda << 16) | (mux_func & 0xFFFF);

    /* SERCOM PAD1 - SCL */
    mux_func = pinmap_function_sercom(scl, sercom_index);
    if (mux_func == (uint32_t)NC) return;
    config_i2c_master.pinmux_pad1 = (scl << 16) | (mux_func & 0xFFFF);

    /* Default baud rate is set to 100kHz */
    pI2C_S(obj)->baud_rate = I2C_MASTER_DEFAULT_BAUD;
    config_i2c_master.baud_rate = pI2C_S(obj)->baud_rate / 1000;

    while(i2c_master_init(&pI2C_S(obj)->master, hw, &config_i2c_master) != STATUS_OK);
    pI2C_S(obj)->mode = I2C_MODE_MASTER;

#if DEVICE_I2C_ASYNCH
    /* Save the i2c object */
    i2c_instances[sercom_index] = (uint32_t)obj;
#endif

    i2c_master_enable(&pI2C_S(obj)->master);
}

/** Configure the I2C frequency.
 *  @param obj The i2c object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz)
{
    /* Temporary variables. */
    int32_t baud_rate;
    int32_t tmp_baud;
    int32_t tmp_baud_hs;
    enum status_code tmp_status_code = STATUS_OK;

    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    /* Return if in Slave mode, slave do not have any baud to set */
    if (pI2C_S(obj)->mode != I2C_MODE_MASTER) return;

    SercomI2cm *const i2c_module = &(pI2C_S(obj)->master.hw->I2CM);

    /* Disable I2C Module */
    i2c_master_disable(&pI2C_S(obj)->master);

    baud_rate = hz / 1000; /* To kHz */

    /* Give a dummy supported value */
    pI2C_S(obj)->baud_rate_high_speed = I2C_MASTER_BAUD_RATE_3400KHZ;

    uint32_t sercom_index = _sercom_get_sercom_inst_index(pI2C_S(obj)->master.hw);

    /* Find and set baudrate. */
    tmp_baud = (int32_t)(div_ceil(
                             system_gclk_chan_get_hz(SERCOM0_GCLK_ID_CORE + sercom_index), (2000*(baud_rate))) - 5);

    /* Check that baudrate is supported at current speed. */
    if (tmp_baud > 255 || tmp_baud < 0) {
        /* Baud rate not supported. */
        tmp_status_code = STATUS_ERR_BAUDRATE_UNAVAILABLE;
    } else {
        /* Find baudrate for high speed */
        tmp_baud_hs = (int32_t)(div_ceil(
                                    system_gclk_chan_get_hz(SERCOM0_GCLK_ID_CORE + sercom_index),
                                    (2000*(pI2C_S(obj)->baud_rate_high_speed))) - 1);

        /* Check that baudrate is supported at current speed. */
        if (tmp_baud_hs > 255 || tmp_baud_hs < 0) {
            /* Baud rate not supported. */
            tmp_status_code = STATUS_ERR_BAUDRATE_UNAVAILABLE;
        }
    }
    if (tmp_status_code != STATUS_ERR_BAUDRATE_UNAVAILABLE) {
        /* Baud rate acceptable. */
        i2c_module->BAUD.reg = SERCOM_I2CM_BAUD_BAUD(tmp_baud) | SERCOM_I2CM_BAUD_HSBAUD(tmp_baud_hs);
        pI2C_S(obj)->baud_rate = hz;
    }

    /* Enable back the I2C Module */
    i2c_master_enable(&pI2C_S(obj)->master);
}

/** Send START command.
 *  @param obj The i2c object
 */
int  i2c_start(i2c_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    if (pI2C_S(obj)->mode == I2C_MODE_MASTER) {
        pI2C_S(obj)->start_pending = 1;
    }
    return 0;
}

/** Send STOP command.
 *  @param obj The i2c object
 */
int  i2c_stop(i2c_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    if (pI2C_S(obj)->mode == I2C_MODE_MASTER) {
        /* Send STOP command */
        i2c_master_send_stop(&pI2C_S(obj)->master);
    } else {
        SercomI2cs *const i2c_hw = &(pI2C_S(obj)->slave.hw->I2CS);
        /* Release line and wait for next start condition */
        i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x2);
    }

    pI2C_S(obj)->start_pending = 0;

    /* TODO: Wait till STOP is send */
    return 0;
}

/** Blocking reading data.
 *  @param obj     The i2c object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return        Number of read bytes
 */
int  i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    enum status_code tmp_status;

#if DEVICE_I2C_ASYNCH
    if (i2c_active(obj)) {
        /* I2C is busy with a job */
        return 0;
    }
#endif

    struct i2c_master_packet packet;
    packet.address = (address & 0xFF) >> 1;
    packet.data_length = length;
    packet.data = (uint8_t*)data;
    packet.ten_bit_address = false;
    packet.high_speed = false;

    if (stop) {
        tmp_status = i2c_master_read_packet_wait(&pI2C_S(obj)->master, &packet);
    } else {
        tmp_status = i2c_master_read_packet_wait_no_stop(&pI2C_S(obj)->master, &packet);
    }

    if (tmp_status == STATUS_OK) {
        return length;
    } else {
        /* Currently, no way to track no of bytes received, so return 0 if fail */
        return 0;
    }
}

/** Blocking sending data.
 *  @param obj     The i2c object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @param stop    Stop to be generated after the transfer is done
 *  @return        Number of written bytes
 */
int  i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    enum status_code tmp_status;

#if DEVICE_I2C_ASYNCH
    if (i2c_active(obj)) {
        /* I2C is busy with a job */
        return 0;
    }
#endif

    struct i2c_master_packet packet;
    packet.address = (address & 0xFF) >> 1;
    packet.data_length = length;
    packet.data = (uint8_t *)data;
    packet.ten_bit_address = false;
    packet.high_speed = false;

    if (stop) {
        tmp_status = i2c_master_write_packet_wait(&pI2C_S(obj)->master, &packet);
    } else {
        tmp_status = i2c_master_write_packet_wait_no_stop(&pI2C_S(obj)->master, &packet);
    }

    if (tmp_status == STATUS_OK) {
        return length;
    } else {
        /* Currently, no way to track no of bytes transmitted, so return 0 if fail */
        return 0;
    }
}

/** Reset I2C peripheral.
 *  @param obj The i2c object
 */
void i2c_reset(i2c_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Send STOP */
    i2c_stop(obj);

    pI2C_S(obj)->start_pending = 0;
}

/** Write address preceded by START condition.
 *  @param obj The i2c object
 *  @param address Address to be placed
 *  @param rw_flag read or write flag
 *  @return 1 if NAK was received, 0 if ACK was received, 2 for timeout.
 */
int  i2c_write_address(i2c_t *obj, int address, int rw_flag)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    enum status_code tmp_status;
    SercomI2cm *const i2c_module = &(pI2C_S(obj)->master.hw->I2CM);

    _i2c_master_wait_for_sync(&pI2C_S(obj)->master);

    /* Set action to ACK. */
    i2c_module->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;

    /* Write 7-bit address + read/write flag */
    i2c_module->ADDR.reg = ((address & 0x7F) << 1) | (rw_flag & 0x01) | (0 << SERCOM_I2CM_ADDR_HS_Pos);

    /* Wait for response on bus. */
    tmp_status = _i2c_master_wait_for_bus(&pI2C_S(obj)->master);
    /* Check for error. */
    if (tmp_status != STATUS_OK) {
        return I2C_ERROR_BUS_BUSY;
    }
    /* Check for address response error unless previous error is detected. */
    tmp_status = _i2c_master_address_response(&pI2C_S(obj)->master);
    if (tmp_status != STATUS_OK) {
        return I2C_ERROR_NO_SLAVE;
    }

    return 0;
}

/** Read one byte.
 *  @param obj The i2c object
 *  @param last Acknowledge
 *  @return The read byte
 */
int  i2c_byte_read(i2c_t *obj, int last)
{
    int data = -1;
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    enum status_code tmp_status;

    if (pI2C_S(obj)->mode == I2C_MODE_MASTER) {
        SercomI2cm *const i2c_module = &(pI2C_S(obj)->master.hw->I2CM);

        if (last) {
            /* Set action to nack. */
            i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;
        } else {
            /* Set action to ack. */
            i2c_module->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
        }

        /* Check that bus ownership is not lost. */
        if (!(i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE(2))) {
            return -1; /* Return invalid data*/
        }

        /* Save data to buffer. */
        _i2c_master_wait_for_sync(&pI2C_S(obj)->master);
        data = i2c_module->DATA.reg;
        /* Wait for response. */
        tmp_status = _i2c_master_wait_for_bus(&pI2C_S(obj)->master);

        /* Check for error. */
        if (tmp_status != STATUS_OK) {
            return -1; /* Return invalid data*/
        }

    } else {
#if DEVICE_I2CSLAVE
        SercomI2cs *const i2c_hw = &(pI2C_S(obj)->slave.hw->I2CS);

        /* Check direction */
        if (i2c_hw->STATUS.reg & SERCOM_I2CS_STATUS_DIR) {
            /* Write request from master, send NACK and return */
            i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_ACKACT;
            i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x3);
            return -1; /* Return invalid data*/
        }

        if (i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH) {
            /* Request from master, Address not yet acknowledged */
            i2c_hw->CTRLB.reg &= ~SERCOM_I2CS_CTRLB_ACKACT;
            i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x3);
            i2c_hw->INTFLAG.reg = SERCOM_I2CS_INTFLAG_AMATCH;
        }
        if (last) {
            /* Set action to nack. */
            i2c_hw->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;
        } else {
            /* Set action to ack. */
            i2c_hw->CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
        }

        /* Wait for next byte or stop condition */
        tmp_status = _i2c_slave_wait_for_bus(&pI2C_S(obj)->slave);
        if (tmp_status != STATUS_OK) {
            /* Timeout, return */
            return -1;
        }

        if (i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_PREC) {
            /* Master sent stop condition, or repeated start, read done */
            /* Clear stop flag */
            i2c_hw->INTFLAG.reg = SERCOM_I2CS_INTFLAG_PREC;
            return -1;
        }

        /* Read data */
        _i2c_slave_wait_for_sync(&pI2C_S(obj)->slave);
        data = i2c_hw->DATA.reg;
#endif
    }

    return data;
}

/** Write one byte.
 *  @param obj The i2c object
 *  @param data Byte to be written
 *  @return 1 if NAK was received, 0 if ACK was received, 2 for timeout.
 */
int  i2c_byte_write(i2c_t *obj, int data)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    enum status_code tmp_status;

    data = data & 0xFF;

    if (pI2C_S(obj)->mode == I2C_MODE_MASTER) {
        SercomI2cm *const i2c_module = &(pI2C_S(obj)->master.hw->I2CM);

        if (pI2C_S(obj)->start_pending) {
            pI2C_S(obj)->start_pending = 0;
            /* Write address */
            return i2c_write_address(obj, (data >> 1), (data & 0x01));
        } else {
            /* Write data */
            /* Check that bus ownership is not lost. */
            if (!(i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE(2))) {
                return I2C_ERROR_NO_SLAVE;
            }

            /* Write byte to slave. */
            _i2c_master_wait_for_sync(&pI2C_S(obj)->master);
            i2c_module->DATA.reg = data;

            /* Wait for response. */
            tmp_status = _i2c_master_wait_for_bus(&pI2C_S(obj)->master);
            /* Check for error. */
            if (tmp_status != STATUS_OK) {
                return I2C_ERROR_BUS_BUSY;
            }

            /* Check for NACK from slave. */
            if (i2c_module->STATUS.reg & SERCOM_I2CM_STATUS_RXNACK) {
                /* Return bad data value. */
                return I2C_ERROR_NO_SLAVE;
            }
        }
    } else {
#if DEVICE_I2CSLAVE
        SercomI2cs *const i2c_hw = &(pI2C_S(obj)->slave.hw->I2CS);

        if (i2c_hw->INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH) {
            /* Read request from master, Address not yet acknowledged */
            i2c_hw->CTRLB.reg &= ~SERCOM_I2CS_CTRLB_ACKACT;
            i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x3);
            i2c_hw->INTFLAG.reg = SERCOM_I2CS_INTFLAG_AMATCH;
        }

        /* Write data */
        _i2c_slave_wait_for_sync(&pI2C_S(obj)->slave);
        i2c_hw->DATA.reg = data;

        /* Wait for response from master */
        tmp_status = _i2c_slave_wait_for_bus(&pI2C_S(obj)->slave);

        if (tmp_status != STATUS_OK) {
            /* Timeout, return */
            return I2C_ERROR_BUS_BUSY;
        }

        if (i2c_hw->STATUS.reg & SERCOM_I2CS_STATUS_RXNACK) {
            /* NACK from master, abort */
            /* Release line */
            i2c_hw->CTRLB.reg |= SERCOM_I2CS_CTRLB_CMD(0x02);

            return I2C_ERROR_NO_SLAVE;
        }
#endif
    }

    return 0;
}

const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_SERCOM_PAD;
}

const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_SERCOM_PAD;
}

const PinMap *i2c_slave_sda_pinmap()
{
    return PinMap_SERCOM_PAD;
}

const PinMap *i2c_slave_scl_pinmap()
{
    return PinMap_SERCOM_PAD;
}


#if DEVICE_I2CSLAVE

/**
 * \defgroup SynchI2C Synchronous I2C Hardware Abstraction Layer for slave
 * @{
 */

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param enable_slave configure I2C in slave mode or not
 *  @return void
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    int i;
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    uint32_t mux_func[2];
    uint32_t sercom_index = _sercom_get_sercom_inst_index(pI2C_S(obj)->master.hw);
    for (i=0; i<2; i++) {
        mux_func[i] = pinmap_function_sercom(pI2C_S(obj)->pins[0], sercom_index);
        if (mux_func[i] == (uint32_t)NC) return;
    }

    if (enable_slave) {
        /* Disable I2C Master module if active */
        i2c_master_disable(&pI2C_S(obj)->master);

        struct i2c_slave_config config_i2c_slave;
        i2c_slave_get_config_defaults(&config_i2c_slave);

        /* SERCOM PAD0 - SDA */
        config_i2c_slave.pinmux_pad0 = (pI2C_S(obj)->pins[0] << 16) | (mux_func[0] & 0xFFFF);
        /* SERCOM PAD1 - SCL */
        config_i2c_slave.pinmux_pad1 = (pI2C_S(obj)->pins[1] << 16) | (mux_func[1] & 0xFFFF);

        i2c_slave_init(&pI2C_S(obj)->slave, pI2C_S(obj)->master.hw, &config_i2c_slave);

        pI2C_S(obj)->mode = I2C_MODE_SLAVE;

        i2c_slave_enable(&pI2C_S(obj)->slave);
    } else {
        if ((pI2C_S(obj)->master.hw) && (pI2C_S(obj)->mode == I2C_MODE_MASTER)) {
            /* Already configured, enable and return */
            i2c_master_enable(&pI2C_S(obj)->master);
            return;
        } else if ((pI2C_S(obj)->slave.hw) && (pI2C_S(obj)->mode == I2C_MODE_SLAVE)) {
            /* Disable slave */
            i2c_slave_disable(&pI2C_S(obj)->slave);
        }

        struct i2c_master_config config_i2c_master;
        /* SERCOM PAD0 - SDA */
        config_i2c_master.pinmux_pad0 = (pI2C_S(obj)->pins[0] << 16) | (mux_func[0] & 0xFFFF);
        /* SERCOM PAD1 - SCL */
        config_i2c_master.pinmux_pad1 = (pI2C_S(obj)->pins[1] << 16) | (mux_func[1] & 0xFFFF);
        /* Baud rate */
        config_i2c_master.baud_rate = pI2C_S(obj)->baud_rate / 1000;

        while(i2c_master_init(&pI2C_S(obj)->master, pI2C_S(obj)->master.hw, &config_i2c_master) != STATUS_OK);
        pI2C_S(obj)->mode = I2C_MODE_MASTER;

        i2c_master_enable(&pI2C_S(obj)->master);
    }
}

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addresses, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int  i2c_slave_receive(i2c_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->slave.hw);

    SercomI2cs *const i2c_module = &(pI2C_S(obj)->slave.hw->I2CS);

    if (i2c_module->INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH) {
        if (i2c_module->STATUS.reg & SERCOM_I2CS_STATUS_DIR) {
            /* Slave is read addressed */
            return 1;
        } else {
            if (!(i2c_module->DATA.reg & 0xFF)) {
                /* General call address detected */
                return 2;
            } else {
                /* Slave is write addressed */
                return 3;
            }
        }
    }

    return 0;
}

/** Blocking reading data.
 *  @param obj     The i2c slave object
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @return        Number of read bytes
 */
int  i2c_slave_read(i2c_t *obj, char *data, int length)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->slave.hw);

    if (!data || !length) return 0;

    enum status_code tmp_status;

    struct i2c_slave_packet packet;
    packet.data_length = length;
    packet.data = (uint8_t*)data;

    tmp_status = i2c_slave_read_packet_wait(&pI2C_S(obj)->slave, &packet);

    if (tmp_status == STATUS_OK) {
        return length;
    } else {
        /* Currently, no way to track no of bytes transmitted, so return 0 */
        return 0;
    }

}

/** Blocking writing data.
 *  @param obj     The i2c slave object
 *  @param data    The buffer for transmitting
 *  @param length  Number of bytes to write
 *  @return        Number of bytes written
 */
int  i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->slave.hw);

    if (!data || !length) return 0;

    enum status_code tmp_status;

    struct i2c_slave_packet packet;
    packet.data_length = length;
    packet.data = (uint8_t *)data;

    tmp_status = i2c_slave_write_packet_wait(&pI2C_S(obj)->slave, &packet);

    if (tmp_status == STATUS_OK) {
        return length;
    } else {
        /* Currently, no way to track no of bytes transmitted, so return 0 */
        return 0;
    }

}

/** Configure I2C slave address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->slave.hw);

    /* Disable I2C Module */
    i2c_slave_disable(&pI2C_S(obj)->slave);

    SercomI2cs *const i2c_hw = &(pI2C_S(obj)->slave.hw->I2CS);

    address = (address & 0xFF) >> 1;
    if (!mask) {
        mask = (0xFE >> 1);
    }
    /* Set the address and address mask */
    i2c_hw->ADDR.reg = (address << SERCOM_I2CS_ADDR_ADDR_Pos) |
                       (mask << SERCOM_I2CS_ADDR_ADDRMASK_Pos) |
                       (0 << SERCOM_I2CS_ADDR_TENBITEN_Pos) |
                       (1 << SERCOM_I2CS_ADDR_GENCEN_Pos);

    /* Enable I2C Module */
    i2c_slave_enable(&pI2C_S(obj)->slave);
}

#endif /* DEVICE_I2CSLAVE */

/**@}*/

#if DEVICE_I2C_ASYNCH

/**
 * \defgroup AsynchI2C Asynchronous I2C Hardware Abstraction Layer
 * @{
 */

/**
* \internal
* Callback for transfer finish.
*
* \param[in,out]  module  Pointer to SPI software instance struct
*/
void i2c_transfer_complete_callback(struct i2c_master_module *const module)
{
    uint32_t sercom_index = _sercom_get_sercom_inst_index(module->hw);

    if (sercom_index >= SERCOM_INST_NUM) {
        /* TODO: Abort operation */
        return;
    }

    i2c_t *obj = (i2c_t*)i2c_instances[sercom_index];

    i2c_master_disable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
    i2c_master_disable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_READ_COMPLETE);
    i2c_master_disable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_ERROR);

    /* Call the handler function */
    if (pI2C_S(obj)->handler) {
        ((I2CHandler)pI2C_S(obj)->handler)();
    }
}

/**
* \internal
* Callback for write complete. Initiate read from here.
*
* \param[in,out]  module  Pointer to SPI software instance struct
*/
void i2c_write_complete_callback(struct i2c_master_module *const module)
{
    uint32_t sercom_index = _sercom_get_sercom_inst_index(module->hw);

    if (sercom_index >= SERCOM_INST_NUM) {
        /* TODO: Abort operation */
        return;
    }

    i2c_t *obj = (i2c_t*)i2c_instances[sercom_index];

    if (!(pI2C_S(obj)->rd_packet.data) || (pI2C_S(obj)->rd_packet.data_length == 0)) {
        /* Call the handler function */
        i2c_transfer_complete_callback(module);
    } else {
        i2c_master_disable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_WRITE_COMPLETE);

        /* Register read complete callback */
        i2c_master_register_callback(&pI2C_S(obj)->master, i2c_transfer_complete_callback, I2C_MASTER_CALLBACK_READ_COMPLETE);
        i2c_master_enable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_READ_COMPLETE);

        /* Initiate read operation */
        if (pI2C_S(obj)->master.send_stop) {
            i2c_master_read_packet_job(&pI2C_S(obj)->master,&pI2C_S(obj)->rd_packet);
        } else {
            i2c_master_read_packet_job_no_stop(&pI2C_S(obj)->master, &pI2C_S(obj)->rd_packet);
        }
    }
}

/** Start i2c asynchronous transfer.
 *  @param obj       The I2C object
 *  @param tx        The buffer to send
 *  @param tx_length The number of words to transmit
 *  @param rx        The buffer to receive
 *  @param rx_length The number of words to receive
 *  @param address   The address to be set - 7bit or 9 bit
 *  @param stop      If true, stop will be generated after the transfer is done
 *  @param handler   The I2C IRQ handler to be set
 *  @param hint      DMA hint usage
 */
void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    /* Return if in Slave mode */
    if (pI2C_S(obj)->mode != I2C_MODE_MASTER) return;

    uint32_t sercom_index = _sercom_get_sercom_inst_index(pI2C_S(obj)->master.hw);

    /* Init i2c packet. */
    pI2C_S(obj)->wr_packet.address     = address >> 1;
    pI2C_S(obj)->wr_packet.data_length = tx_length;
    pI2C_S(obj)->wr_packet.data        = (uint8_t *)tx;

    pI2C_S(obj)->rd_packet.address     = address >> 1;
    pI2C_S(obj)->rd_packet.data_length = rx_length;
    pI2C_S(obj)->rd_packet.data        = rx;

    /* Save event mask and handler function pointer */
    pI2C_S(obj)->events = event;
    pI2C_S(obj)->handler = handler;

    /* TODO: Current implementation is interrupt based only */

    /* Set interrupt handler to default handler of ASF */
    /* Enable interrupt */
    NVIC_SetVector((IRQn_Type)((uint32_t)SERCOM0_IRQn + sercom_index), sercom_irq_handlers[sercom_index]);
    NVIC_EnableIRQ((IRQn_Type)((uint32_t)SERCOM0_IRQn + sercom_index));

    /* Register callbacks */
    i2c_master_register_callback(&pI2C_S(obj)->master, i2c_transfer_complete_callback, I2C_MASTER_CALLBACK_ERROR);
    i2c_master_enable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_ERROR);
    if (tx && tx_length) {
        i2c_master_register_callback(&pI2C_S(obj)->master, i2c_write_complete_callback, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
        i2c_master_enable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_WRITE_COMPLETE);

        /* Start I2C write */
        if (stop) {
            i2c_master_write_packet_job(&pI2C_S(obj)->master, &pI2C_S(obj)->wr_packet);
        } else {
            i2c_master_write_packet_job_no_stop(&pI2C_S(obj)->master, &pI2C_S(obj)->wr_packet);
        }
    } else if (rx && rx_length) {
        i2c_master_register_callback(&pI2C_S(obj)->master, i2c_transfer_complete_callback, I2C_MASTER_CALLBACK_READ_COMPLETE);
        i2c_master_enable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_READ_COMPLETE);

        /* Start I2C read */
        if (stop) {
            i2c_master_read_packet_job(&pI2C_S(obj)->master,&pI2C_S(obj)->rd_packet);
        } else {
            i2c_master_read_packet_job_no_stop(&pI2C_S(obj)->master, &pI2C_S(obj)->rd_packet);
        }
    } else {
        /* Nothing to transfer, invoke callback */
        i2c_transfer_complete_callback(&pI2C_S(obj)->master);
    }
}

/** The asynchronous IRQ handler
 *  @param obj The I2C object which holds the transfer information
 *  @return event flags if a transfer termination condition was met or 0 otherwise.
 */
uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    uint32_t event_mask = pI2C_S(obj)->events;

    /* TODO: Current implementation is interrupt based only */

    switch (pI2C_S(obj)->master.status) {
        case STATUS_OK:
            /* Transfer is complete */
            return (I2C_EVENT_TRANSFER_COMPLETE & event_mask);

        case STATUS_ERR_BAD_ADDRESS:
            /* Received a NACK */
            return (I2C_EVENT_ERROR_NO_SLAVE & event_mask);

        case STATUS_ERR_PACKET_COLLISION:
            /* An error occurred in between transfer */
            return (I2C_EVENT_ERROR & event_mask);

        default:
            return 0;
    }

    //return 0;
}

/** Attempts to determine if I2C peripheral is already in use.
 *  @param obj The I2C object
 *  @return non-zero if the I2C module is active or zero if it is not
 */
uint8_t i2c_active(i2c_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    return (pI2C_S(obj)->master.status == STATUS_BUSY);
}

/** Abort ongoing asynchronous transaction.
 *  @param obj The I2C object
 */
void i2c_abort_asynch(i2c_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(pI2C_S(obj)->master.hw);

    /* Pointer to the hardware module instance */
    SercomI2cm *const i2c_module = &(pI2C_S(obj)->master.hw->I2CM);

    /* Abort ongoing job */

    /* Stop packet operation */
    i2c_module->INTENCLR.reg = SERCOM_I2CM_INTENCLR_MB | SERCOM_I2CM_INTENCLR_SB;

    pI2C_S(obj)->master.buffer_length = 0;
    pI2C_S(obj)->master.buffer_remaining = 0;

    /* Send nack and stop command unless arbitration is lost */
    if ((pI2C_S(obj)->master.status != STATUS_ERR_PACKET_COLLISION) &&  pI2C_S(obj)->master.send_stop) {
        _i2c_master_wait_for_sync(&pI2C_S(obj)->master);
        i2c_module->CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT | SERCOM_I2CM_CTRLB_CMD(3);
    }

    /* Disable any registered callback */
    i2c_master_disable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_WRITE_COMPLETE);
    i2c_master_disable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_READ_COMPLETE);
    i2c_master_disable_callback(&pI2C_S(obj)->master, I2C_MASTER_CALLBACK_ERROR);

    pI2C_S(obj)->master.status = STATUS_ABORTED;
}

#endif

#endif  // #if DEVICE_I2C
