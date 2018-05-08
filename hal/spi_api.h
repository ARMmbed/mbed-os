/** \ingroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

#include <stdbool.h>
#include <stdint.h>
#include "device.h"

#if DEVICE_SPI
/**
 * \defgroup hal_spi SPI: Serial peripheral interface HAL API.
 * Low level interface to the serial peripheral interface of a target.
 *
 * A SPI peripheral might be used by multiple spi_t as it only represents a communication
 * channel towards a slave (or a master).
 * Two SPI instances using the same peripheral can be identified by their SS pin.
 *
 * # Defined behaviour
 * - `spi_init()` returns `SPI_RESULT_OK` if the initialization was successful.
 * - `spi_init()` returns `SPI_RESULT_INVALID_PARAM` if at least one of the given parameters is
 *   undefined (NULL).
 * - `spi_init()` returns `SPI_RESULT_ALREADY_INITIALIZED` if another SPI is already using the same
 *   SS pin.
 * - `spi_init()` returns `SPI_RESULT_CONFIG_UNSUPPORTED` if the peripheral does not support the
 *   requested configuration set.
 * - Multiple spi_t can use the same peripheral provided that they use different SS pins.
 * - `spi_transfer()` locks the peripheral and prevents any other transaction to occur until
 *   the operation is completed (including asynchronous transactions) on this peripheral.
 * - `spi_transfer()` behaves as such depending on its spi_transfer_arg_t parameter :
 *   - Sends `tx_count` symbols.
 *   - If tx is not NULL, the tx buffer must be at least `tx_count` symbols long.
 *   - If tx is NULL a fill symbol is sent instead. See spi_init_t.
 *   - Receives `rx_count` symbols.
 *   - If rx is not NULL, the rx buffer must be at least `rx_count` symbols long.
 *   - If rx is NULL, all read symbols are dropped.
 * - `spi_transfer()` returns true if the operation has completed successfully and false otherwise.
 * - `spi_transfer()` returns false if at least one of its parameters is undefined (NULL).
 * - `spi_transfer()` returns false if the peripheral is busy with another transaction (from this
 *   spi_t or any other sharing the same peripheral).
 * - `spi_transfer()` returns false if any error occurs during the transfer. Errors can be but are
 *   not limited to :
 *   - conflicting master ;
 *   - rx or tx buffer overflow ;
 *   - rx or tx buffer underflow.
 * - `spi_data_available()` is only valid in slave mode.
 * - `spi_data_available()` returns true if a symbol is available for reading.
 * - `spi_data_available()` returns false if passed a NULL pointer as its spi_t argument.
 * - `spi_free()` does nothing if passed a NULL pointer.
 * - `spi_free()` de-initialize and eventually disable the clock of the peripheral if it is no longer
 *    in use.
 *
 * # Undefined behaviour
 * - Calling any function other than `spi_init()` before the initialization of the SPI.
 * - Calling any function other than `spi_init()` after calling `spi_free()`.
 *
 * @{
 */

/**
 * This enumerates the possible transmission mode of a SPI peripheral.
 * The SPI_MODE_QUAD_IO mode can sometimes also be backed by a QSPI peripheral in "legacy SPI" mode.
 *
 * @warning Do not confuse with clock_polarity and clock_phase in spi_init_t.
 */
typedef enum spi_mode_t {
    SPI_MODE_SIMPLEX, /**< Unidirectionnal communication on a single wire. */
    SPI_MODE_HALF_DUPLEX, /**< Bidirectionnal communication on a single wire. */
    SPI_MODE_FULL_DUPLEX, /**< Bidrectionnal communication on two wire (MISO/MOSI). */
    SPI_MODE_DUAL_IO, /**< Half-duplex communication on a two wire. */
    SPI_MODE_QUAD_IO, /**< Half-duplex communication on a four wire. */
} spi_mode_t;

/**
 * SPI object.
 * The actual definition of this structure is delegated to the device implementation of the hal.
 */
typedef struct spi_s spi_t;

/**
 * This structure groups all initialization parameters required by a SPI interface.
 */
typedef struct spi_init_s {
    PinName ss;         /**< Slave select pin. */
    PinName miso;
    PinName mosi;       /**< Might not be connected in 3-wire mode. */
    PinName mclk;

    uint32_t fill_symbol; /**< only the n lower bits will be used. */
    uint32_t clock_frequency; /**< MCLK frequency in Hz. */

    bool is_master;  /**< True to configure the device in Master mode */
    bool msb_first;  /**< True to send/receive the most significant bit first. */
    bool clock_phase; /**< True if data line is valid when leaving active state. */
    bool clock_polarity; /**< True if the clock's rest state is high (+Vcc). */
    uint32_t word_length; /**< Length of a symbol in bit. */

    spi_mode_t  mode;   /**< Transmission mode. See spi_mode_t. */
} spi_init_t;

/**
 * This enumerates the possible result of the spi_init function.
 */
typedef enum spi_result_t {
    SPI_RESULT_OK, /**< Operation successful. */
    SPI_RESULT_CONFIG_UNSUPPORTED, /**< The required parameters are not supported by the device. */
    SPI_RESULT_INVALID_PARAM, /**< The given parameter(s) is/are invalid. */
    SPI_RESULT_ALREADY_INITIALIZED, /**< The requested peripheral/SS pin is already initialized. */
} spi_result_t;

/**
 * This structure groups all required data to handle a SPI transaction.
 *
 * - If tx is NULL a fill symbol is sent instead. See spi_init_t.
 * - If tx is not NULL, the tx buffer must be at least `tx_count` symbols long.
 *
 * - If rx is NULL, all read symbols are dropped.
 * - If rx is not NULL, the rx buffer must be at least `rx_count` symbols long.
 *
 * A symbol might be bigger than a byte. In such case symbols are read/written following the
 * platform's endianness.
 */
typedef struct spi_transfer_args_s {
    const uint8_t *tx; /**< A buffer containing the data to be sent. */
    uint32_t tx_count; /**< The number of symbol to send. */
    uint8_t *rx; /**< A buffer to store the received data. */
    uint32_t rx_count; /**< The number of symbol to read. */
} spi_transfer_args_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes a SPI instance.
 *
 * Instances of spi_t may use the same hardware interface, as long as they are configured to use
 * different SS pin.
 *
 * @param[in,out] obj   A pointer to a spi_t object.
 * @param[in]     init  Initialization parameters.
 *
 * @return SPI_RESULT_OK on success. See spi_result_t for more details about failures.
 */
spi_result_t spi_init(spi_t *obj, spi_init_t *init);

/**
 * Processes a transfer blocking until completion.
 * This function locks the peripheral and prevents any other transaction to occur until the
 * operation is completed (including asynchronous transactions).
 * It will return `true` on success or `false` if :
 * - at least one of its parameters is undefined (NULL).
 * - the peripheral is busy with another transaction (from this spi_t or any other sharing the same
 *   peripheral).
 * - any error occurs during the transfer. Errors can be but are not limited to :
 *   - conflicting master ;
 *   - rx or tx buffer overflow ;
 *   - rx or tx buffer underflow.
 *
 * It sends `tx_count` symbols from the buffer pointed by `tx` or a fill symbol is `tx` is NULL.
 * It receives `rx_count` symbols from the peripheral and stores them to `rx` if it is not NULL,
 * else it will discard the read symbols.
 *
 * @param[in,out] obj   A pointer to a spi_t object.
 * @param[in,out] args  A pointer to a spi_transfer_args_t object.
 *
 * @return True on success.
 */
bool spi_transfer(spi_t *obj, spi_transfer_args_t *args);

/**
 * Frees the SPI instance.
 *
 * @param[in,out] obj   A pointer to a spi_t object.
 *
 * @return `true` on success.
 */
bool spi_free(spi_t *obj);

#if DEVICE_SPISLAVE
/**
 * \defgroup hal_spi_deprecated Deprecated API for SPI Slave.
 * The following function is deprecated and is provided solely to maintain compatibility with the
 * \ref mbed::SPISlave driver class.
 * @{
 */

/**
 * Returns true if a symbol is available for reading.
 *
 * @param[in,out] obj   A pointer to a spi_t object.
 */
bool spi_data_available(spi_t *obj);
/**
 * @}
 */
#endif /* DEVICE_SPISLAVE */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_SPI */

#endif /* MBED_SPI_API_H */
/**
 * @}
 */

