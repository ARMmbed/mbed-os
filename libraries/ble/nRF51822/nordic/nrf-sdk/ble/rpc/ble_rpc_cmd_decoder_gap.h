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
 * @defgroup ble_rpc_cmd_decoder_gap GAP Command Decoder
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief   Decoder for serialized GAP commands from Application Chip.
 *
 * @details This file contains the declaration of the function that decodes the serialized GAP
 *          commands from Application Chip and calls the appropriate BLE stack API.
 */

#ifndef BLE_RPC_CMD_DECODER_GAP_H__
#define BLE_RPC_CMD_DECODER_GAP_H__

#include <stdint.h>

/**@brief Function for processing the encoded GAP command from Application Chip.
 *
 * @details     This function will decode the encoded command and call the appropriate BLE Stack
 *              API. It will then create a Command Response packet with the return value from the
 *              stack API encoded in it and will send it to the transport layer for transmission to
 *              the application controller chip.

 * @param[in]   p_command      The encoded command.
 * @param[in]   op_code        Operation code of the command.
 * @param[in]   command_len    Length of the encoded command.
 *
 * @retval      NRF_SUCCESS    If the decoding of the command was successful, the soft device API
 *                             was called, and the command response was sent to peer, otherwise an
 *                             error code.
 *                             If the transport layer returns an error code while sending
 *                             the Command Response, the same error code will be returned by this
 *                             function (see @ref hci_transport_pkt_write for the list of
 *                             error codes).
 */
uint32_t ble_rpc_cmd_gap_decode(uint8_t * p_command, uint8_t op_code, uint32_t command_len);

#endif // BLE_RPC_CMD_DECODER_GAP_H__

/** @} */
