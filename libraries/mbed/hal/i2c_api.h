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
#ifndef MBED_I2C_API_H
#define MBED_I2C_API_H

#include "device.h"
#include "buffer.h"
#include "dma_api.h"

#if DEVICE_I2C

/**
 * @defgroup I2CEvents I2C Events Macros
 *
 * @{
 */
#define I2C_EVENT_ERROR               (1 << 1)
#define I2C_EVENT_ERROR_NO_SLAVE      (1 << 2)
#define I2C_EVENT_TRANSFER_COMPLETE   (1 << 3)
#define I2C_EVENT_TRANSFER_EARLY_NACK (1 << 4)
#define I2C_EVENT_ALL                 (I2C_EVENT_ERROR |  I2C_EVENT_TRANSFER_COMPLETE | I2C_EVENT_ERROR_NO_SLAVE | I2C_EVENT_TRANSFER_EARLY_NACK)

/**@}*/

#if DEVICE_I2C_ASYNCH
/** Asynch i2c hal structure
 */
typedef struct {
    struct i2c_s    i2c;     /**< Target specific i2c structure */
    struct buffer_s tx_buff; /**< Tx buffer */
    struct buffer_s rx_buff; /**< Rx buffer */
} i2c_t;

#else
/** Non-asynch i2c hal structure
 */
typedef struct i2c_s i2c_t;

#endif

enum {
  I2C_ERROR_NO_SLAVE = -1,
  I2C_ERROR_BUS_BUSY = -2
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup GeneralI2C I2C Configuration Functions
 * @{
 */

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configure its specifieds pins.
 *  @param obj  The i2c object
 *  @param sda  The sda pin
 *  @param scl  The scl pin
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl);

/** Configure the I2C frequency.
 *  @param obj The i2c object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz);

/** Send START command.
 *  @param obj The i2c object
 */
int  i2c_start(i2c_t *obj);

/** Send STOP command.
 *  @param obj The i2c object
 */
int  i2c_stop(i2c_t *obj);

/** Blocking reading data.
 *  @param obj     The i2c object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of read bytes
 */
int  i2c_read(i2c_t *obj, int address, char *data, int length, int stop);

/** Blocking sending data.
 *  @param obj     The i2c object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to wrte
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of written bytes
 */
int  i2c_write(i2c_t *obj, int address, const char *data, int length, int stop);

/** Reset I2C peripheral. TODO: The action here. Most of the implementation sends stop().
 *  @param obj The i2c object
 */
void i2c_reset(i2c_t *obj);

/** Read one byte.
 *  @param obj The i2c object
 *  @param last Acknoledge
 *  @return The read byte
 */
int  i2c_byte_read(i2c_t *obj, int last);

/** Write one byte.
 *  @param obj The i2c object
 *  @param data Byte to be written
 *  @return 1 if NAK was received, 0 if ACK was received, 2 for timeout.
 */
int  i2c_byte_write(i2c_t *obj, int data);

/**@}*/

#if DEVICE_I2CSLAVE

/**
 * \defgroup SynchI2C Synchronous I2C Hardware Abstraction Layer for slave
 * @{
 */

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @return non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave);

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addresses, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int  i2c_slave_receive(i2c_t *obj);

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @return non-zero if a value is available
 */
int  i2c_slave_read(i2c_t *obj, char *data, int length);

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @return non-zero if a value is available
 */
int  i2c_slave_write(i2c_t *obj, const char *data, int length);

/** Configure I2C address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask);

#endif

/**@}*/

#if DEVICE_I2C_ASYNCH

/**
 * \defgroup AsynchI2C Asynchronous I2C Hardware Abstraction Layer
 * @{
 */

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
void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint);

/** The asynchronous IRQ handler
 *  @param obj The I2C object which holds the transfer information
 *  @return event flags if a transfer termination condition was met or 0 otherwise.
 */
uint32_t i2c_irq_handler_asynch(i2c_t *obj);

/** Attempts to determine if I2C peripheral is already in use.
 *  @param obj The I2C object
 *  @return non-zero if the I2C module is active or zero if it is not
 */
uint8_t i2c_active(i2c_t *obj);

/** Abort ongoing asynchronous transaction.
 *  @param obj The I2C object
 */
void i2c_abort_asynch(i2c_t *obj);

#endif

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif
