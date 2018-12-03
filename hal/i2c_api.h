
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

#include <stdbool.h>

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

typedef struct i2c i2c_t;

typedef struct i2c_async_event {
    uint32_t transferred;
    bool error;
} i2c_async_event_t;

typedef void (*i2c_async_handler_f)(i2c_t* obj, i2c_async_event_t* event, void *ctx);

/** Asynch I2C HAL structure
 */
struct i2c {
    struct i2c_s i2c;        /**< Target specific I2C structure */
    struct buffer_s tx_buff; /**< Tx buffer */
    struct buffer_s rx_buff; /**< Rx buffer */
    i2c_async_handler_f handler;
    void* ctx;
};

#else
/** Non-asynch I2C HAL structure
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

typedef struct {
    /**< Minimum frequency supported must be set by target device */
    uint32_t    minimum_frequency;
    /**< Maximum frequency supported must be set by target device */
    uint32_t    maximum_frequency;
	/**< If true, the device can handle I2C slave mode. */
    bool      supports_slave_mode;
    /**< If true, supports 10-bit addressing. */
    bool      supports_10bit_addressing;
    /**< If true, the device handle multimaster collisions and arbitration safely*/
    bool      supports_multi_master;
    /**< If true, supports configuring clock stretching. */
    bool supports_clock_stretching;
} i2c_capabilities_t;

/**
 * \defgroup hal_GeneralI2C I2C Configuration Functions
 * @{
 */

/** Fills structure indicating supported features and frequencies on the current
 *  platform.
 *
 * @param[out] capabilities  Capabilities structure filled with supported
 *                           configurations.
 */
void i2c_get_capabilities(i2c_capabilities_t *capabilities);

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its pins.
 *
 *  @param obj       The I2C object
 *  @param sda       The sda pin
 *  @param scl       The scl pin
 *  @param is_slave  Choose whether the peripheral is initialised as master or
 *                   slave.
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl, bool is_slave);

/** Release the I2C object.
 *
 *  @param obj The I2C object
 */
void i2c_free(i2c_t *obj);

/** Configure the frequency in Hz the I2C peripheral should operate at.
 *
 *  @param obj        The I2C object
 *  @param frequency  Frequency in Hz
 *
 *  @returns The actual frequency that the peripheral will be generating to
 *           allow a user adjust its strategy in case the target cannot be
 *           reached.
 */
uint32_t i2c_frequency(i2c_t *obj, uint32_t frequency);

/** Configure the timeout duration in milliseconds the I2C peripheral should
 *  allow the slave peripheral to stretch the clock for before timing out.
 *
 *  @param obj        The I2C object
 *  @param timeout    Clock stretching timeout in milliseconds.
 */
void i2c_timeout(i2c_t *obj, uint32_t timeout);

/** Send START command
 *
 *  @param obj The I2C object
 *  @returns   True if slave responds with ACK, false otherwise.
 */
bool i2c_start(i2c_t *obj);

/** Send STOP command
 *
 *  @param obj The I2C object
 *  @returns   True if slave responds with ACK, false otherwise.
 */
bool i2c_stop(i2c_t *obj);

/** Blocking sending data
 *
 *  This function transmits data, when the peripheral is configured as Master to
 *  the selected slave, and when configured as Slave transmits data to the
 *  Master.
 *
 *  This function is blocking, it will return when the transfer is complete or a
 *  timeout event is triggered. The number of bytes transmitted is returned by
 *  the function after the operation is completed. Transmit operation cannot be
 *  cancelled or aborted.
 *
 *  The data buffer must stay allocated during the duration of the transfer and
 *  the contents must not be modified. The value of the specified `address` is
 *  ignored when configured in slave mode, in master mode it contains the
 *  address of the target peripheral. This is a 7-bit value unless 10-bit
 *  addressing is configured and supported by the target.
 *
 *  When in master mode the operation consists of:
 *   - Address the slave as a Master transmitter.
 *   - Transmit data to the addressed slave.
 *   - Generate a STOP condition if the specified `stop` field is true.
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @param stop    If true, stop will be generated after the transfer is done
 *
 *  @note If the current platform supports multimaster operation the transfer
 *        will block until the peripheral can gain arbitration of the bus and
 *        complete the transfer. If the device does not support multimaster
 *        operation this function is not safe to execute when the bus is shared
 *        with another device in master mode.
 *
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int32_t i2c_write(i2c_t *obj, uint16_t address, const void *data,
                  uint32_t length, bool stop);

/** Blocking reading data
 *
 *  This function receives data, when the peripheral is configured as Master
 *  from the selected slave, and when configured as Slave from the Master.
 *
 *  This function is blocking, it will return when the transfer is complete or a
 *  timeout event is triggered. The number of bytes received is returned by
 *  the function after the operation is completed. Receive operation cannot be
 *  cancelled or aborted.
 *
 *  When in master mode the operation consists of:
 *   - Address the slave as a Master receiver.
 *   - Receive data from the addressed slave.
 *   - Generate a STOP condition if the specified `stop` field is true.
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param last    If true, indicates that the transfer contains the last byte
 *                 to be sent.
 *
 *  @note If the current platform supports multimaster operation the transfer
 *        will block until the peripheral can gain arbitration of the bus and
 *        complete the transfer. If the device does not support multimaster
 *        operation this function is not safe to execute when the bus is shared
 *        with another device in master mode.
 *
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int32_t i2c_read(i2c_t *obj, uint16_t address, void *data, uint32_t length,
                 bool last);

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

typedef enum {
    NoData         = 0, // Slave has not been addressed.
    ReadAddressed  = 1, // Master has requested a read from this slave.
    WriteGeneral   = 2, // Master is writing to all slaves.
    WriteAddressed = 3  // Master is writing to this slave.
} i2c_slave_status_t;

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - i2c_slave_status_t indicating what mode the peripheral
 *          is configured in.
 */
i2c_slave_status_t i2c_slave_status(i2c_t *obj);

/** Configure I2C address.
 *
 *  @note This function does nothing when configured in master mode.
 *
 *  @param obj     The I2C object
 *  @param address The address to be set
 */
void i2c_slave_address(i2c_t *obj, uint16_t address);

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
 */
void i2c_transfer_async(i2c_t *obj, const void *tx, uint32_t tx_length,
                        void *rx, uint32_t rx_length, uint16_t address,
                        bool stop, i2c_async_handler_f handler, void *ctx);

/** Abort asynchronous transfer
 *
 *  This function does not perform any check - that should happen in upper
 *  layers.
 *
 *  @param obj The I2C object
 */
void i2c_abort_async(i2c_t *obj);

#endif

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
