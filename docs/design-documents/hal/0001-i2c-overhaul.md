# I2C Design Document


## Description

This update to the I2C HAL API is the first stage in an initiative to standardise and gain consistency across existing and future HAL APIs and their implementations.

## Motivation

Many of the current HAL API interfaces, including the I2C API, do not provide a clear, concrete specification to develop implementations against. As a result, the existing implementations lack consistency and do not have uniform behaviour across all partner boards.

The inconsistencies in the current implementations due to poorly defined specifications are listed below, these will be fixed as part of this overhaul.

As well as fixing existing inconsistencies there are a few unimplemented features that have been requested from the API, or unused features that have been overlooked, these will be added and removed respectively.

Additionally, since the original implementations were added for the API a large number of additional boards have been supported, the overhaul to the API allows partners the opportunity to consolidate the implementations for a family of boards into a single implementation, which will reduce the development overhead in future if any changes are made down the line.

### Inconsistencies

- The implementation for `i2c_reset` varies across all platforms.

  The purpose of this function is to add a method of resetting the i2c bus. Most platform implementations do not implement this function correctly, either the implementation acts as a NOP and does nothing, or it calls `i2c_stop` which already has a functional representation in the API and is designed to perform a different action.

- The implementation of the `i2c_start` and `i2c_stop` functions are not well defined.

  The return value of the function is not defined in the header file, the value has been left open to interpretation by the partner implementations, for example, some implementations return 0 on all conditions. Some implementations have added timeouts to the functionality of the function and return 1 on timeouts. This needs to be defined correctly to be uniform across all implementations.

- The `i2c_slave_address` function has a parameter called `mask`, this parameter is listed as unused but some implementations use it to mask the `address` parameter.

  This parameter should be removed as it is not required by the API. It can be assumed that the address will be masked to exclude the read/write bit from the address, the behaviour of this does not need to change.

- The `i2c_slave_address` function has a parameter called `idx`, this parameter is listed as unused but is utilised in some API implementations.

  This parameter is not required by the API, it should be removed to keep behaviour uniform across all platforms.

- The implementation of the `i2c_byte_write` differs between platforms.

  The API documentation indicates that the return value should be 0 on success, 1 on NAK, 2 on timeout. Not all platforms return a value or handle a timeout. Some platforms return a completely different set of status, the Atmel implementation returns an `I2C_ERROR ` which does not map to the documented return value.

- The return value of the `i2c_start` and `i2c_stop` functions are not documented.

  The function returns an integer but there is no documentation to suggest what the return value should be. Some implementations return 0 in all cases, and some use it to return error values, if this function is to return a value, the values should be standardised.

- Some platforms overload the `i2c_slave_read`, `i2c_slave_write`, `i2c_read`, and `i2c_write` return values with statuses that aren't documented.

  These functions are documented to return the number of bytes written or read. Some implementations will return a non-zero value if the write/read fails, this is not documented anywhere and the behaviour is not common across all implementations.

- Some platforms overload the `i2c_read` and `i2c_write` return values with statuses that aren't documented.

  The functions are documented to return the number of bytes written or read. Some implementations will return a non-zero value if the write/read fails, this is not documented anywhere and the behaviour is not common across all implementations.

- `i2c_master_transfer` takes a `uint32_t` instead of a function pointer type for its `handler` argument.

- `i2c_slave_receive` should return an enumeration of the slave mode, currently it returns one of 4 integers depending on what mode the peripheral is in, this is much more suited to an enumeration.

- The behaviour of the `stop` parameter in `i2c_write`  is not consistent across platforms.

  The parameter is intended to issue the `STOP` command when a transfer has finished. Not all platforms use this bit, some platforms ignore the argument and either always issue a `STOP` command, or never. This should be fixed.

- The behaviour of the I2C peripheral is not uniform in multimaster configurations. There are no specifications indicating what a device should do when needs to handle collision or arbitration loss due to other master devices sharing the bus. Some platforms will transparently handle arbitration and others will return an error to the user API, and some do not handle multimaster.

## Use cases

List of drivers and examples currently using the I2C interface:

* [I2C EEPROM Driver](https://github.com/ARMmbed/i2cee-driver)

### Other use cases

- I2C Slave

  In this use case the peripheral receives the clock instead of generating it and responds when addressed by the master.

- I2C Async

  These are basically the same the regular ones except that the function call should return immediately and a callback should be triggered once the operation is completed or error detected.

- I2C MultiMaster

  The I2C specification defines I2C as a multimaster bus. This allows multiple I2C devices in master mode to concurrently control a slave device on the same bus. The requirements on the master hardware is the ability to handle transfer collisions when both devices attempt to communicate with the slave simultaneously.

## API changes

### General changes

- **Add** an `i2c_free` function to the API.

  All new HAL APIs must provide a free function to act as a destructor so that the resources used by the i2c instance can be reused. This is useful for OOP functionality in the platform API, and potentially reducing power consumption through the freeing of resources.

- **Add** `i2c_timeout` function to the API.

  Sets the transmision timeout to use for the following blocking transfers. This timeout duration is not currently configurable, adding this function will allow this to be set to a specific period before failing the transfer with a timeout error. Calling this function will replace default timeout value. Default timeout value is based on I2C frequency and is computed as triple amount of time it would take to send data over I2C. The timeout value should count the additional time needed by arbitration and clock stretching if any of these can happen

- **Add** an `i2c_get_capabilities` function to API return supported capabilities and constraints on the currently running platform.

- **Add** specification enforcing multimaster support on platforms that support it. Not all partner boards support multimaster, any platform that does support it must handle arbitration with other masters on the same bus and return `I2C_ERROR_ARBITRATION_LOST` status when loses arbitration.

- **Remove** `i2c_reset` function from the API.

  Most target implementations do not implement this function correctly. Most implementations invoke the `stop` command, or directly call the `i2c_stop` from this function which already has a function associated with it in the API. This function is not implemented by the user facing platform API so this change does not affect users.

- **Change** the `frequency` parameter of `i2c_frequency` from `int` to `uint32_t`.

  Frequency should be unsigned as a signed value does not make sense in this context. `int` also does not have a guaranteed size.

- **Add** a return value to the `i2c_frequency` which indicates the frequency that the peripheral was configured to.

  The frequency requested may not be supported by the I2C peripheral, the peripheral will select the nearest supported frequency instead (but not greater then requested), this selected frequency will be returned to inform the caller of the difference.

- **Change** the `stop` parameter for the transfer function from an `int` value to a `bool` value.

  This function argument does not make sense as an `int` value other than for outdated compatibility reasons, the `bool` value expresses the intent more accurately.

- **Add** two more error status.

  `I2C_ERROR_TIMEOUT` returned by `i2c_write` and `i2c_read` when transmission timeout occurs.
  `I2C_ERROR_ARBITRATION_LOST` returned by `i2c_write` and `i2c_read` and passed in `i2c_async_event_t` when lost the arbitration.


### Sync API changes

The main changes involve the removal of the single byte read/write functions and rolling their functionality into the block read/write functions, removing unnecessary parameters from functions and amending their types.

- **Remove** the `i2c_byte_read` and `i2c_byte_write` functions from the API and integrate the functionality into `i2c_read` and `i2c_write`.

  The functionality of these calls can be implemented as part of the normal `i2c_read` and `i2c_write` functions. Sending individual bytes of data is inefficient and should be avoided where possible.

- **Change** the `address` parameter in `i2c_write` and `i2c_read` functions from `int` to `uint16_t`.

  The address parameter is up to a 10-bit value, specifying a type with correct sign and size is sensible.

- **Remove** the return values from `i2c_start` and `i2c_stop`

  Most platforms cannot detect failure at this point and a return status does not make sense at this point.

- **Change** the `length` parameter in `i2c_write` and `i2c_read` functions from `int` to `uint32_t`.

  The length parameter cannot be signed.

- **Change** the `data` parameter in `i2c_write` and `i2c_read` functions from `char*` to `uint8_t*`.

### Slave API changes

The main changes involve removing the slave specific read/write functions and rolling them into the normal read/write functions, removing most of the slave configuration which can be handled at construction by the `init` function.

- **Remove** the `i2c_slave_mode` function, add an `is_slave` parameter to the `i2c_init` function.

  The decision to initialise the peripheral in master or slave mode should be decided at construction time. This simplifies the API as it removes the need for two separate functions to initialise slave mode `i2c_slave_mode` and `i2c_slave_address` .

- **Remove** the I2C slave specific transfer functions: `i2c_slave_read`, `i2c_slave_write`.

  These functions are superfluous and can be rolled into the existing `i2c_read` and `i2c_write` functions. The `transfer` function will execute the slave read/write based on the current configuration of the peripheral.

- **Change** `i2c_slave_receive` to `i2c_slave_status` and the return type from an integer to an enumeration.

  The function returns which mode the peripheral is currently operating in as an integer, this is better expressed as an enumeration.

### Async API changes

- **Remove** the `DMAUsage` argument for asynchronous transfers.

  Currently the `DMAUsage` argument of the `i2c_transfer_asynch` function is unimplemented, the argument is unused by all `I2C` implementations. There is no real demand for it so can be removed from the API.

- **Change** the `stop` parameter from the `i2c_transfer_async` from an `uint32_t` to a `bool`.

  The stop parameter indicates whether or not the function should send a `STOP` command after the transfer has completed, there is no reason at all for this to be a `uint32_t`.

- **Change** the return type of `i2c_transfer_asynch` to indicate whether or not a transfer has been scheduled or not.

  The function now returns a `bool` indicating if a transfer was scheduled or not, which can occur if the peripheral is already busy.

- **Remove** the `i2c_irq_handler_asynch` function from the API.

  The event is now passed as an argument to the callback this method is no longer required.

- **Remove** the `event` parameter from the `i2c_transfer_async` function.

  The callback will now be invoked on any event with the event as an argument.

- **Remove** the `i2c_active` function from the API.

  The async callback is now always invoked on async operation termination (unless cancelled), this status can be tracked from driver layer without any HAL request.

### The new API

```c++
/** Transmission error codes */
enum {
    I2C_ERROR_NO_SLAVE = -1,
    I2C_ERROR_BUS_BUSY = -2,
    I2C_ERROR_TIMEOUT  = -3,
    I2C_ERROR_ARBITRATION_LOST = -4
};

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
    bool      supports_clock_stretching;
} i2c_capabilities_t;

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
 *  @param obj The I2C object
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
int32_t i2c_write(i2c_t *obj, uint16_t address, const uint8_t *data, uint32_t length, bool stop);

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
int32_t i2c_read(i2c_t *obj, uint16_t address, uint8_t *data, uint32_t length, bool stop);

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
 *  @note Callback is invoked when async transfer completes or when error detected.
 */
typedef void (*i2c_async_handler_f)(i2c_t *obj, i2c_async_event_t *event, void *ctx);

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
```

## Behaviours

### Defined behaviours

- `i2c_init`:
  - Initialises the peripheral pins specified in the input parameters.
  - Initialises the peripheral in master mode if `is_slave` is false.
  - Initialises the peripheral in slave mode if `is_slave` is true and `supports_slave_mode` is true.
- `i2c_free`:
  - Resets the pins used to initialise the peripheral to their default state.
  - Disables the peripheral clock.
- `i2c_get_capabilities`:
  - Fills the contents of the `i2c_capabilities_t` parameter
- `i2c_frequency`:
  - Returns the actual frequency that will be used.
  - Sets the frequency to use for the transfer.
  - Must leave all other configuration unchanged.
- `i2c_timeout`:
  - Sets the transmision timeout to use for the following blocking transfers.
  - If the timeout is not set the default timeout is used.
  - Default timeout value is based on I2C frequency. Is computed as triple amount of time it would take to send data over I2C
- `i2c_write`:
  - Writes `length` number of symbols to the bus.
  - Returns the number of symbols sent to the bus.
  - Returns an error code if transfer fails.
  - Generates a stop condition on the bus at the end of the transfer if `stop` parameter is true.
  - Handles transfer collisions and loss of arbitration if the platform supports multimaster in hardware.
  - The transfer will timeout and return `I2C_ERROR_TIMEOUT ` if the transfer takes longer than the configured timeout duration.
- `i2c_read`:
  - Reads `length` symbols from the bus.
  - Returns the number of symbols received from the bus.
  - Returns an error code if transfer fails.
  - Generates a stop condition on the bus at the end of the transfer if `stop` parameter is true.
  - Handles transfer collisions and loss of arbitration if the platform supports multimaster in hardware.
  - The transfer will timeout and return `I2C_ERROR_TIMEOUT ` if the transfer takes longer than the configured timeout duration.
- `i2c_start`:
  - Generates I2C START condition on the bus in master mode.
  - Does nothing if called when the peripheral is configured in slave mode.
- `i2c_stop`:
  - Generates I2C STOP condition on the bus in master mode.
  - Does nothing if called when the peripheral is configured in slave mode.
- `i2c_slave_status`:
  - Indicates which mode the peripheral has been addressed in.
  - Returns not addressed when called in master mode.
- `i2c_slave_address`:
  - Sets the address of the peripheral to the `address` parameter.
  - Does nothing if called in master mode.
- `i2c_transfer_async`:
  - Returns immediately with a `bool` indicating whether the transfer was successfully scheduled or not.
  - The callback given to `i2c_transfer_async` is invoked when the transfer finishes or error occurs.
  - Must save the handler and context pointers inside the `obj` pointer.
  - The context pointer is passed to the callback on transfer completion.
  - The callback must be invoked on completion unless the transfer is aborted.
  - May handle transfer collisions and loss of arbitration if the platform supports multimaster in hardware and enabled in API.
  - `i2c_async_event_t` must be filled with the number of symbols sent to the bus during transfer.
- `i2c_abort_async`:
  - Aborts any on-going async transfers.

### Undefined behaviours

- Use of a `null` pointer as an argument to any function.
- Calling any `I2C` function before calling `i2c_init` or after calling `i2c_free`.
- Initialising the `I2C` peripheral with invalid `SDA` and `SCL` pins.
- Initialising the peripheral in slave mode if slave mode is not supported, indicated by `i2c_get_capabilities`.
- Operating the peripheral in slave mode without first specifying and address using `i2c_address`
- Setting an address using `i2c_address` after initialising the peripheral in master mode.
- Setting an address to an `I2C` reserved value.
- Setting an address larger than the 7-bit supported maximum if 10-bit addressing is not supported.
- Setting an address larger than the 10-bit supported maximum.
- Setting a frequency outside the supported range given by `i2c_get_capabilities`
- Using the device in a multimaster configuration when `supports_multimaster_mode` is false.
- Specifying an invalid address when calling any `read` or `write` functions.
- Setting the length of the transfer or receive buffers to larger than the buffers are.
- Passing an invalid pointer as `handler` to `i2c_transfer_async`.
- Calling `i2c_transfer_abort` when no transfer is currently in progress.

## Impact on Partner implementations

For each target implementation the existing API must be refactored or reimplemented to conform to the updated API. Given that many partners have a single implementation for each board family, generally only one target will need to be updated per partner.

- STM32 - No implementations: 1

  There is a single I2C implementation that implements the interface for all partner boards. Updating the implementation to match the new API will require a single rewrite of this implementation, though ensuring the changes work without regressions on all of the boards may be difficult.

- K64F - No implementations: 3

  There are three implementations for I2C. One for `K20` family boards, one for `KL` family boards, and one for `MCUX` boards. All three implementations would require rewriting for the new API.

- NORDIC - No implementations: 2

  There are two implementations one for `NRF5*` family boards, and one specifically for `NRF51822`. There doesn't seem to be an apparent reason why these couldn't be unified into a single implementation when the API is updated.

- Atmel - No implementations: 2

  One implementation for `M0+` family boards, another implementation for `M4` boards.

- Silicon Labs - No implementations: 1

  A single implementation for all boards, only a single update required.

- Maxim - No implementation 6

  There are 6 instances of the `i2c_api` driver, only 3 of these are unique and the rest are copies in different folders so realistically only 3 updates are required.


## Drawbacks

These are breaking changes to the API. The changes will require each partner to reimplement their current I2C drivers to conform with the updated API.


## FAQ

