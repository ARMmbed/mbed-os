
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_SPI_API_H
#define MBED_SPI_API_H

#include <stdint.h>
#include <stdbool.h>

#include "PeripheralNames.h"
#include "PinNames.h"

#include "hal/dma_api.h"
#include "hal/buffer.h"

#if DEVICE_SPI

#ifdef __cplusplus
extern "C" {
#endif

/*
 * \defgroup hal_spi Serial Peripheral Interface
 * ## Behaviours
 * ### Defined Behaviours
 *
 * - `spi_get_module()` returns the `SPIName` unique identifier to the peripheral associated to this SPI channel.
 * - `spi_get_capabilities()` fills the given `spi_capabilities_t` instance
 * - `spi_get_capabilities()` should consider the `ssel` pin when evaluation the `support_slave_mode` capability.
 *   If the given `ssel` pin cannot be managed by hardware in slave mode, `support_slave_mode` should be false.
 * - At least a symbol width of 8bit must be supported.
 * - The supported frequency range must include the range [0.2..2] MHz.
 * - The shortest part of the duty cycle must not be shorter than 50% of the expected period.
 * - `spi_init()` initializes the pins leaving the configuration registers unchanged.
 * - `spi_init()` if `is_slave` is false:
 *     - if `ssel` is `NC` the hal implementation ignores this pin.
 *     - if `ssel` is not `NC` then the hal implementation owns the pin and its management.
 * - When managed by the hal implementation, `ssel` is always considered active low.
 * - When the hardware supports the half-duplex (3-wire) mode, if `miso` (exclusive) or `mosi` is missing in any function that expects pins, the bus is assumed to be half-duplex.
 * - `spi_free()` resets the pins to their default state.
 * - `spi_free()` disables the peripheral clock.
 * - `spi_format()` sets :
 *   - the number of bits per symbol
 *   - the mode :
 *     0. Clock idle state is *low*, data are sampled when the clock becomes *active* (polarity = 0, phase = 0)
 *     1. Clock idle state is *low*, data are sampled when the clock becomes *inactive* (polarity = 0, phase = 1)
 *     2. Clock idle state is *high*, data are sampled when the clock becomes *active* (polarity = 1, phase = 0)
 *     3. Clock idle state is *high*, data are sampled when the clock becomes *inactive* (polarity = 1, phase = 1)
 *   - the bit ordering (lsb/msb first).
 * - `spi_format()` updates the configuration of the peripheral except the baud rate generator.
 * - `spi_frequency()` sets the frequency to use during the transfer.
 * - `spi_frequency()` returns the actual frequency that will be used.
 * - `spi_frequency()` updates the baud rate generator leaving other configurations unchanged.
 * - `spi_init()`, `spi_frequency()` and `spi_format()` must be called at least once each before initiating any transfer.
 * - `spi_transfer()` :
 *   - writes `tx_len` symbols to the bus.
 *   - reads `rx_len` symbols from the bus.
 *   - if `rx` is NULL then inputs are discarded.
 *   - if `tx` is NULL then `fill_symbol` is used instead.
 *   - returns the number of symbol clocked on the bus during this transfer.
 *   - expects symbols types to be the closest stdint type bigger or equal to its size following the platform's endianness.
 *     e.g.:
 *     - 7bits => uint8_t
 *     - 15bits => uint16_t
 *     - 16bits => uint16_t
 *     - 17bits => uint32_t
 *   - In Full-duplex mode :
 *     - if `rx_len` > `tx_len` then it sends `(rx_len-tx_len)` additional `fill_symbol` to the bus.
 *   - In Half-duplex mode :
 *     - as master, `spi_transfer()` sends `tx_len` symbols and then reads `rx_len` symbols.
 *     - as slave, `spi_transfer()` receives `rx_len` symbols and then sends `tx_len` symbols.
 * - `spi_transter_async()` schedules a transfer to be process the same way `spi_transfer()` would have but asynchronously.
 * - `spi_transter_async()` returns immediately with a boolean indicating whether the transfer was successfully scheduled or not.
 * - The callback given to `spi_transfer_async()` is invoked when the transfer completes (with a success or an error).
 * - `spi_transfer_async()` saves the handler and the `ctx` pointer.
 * - The `ctx` is passed to the callback on transfer completion.
 * - Unless if the transfer is aborted, the callback is invoked on completion. The completion maybe when all symbols have been transmitted
 *   or when in slave mode the master de-asserts the chip select.
 * - The `spi_transfer_async()` function may use the `DMAUsage` hint to select the appropriate async algorithm.
 * - The `spi_async_event_t` must be filled with the number of symbol clocked on the bus during this transfer and a boolean value indicated if an error has occurred.
 * - `spi_transfer_async_abort()` aborts an on-going async transfer.
 *
 * ### Undefined Behaviours
 * - Calling `spi_init()` multiple times on the same `spi_t` without `spi_free()`'ing it first.
 * - Calling any method other than `spi_init()` on a non-initialized or freed `spi_t`.
 * - Passing both `miso` and `mosi` as `NC` to `spi_get_module` or `spi_init`.
 * - Passing `miso` or `mosi` as `NC` on target that does not support half-duplex mode.
 * - Passing `mclk` as `NC`  to `spi_get_module` or `spi_init`.
 * - Passing an invalid pointer as `cap` to `spi_get_capabilities`.
 * - Passing pins that cannot be on the same peripheral.
 * - Passing an invalid pointer as `obj` to any method.
 * - Giving a `ssel` pin to `spi_init()` when using in master mode.
 *   SS must be managed by hardware in slave mode and must **NOT** be managed by hardware in master mode.
 * - Setting a frequency outside of the range given by `spi_get_capabilities()`.
 * - Setting a frequency in slave mode.
 * - Setting `bits` in `spi_format` to a value out of the range given by `spi_get_capabilities()`.
 * - Passing an invalid pointer as `fill_symbol` to `spi_transfer` and `spi_transfer_async` while they would be required by the transfer (`rx_len != tx_len` or `tx==NULL`).
 * - Passing an invalid pointer as `handler` to `spi_transfer_async`.
 * - Calling `spi_transfer_async_abort()` while no async transfer is being processed (no transfer or a synchronous transfer).
 * - In half-duplex mode, any mechanism (if any is present) to detect or prevent collision is implementation defined.
 */


typedef struct spi_s spi_t;

/**
 * Describes the capabilities of a SPI peripherals
 */
typedef struct {
    /** Minimum frequency supported must be set by target device and it will be assessed during
     *  testing.
     */
    uint32_t    minimum_frequency;
    /** Maximum frequency supported must be set by target device and it will be assessed during
     *  testing.
     */
    uint32_t    maximum_frequency;
    /** Each bit represents the corresponding word length. lsb => 1bit, msb => 32bit. */
    uint32_t    word_length;
    bool        support_slave_mode; /**< If true, the device can handle SPI slave mode using hardware management on the specified ssel pin. */
    bool        half_duplex; /**< If true, the device also supports SPI transmissions using only 3 wires. */
} spi_capabilities_t;

/**
 * SPI modes.
 */
typedef enum _spi_mode_t {
    SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE,
    SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE,
    SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE,
    SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE,
} spi_mode_t;

/**
 * SPI bit ordering.
 */
typedef enum _spi_bit_ordering_t {
    SPI_BIT_ORDERING_MSB_FIRST,
    SPI_BIT_ORDERING_LSB_FIRST,
} spi_bit_ordering_t;

/**
 * Event data reported to interrupt's callback.
 */
typedef struct _spi_async_event_t {
    uint32_t transfered;
    bool     error;
} spi_async_event_t;

/**
 * Interrupt callback signature.
 */
typedef void (*spi_async_handler_f)(spi_t *obj, void *ctx, spi_async_event_t *event);

/**
 * Returns a variant of the SPIName enum uniquely identifying a SPI peripheral of the device.
 */
SPIName spi_get_module(PinName mosi, PinName miso, PinName mclk);

/**
 * Fills the given spi_capabilities_t structure with the capabilities of the given peripheral.
 */
void spi_get_capabilities(SPIName name, PinName ssel, spi_capabilities_t *cap);

/**
 * Initialized a spi peripheral.
 *
 * @param obj       SPI peripheral.
 * @param is_slave  True if the peripheral is used as a slave.
 * @param mosi      Master Out Slave In pin.
 * @param miso      Master In Slave Out pin.
 * @param mclk      Clock pin.
 * @param ssel      Slave Select pin.
 */
void spi_init(spi_t *obj, bool is_slave, PinName mosi, PinName miso, PinName mclk, PinName ssel);
/**
 * Configure the format to be used by the SPI peripheral.
 *
 * @param obj       SPI peripheral.
 * @param bits      Number of bits per symbol.
 * @param mode      SPI mode to use.
 * @param bit_ordering SPI bit ordering.
 */
void spi_format(spi_t *obj, uint8_t bits, spi_mode_t mode, spi_bit_ordering_t bit_ordering);
/**
 * Set the frequency target.
 * This function tries to acheive the closest frequency possible prefering a value lower than the target.
 *
 * @param obj       SPI peripheral.
 * @param hz        Target frequency in herz.
 */
uint32_t spi_frequency(spi_t *obj, uint32_t hz);
/**
 * Transfer data blocking until the end of the operation.
 *
 * @param obj       SPI periheral.
 * @param tx        Buffer so send.
 * @param tx_len    Number of symbol in tx.
 * @param rx        Buffer to store received symbols.
 * @param rx_len    Number of symbol to store in rx.
 * @param fill_symbol Symbol to send if tx_len is less than rx_len.
 */
uint32_t spi_transfer(spi_t *obj, const void *tx, uint32_t tx_len, void *rx, uint32_t rx_len, const void *fill_symbol);

#ifdef DEVICE_SPI_ASYNCH
/**
 * \defgroup AsynchSPI Asynchronous SPI Hardware Abstraction Layer
 * @{
 */

/**
 * Transfer data returning immediately.
 *
 * @param obj       SPI periheral.
 * @param tx        Buffer so send.
 * @param tx_len    Number of symbol in tx.
 * @param rx        Buffer to store received symbols.
 * @param rx_len    Number of symbol to store in rx.
 * @param fill_symbol Symbol to send if tx_len is less than rx_len.
 * @param handler   Callback to invoke at the end of the operation.
 * @param ctx       Argument passed to the callback.
 * @param hint      Hint to help determine the DMA allocation strategy.
 */
bool spi_transfer_async(spi_t *obj, const void *tx, uint32_t tx_len, void *rx, uint32_t rx_len, const void *fill_symbol, spi_async_handler_f handler, void *ctx, DMAUsage hint);
/**
 * Abort the current asynchronous operation.
 */
void spi_transfer_async_abort(spi_t *obj);
#endif // DEVICE_SPI_ASYNCH

/**@}*/

void spi_free(spi_t *obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SPI_DEVICE

#endif // MBED_SPI_API_H

/** @}*/
