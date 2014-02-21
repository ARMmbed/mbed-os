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
 * @defgroup ble_rpc_cmd_encoder Command Encoder
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief   Encoder for serialized commands from Application Chip.
 *
 * @details This file contains the declaration of the functions that encode serialized commands
 *          from Application Chip.
 */

#ifndef BLE_RPC_CMD_ENCODER_H__
#define BLE_RPC_CMD_ENCODER_H__

#include <stdint.h>
#include "ble_rpc_defines.h"

/**@brief Command response type. */
typedef struct
{
    uint8_t  op_code;   /**< Operation code for which this response applies. */
    uint32_t err_code;  /**< Error code received for this response applies. */
} cmd_response_t;

/**@brief Function for initializing the BLE S110 RPC Command Encoder module.
 *
 * @details This function uses the HCI Transport module, \ref hci_transport and executes
 *          \ref hci_transport_tx_done_register and \ref hci_transport_tx_alloc . All errors
 *          returned by those functions are passed on by this function.
 *
 * @retval NRF_SUCCESS      Upon success
 * @return                  Errors from \ref hci_transport  and \ref hci_transport_tx_alloc .
 */
uint32_t ble_rpc_cmd_encoder_init(void);

/**@brief Function for blocking in a loop, using WFE to allow low power mode, while awaiting a
 *        response from the connectivity chip.
 *
 * @param[in] op_code       The Operation Code for which a response message is expected.
 *
 * @return The decoded error code received from the connectivity chip.
 */
uint32_t ble_rpc_cmd_resp_wait(uint8_t op_code);

/**@brief Function for handling the command response packet.
 *
 * @details  This function will be called when a command response is received in the transport
 *           layer. The response is decoded and returned to the waiting caller.
 *
 * @param[in] p_packet      The packet from the transport layer.
 * @param[in] packet_length The length of the packet.
 */
void ble_rpc_cmd_rsp_pkt_received(uint8_t * p_packet, uint16_t packet_length);

#endif  // BLE_RPC_CMD_ENCODER_H__

/**@} */
