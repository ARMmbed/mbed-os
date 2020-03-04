/***************************************************************************//**
* \file cyhal_spi.h
*
* \brief
* Provides a high level interface for interacting with the Cypress SPI.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

/**
* \addtogroup group_hal_spi SPI (Serial Peripheral Interface)
* \ingroup group_hal
* \{
* High level interface for interacting with the Serial Peripheral Interface (SPI).
*
* The SPI protocol is a synchronous serial interface protocol. Devices operate
* in either master or slave mode. The master initiates the data transfer.
*
* Motorola SPI modes 0, 1, 2, and 3 are supported, with either MSB or LSB first.
* The operating mode and data frame size can be configured via \ref cyhal_spi_cfg_t.
*
* \section section_spi_features Features
* * Supports master and slave functionality.
* * Supports Motorola modes - 0, 1, 2 and 3 - \ref cyhal_spi_mode_t
* * MSb or LSb first shift direction - \ref cyhal_spi_mode_t
* * Master supports up to four slave select lines
* * Supports data frame size of 8 or 16 bits
* * Configurable interrupt and callback assignment on SPI events:
* Data transfer to FIFO complete, Transfer complete and Transmission error - \ref cyhal_spi_event_t
* * Supports changing baud rate of the transaction in run time.
* * Provides functions to send/receive a single byte or block of data.
*
* \section section_spi_quickstart Quick Start
*
* Initialise a SPI master or slave interface using \ref cyhal_spi_init() and provide the SPI pins (<b>mosi</b>, <b>miso</b>, <b>sclk</b>, <b>ssel</b>),
* number of bits per frame (<b>data_bits</b>) and SPI Motorola <b>mode</b>. The data rate can be set using \ref cyhal_spi_set_frequency(). <br>
* See \ref section_spi_snippets for code snippets to send or receive the data.
*
* \section section_spi_snippets Code snippets
*
* \subsection subsection_spi_snippet_1 Snippet 1: SPI Master - Single byte transfer operation (Read and Write)
* The following code snippet initialises an SPI Master interface using the \ref cyhal_spi_init(). The data rate of transfer is set using \ref cyhal_spi_set_frequency().
* The code snippet shows how to transfer a single byte of data using \ref cyhal_spi_send() and \ref cyhal_spi_recv().
* \snippet spi.c snippet_cyhal_spi_master_byte_operation
*
* \subsection subsection_spi_snippet_2 Snippet 2: SPI Slave - Single byte transfer operation (Read and Write)
* The following code snippet initialises an SPI Slave interface using the \ref cyhal_spi_init(). The data rate of transfer is set using \ref cyhal_spi_set_frequency.
* The code snippet shows how to transfer a single byte of data using \ref cyhal_spi_send() and \ref cyhal_spi_recv.
* \snippet spi.c snippet_cyhal_spi_slave_byte_operation
*
* \subsection subsection_spi_snippet_3 Snippet 3: SPI Block Data transfer
* The following snippet sends and receives an array of data in a single SPI transaction using \ref cyhal_spi_transfer(). The example
* uses SPI master to transmit 5 bytes of data and receive 5 bytes of data in a single transaction.
* \snippet spi.c snippet_cyhal_spi_block_data_transfer
*
* \subsection subsection_spi_snippet_4 Snippet 4: Interrupts on SPI events
* SPI interrupt events ( \ref cyhal_spi_event_t) can be mapped to an interrupt and assigned to a callback function.
* The callback function needs to be first registered and then the event needs to be enabled.
* The following snippet initialises a SPI master to perform a block transfer using \ref cyhal_spi_transfer_async(). This is a non-blocking function.
*  A callback function is registered using \ref cyhal_spi_register_callback to notify whenever the SPI transfer is complete.
* \snippet spi.c snippet_cyhal_spi_interrupt_callback_events

* \section subsection_spi_moreinfor More Information
*
* * <a href="https://github.com/cypresssemiconductorco/mtb-example-psoc6-spi-master"><b>mtb-example-psoc6-spi-master</b></a>: This example project demonstrates
* use of SPI (HAL) resource in PSoC® 6 MCU in Master mode to write data to an SPI slave.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Bad argument */
#define CYHAL_SPI_RSLT_BAD_ARGUMENT                    (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SPI, 0))
/** Failed to initialize SPI clock */
#define CYHAL_SPI_RSLT_CLOCK_ERROR                     (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SPI, 1))
/** Failed to Transfer SPI data */
#define CYHAL_SPI_RSLT_TRANSFER_ERROR                  (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SPI, 2))
/** Provided clock is not supported by SPI */
#define CYHAL_SPI_RSLT_CLOCK_NOT_SUPPORTED             (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SPI, 3))
/** Provided PIN configuration is not supported by SPI */
#define CYHAL_SPI_RSLT_PIN_CONFIG_NOT_SUPPORTED        (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SPI, 5))
/** Provided PIN configuration is not supported by SPI */
#define CYHAL_SPI_RSLT_INVALID_PIN_API_NOT_SUPPORTED   (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SPI, 6))
/** The requested resource type is invalid */
#define CYHAL_SPI_RSLT_ERR_INVALID_PIN                 (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SPI, 7))

/** Compatibility define for cyhal_spi_set_frequency. */
#define cyhal_spi_frequency cyhal_spi_set_frequency

/** SPI interrupt triggers */
typedef enum {
    /** All transfer data has been moved into data FIFO */
    CYHAL_SPI_IRQ_DATA_IN_FIFO        = 1 << 1,
    /** Transfer complete. */
    CYHAL_SPI_IRQ_DONE                = 1 << 2,
    /** An error occurred while transferring data */
    CYHAL_SPI_IRQ_ERROR               = 1 << 3,
} cyhal_spi_event_t;

/** Handler for SPI interrupts */
typedef void (*cyhal_spi_event_callback_t)(void *callback_arg, cyhal_spi_event_t event);

/** SPI operating modes */
typedef enum
{
    /** Standard motorola SPI CPOL=0, CPHA=0 with MSB first operation */
    CYHAL_SPI_MODE_00_MSB,
    /** Standard motorola SPI CPOL=0, CPHA=0 with LSB first operation */
    CYHAL_SPI_MODE_00_LSB,
    /** Standard motorola SPI CPOL=0, CPHA=1 with MSB first operation */
    CYHAL_SPI_MODE_01_MSB,
    /** Standard motorola SPI CPOL=0, CPHA=1 with LSB first operation */
    CYHAL_SPI_MODE_01_LSB,
    /** Standard motorola SPI CPOL=1, CPHA=0 with MSB first operation */
    CYHAL_SPI_MODE_10_MSB,
    /** Standard motorola SPI CPOL=1, CPHA=0 with LSB first operation */
    CYHAL_SPI_MODE_10_LSB,
    /** Standard motorola SPI CPOL=1, CPHA=1 with MSB first operation */
    CYHAL_SPI_MODE_11_MSB,
    /** Standard motorola SPI CPOL=1, CPHA=1 with LSB first operation */
    CYHAL_SPI_MODE_11_LSB,
} cyhal_spi_mode_t;

/** @brief Initial SPI configuration. */
typedef struct
{
    cyhal_spi_mode_t mode; //!< The operating mode
    uint8_t data_bits; //!< The number of bits per transfer
    bool is_slave; //!< Whether the peripheral is operating as slave or master
} cyhal_spi_cfg_t;

/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 * @param[in]  mosi The pin to use for MOSI
 * @note At least MOSI or MISO pin should be non-NC
 * @param[in]  miso The pin to use for MISO
 * @note At least MOSI or MISO pin should be non-NC
 * @param[in]  sclk The pin to use for SCLK
 * @note This pin cannot be NC
 * @param[in]  ssel The pin to use for SSEL
 * @note This pin can be NC
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be allocated
 * @param[in]  bits      The number of bits per frame
 * @note bits should be 8 or 16
 * @param[in]  mode      The SPI mode (clock polarity, phase, and shift direction)
 * @param[in]  is_slave  false for master mode or true for slave mode operation
 * @return The status of the init request
 */
cy_rslt_t cyhal_spi_init(cyhal_spi_t *obj, cyhal_gpio_t mosi, cyhal_gpio_t miso, cyhal_gpio_t sclk, cyhal_gpio_t ssel, const cyhal_clock_divider_t *clk,
                        uint8_t bits, cyhal_spi_mode_t mode, bool is_slave);

/** Release a SPI object
 *
 * Return the peripheral, pins and clock owned by the SPI object to their reset state
 * @param[in,out] obj The SPI object to deinitialize
 */
void cyhal_spi_free(cyhal_spi_t *obj);

/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * @param[in,out] obj The SPI object to configure
 * @param[in]     hz  The baud rate in Hz
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_spi_set_frequency(cyhal_spi_t *obj, uint32_t hz);

/** Get a received value out of the SPI receive buffer
 *
 * In Master mode - transmits fill-in value and read the data from RxFifo
 * In Slave mode - Blocks until a value is available
 * @param[in] obj   The SPI peripheral to read
 * @param[in] value The value received
 * @return The status of the read request
 * @note
 * - In Master mode, MISO pin required to be non-NC for this API to operate
 * - In Slave mode, MOSI pin required to be non-NC for this API to operate
 */
cy_rslt_t cyhal_spi_recv(cyhal_spi_t *obj, uint32_t* value);

/** Send a byte out
 *
 * In Master mode transmits value to slave and read/drop a value from the RxFifo.
 * In Slave mode writes a value to TxFifo
 * @param[in] obj   The SPI peripheral to use for sending
 * @param[in] value The value to send
 * @return The status of the write request
 * @note
 * - In Master mode, MOSI pin required to be non-NC for this API to operate
 * - In Slave mode, MISO pin required to be non-NC for this API to operate
 */
cy_rslt_t cyhal_spi_send(cyhal_spi_t *obj, uint32_t value);

/** Write a block out and receive a value
 *
 *  The total number of bytes sent and received will be the maximum of
 *  tx_length and rx_length. The bytes written will be padded with the
 *  value 0xff.
 *
 * @param[in] obj           The SPI peripheral to use for sending
 * @param[in] tx            Pointer to the byte-array of data to write to the device
 * @param[in,out] tx_length Number of bytes to write, updated with the number actually written
 * @param[out] rx           Pointer to the byte-array of data to read from the device
 * @param[in,out] rx_length Number of bytes to read, udpated with the number actually read
 * @param[in] write_fill    Default data transmitted while performing a read
 * @return The status of the transfer request
 * @note Both MOSI and MISO pins required to be non-NC for this API to operate
 */
cy_rslt_t cyhal_spi_transfer(cyhal_spi_t *obj, const uint8_t *tx, size_t tx_length, uint8_t *rx, size_t rx_length, uint8_t write_fill);

/** Begin the SPI transfer. Buffer pointers and lengths are specified in tx_buff and rx_buff
 *
 * @param[in] obj           The SPI object that holds the transfer information
 * @param[in] tx            The transmit buffer
 * @param[in,out] tx_length The number of bytes to transmit
 * @param[out] rx           The receive buffer
 * @param[in,out] rx_length The number of bytes to receive
 * @return The status of the transfer_async request
 * @note Both MOSI and MISO pins required to be non-NC for this API to operate
 */
cy_rslt_t cyhal_spi_transfer_async(cyhal_spi_t *obj, const uint8_t *tx, size_t tx_length, uint8_t *rx, size_t rx_length);

/** Checks if the specified SPI peripheral is in use
 *
 * @param[in] obj  The SPI peripheral to check
 * @return Indication of whether the SPI is still transmitting
 */
bool cyhal_spi_is_busy(cyhal_spi_t *obj);

/** Abort an SPI transfer
 *
 * @param[in] obj The SPI peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cyhal_spi_abort_async(cyhal_spi_t *obj);

/** The SPI callback handler registration
 *
 * @param[in] obj         The SPI object
 * @param[in] callback     The callback handler which will be invoked when the interrupt fires
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_spi_register_callback(cyhal_spi_t *obj, cyhal_spi_event_callback_t callback, void *callback_arg);

/** Configure SPI interrupt. This function is used for word-approach
 *
 * @param[in] obj      The SPI object
 * @param[in] event    The SPI event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable   True to turn on interrupts, False to turn off
 */
void cyhal_spi_enable_event(cyhal_spi_t *obj, cyhal_spi_event_t event, uint8_t intrPriority, bool enable);

/*******************************************************************************
* Backward compatibility macro. The following code is DEPRECATED and must
* not be used in new projects
*******************************************************************************/
/** \cond INTERNAL */
typedef cyhal_spi_event_t             cyhal_spi_irq_event_t;
/** \endcond */

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_SPI_IMPL_HEADER
#include CYHAL_SPI_IMPL_HEADER
#endif /* CYHAL_SPI_IMPL_HEADER */

/** \} group_hal_spi */
