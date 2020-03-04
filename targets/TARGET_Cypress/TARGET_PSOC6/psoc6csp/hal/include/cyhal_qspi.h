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
* High level interface for interacting with the Quad Serial Peripheral Interface (QSPI) interface.
*
* The QSPI block supports sending commands to and receiving commands from an
* another device (often an external memory) via single, dual, quad, or octal SPI.
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

/** QSPI Bus width
 *
 * Some parts of commands provide variable bus width
 */
typedef enum cyhal_qspi_bus_width {
    CYHAL_QSPI_CFG_BUS_SINGLE                       = 1,
    CYHAL_QSPI_CFG_BUS_DUAL                         = 2,
    CYHAL_QSPI_CFG_BUS_QUAD                         = 4,
    CYHAL_QSPI_CFG_BUS_OCTAL                        = 8,
} cyhal_qspi_bus_width_t;

/** Size in bits */
typedef enum cyhal_qspi_size {
    CYHAL_QSPI_CFG_SIZE_8                           = 8,
    CYHAL_QSPI_CFG_SIZE_16                          = 16,
    CYHAL_QSPI_CFG_SIZE_24                          = 24,
    CYHAL_QSPI_CFG_SIZE_32                          = 32,
} cyhal_qspi_size_t;

/** QSPI interrupt triggers */
typedef enum {
    CYHAL_QSPI_EVENT_NONE                           = 0,            /**< No event >*/
    CYHAL_QSPI_IRQ_TRANSMIT_DONE                    = 1 << 0,       /**< Async transmit done. >*/
    CYHAL_QSPI_IRQ_RECEIVE_DONE                     = 1 << 1,       /**< Async receive done. >*/
} cyhal_qspi_event_t;

#define CYHAL_QSPI_RSLT_ERR_BUS_WIDTH (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 0)) /**< Bus width Error. >*/
#define CYHAL_QSPI_RSLT_ERR_PIN (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 1)) /**< Pin related Error. >*/
#define CYHAL_QSPI_RSLT_ERR_DATA_SEL (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 2)) /**< Data select Error. >*/
#define CYHAL_QSPI_RSLT_ERR_INSTANCE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 3)) /**< QSPI instance related Error. >*/
#define CYHAL_QSPI_RSLT_ERR_FREQUENCY (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_QSPI, 4)) /**< Clock frequency error. >*/

/** @brief QSPI command settings */
typedef struct cyhal_qspi_command {
    struct {
        cyhal_qspi_bus_width_t bus_width;               /**< Bus width for the instruction >*/
        uint8_t value;                                  /**< Instruction value >*/
        bool disabled;                                  /**< Instruction phase skipped if disabled is set to true >*/
    } instruction;
    struct {
        cyhal_qspi_bus_width_t bus_width;               /**< Bus width for the address >*/
        cyhal_qspi_size_t size;                         /**< Address size >*/
        uint32_t value;                                 /**< Address value >*/
        bool disabled;                                  /**< Address phase skipped if disabled is set to true >*/
    }  address;
    struct {
        cyhal_qspi_bus_width_t bus_width;               /**< Bus width for mode bits  >*/
        cyhal_qspi_size_t size;                         /**< Mode bits size >*/
        uint32_t value;                                 /**< Mode bits value >*/
        bool disabled;                                  /**< Mode bits phase skipped if disabled is set to true >*/
    } mode_bits;
    uint8_t dummy_count;                                /**< Dummy cycles count >*/
    struct {
        cyhal_qspi_bus_width_t bus_width;               /**< Bus width for data >*/
    } data;
} cyhal_qspi_command_t;

/** Handler for QSPI callbacks */
typedef void (*cyhal_qspi_event_callback_t)(void *callback_arg, cyhal_qspi_event_t event);

/** Initialize QSPI peripheral.
 *
 * It should initialize QSPI pins (io0-io7, sclk and ssel), set frequency, clock polarity and phase mode.
 *  The clock for the peripheral should be enabled
 *
 * @param[out] obj  QSPI object
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
 * Actual frequency may differ from the desired frequency due to available dividers and the bus clock
 * Configures the QSPI peripheral's baud rate
 * @param[in] obj The QSPI object to configure
 * @param[in] hz  The baud rate in Hz
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_qspi_set_frequency(cyhal_qspi_t *obj, uint32_t hz);

/** Receive a command and block of data, synchronously.
 *
 * @param[in]     obj QSPI object
 * @param[in]     command QSPI command
 * @param[out]    data RX buffer
 * @param[in,out] length in - RX buffer length in bytes, out - number of bytes read
 * @return The status of the read request
 */
cy_rslt_t cyhal_qspi_read(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, void *data, size_t *length);

/** Receive a command and block of data in asynchronous mode. This requires __enable_irq() to have been called
 * in order to work. To receive a notification when the read is complete, enable and register a callback
 * using cyhal_qspi_irq_enable and cyhal_qspi_register_irq.
 *
 * @param[in]     obj QSPI object
 * @param[in]     command QSPI command
 * @param[out]    data RX buffer
 * @param[in,out] length in - RX buffer length in bytes, out - number of bytes read
 * @return The status of the read request
 */
cy_rslt_t cyhal_qspi_read_async(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, void *data, size_t *length);

/** Send a command and block of data, synchronously.
 *
 * @param[in]     obj     QSPI object
 * @param[in]     command QSPI command
 * @param[in]     data    TX buffer
 * @param[in,out] length  in - TX buffer length in bytes, out - number of bytes written
 * @return The status of the write request
 */
cy_rslt_t cyhal_qspi_write(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *data, size_t *length);

/** Send a command and block of data in asynchronous mode. This requires __enable_irq() to have been called
 * in order to work. To receive a notification when the read is complete, enable and register a callback
 * using cyhal_qspi_irq_enable and cyhal_qspi_register_irq.
 *
 * @param[in]     obj     QSPI object
 * @param[in]     command QSPI command
 * @param[in]     data    TX buffer
 * @param[in,out] length  in - TX buffer length in bytes, out - number of bytes written
 * @return The status of the write request
 */
cy_rslt_t cyhal_qspi_write_async(cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *data, size_t *length);

/** Send a command (and optionally data) and get the response. Can be used to send/receive device specific commands
 *
 * @param[in]     obj     QSPI object
 * @param[in]     command QSPI command
 * @param[in]     tx_data TX buffer
 * @param[in,out] tx_size in - TX buffer length in bytes, out - bytes actually written
 * @param[out]    rx_data RX buffer
 * @param[in,out] rx_size in - RX buffer length in bytes, out - bytes actually read
 * @return The status of the transfer request
 */
cy_rslt_t cyhal_qspi_transfer(
    cyhal_qspi_t *obj, const cyhal_qspi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data,
    size_t rx_size
);

/** The QSPI event handler registration
 *
 * @param[in] obj         The QSPI object
 * @param[in] callback     The callback handler which will be invoked when the interrupt fires
 * @param[in] callback_arg Generic argument that will be provided to the handler when called
 */
void cyhal_qspi_register_callback(cyhal_qspi_t *obj, cyhal_qspi_event_callback_t callback, void *callback_arg);

/** Configure QSPI interrupt enablement.
 *
 * @param[in] obj           The QSPI object
 * @param[in] event         The QSPI event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_qspi_enable_event(cyhal_qspi_t *obj, cyhal_qspi_event_t event, uint8_t intrPriority, bool enable);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_QSPI_IMPL_HEADER
#include CYHAL_QSPI_IMPL_HEADER
#endif /* CYHAL_QSPI_IMPL_HEADER */

/** \} group_hal_qspi */
