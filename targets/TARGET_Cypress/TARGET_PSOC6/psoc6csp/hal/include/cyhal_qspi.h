/***************************************************************************//**
* \file cyhal_qspi.h
*
* \brief
* Provides a high level interface for interacting with the Cypress Quad-SPI.
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
* \addtogroup group_hal_qspi QSPI (Quad Serial Peripheral Interface)
* \ingroup group_hal
* \{
* High level interface for interacting with the Quad-SPI interface.
*
* QSPI is an SPI-based communication interface, often used with external memory devices.
* The QSPI driver supports sending and receiving commands to/from from another
* device via a single, dual, quad, or octal SPI interface.
*
* \section subsection_qspi_features Features
* * Standard SPI Master interface
* * Supports Single/Dual/Quad/Octal SPI memories
* * Supports Dual-Quad SPI mode
* * Execute-In-Place (XIP) from external Quad SPI Flash
* * Supports external serial memory initialization via Serial Flash Discoverable Parameters (SFDP) standard
*
* \section subsection_qspi_code_snippets Code Snippets
* \note The following snippets show commands specific to the
* <a href="https://www.cypress.com/documentation/datasheets/s25fl512s-512-mbit-64-mbyte-30v-spi-flash-memory">S25FL512S Cypress NOR Flash device</a>.
* Refer to the datasheet of the external memory device for device specific memory commands.
* \subsection subsection_qspi_snippet_1 Code Snippet 1: Initializing the cyhal_qspi_command_t structure
* The following code snip demonstrates an example for initializing the cyhal_qspi_command_t structure for
* any given flash command. The cyhal_qspi_command_t.mode_bits structure has several other components which should
* be set as per the command. Mode bits are not required for single SPI read command, hence, mode_bits.disabled
* is set to TRUE in the below example code.
* \snippet qspi.c snippet_cyhal_qspi_structure_initialisation
* \subsection subsection_qspi_snippet_2 Code Snippet 2: QSPI initialization and Reading Flash memory
* This example function demonstrates the initialization of the QSPI component and use of the cyhal_qspi_read() function
* to complete the read operation and receive the read data in a buffer.
* \snippet qspi.c snippet_cyhal_qspi_read
* \subsection subsection_qspi_snippet_3 Code Snippet 3: Erasing Flash memory
* The following code snippet demonstrates the use of cyhal_qspi_transfer() API for sending single byte instruction
* that may or may not need any address or data bytes. It also shows the usage of status register read command within
* a while loop to poll the WIP bit status.
* \snippet qspi.c snippet_cyhal_qspi_erase
* \note Flash memories need erase operation before programming.
* \subsection subsection_qspi_snippet_4 Code Snippet 4: Programming Flash memory
* This code snippet demonstrates the usage cyhal_qspi_write() API for executing program operation on flash memory.
* \snippet qspi.c snippet_cyhal_qspi_program
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_hal_results_qspi QSPI HAL Results
 *  QSPI specific return codes
 *  \ingroup group_hal_results
 *  \{ *//**
 */

/** Bus width Error. */
#define CYHAL_QSPI_RSLT_ERR_BUS_WIDTH                   \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 0))
/** Pin related Error. */
#define CYHAL_QSPI_RSLT_ERR_PIN                         \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 1))
/** Data select Error. */
#define CYHAL_QSPI_RSLT_ERR_DATA_SEL                    \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 2))
/** QSPI instance related error. */
#define CYHAL_QSPI_RSLT_ERR_INSTANCE                    \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 3))
/** Clock frequency error. */
#define CYHAL_QSPI_RSLT_ERR_FREQUENCY                   \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 4))
/** Waiting for certain event error. */
#define CYHAL_QSPI_RSLT_ERR_TIMEOUT                     \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 5))
/** Cannot configure SSEL signal */
#define CYHAL_QSPI_RSLT_ERR_CANNOT_CONFIG_SSEL          \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 6))
/** Cannot switch to specified SSEL signal */
#define CYHAL_QSPI_RSLT_ERR_CANNOT_SWITCH_SSEL          \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 7))

/**
 * \}
 */

/** QSPI Bus width. Some parts of commands provide variable bus width. */

typedef enum cyhal_qspi_bus_width {
    CYHAL_QSPI_CFG_BUS_SINGLE                       = 1,            /**< Normal SPI Mode */
    CYHAL_QSPI_CFG_BUS_DUAL                         = 2,            /**< Dual SPI Mode */
    CYHAL_QSPI_CFG_BUS_QUAD                         = 4,            /**< Quad SPI Mode */
    CYHAL_QSPI_CFG_BUS_OCTAL                        = 8,            /**< Octal SPI Mode */
} cyhal_qspi_bus_width_t;

/** Address size in bits */
typedef enum cyhal_qspi_size {
    CYHAL_QSPI_CFG_SIZE_8                           = 8,            /**< 8 bits address */
    CYHAL_QSPI_CFG_SIZE_16                          = 16,           /**< 16 bits address */
    CYHAL_QSPI_CFG_SIZE_24                          = 24,           /**< 24 bits address */
    CYHAL_QSPI_CFG_SIZE_32                          = 32,           /**< 32 bits address */
} cyhal_qspi_size_t;

/** QSPI interrupt triggers */
typedef enum {
    CYHAL_QSPI_EVENT_NONE                           = 0,            /**< No event */
    CYHAL_QSPI_IRQ_TRANSMIT_DONE                    = 1 << 0,       /**< Async transmit done */
    CYHAL_QSPI_IRQ_RECEIVE_DONE                     = 1 << 1,       /**< Async receive done */
} cyhal_qspi_event_t;

/** @brief QSPI command settings */
typedef struct cyhal_qspi_command {
    struct {
        cyhal_qspi_bus_width_t bus_width;               /**< Bus width for the instruction */
        uint8_t value;                                  /**< Instruction value */
        bool disabled;                                  /**< Instruction phase skipped if disabled is set to true */
    } instruction;                                      /**< Instruction structure */
    struct {
        cyhal_qspi_bus_width_t bus_width;               /**< Bus width for the address */
        cyhal_qspi_size_t size;                         /**< Address size */
        uint32_t value;                                 /**< Address value */
        bool disabled;                                  /**< Address phase skipped if disabled is set to true */
    } address;                                          /**< Address structure */
    struct {
        cyhal_qspi_bus_width_t bus_width;               /**< Bus width for mode bits  */
        cyhal_qspi_size_t size;                         /**< Mode bits size */
        uint32_t value;                                 /**< Mode bits value */
        bool disabled;                                  /**< Mode bits phase skipped if disabled is set to true */
    } mode_bits;                                        /**< Mode bits structure */
    uint8_t dummy_count;                                /**< Dummy cycles count */
    struct {
        cyhal_qspi_bus_width_t bus_width;               /**< Bus width for data */
    } data;                                             /**< Data structure */
} cyhal_qspi_command_t;

/** Handler for QSPI callbacks */
typedef void (*cyhal_qspi_event_callback_t)(void *callback_arg, cyhal_qspi_event_t event);


/** Initialize QSPI peripheral.
 *
 * It should initialize QSPI pins (io0-io7, sclk and ssel), set frequency, clock polarity and phase mode.
 *  The clock for the peripheral should be enabled
 *
 * @param[out] obj  Pointer to a QSPI object. The caller must allocate the memory
 *  for this object but the init function will initialize its contents.
 * @param[in]  io0  Data pin 0
 * @param[in]  io1  Data pin 1
 * @param[in]  io2  Data pin 2
 * @param[in]  io3  Data pin 3
 * @param[in]  io4  Data pin 4
 * @param[in]  io5  Data pin 5
 * @param[in]  io6  Data pin 6
 * @param[in]  io7  Data pin 7
 * @param[in]  sclk The clock pin
 * @param[in]  ssel The chip select pin
 * @note Provided slave select pin will be set as active. This can be changed (as well as additional ssel pins can
 * be added) by \ref cyhal_qspi_slave_select_config and \ref cyhal_qspi_select_active_ssel functions.
 * @param[in]  hz   The bus frequency
 * @param[in]  mode Clock polarity and phase mode (0 - 3)
 * @return The status of the init request
 */
cy_rslt_t cyhal_qspi_init(
    cyhal_qspi_t *obj, cyhal_gpio_t io0, cyhal_gpio_t io1, cyhal_gpio_t io2, cyhal_gpio_t io3, cyhal_gpio_t io4, cyhal_gpio_t io5,
    cyhal_gpio_t io6, cyhal_gpio_t io7, cyhal_gpio_t sclk, cyhal_gpio_t ssel, uint32_t hz, uint8_t mode
);

/** Deinitilize QSPI peripheral
 *
 * It should release pins that are associated with the QSPI object, and disable clocks for QSPI peripheral module
 *  that was associated with the object
 *
 * @param[in,out] obj QSPI object
 */
void cyhal_qspi_free(cyhal_qspi_t *obj);

/** Set the QSPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and the bus clock. Function will
 * apply achieved frequency only if it is in +0% /-10% deviation bounds from desired.
 * Use @ref cyhal_qspi_get_frequency function to get actual frequency value that was achieved and set.
 *
 * @param[in] obj The QSPI object to configure
 * @param[in] hz  The baud rate in Hz
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_qspi_set_frequency(cyhal_qspi_t *obj, uint32_t hz);

/** Get the actual frequency that QSPI is configured for
 *
 * @param[in] obj The QSPI object to configure
 * @return Frequency in Hz
 */
uint32_t cyhal_qspi_get_frequency(cyhal_qspi_t *obj);

/** Configures provided pin to work as QSPI slave select (SSEL)
 *
 * Multiple pins can be configured as QSPI slave select pins. Please refer to device datasheet for details.
 * Switching between configured slave select pins is done by \ref cyhal_qspi_select_active_ssel function.
 * Unless modified with this function, the SSEL pin provided as part of \ref cyhal_qspi_init is the default.
 * @param[in] obj   The QSPI object to configure
 * @param[in] ssel  Pin to be configured as QSPI SSEL
 * @return The status of ssel pin configuration
 */
cy_rslt_t cyhal_qspi_slave_select_config(cyhal_qspi_t *obj, cyhal_gpio_t ssel);

/** Selects an active slave select (SSEL) line from one of available
 *
 * SSEL pin should be configured by \ref cyhal_qspi_slave_select_config or \ref cyhal_qspi_init functions prior to
 * selecting it as active.
 * @param[in] obj   The QSPI object to configure
 * @param[in] ssel  SSEL pin to be set as active
 * @return CY_RSLT_SUCCESS if slave select was switched successfully, otherwise - CYHAL_QSPI_RSLT_ERR_CANNOT_SWITCH_SSEL
 */
cy_rslt_t cyhal_qspi_select_active_ssel(cyhal_qspi_t *obj, cyhal_gpio_t ssel);

/** Receive a command and block of data, synchronously.
 *
 * This will read either `length` bytes or the number of bytes that are currently available in the
 * receive buffer, whichever is less, then return. The value pointed to by `length` will be updated
 * to reflect the number of bytes that were actually read.
 *
 * @param[in]  obj      QSPI object
 * @param[in]  command  QSPI command
 * @param[out] data     RX buffer
 * @param[in]  length   RX buffer length in bytes
 * @return The status of the read request
 */
cy_rslt_t cyhal_qspi_read(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, void *data, size_t *length);

/** Receive a command and block of data in asynchronous mode.
 *
 * This will transfer `length` bytes into the buffer pointed to by `data` in the background. When the
 * requested quantity of data has been read, the @ref CYHAL_QSPI_IRQ_RECEIVE_DONE event will be raised.
 * See @ref cyhal_qspi_register_callback and @ref cyhal_qspi_enable_event.
 *
 * @param[in]  obj      QSPI object
 * @param[in]  command  QSPI command
 * @param[out] data     RX buffer
 * @param[in]  length   RX buffer length in bytes
 * @return The status of the read request
 */
cy_rslt_t cyhal_qspi_read_async(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, void *data, size_t *length);

/** Send a command and block of data, synchronously.
 *
 * This will write either `length` bytes or until the write buffer is full, whichever is less,
 * then return. The value pointed to by `length` will be updated to reflect the number of bytes
 * that were actually written.
 *
 * @param[in] obj      QSPI object
 * @param[in] command  QSPI command
 * @param[in] data     TX buffer
 * @param[in] length   TX buffer length in bytes
 * @return The status of the write request
 */
cy_rslt_t cyhal_qspi_write(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *data, size_t *length);

/** Send a command and block of data in asynchronous mode.
 *
 * This will transfer `length` bytes into the tx buffer in the background. When the requested
 * quantity of data has been queued in the transmit buffer, the @ref CYHAL_QSPI_IRQ_TRANSMIT_DONE
 * event will be raised. See @ref cyhal_qspi_register_callback and @ref cyhal_qspi_enable_event.
 *
 * @param[in] obj      QSPI object
 * @param[in] command  QSPI command
 * @param[in] data     TX buffer
 * @param[in] length   TX buffer length in bytes
 * @return The status of the write request
 */
cy_rslt_t cyhal_qspi_write_async(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *data, size_t *length);

/** Send a command (and optionally data) and get the response. Can be used to send/receive device specific commands
 *
 * @param[in]  obj      QSPI object
 * @param[in]  command  QSPI command
 * @param[in]  tx_data  TX buffer
 * @param[in]  tx_size  TX buffer length in bytes
 * @param[out] rx_data  RX buffer
 * @param[in]  rx_size  RX buffer length in bytes
 * @return The status of the transfer request
 */
cy_rslt_t cyhal_qspi_transfer(
    cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data,
    size_t rx_size
);

/** Register a QSPI event handler
 *
 * This function will be called when one of the events enabled by \ref cyhal_qspi_enable_event occurs.
 *
 * @param[in] obj          The QSPI object
 * @param[in] callback     The callback handler which will be invoked when the interrupt fires
 * @param[in] callback_arg Generic argument that will be provided to the handler when called
 */
void cyhal_qspi_register_callback(cyhal_qspi_t *obj, cyhal_qspi_event_callback_t callback, void *callback_arg);

/** Configure QSPI interrupt enablement.
 *
 * When an enabled event occurs, the function specified by \ref cyhal_qspi_register_callback will be called.
 *
 * @param[in] obj            The QSPI object
 * @param[in] event          The QSPI event type
 * @param[in] intr_priority  The priority for NVIC interrupt events
 * @param[in] enable         True to turn on interrupts, False to turn off
 */
void cyhal_qspi_enable_event(cyhal_qspi_t *obj, cyhal_qspi_event_t event, uint8_t intr_priority, bool enable);


#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_QSPI_IMPL_HEADER
#include CYHAL_QSPI_IMPL_HEADER
#endif /* CYHAL_QSPI_IMPL_HEADER */

/** \} group_hal_qspi */
