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
 * @brief Definition of HCI Transport Layer configurable parameters
 */

#ifndef HCI_TRANSPORT_CFG_H__
#define HCI_TRANSPORT_CFG_H__

/** This section covers configurable parameters for the HCI Transport SLIP layer. */
#define HCI_SLIP_UART_RX_PIN_NUMBER  1                      /**< Defines the UART RX pin number. The default pin for the board is chosen, but can be overwritten. */

#define HCI_SLIP_UART_TX_PIN_NUMBER  2                      /**< Defines the UART TX pin number. The default pin for the board is chosen, but can be overwritten. */

#define HCI_SLIP_UART_RTS_PIN_NUMBER 3                     /**< Defines the UART RTS pin number. The default pin for the board is chosen, but can be overwritten. */

#define HCI_SLIP_UART_CTS_PIN_NUMBER 4                     /**< Defines the UART CTS pin number. The default pin for the board is chosen, but can be overwritten. */

#define HCI_SLIP_UART_MODE           APP_UART_FLOW_CONTROL_ENABLED      /**< Defines the UART mode to be used. Use UART Low Power with Flow Control - Valid values are defined in \ref app_uart_flow_control_t. For further information on the UART Low Power mode, please refer to: \ref app_uart . */

#define HCI_SLIP_UART_BAUDRATE       UART_BAUDRATE_BAUDRATE_Baud38400   /**< Defines the UART Baud rate. Default is 38400 baud. */

/** This section covers configurable parameters for the HCI Transport layer that are used for calculating correct value for the retransmission timer timeout. */
#define MAX_PACKET_SIZE_IN_BITS      8000u                              /**< Maximum size of a single application packet in bits. */      
#define USED_BAUD_RATE               38400u                             /**< The used uart baudrate. */

#endif // HCI_TRANSPORT_CFG_H__

/** @} */
