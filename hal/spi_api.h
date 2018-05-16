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
 * # Programming model
 * This is an OOP like API. spi_t is used a "this".<br/>
 * Methods not having this type as their first parameter are **class** methods.<br/>
 * Methods with an spi_t object as their first parameter are **instance** methods.<br/>
 * `spi_init()` acts as the "constructor"(/initialiser) of this *class*.<br/>
 * `spi_free()` acts as the "destructor"(/deinitialiser) of this *class*.<br/>
 *
 * # Defined behaviours
 * - Multiple instances can share pins such as MISO/MOSI/MCLK but not their SS pin. This pin is used
 *   to distinguish communication channels (master or slaves).
 * - As the instance allocation is at the user's responsibility, the hal implementation cannot keep
 *   a copy of the pointer (keep an alias to the object) as it may be stack allocated and/or moved.
 * - The hal implementation must support both clock phases.
 * - The hal implementation must support both clock polarities.
 * - The hal implementation must support either master or slave or both modes.
 * - The hal implementation must support either lsb first or msb first or both bit ordering.
 * - The hal implementation must support either continuous or noncontinuous or both modes.
 * - The hal implementation must support at least 1 word length in the range 4 to 32.
 * - The hal implementation must support the simplex and full duplex modes.
 * - The hal implementation may or may not support the half-duplex, dual and/or quad-io modes.
 * - The generated clock must be lower than or equal to the requested frequency minimising the
 *   deviation.
 *
 *
 * - spi_get_capabilities(..)
 *   - returns NULL if passed a NULL pointer.
 *   - returns NULL if the peripheral cannot be identified.
 *   - returns a constant pointer to a constant structure describing the capabilities of the
 *     related peripheral.
 *   - returned value is independent from the SPI channel usage.
 * - spi_init(..)
 *   - returns SPI_RESULT_INVALID_PARAM if any of the parameters is NULL.
 *   - returns SPI_RESULT_DEVICE_BUSY if the identifying set of pin is already associated to an
 *     instance of spi_t.
 *   - returns SPI_RESULT_CONFIG_UNSUPPORTED if the device can not achieve the requested parameters
 *     (Frequency, mode ...).
 *   - returns SPI_RESULT_OK if the device device was associated successfully to the object.
 * - spi_transfer(..)
 *   - returns `true` if the operation succeeded.
 *   - returns `false` if any of the parameters is NULL.
 *   - returns `false` if the peripheral is busy with another transaction.
 *   - returns `false` if an error occurred. An error can be due to :
 *     - conflicting master ;
 *     - rx or tx overflow ;
 *     - rx or tx underflow...
 *   - sends and received up to `max(tx_count, rx_count)`.
 *   - if `tx` is not NULL then writes up to `tx_count` symbol from `tx` then sends
 *     `max(tx_count, rx_count) - tx_count` times the `fill_symbol` .
 *   - if `rx` is not NULL then stores in `rx` up to `rx_count` symbol and continue popping symbol
 *     from the peripheral until `max(tx_count, rx_count) - rx_count` is reached. All spare symbol
 *     are dropped.
 *   - if the device is available, it locks the device and blocks until completion (or error).
 * - spi_free(..)
 *   - does nothing if passed a NULL pointer.
 *   - cancel all pending or on going transaction/transfer and then deinitialise (clock out) this
 *     device if it was the last device associated to this peripheral.
 * - spi_data_available(..)
 *   - returns false if passed a NULL pointer.
 *   - in master mode, returns false.
 *   - in slave mode, returns true if data is available for read.
 *
 * # Undefined behaviours
 * - calling any instance method before `spi_init()`.
 * - calling any other instance method than `spi_init()` after `spi_free()`.
 * - calling any instance method while another one is running.
 * - calling any method from an interrupt context.
 *
 * # Lexicon
 * ## Transfer/Transaction
 * In this documentation a *transfer* is an operation of emission, reception or both that occur
 * during a single call to spi_transfer().
 *
 * ## Continuous mode
 * In this mode SS is kept asserted between words. For example with 4 bits words msb first,
 * continuous mode would transmit 5 symbols: 0x9 0x4 0x8 0xF 0x0
 * ```
 *        ___   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _   _
 * MCLK      \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
 *        _                                                                                 _
 * SS      \_______________________________________________________________________________/
 *        _ ___         ___     ___         ___             _______________                 _
 * DATA   _X   \_______/   \___/   \_______/   \___________/               \_______________X_
 * _
 * ```
 * where non-continuous mode would require a "pause" of a defined duration between symbols thus
 * reducing the overall throughput of the bus : 0x9 0x9 0x3 0x8
 * ```
 *        ___   _   _   _   _  ___   _   _   _   _  ___   _   _   _   _  ___   _   _   _   __
 * MCLK      \_/ \_/ \_/ \_/ --   \_/ \_/ \_/ \_/ --   \_/ \_/ \_/ \_/ --   \_/ \_/ \_/ \_/
 *        _                 _  _                 _  _                 _  _                 __
 * SS      \_______________/ -- \_______________/ -- \_______________/ -- \_______________/
 *        _ ___         _____  _____         _____  _         _________  _____             __
 * DATA   _X   \_______/   X_--_X   \_______/   X_--_X_______/       X_--_X   \___________X__
 * _
 * ```
 * ## Symbol/Word
 * Even though documentations usually uses `word` to mean the unit of data that is sent/received
 * it is ambiguous as it may not correspond to an actual `word` on the platform. To avoid any
 * confusion we will use in this API the word `symbol` instead.
 * ## SPI channel
 * This is the virtual communication channel created between a master and its slave.
 * @{
 */

/**
 * This enumerates the possible transmission mode of a SPI peripheral.
 *
 * The SPI_MODE_QUAD_IO mode may also be backed by a QSPI peripheral in "legacy SPI" mode.
 */
typedef enum {
    SPI_MODE_SIMPLEX, /**< Unidirectional communication on a single wire. */
    SPI_MODE_HALF_DUPLEX, /**< Bidirectional communication on a single wire. */
    SPI_MODE_FULL_DUPLEX, /**< Bidirectional communication on two wire (MISO/MOSI). */
    SPI_MODE_DUAL_IO, /**< Half-duplex communication on 2 wires (transferring 2 bits per clock).*/
    SPI_MODE_QUAD_IO, /**< Half-duplex communication on 4 wires (transferring 4 bits per clock).*/
} spi_mode_t;

/**
 * This is the hal level SPI "class".
 *
 * The actual definition of this structure is delegated to the device implementation of the hal.
 */
typedef struct spi_t spi_t;

/**
 * This holds together the pins related to a single SPI channel.
 *
 * Any pin but the Slave Select pin may be shared between any number of SPI channels.
 * The Slave Select pin serves as a "unique identifier".
 */
typedef struct {
    PinName ss; /**< Slave select pin. */
    PinName miso; /**< Master In Slave our or D1 in DUAL and QUADIO mode */
    /** Master Out Slave In, D2 in DUAL and QUARD-IO mode or may not be connected in 3-wires mode.*/
    PinName mosi;
    PinName mclk; /**< Master Clock pin. */
    PinName d3; /**< Used in QUADIO mode. */
    PinName d4; /**< Used in QUADIO mode. */
} spi_pins_t;

/**
 * This structure groups all initialisation parameters required by a SPI interface.
 */
typedef struct {
    spi_pins_t pins; /**< Pins to use for this channel. */

    /**< This symbol will be sent as a place holder in full-duplex mode in case more symbols are to
         be read than to be sent. */
    uint32_t fill_symbol;
    uint32_t clock_frequency; /**< MCLK frequency in Hz. */

    uint32_t word_length; /**< Length of a symbol in bit. */
    bool is_master; /**< True to configure the device in Master mode. */
    bool msb_first; /**< True to send/receive the most significant bit first. */
    bool clock_phase; /**< True if data line is valid when leaving active state. */
    bool clock_polarity; /**< True if the clock's rest state is high (+Vcc). */
    bool continuous_mode; /**< True to use the continuous mode. */

    spi_mode_t  mode; /**< Transmission mode. See spi_mode_t. */
} spi_init_t;

/**
 * This enumerates the possible result of the spi_init function.
 */
typedef enum {
    SPI_RESULT_OK = 0, /**< Operation successful. */
    SPI_RESULT_CONFIG_UNSUPPORTED = -1, /**< The required parameters are not supported by the device. */
    SPI_RESULT_INVALID_PARAM = -2, /**< The given parameter(s) is/are invalid. */
    SPI_RESULT_DEVICE_BUSY = -3, /**< The requested peripheral/SS p)in is already initialised. */
} spi_result_t;

/**
 * This is describes the capabilities of a SPI channel.
 * The elements of this structure are independent. This means that a supported feature is supported
 * regardless of other feature settings.
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
    enum {
        SPI_CONTINUOUS_MODE_NONCONTINUOUS, /**< The channel only supports the noncontinuous mode. */
        SPI_CONTINUOUS_MODE_CONTINUOUS, /**< The channel only supports the continuous mode. */
        SPI_CONTINUOUS_MODE_BOTH, /**< The channel supports both mode. */
    } continuous_mode;
    enum {
        SPI_AUTHORITY_MASTER, /**< The device only supports the master mode. */
        SPI_AUTHORITY_SLAVE, /**< The device only supports the slave mode. */
        SPI_AUTHORITY_BOTH, /**< The device supports both modes. */
    } authority; /**< Describes the bus authority capability of this channel. */
    bool        support_half_duplex; /**< The device supports the half duplex (aka 3 wires) mode. */
    bool        support_dual_io; /**< The device supports the 2 wires mode. */
    bool        support_quad_io; /**< The device supports the 4 wires mode. */
    enum {
        SPI_BITORDERING_MSB_FIRST, /**< The device can only send data msb first */
        SPI_BITORDERING_LSB_FIRST, /**< The device can only send data lsb first */
        SPI_BITORDERING_BOTH, /**< The device can send data in both bit ordering */
    } bit_ordering;
} spi_capabilities_t;

/**
 * This structure groups all required data to handle a SPI transaction.
 *
 * A symbol might be bigger than a byte. In such case symbols are read/written following the
 * platform's endianness.
 */
typedef struct {
    const uint8_t *tx; /**< A buffer containing the data to be sent. */
    uint32_t tx_count; /**< The number of symbol to send. */
    uint8_t *rx; /**< A buffer to store the received data. */
    uint32_t rx_count; /**< The number of symbol to read. */
} spi_transfer_args_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Obtains the capabilities of a SPI channel.
 *
 * - returns NULL if passed a NULL pointer.
 * - returns NULL if the peripheral cannot be identified.
 * - returns a constant pointer to a constant structure describing the capabilities of the
 *   related peripheral.
 * - returned value is independent from the SPI channel usage.
 *
 * @param[in]   pins    spi_pins_t structure identifying a SPI channel.
 * @return NULL if the device cannot be identified or a constant pointer to a constant structure
 *         describing the capabilities of the device.
 */
const spi_capabilities_t *const spi_get_capabilities(const spi_pins_t *pins);

/**
 * Initialises a SPI instance.
 *
 * - returns SPI_RESULT_INVALID_PARAM if any of the parameters is NULL.
 * - returns SPI_RESULT_DEVICE_BUSY if the identifying set of pin is already associated to an
 *   instance of spi_t.
 * - returns SPI_RESULT_CONFIG_UNSUPPORTED if the device can not achieve the requested parameters
 *   (Frequency, mode ...).
 * - returns SPI_RESULT_OK if the device device was associated successfully to the object.
 *
 * @param[in,out] obj   A spi_t instance to initialise.
 * @param[in]     init  Initialisation parameters.
 *
 * @return SPI_RESULT_OK on success. See spi_result_t for more details about failures.
 */
spi_result_t spi_init(spi_t *obj, const spi_init_t *init);

/**
 * Processes a transfer blocking until completion if the device is available.
 *
 * - returns `true` if the operation succeeded.
 * - returns `false` if any of the parameters is NULL.
 * - returns `false` if the peripheral is busy with another transaction.
 * - returns `false` if an error occurred. An error can be due to :
 *   - conflicting master ;
 *   - rx or tx overflow ;
 *   - rx or tx underflow...
 * - sends and received up to `max(tx_count, rx_count)`.
 * - if `tx` is not NULL then writes up to `tx_count` symbol from `tx` then sends
 *   `max(tx_count, rx_count) - tx_count` times the `fill_symbol` .
 * - if `rx` is not NULL then stores in `rx` up to `rx_count` symbol and continue popping symbol
 *   from the peripheral until `max(tx_count, rx_count) - rx_count` is reached. All spare symbol
 *   are dropped.
 * - if the device is available, it locks the device and blocks until completion (or error).
 *
 * @warning The spi_transfer_args_t instance must live for the whole duration of this method.
 *
 * @param[in,out] obj   An initialised spi_t instance.
 * @param[in]     args  An spi_transfer_args_t instance.
 *
 * @return True on success.
 */
bool spi_transfer(spi_t *obj, const spi_transfer_args_t *args);

/**
 * Frees the SPI instance.
 *
 * - does nothing if passed a NULL pointer.
 * - cancel all pending or on going transaction/transfer and then deinitialise (clock out) this
 *   device if it was the last device associated to this peripheral.
 *
 * @param[in,out] obj   An initialised spi_t instance.
 *
 * @return `true` on success.
 */
void spi_free(spi_t *obj);

#if DEVICE_SPISLAVE
/**
 * \defgroup hal_spi_deprecated SPI: Deprecated API for SPISlave class.
 * The following function is deprecated and is provided solely to maintain compatibility with the
 * \ref mbed::SPISlave driver class.
 * @{
 */

/**
 * Returns true if a symbol is available for reading.
 *
 * - returns false if passed a NULL pointer.
 * - in master mode, returns false.
 * - in slave mode, returns true if data is available for read.
 *
 * @param[in] obj   A pointer to a spi_t object.
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

