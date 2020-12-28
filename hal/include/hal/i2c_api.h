
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#ifndef MBED_I2C_API_H
#define MBED_I2C_API_H

#include "device.h"
#include "pinmap.h"
#include "hal/buffer.h"

#if DEVICE_I2C_ASYNCH
#include "hal/dma_api.h"
#endif

#if DEVICE_I2C

/**
 * @defgroup hal_I2CEvents I2C Events Macros
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
/** Asynch I2C HAL structure
 */
typedef struct {
    struct i2c_s    i2c;     /**< Target specific I2C structure */
    struct buffer_s tx_buff; /**< Tx buffer */
    struct buffer_s rx_buff; /**< Rx buffer */
} i2c_t;

#else
/** Non-asynch I2C HAL structure
 */
typedef struct i2c_s i2c_t;

#endif

enum {
    I2C_ERROR_NO_SLAVE = -1,
    I2C_ERROR_BUS_BUSY = -2
};

typedef struct {
    int peripheral;
    PinName sda_pin;
    int sda_function;
    PinName scl_pin;
    int scl_function;
} i2c_pinmap_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_GeneralI2C I2C Configuration Functions
 *
 * # Defined behavior
 * * ::i2c_init initializes i2c_t control structure
 * * ::i2c_init configures the pins used by I2C
 * * ::i2c_free returns the pins owned by the I2C object to their reset state
 * * ::i2c_frequency configure the I2C frequency
 * * ::i2c_start sends START command
 * * ::i2c_read reads `length` bytes from the I2C slave specified by `address` to the `data` buffer
 * * ::i2c_read reads generates a stop condition on the bus at the end of the transfer if `stop` parameter is non-zero
 * * ::i2c_read reads returns the number of symbols received from the bus
 * * ::i2c_write writes `length` bytes to the I2C slave specified by `address` from the `data` buffer
 * * ::i2c_write generates a stop condition on the bus at the end of the transfer if `stop` parameter is non-zero
 * * ::i2c_write returns zero on success, error code otherwise
 * * ::i2c_reset resets the I2C peripheral
 * * ::i2c_byte_read reads and return one byte from the specfied I2C slave
 * * ::i2c_byte_read uses `last` parameter to inform the slave that all bytes have been read
 * * ::i2c_byte_write writes one byte to the specified I2C slave
 * * ::i2c_byte_write returns 0 if NAK was received, 1 if ACK was received, 2 for timeout
 * * ::i2c_slave_mode enables/disables I2S slave mode
 * * ::i2c_slave_receive returns: 1 - read addresses, 2 - write to all slaves, 3 write addressed, 0 - the slave has not been addressed
 * * ::i2c_slave_read reads `length` bytes from the I2C master to the `data` buffer
 * * ::i2c_slave_read returns non-zero if a value is available, 0 otherwise
 * * ::i2c_slave_write writes `length` bytes to the I2C master from the `data` buffer
 * * ::i2c_slave_write returns non-zero if a value is available, 0 otherwise
 * * ::i2c_slave_address configures I2C slave address
 * * ::i2c_transfer_asynch starts I2C asynchronous transfer
 * * ::i2c_transfer_asynch writes `tx_length` bytes to the I2C slave specified by `address` from the `tx` buffer
 * * ::i2c_transfer_asynch reads `rx_length` bytes from the I2C slave specified by `address` to the `rx` buffer
 * * ::i2c_transfer_asynch generates a stop condition on the bus at the end of the transfer if `stop` parameter is non-zero
 * * The callback given to ::i2c_transfer_asynch is invoked when the transfer completes
 * * ::i2c_transfer_asynch specifies the logical OR of events to be registered
 * * The ::i2c_transfer_asynch function may use the `DMAUsage` hint to select the appropriate async algorithm
 * * ::i2c_irq_handler_asynch returns event flags if a transfer termination condition was met, otherwise returns 0.
 * * ::i2c_active returns non-zero if the I2C module is active or 0 if it is not
 * * ::i2c_abort_asynch aborts an on-going async transfer
 *
 * # Undefined behavior
 * * Calling ::i2c_init multiple times on the same `i2c_t`
 * * Calling any function other than ::i2c_init on a non-initialized `i2c_t`
 * * Initialising the `I2C` peripheral with invalid `SDA` and `SCL` pins.
 * * Passing pins that cannot be on the same peripheral
 * * Passing an invalid pointer as `obj` to any function
 * * Use of a `null` pointer as an argument to any function.
 * * Initialising the peripheral in slave mode if slave mode is not supported
 * * Operating the peripheral in slave mode without first specifying and address using ::i2c_slave_address
 * * Setting an address using i2c_slave_address after initialising the peripheral in master mode
 * * Setting an address to an `I2C` reserved value
 * * Specifying an invalid address when calling any `read` or `write` functions
 * * Setting the length of the transfer or receive buffers to larger than the buffers are
 * * Passing an invalid pointer as `handler`
 * * Calling ::i2c_abort_async when no transfer is currently in progress
 *
 *
 * @{
 */

/**
 * \defgroup hal_GeneralI2C_tests I2C hal tests
 * The I2C HAL tests ensure driver conformance to defined behaviour.
 *
 * To run the I2C hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal_fpga_ci_test_shield-i2c
 *
 */

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 *  @param obj  The I2C object
 *  @param pinmap  Pinmap pointer to structure which holds static pinmap
 */
void i2c_init_direct(i2c_t *obj, const i2c_pinmap_t *pinmap);

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 *  @param obj  The I2C object
 *  @param sda  The sda pin
 *  @param scl  The scl pin
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl);

/** Release a I2C object
 *
 * Return the pins owned by the I2C object to their reset state
 * @param obj The I2C object to deinitialize
 */
void i2c_free(i2c_t *obj);

/** Configure the I2C frequency
 *
 *  @param obj The I2C object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz);

/** Send START command
 *
 *  @param obj The I2C object
 */
int  i2c_start(i2c_t *obj);

/** Send STOP command
 *
 *  @param obj The I2C object
 */
int  i2c_stop(i2c_t *obj);

/** Blocking reading data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of read bytes
 */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop);

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
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop);

/** Reset I2C peripheral. TODO: The action here. Most of the implementation sends stop()
 *
 *  @param obj The I2C object
 */
void i2c_reset(i2c_t *obj);

/** Read one byte
 *
 *  @param obj The I2C object
 *  @param last Acknoledge
 *  @return The read byte
 */
int i2c_byte_read(i2c_t *obj, int last);

/** Write one byte
 *
 *  @param obj The I2C object
 *  @param data Byte to be written
 *  @return 0 if NAK was received, 1 if ACK was received, 2 for timeout.
 */
int i2c_byte_write(i2c_t *obj, int data);

/** Get the pins that support I2C SDA
 *
 * Return a PinMap array of pins that support I2C SDA in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *i2c_master_sda_pinmap(void);

/** Get the pins that support I2C SCL
 *
 * Return a PinMap array of pins that support I2C SCL in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *i2c_master_scl_pinmap(void);

/** Get the pins that support I2C SDA
 *
 * Return a PinMap array of pins that support I2C SDA in
 * slave mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *i2c_slave_sda_pinmap(void);

/** Get the pins that support I2C SCL
 *
 * Return a PinMap array of pins that support I2C SCL in
 * slave mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *i2c_slave_scl_pinmap(void);

/**@}*/

#if DEVICE_I2CSLAVE

/**
 * \defgroup SynchI2C Synchronous I2C Hardware Abstraction Layer for slave
 * @{
 */

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param enable_slave Enable i2c hardware so you can receive events with ::i2c_slave_receive
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
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @return non-zero if a value is available
 */
int  i2c_slave_read(i2c_t *obj, char *data, int length);

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
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
 * \defgroup hal_AsynchI2C Asynchronous I2C Hardware Abstraction Layer
 * @{
 */

/** Start I2C asynchronous transfer
 *
 *  @param obj       The I2C object
 *  @param tx        The transmit buffer
 *  @param tx_length The number of bytes to transmit
 *  @param rx        The receive buffer
 *  @param rx_length The number of bytes to receive
 *  @param address   The address to be set - 7bit or 9bit
 *  @param stop      If true, stop will be generated after the transfer is done
 *  @param handler   The I2C IRQ handler to be set
 *  @param event     Event mask for the transfer. See \ref hal_I2CEvents
 *  @param hint      DMA hint usage
 */
void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint);

/** The asynchronous IRQ handler
 *
 *  @param obj The I2C object which holds the transfer information
 *  @return Event flags if a transfer termination condition was met, otherwise return 0.
 */
uint32_t i2c_irq_handler_asynch(i2c_t *obj);

/** Attempts to determine if the I2C peripheral is already in use
 *
 *  @param obj The I2C object
 *  @return Non-zero if the I2C module is active or zero if it is not
 */
uint8_t i2c_active(i2c_t *obj);

/** Abort asynchronous transfer
 *
 *  This function does not perform any check - that should happen in upper layers.
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

/** @}*/
