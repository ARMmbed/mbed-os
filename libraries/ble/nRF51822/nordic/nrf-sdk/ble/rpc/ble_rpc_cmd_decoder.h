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
 * @defgroup ble_rpc_cmd_decoder Command Decoder
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief   Decoder for serialized commands from Application Chip.
 *
 * @details This file contains declaration of common functions used for sending responses back to
 *          Application Chip after the command is processed, and function for processing commands
 *          received by the transport layer.
 */

#ifndef BLE_RPC_CMD_DECODER_H__
#define BLE_RPC_CMD_DECODER_H__

#include <stdint.h>

#define RPC_DECODER_LENGTH_CHECK(LEN, INDEX, CMD) if ( INDEX > LEN) \
        return ble_rpc_cmd_resp_send(CMD, NRF_ERROR_INVALID_LENGTH);

/**@brief Function for sending a Command Response packet to the Application Chip through the transport
 *        layer.
 *
 * @param[in] op_code          The op code of the command for which the Command Response is sent.
 * @param[in] status           The status field to be encoded into the Command Response.
 *
 * @retval NRF_SUCCESS         On successful write of Command Response, otherwise an error code.
 *                             If the transport layer returns an error code while sending
 *                             the Command Response, the same error code will be returned by this
 *                             function (see @ref hci_transport_pkt_write for the list of
 *                             error codes).
 */
uint32_t ble_rpc_cmd_resp_send(uint8_t op_code, uint32_t status);

/**@brief Function for sending a command response with additional data to the Application Chip through
 *        the transport layer.
 *
 * @param[in]   op_code        The op code of the command for which the Command Response is sent.
 * @param[in]   status         The status field to be encoded into the Command Response.
 * @param[in]   p_data         The data to be sent along with the status.
 * @param[in]   data_len       The length of the additional data.
 *
 * @retval      NRF_SUCCESS    On successful write of Command Response, otherwise an error code.
 *                             If the transport layer returns an error code while sending
 *                             the Command Response, the same error code will be returned by this
 *                             function (see @ref hci_transport_pkt_write for the list of
 *                             error codes).
 */
uint32_t ble_rpc_cmd_resp_data_send(uint8_t               op_code,
                                    uint8_t               status,
                                    const uint8_t * const p_data,
                                    uint16_t              data_len);

/**@brief Function for scheduling an RPC command event to be processed in main-thread.
 *
 * @details     The function will read the arrived packet from the transport layer
 *              which is passed for decoding by the rpc_cmd_decoder module.
 *
 * @param[in]   p_event_data   Event data. This will be NULL as rpc_evt_schedule
 *                             does not set any data.
 * @param[in]   event_size     Event data size. This will be 0 as rpc_evt_schedule
 *                             does not set any data.
 */
void ble_rpc_cmd_handle(void * p_event_data, uint16_t event_size);

#endif // BLE_RPC_CMD_DECODER_H__

/** @} */
