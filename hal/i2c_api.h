/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

/** \addtogroup hal */
/** @{*/

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
 * \defgroup hal_i2c I2C hal
 *
 * The I2C hal provides a low level interface to the I2C interface of a target.
 *
 * # Defined behaviour
 * * The function ::i2c_init initialises the peripheral pins specified in the input parameters,
 * initialises the peripheral in master mode if `is_slave` is false,
 * initialises the peripheral in slave mode if `is_slave` is true and `supports_slave_mode` is true  - Verified by test ::fpga_i2c_test_init_free.
 * * The function ::i2c_free resets the pins used to initialise the peripheral to their default state,
 * disables the peripheral clock - Verified by test ::fpga_i2c_test_init_free.
 * * The function ::i2c_get_capabilities fills the contents of the `i2c_capabilities_t` parameter - Verified by test ::fpga_i2c_test_get_capabilities.
 * * The function ::i2c_frequency sets the frequency to use for the transfer, returns the actual frequency that will be used,
 * must leave all other configuration unchanged - Verified by test ::fpga_i2c_test_frequency.
 * * The function ::i2c_timeout sets the transmision timeout to use for the following blocking transfers,
 * if the timeout is not set the default timeout is used,
 * default timeout value is based on I2C frequency. Is computed as triple amount of time it would take to send data over I2C - Verified by test ::fpga_i2c_test_blocking_transmission_timeout.
 * * The function ::i2c_write writes `length` number of symbols to the bus, returns the number of symbols sent to the bus,
 * returns an error code if transfer fails, generates a stop condition on the bus at the end of the transfer if `stop` parameter is true,
 * handles transfer collisions and loss of arbitration if the platform supports multimaster in hardware,
 * the transfer will timeout and return `I2C_ERROR_TIMEOUT ` if the transfer takes longer than the configured timeout duration - Verified by test ::fpga_i2c_test_blocking_write_read.
 * * The function ::i2c_read reads `length` symbols from the bus, returns the number of symbols received from the bus,
 * returns an error code if transfer fails, generates a stop condition on the bus at the end of the transfer if `stop` parameter is true,
 * handles transfer collisions and loss of arbitration if the platform supports multimaster in hardware,
 * the transfer will timeout and return `I2C_ERROR_TIMEOUT ` if the transfer takes longer than the configured timeout duration - Verified by test ::fpga_i2c_test_blocking_write_read.
 * * The function ::i2c_start generates I2C START condition on the bus in master mode, does nothing if called when the peripheral is configured in slave mode - not tested.
 * * The function ::i2c_stop generates I2C STOP condition on the bus in master mode, does nothing if called when the peripheral is configured in slave mode
 * - Verified by test ::fpga_i2c_test_blocking_write_read ::fpga_i2c_test_async_write_read.
 * * The function ::i2c_slave_status indicates which mode the peripheral has been addressed in, Returns not addressed when called in master mode - not tested.
 * * The function ::i2c_slave_address sets the address of the peripheral to the `address` parameter, does nothing if called in master mode - not tested.
 * * The function ::i2c_transfer_async returns immediately with a `bool` indicating whether the transfer was successfully scheduled or not,
 * the callback given to `i2c_transfer_async` is invoked when the transfer finishes or error occurs,
 * must save the handler and context pointers inside the `obj` pointer, the context pointer is passed to the callback on transfer completion,
 * the callback must be invoked on completion unless the transfer is aborted, the callback must be invoked on completion unless the transfer is aborted,
 * may handle transfer collisions and loss of arbitration if the platform supports multimaster in hardware and enabled in API,
 * `i2c_async_event_t` must be filled with the number of symbols sent to the bus during transfer - Verified by test ::fpga_i2c_test_async_write_read.
 * * The function ::i2c_abort_async Aborts any on-going async transfers - Verified by test ::fpga_i2c_test_async_abort.
 *
 * # Undefined behaviours
 *
 * * Use of a `null` pointer as an argument to any function.
 * * Calling any `I2C` function before calling ::i2c_init or after calling ::i2c_free.
 * * Initialising the `I2C` peripheral with invalid `SDA` and `SCL` pins.
 * * Initialising the peripheral in slave mode if slave mode is not supported, indicated by ::i2c_get_capabilities.
 * * Operating the peripheral in slave mode without first specifying and address using ::i2c_slave_address.
 * * Setting an address using i2c_slave_address after initialising the peripheral in master mode.
 * * Setting an address to an `I2C` reserved value.
 * * Setting an address larger than the 7-bit supported maximum if 10-bit addressing is not supported.
 * * Setting an address larger than the 10-bit supported maximum.
 * * Setting a frequency outside the supported range given by ::i2c_get_capabilities.
 * * Using the device in a multimaster configuration when `supports_multimaster_mode` is false.
 * * Specifying an invalid address when calling any `read` or `write` functions.
 * * Setting the length of the transfer or receive buffers to larger than the buffers are.
 * * Passing an invalid pointer as `handler` to ::i2c_transfer_async.
 * * Calling ::i2c_abort_async when no transfer is currently in progress.
 *
 * @{
 */

/**
 * \defgroup hal_i2c_tests I2C hal tests
 * The I2C test validate proper implementation of the I2C hal.
 *
 * To run the I2C hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal_fpga_ci_test_shield-i2c
 */


/**
 * @defgroup hal_I2CEvents I2C Events Macros
 *
 * @{
 */
#define I2C_EVENT_ERROR               (1 << 1)
#define I2C_EVENT_ERROR_NO_SLAVE      (1 << 2)
#define I2C_EVENT_TRANSFER_COMPLETE   (1 << 3)
#define I2C_EVENT_TRANSFER_EARLY_NACK (1 << 4)
#define I2C_EVENT_ARBITRATION_LOST    (1 << 5)
#define I2C_EVENT_ALL                 (I2C_EVENT_ERROR |  \
                                       I2C_EVENT_TRANSFER_COMPLETE | \
                                       I2C_EVENT_ERROR_NO_SLAVE | \
                                       I2C_EVENT_TRANSFER_EARLY_NACK | \
                                       I2C_EVENT_ARBITRATION_LOST)

/**@}*/

typedef struct i2c i2c_t;

#if DEVICE_I2C_ASYNCH
/** Structure describing the status of async transfer */
typedef struct i2c_async_event {
    uint32_t sent_bytes;
    uint32_t received_bytes;
    int32_t error_status; // error description I2C_ERROR_XXX
    bool error;
} i2c_async_event_t;

/** Asynchronous transfer callback.
 *
 *  @param obj       The I2C object
 *  @param event     Pointer to the event holding async transfer status
 *  @param ctx       The context pointer
 *
 *   @note Callback is invoked when async transfer completes or when error detected.
 */
typedef void (*i2c_async_handler_f)(i2c_t *obj, i2c_async_event_t *event, void *ctx);
#endif

/** I2C HAL structure */
struct i2c {
    struct i2c_s i2c;        /**< Target specific I2C structure */
#if DEVICE_I2C_ASYNCH
    struct buffer_s tx_buff; /**< Tx buffer */
    struct buffer_s rx_buff; /**< Rx buffer */
    i2c_async_handler_f handler;
    void *ctx;
#endif
};

/** Transmission error codes */
enum {
    I2C_ERROR_NO_SLAVE = -1,
    I2C_ERROR_BUS_BUSY = -2,
    I2C_ERROR_TIMEOUT  = -3,
    I2C_ERROR_ARBITRATION_LOST = -4
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
    bool        supports_slave_mode;
    /**< If true, supports 10-bit addressing. */
    bool        supports_10bit_addressing;
    /**< If true, the device handle multimaster collisions and arbitration safely*/
    bool        supports_multi_master;
    /**< If true, supports configuring clock stretching. */
    bool        supports_clock_stretching;
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

/** Enable or disable clock stretching for the I2C peripheral.
 *
 * @param obj     The I2C object
 * @param enabled If 'true' enable clock stretching on the given I2C peripheral,
 *                otherwise disable it.
 */
void i2c_set_clock_stretching(i2c_t *obj, bool enabled);

/** Configure the timeout duration in microseconds for blocking transmission
 *
 *  @param obj        The I2C object
 *  @param timeout    Transmission timeout in microseconds.
 *
 *  @note If no timeout is set the default timeout is used.
 *        Default timeout value is based on I2C frequency.
 *        Byte timeout is computed as triple amount of time it would take
 *        to send 10bit over I2C and is expressed by the formula:
 *        byte_timeout = 3 * (1/frequency * 10 * 1000000)
 */
void i2c_timeout(i2c_t *obj, uint32_t timeout);

/** Send START command
 *
 *  @param obj The I2C object.
 */
void i2c_start(i2c_t *obj);

/** Send STOP command
 *
 *  @param obj The I2C object
 */
void i2c_stop(i2c_t *obj);

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
 *  @param address 7/10-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @param stop    If true, stop will be generated after the transfer is done
 *
 *  @note If the current platform supports multimaster operation the peripheral
 *        will perform arbitration automatically when detecting collisions and
 *        complete the transfer or return I2C_ERROR_ARBITRATION_LOST
 *        when loses arbitration.
 *
 *        Additional time for arbitration or clock stretching should by count
 *        by setting appropriate timeout value.
 *
 *        When no transmision timeout was set by the user the default timeout value will
 *        be used. It will count one additional byte for addressing stage:
 *        default_timeout = (length + 1) * byte_timeout.
 *
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int32_t i2c_write(i2c_t *obj, uint16_t address, const uint8_t *data,
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
 *  @param address 7/10-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    If true, stop will be generated after the transfer is done
 *
 *  @note If the current platform supports multimaster operation the peripheral
 *        will perform arbitration automatically when detecting collisions and
 *        complete the transfer or return I2C_ERROR_ARBITRATION_LOST
 *        when loses arbitration.
 *
 *        Additional time for arbitration or clock stretching should by count
 *        by setting appropriate timeout value.
 *
 *        When no transmision timeout was set by the user the default timeout value will
 *        be used. It will count one additional byte for addressing stage:
 *        default_timeout = (length + 1) * byte_timeout.
 *
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int32_t i2c_read(i2c_t *obj, uint16_t address, uint8_t *data, uint32_t length,
                 bool stop);

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

/** Slave status
 *
 *  @note Default status is Idle.
 */
typedef enum {
    Idle           = 0, // Slave has not been addressed.
    ReadAddressed  = 1, // Master has requested a read from this slave.
    WriteGeneral   = 2, // Master is writing to all slaves.
    WriteAddressed = 3  // Master is writing to this slave.
} i2c_slave_status_t;

/** Check to see if the I2C slave has been addressed.
 *
 *  @param obj The I2C object
 *  @return The status - i2c_slave_status_t indicating what state the peripheral
 *          is configured in.
 */
i2c_slave_status_t i2c_slave_status(i2c_t *obj);

/** Configure I2C address.
 *
 *  @note This function does nothing when configured in master mode.
 *
 *  @param obj     The I2C object
 *  @param address The address to be set - 7bit or 10bit
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
 *  @param address   The address to be set - 7bit or 10bit
 *  @param stop      If true, stop will be generated after the transfer is done
 *  @param handler   The I2C IRQ handler to be set
 *  @param ctx       The context pointer
 *  @return          true if the transfer was successfully scheduled, false otherwise
 */
bool i2c_transfer_async(i2c_t *obj, const uint8_t *tx, uint32_t tx_length,
                        uint8_t *rx, uint32_t rx_length, uint16_t address,
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
