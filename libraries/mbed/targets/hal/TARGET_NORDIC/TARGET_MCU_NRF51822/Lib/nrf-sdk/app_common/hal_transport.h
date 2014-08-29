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
 
/**@file
 *
 * @defgroup hci_transport HCI Transport
 * @{
 * @ingroup  app_common
 *
 * @brief    HCI transport module implementation.
 *
 * This module implements certain specific features from the three-wire UART transport layer, 
 * defined by the Bluetooth specification version 4.0 [Vol 4] part D.
 *
 * \par Features supported
 * - Transmission and reception of Vendor Specific HCI packet type application packets.
 * - Transmission and reception of reliable packets: defined by chapter 6 of the specification.
 *
 * \par Features not supported
 * - Link establishment procedure: defined by chapter 8 of the specification.
 * - Low power: defined by chapter 9 of the specification. 
 *
 * \par Implementation specific behaviour
 * - As Link establishment procedure is not supported following static link configuration parameters
 * are used:
 * + TX window size is 1.
 * + 16 bit CCITT-CRC must be used.
 * + Out of frame software flow control not supported.
 * + Parameters specific for resending reliable packets are compile time configurable (clarifed 
 * later in this document).
 * + Acknowledgement packet transmissions are not timeout driven , meaning they are delivered for 
 * transmission within same context which the corresponding application packet was received. 
 *
 * \par Implementation specific limitations
 * Current implementation has the following limitations which will have impact to system wide 
 * behaviour:
 * - Delayed acknowledgement scheduling not implemented: 
 * There exists a possibility that acknowledgement TX packet and application TX packet will collide 
 * in the TX pipeline having the end result that acknowledgement packet will be excluded from the TX 
 * pipeline which will trigger the retransmission algorithm within the peer protocol entity.
 * - Delayed retransmission scheduling not implemented:  
 * There exists a possibility that retransmitted application TX packet and acknowledgement TX packet
 * will collide in the TX pipeline having the end result that retransmitted application TX packet 
 * will be excluded from the TX pipeline.
 * - Processing of the acknowledgement number from RX application packets:
 * Acknowledgement number is not processed from the RX application packets having the end result 
 * that unnecessary application packet retransmissions can occur.
 *
 * The application TX packet processing flow is illustrated by the statemachine below. 
 *
 * @image html hci_transport_tx_sm.png "TX - application packet statemachine"
 *
 * \par Component specific configuration options
 *
 * The following compile time configuration options are available, and used to configure the 
 * application TX packet retransmission interval, in order to suite various application specific 
 * implementations:
 * - MAC_PACKET_SIZE_IN_BITS Maximum size of a single application packet in bits. 
 * - USED_BAUD_RATE Used uart baudrate. 
 *
 * The following compile time configuration option is available to configure module specific 
 * behaviour:
 * - MAX_RETRY_COUNT Max retransmission retry count for applicaton packets.
 */
 
#ifndef HCI_TRANSPORT_H__
#define HCI_TRANSPORT_H__

#include <stdint.h>
#include "nrf_error.h"

#define HCI_TRANSPORT_PKT_HEADER_SIZE  (2)   /**< Size of transport packet header */

/**@brief Generic event callback function events. */
typedef enum
{
    HCI_TRANSPORT_RX_RDY,               /**< An event indicating that RX packet is ready for read. */
    HCI_TRANSPORT_EVT_TYPE_MAX          /**< Enumeration upper bound. */  
} hci_transport_evt_type_t;

/**@brief Struct containing events from the Transport layer.
 */
typedef struct
{
    hci_transport_evt_type_t evt_type;  /**< Type of event. */
} hci_transport_evt_t;

/**@brief Transport layer generic event callback function type.
 *
 * @param[in] event                Transport layer event.  
 */
typedef void (*hci_transport_event_handler_t)(hci_transport_evt_t event);

/**@brief TX done event callback function result codes. */
typedef enum
{
    HCI_TRANSPORT_TX_DONE_SUCCESS,      /**< Transmission success, peer transport entity has acknowledged the transmission. */
    HCI_TRANSPORT_TX_DONE_FAILURE       /**< Transmission failure. */    
} hci_transport_tx_done_result_t;

/**@brief Transport layer TX done event callback function type.
 *
 * @param[in] result                TX done event result code. 
 */
typedef void (*hci_transport_tx_done_handler_t)(hci_transport_tx_done_result_t result);

/**@brief Function for registering a generic event handler.
 *
 * @note Multiple registration requests will overwrite any possible existing registration. 
 *
 * @param[in] event_handler         The function to be called by the transport layer upon an event.
 *
 * @retval NRF_SUCCESS              Operation success.
 * @retval NRF_ERROR_NULL           Operation failure. NULL pointer supplied.    
 */
uint32_t hci_transport_evt_handler_reg(hci_transport_event_handler_t event_handler);

/**@brief Function for registering a handler for TX done event.
 *
 * @note Multiple registration requests will overwrite any possible existing registration. 
 *
 * @param[in] event_handler         The function to be called by the transport layer upon TX done
 *                                  event.
 *
 * @retval NRF_SUCCESS              Operation success.
 * @retval NRF_ERROR_NULL           Operation failure. NULL pointer supplied.    
 */
uint32_t hci_transport_tx_done_register(hci_transport_tx_done_handler_t event_handler);
                                        
/**@brief Function for opening the transport channel and initializing the transport layer.
 *
 * @warning Must not be called for a channel which has been allready opened. 
 * 
 * @retval NRF_SUCCESS              Operation success.
 * @retval NRF_ERROR_INTERNAL       Operation failure. Internal error ocurred. 
 */
uint32_t hci_transport_open(void);

/**@brief Function for closing the transport channel.
 *
 * @note Can be called multiple times and also for not opened channel.
 * 
 * @retval NRF_SUCCESS              Operation success.  
 */
uint32_t hci_transport_close(void);

/**@brief Function for allocating tx packet memory.
 * 
 * @param[out] pp_memory            Pointer to the packet data.
 * 
 * @retval NRF_SUCCESS              Operation success. Memory was allocated.
 * @retval NRF_ERROR_NO_MEM         Operation failure. No memory available.
 * @retval NRF_ERROR_NULL           Operation failure. NULL pointer supplied.   
 */
uint32_t hci_transport_tx_alloc(uint8_t ** pp_memory);

/**@brief Function for freeing tx packet memory.
 *
 * @note Memory management works in FIFO principle meaning that free order must match the alloc 
 *       order.
 * 
 * @retval NRF_SUCCESS              Operation success. Memory was freed.   
 */
uint32_t hci_transport_tx_free(void);

/**@brief Function for writing a packet.
 *
 * @note Completion of this method does not guarantee that actual peripheral transmission would 
 *       have completed. 
 *
 * @note In case of 0 byte packet length write request, message will consist of only transport 
 *       module specific headers.  
 *
 * @note The buffer provided to this function must be allocated through @ref hci_transport_tx_alloc
 *       function.
 *
 * @retval NRF_SUCCESS              Operation success. Packet was added to the transmission queue 
 *                                  and an event will be send upon transmission completion. 
 * @retval NRF_ERROR_NO_MEM         Operation failure. Transmission queue is full and packet was not
 *                                  added to the transmission queue. User should wait for 
 *                                  a appropriate event prior issuing this operation again.
 * @retval NRF_ERROR_DATA_SIZE      Operation failure. Packet size exceeds limit.
 * @retval NRF_ERROR_NULL           Operation failure. NULL pointer supplied.  
 * @retval NRF_ERROR_INVALID_STATE  Operation failure. Channel is not open.
 * @retval NRF_ERROR_INVALID_ADDR   Operation failure. Buffer provided is not allocated through
 *                                  hci_transport_tx_alloc function.
 */
uint32_t hci_transport_pkt_write(const uint8_t * p_buffer, uint16_t length);

/**@brief Function for extracting received packet.
 *
 * @note Extracted memory can't be reused by the underlying transport layer untill freed by call to 
 *       hci_transport_rx_pkt_consume().
 *
 * @param[out] pp_buffer            Pointer to the packet data.
 * @param[out] p_length             Length of packet data in bytes.  
 *
 * @retval NRF_SUCCESS              Operation success. Packet was extracted. 
 * @retval NRF_ERROR_NO_MEM         Operation failure. No packet available to extract.
 * @retval NRF_ERROR_NULL           Operation failure. NULL pointer supplied.   
 */
uint32_t hci_transport_rx_pkt_extract(uint8_t ** pp_buffer, uint16_t * p_length);

/**@brief Function for consuming extracted packet described by p_buffer.
 *
 * RX memory pointed to by p_buffer is freed and can be reused by the underlying transport layer.
 *
 * @param[in] p_buffer              Pointer to the buffer that has been consumed.
 *
 * @retval NRF_SUCCESS              Operation success. 
 * @retval NRF_ERROR_NO_MEM         Operation failure. No packet available to consume. 
 * @retval NRF_ERROR_INVALID_ADDR   Operation failure. Not a valid pointer. 
 */
uint32_t hci_transport_rx_pkt_consume(uint8_t * p_buffer);

#endif // HCI_TRANSPORT_H__

/** @} */
