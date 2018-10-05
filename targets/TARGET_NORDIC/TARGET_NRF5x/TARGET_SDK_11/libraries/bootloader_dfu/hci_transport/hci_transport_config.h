/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


/**@file
 *
 * @defgroup ble_sdk_bootloader_hci_congfig HCI Transport Layer Configuration
 * @{
 * @ingroup dfu_bootloader_api
 * @brief Definition of HCI Transport Layer configurable parameters
 */

#ifndef HCI_TRANSPORT_CONFIG_H__
#define HCI_TRANSPORT_CONFIG_H__

#include "boards.h"                                                     /**< Default include for boards.h which means that default pin numbers will be used for RX, TX, CTS, and RTS on the UART. Other pin number can be used if desired. */

/** This section covers configurable parameters for the HCI Transport SLIP layer. */
#define HCI_SLIP_UART_RX_PIN_NUMBER  RX_PIN_NUMBER                      /**< Defines the UART RX pin number. The default pin for the board is chosen, but can be overwritten. */

#define HCI_SLIP_UART_TX_PIN_NUMBER  TX_PIN_NUMBER                      /**< Defines the UART TX pin number. The default pin for the board is chosen, but can be overwritten. */

#define HCI_SLIP_UART_RTS_PIN_NUMBER RTS_PIN_NUMBER                     /**< Defines the UART RTS pin number. The default pin for the board is chosen, but can be overwritten. */

#define HCI_SLIP_UART_CTS_PIN_NUMBER CTS_PIN_NUMBER                     /**< Defines the UART CTS pin number. The default pin for the board is chosen, but can be overwritten. */

#define HCI_SLIP_UART_MODE           APP_UART_FLOW_CONTROL_DISABLED     /**< Defines the UART mode to be used. Use UART Low Power with Flow Control - Valid values are defined in \ref app_uart_flow_control_t. For further information on the UART Low Power mode, please refer to: \ref app_uart . */

#define HCI_SLIP_UART_BAUDRATE       UART_BAUDRATE_BAUDRATE_Baud38400   /**< Defines the UART Baud rate. Default is 38400 baud. */

/** This section covers configurable parameters for the HCI Transport layer that are used for calculating correct value for the retransmission timer timeout. */
#define MAX_PACKET_SIZE_IN_BITS      8000u                              /**< Maximum size of a single application packet in bits. */      
#define USED_BAUD_RATE               38400u                             /**< The used uart baudrate. */

#endif // HCI_TRANSPORT_CONFIG_H__

/** @} */
