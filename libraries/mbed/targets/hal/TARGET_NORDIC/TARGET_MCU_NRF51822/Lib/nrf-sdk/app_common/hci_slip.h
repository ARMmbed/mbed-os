/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
 
/** @file
 *
 * @defgroup hci_slip SLIP module
 * @{
 * @ingroup app_common
 *
 * @brief SLIP layer for supporting packet framing in HCI transport.
 *
 * @details This module implements SLIP packet framing as described in the Bluetooth Core
 *          Specification 4.0, Volume 4, Part D, Chapter 3 SLIP Layer.
 *
 *          SLIP framing ensures that all packets sent on the UART are framed as:
 *          <0xC0> SLIP packet 1 <0xC0> <0xC0> SLIP packet 2 <0xC0>.
 *
 *          The SLIP layer uses events to notify the upper layer when data transmission is complete
 *          and when a SLIP packet is received.
 */

#ifndef HCI_SLIP_H__
#define HCI_SLIP_H__

#include <stdint.h>

/**@brief Event types from the SLIP Layer. */
typedef enum
{
    HCI_SLIP_RX_RDY,                        /**< An event indicating that an RX packet is ready to be read. */
    HCI_SLIP_TX_DONE,                       /**< An event indicating write completion of the TX packet provided in the function call \ref hci_slip_write . */
    HCI_SLIP_RX_OVERFLOW,                   /**< An event indicating that RX data has been discarded due to lack of free RX memory. */
    HCI_SLIP_ERROR,                         /**< An event indicating that an unrecoverable error has occurred. */
    HCI_SLIP_EVT_TYPE_MAX                   /**< Enumeration upper bound. */
} hci_slip_evt_type_t;

/**@brief Structure containing an event from the SLIP layer.
 */
typedef struct
{
    hci_slip_evt_type_t evt_type;           /**< Type of event. */
    const uint8_t *     packet;             /**< This field contains a pointer to the packet for which the event relates, i.e. SLIP_TX_DONE: the packet transmitted, SLIP_RX_RDY: the packet received, SLIP_RX_OVERFLOW: The packet which overflow/or NULL if no receive buffer is available. */
    uint32_t            packet_length;      /**< Packet length, i.e. SLIP_TX_DONE: Bytes transmitted, SLIP_RX_RDY: Bytes received, SLIP_RX_OVERFLOW: index at which the packet overflowed. */
} hci_slip_evt_t;

/**@brief Function for the SLIP layer event callback.
 */
typedef void (*hci_slip_event_handler_t)(hci_slip_evt_t event);

/**@brief Function for registering the event handler provided as parameter and this event handler
 *        will be used by SLIP layer to send events described in \ref hci_slip_evt_type_t.
 *
 * @note Multiple registration requests will overwrite any existing registration. 
 *
 * @param[in] event_handler         This function is called by the SLIP layer upon an event.
 *
 * @retval NRF_SUCCESS              Operation success.
 */
uint32_t hci_slip_evt_handler_register(hci_slip_event_handler_t event_handler);

/**@brief Function for opening the SLIP layer. This function must be called before
 *        \ref hci_slip_write and before any data can be received.
 *
 * @note Can be called multiple times. 
 * 
 * @retval NRF_SUCCESS              Operation success.
 *
 * The SLIP layer module will propagate errors from underlying sub-modules.
 * This implementation is using UART module as a physical transmission layer, and hci_slip_open
 * executes \ref app_uart_init . For an extended error list, please refer to \ref app_uart_init .
 */
uint32_t hci_slip_open(void);

/**@brief Function for closing the SLIP layer. After this function is called no data can be
 *        transmitted or received in this layer.
 *
 * @note This function can be called multiple times and also for an unopened channel.
 * 
 * @retval NRF_SUCCESS              Operation success.  
 */
uint32_t hci_slip_close(void);

/**@brief Function for writing a packet with SLIP encoding. Packet transmission is confirmed when
 *        the HCI_SLIP_TX_DONE event is received by the function caller.
 *
 * @param[in] p_buffer              Pointer to the packet to transmit.
 * @param[in] length                Packet length, in bytes.
 *
 * @retval NRF_SUCCESS              Operation success. Packet was encoded and added to the 
 *                                  transmission queue and an event will be sent upon transmission 
 *                                  completion.
 * @retval NRF_ERROR_NO_MEM         Operation failure. Transmission queue is full and packet was not
 *                                  added to the transmission queue. Application shall wait for
 *                                  the \ref HCI_SLIP_TX_DONE event. After HCI_SLIP_TX_DONE this
 *                                  function can be executed for transmission of next packet.
 * @retval NRF_ERROR_INVALID_ADDR   If a NULL pointer is provided.
 * @retval NRF_ERROR_INVALID_STATE  Operation failure. Module is not open.
 */
uint32_t hci_slip_write(const uint8_t * p_buffer, uint32_t length);

/**@brief Function for registering a receive buffer. The receive buffer will be used for storage of
 *        received and SLIP decoded data.
 *        No data can be received by the SLIP layer until a receive buffer has been registered.
 *
 * @note  The lifetime of the buffer must be valid during complete reception of data. A static
 *        buffer is recommended.
 *
 * @warning Multiple registration requests will overwrite any existing registration.
 *
 * @param[in]  p_buffer             Pointer to receive buffer. The received and SLIP decoded packet
 *                                  will be placed in this buffer.
 * @param[in]  length               Buffer length, in bytes.
 *
 * @retval NRF_SUCCESS              Operation success. 
 */
uint32_t hci_slip_rx_buffer_register(uint8_t * p_buffer, uint32_t length);
 
#endif // HCI_SLIP_H__
 
/** @} */
