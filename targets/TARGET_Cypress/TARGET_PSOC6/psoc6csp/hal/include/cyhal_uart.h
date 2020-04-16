/***************************************************************************//**
* \file cyhal_uart.h
*
* \brief
* Provides a high level interface for interacting with the Cypress UART.
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
* \addtogroup group_hal_uart UART (Universal Asynchronous Receiver-Transmitter)
* \ingroup group_hal
* \{
* High level interface for interacting with the Universal Asynchronous Receiver-Transmitter (UART).
*
* The Universal Asynchronous Receiver/Transmitter (UART) protocol is an
* asynchronous serial interface protocol. UART communication is typically
* point-to-point. The UART interface consists of two signals:
* * TX: Transmitter output
* * RX: Receiver input
*
* Additionally, two side-band signals are used to implement flow control in
* UART. Note that the flow control applies only to TX functionality.
* * Clear to Send (CTS): This is an input signal to the transmitter.
*   When active, it indicates that the slave is ready for the master to
*   transmit data.
* * Ready to Send (RTS): This is an output signal from the receiver. When
*   active, it indicates that the receiver is ready to receive data
*
* Flow control can be configured via cyhal_uart_set_flow_control()
*
* The data frame size, STOP bits, and parity can be configured via cyhal_uart_cfg_t.
* The UART contains dedicated hardware buffers for transmit and receive. Optionally,
* either these can be augmented with a software buffer.
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

/** The requested resource type is invalid */
#define CYHAL_UART_RSLT_ERR_INVALID_PIN (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_UART, 0))
/** Failed to configure power management callback */
#define CYHAL_UART_RSLT_ERR_PM_CALLBACK (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_UART, 1))
/** The getc call timed out with no received data */
#define CY_RSLT_ERR_CSP_UART_GETC_TIMEOUT (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_UART, 2))
/** The actual baud rate is greater than 10% off the requested baud rate */
#define CY_RSLT_WRN_CSP_UART_BAUD_TOLERANCE (CY_RSLT_CREATE(CY_RSLT_TYPE_WARNING, CYHAL_RSLT_MODULE_UART, 3))
/** The baud rate to set to if no clock is specified in the init function */
#define CYHAL_UART_DEFAULT_BAUD 115200
/** The maximum allowable difference between baud requested and actual baud **/
#define CYHAL_UART_MAX_BAUD_PERCENT_DIFFERENCE 10

/** UART Parity */
typedef enum
{
    CYHAL_UART_PARITY_NONE,   /**< UART has no parity check   */
    CYHAL_UART_PARITY_EVEN,   /**< UART has even parity check */
    CYHAL_UART_PARITY_ODD,    /**< UART has odd parity check  */
} cyhal_uart_parity_t;

/** Enum to enable/disable/report interrupt cause flags. */
typedef enum
{
    CYHAL_UART_IRQ_NONE                = 0, //!< No interrupt
    CYHAL_UART_IRQ_TX_TRANSMIT_IN_FIFO = 1 << 1, //!< All tx data from transmit has been moved to uart FIFO
    CYHAL_UART_IRQ_TX_DONE             = 1 << 2, //!< All tx data has been transmitted
    CYHAL_UART_IRQ_TX_ERROR            = 1 << 3, //!< An error occurred in tx
    CYHAL_UART_IRQ_RX_FULL             = 1 << 4, //!< The rx software buffer is full, additional data are store into fifo buffer.
    CYHAL_UART_IRQ_RX_DONE             = 1 << 5, //!< All rx data has been received
    CYHAL_UART_IRQ_RX_ERROR            = 1 << 6, //!< An error occurred in rx
    CYHAL_UART_IRQ_RX_NOT_EMPTY        = 1 << 7, //!< The rx hardware buffer is not empty
    CYHAL_UART_IRQ_TX_EMPTY            = 1 << 8, //!< The tx hardware buffer is empty
} cyhal_uart_event_t;

/** @brief Initial UART configuration */
typedef struct
{
    uint32_t data_bits; //!< The number of start bits
    uint32_t stop_bits; //!< The number of stop bits
    cyhal_uart_parity_t parity; //!< The parity
    uint8_t *rx_buffer; //!< The rx software buffer pointer, if NULL, no rx software buffer will be used
    uint32_t rx_buffer_size; //!< The number of bytes in the rx software buffer
} cyhal_uart_cfg_t;

/** UART callback function type */
typedef void (*cyhal_uart_event_callback_t)(void *callback_arg, cyhal_uart_event_t event);

/** Initialize the uart peripheral. It sets the default parameters for uart
 *  peripheral, and configures its specifieds pins.
 *
 * @param[out] obj The uart object
 * @param[in]  tx  The TX pin name, if no TX pin use NC
 * @param[in]  rx  The RX pin name, if no RX pin use NC
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be,
 *                  allocated and the default baud rate set
 * @param[in]  cfg The uart configuration data for data bits, stop bits and parity,
 *                  if not provided, default values of (8, 1, none) will be used
 * @return The status of the init request
 */
cy_rslt_t cyhal_uart_init(cyhal_uart_t *obj, cyhal_gpio_t tx, cyhal_gpio_t rx, const cyhal_clock_divider_t *clk, const cyhal_uart_cfg_t *cfg);

/** Release the uart peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The uart object
 */
void cyhal_uart_free(cyhal_uart_t *obj);

/** Configure the baud rate
 *
 * @param[in,out] obj      The uart object
 * @param[in]     baudrate The baud rate to be configured
 * @param[out]    actualbaud The actual baud rate achieved by the HAL
 *                Specify NULL if you do not want this information.
 * @return The status of the set_baud request
 */
cy_rslt_t cyhal_uart_set_baud(cyhal_uart_t *obj, uint32_t baudrate, uint32_t *actualbaud);

/** Configure the data bits, stop bits, and parity
 *
 * @param[in,out] obj      The uart object
 * @param[in]     cfg      The uart configuration data for data bits, stop bits and parity.
 *                         rx_buffer and rx_buffer_size are ignored.
 * @return The status of the configure request
 */
cy_rslt_t cyhal_uart_configure(cyhal_uart_t *obj, const cyhal_uart_cfg_t *cfg);

/** Get character. This is a blocking call, waiting for a character
 *
 * @param[in] obj    The uart object
 * @param[out] value The value read from the serial port
 * @param[in] timeout The time in ms to spend attempting to receive from serial port
 *                    timeout = zero is wait forever
 * @return The status of the getc request
 */
cy_rslt_t cyhal_uart_getc(cyhal_uart_t *obj, uint8_t *value, uint32_t timeout);

/** Send a character. This is a blocking call, waiting for the peripheral to be available
 *  for writing
 *
 * @param[in] obj The uart object
 * @param[in] value The character to be sent
 * @return The status of the putc request
 */
cy_rslt_t cyhal_uart_putc(cyhal_uart_t *obj, uint32_t value);

/** Check the number of bytes avaialable to read from the receive buffers
 *
 * @param[in]  obj      The uart object
 * @return The number of readable bytes
 */
uint32_t cyhal_uart_readable(cyhal_uart_t *obj);

/** Check the number of bytes than can be written to the transmit buffer
 *
 * @param[in]  obj      The uart object
 * @return The number of bytes that can be written
 */
uint32_t cyhal_uart_writable(cyhal_uart_t *obj);

/** Clear the uart peripheral buffers
 *
 * @param[in] obj The uart object
 * @return The status of the clear request
 */
cy_rslt_t cyhal_uart_clear(cyhal_uart_t *obj);

/** Configure the uart for the flow control. It sets flow control in the hardware
 *  if a uart peripheral supports it, otherwise software emulation is used.
 *
 * @param[in,out] obj    The uart object
 * @param[in]     cts    The TX pin name
 * @param[in]     rts    The RX pin name
 * @return The status of the init_flow_control request
 */
cy_rslt_t cyhal_uart_set_flow_control(cyhal_uart_t *obj, cyhal_gpio_t cts, cyhal_gpio_t rts);

/** Begin synchronous TX transfer. The used buffer is specified in the uart object,
 *  tx_buff
 *
 * @param[in]     obj        The uart object
 * @param[in]     tx         The transmit buffer
 * @param[in,out] tx_length  [in] The number of bytes to transmit, [out] number actually transmitted
 * @return The status of the tx request
 */
cy_rslt_t cyhal_uart_write(cyhal_uart_t *obj, void *tx, size_t *tx_length);

/** Begin synchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the uart object - rx_buff
 *
 * @param[in]     obj       The uart object
 * @param[in]     rx        The receive buffer
 * @param[in,out] rx_length [in] The number of bytes to receive, [out] number actually received
 * @return The status of the rx request
 */
cy_rslt_t cyhal_uart_read(cyhal_uart_t *obj, void *rx, size_t *rx_length);

/** Begin asynchronous TX transfer. The transmit buffer is a user defined buffer that will be
 *  sent on the uart. The user must register a callback with cyhal_uart_irq_register_callback. If
 *  desired, TX callback events can be enabled using cyhal_uart_enable_event with the appropriate
 *  events.
 *
 * @param[in] obj     The uart object
 * @param[in] tx      The transmit buffer
 * @param[in] length  The number of bytes to transmit
 * @return The status of the tx_async request
 */
cy_rslt_t cyhal_uart_write_async(cyhal_uart_t *obj, void *tx, size_t length);

/** Begin asynchronous RX transfer. Recevied data is placed in the user specified buffer.
 *  The user must register a callback with cyhal_uart_irq_register_callback. RX callback events
 *  can be enabled using cyhal_uart_enable_event with the appropriate events.
 *
 * @param[in]  obj     The uart object
 * @param[out] rx      The user specified receive buffer
 * @param[in]  length  The number of bytes to receive
 * @return The status of the rx_async request
 */
cy_rslt_t cyhal_uart_read_async(cyhal_uart_t *obj, void *rx, size_t length);

/** Attempts to determine if the uart peripheral is already in use for TX
 *
 * @param[in]  obj    The uart object
 * @return Is the TX channel active
 */
bool cyhal_uart_is_tx_active(cyhal_uart_t *obj);

/** Attempts to determine if the uart peripheral is already in use for RX
 *
 * @param[in]  obj    The uart object
 * @return Is the RX channel active
 */
bool cyhal_uart_is_rx_active(cyhal_uart_t *obj);

/** Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 *  flushes the TX hardware buffer if TX FIFO is used
 *
 * @param[in] obj The uart object
 * @return The status of the tx_abort request
 */
cy_rslt_t cyhal_uart_write_abort(cyhal_uart_t *obj);

/** Abort the ongoing read transaction. It disables the enabled interrupt for RX and
 *  flushes the RX hardware buffer if RX FIFO is used
 *
 * @param[in] obj The uart object
 * @return The status of the read_abort request
 */
cy_rslt_t cyhal_uart_read_abort(cyhal_uart_t *obj);

/** The uart callback handler registration
 *
 * @param[in] obj          The uart object
 * @param[in] callback     The callback handler which will be invoked when the interrupt fires
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_uart_register_callback(cyhal_uart_t *obj, cyhal_uart_event_callback_t callback, void *callback_arg);

/** Configure uart interrupt. This function is used for word-approach
 *
 * @param[in] obj           The uart object
 * @param[in] event         The uart event type, this argument supports the bitwise-or of multiple enum flag values
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_uart_enable_event(cyhal_uart_t *obj, cyhal_uart_event_t event, uint8_t intrPriority, bool enable);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_UART_IMPL_HEADER
#include CYHAL_UART_IMPL_HEADER
#endif /* CYHAL_UART_IMPL_HEADER */

/** \} group_hal_uart */
