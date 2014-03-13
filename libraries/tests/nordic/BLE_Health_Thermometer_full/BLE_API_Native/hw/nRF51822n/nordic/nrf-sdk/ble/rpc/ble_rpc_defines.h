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
 * @defgroup rpc_cmd_defines Defines related to serialized BLE commands.
 * @{
 * @ingroup ble_sdk_lib
 *
 * @brief Defines for serialized BLE commands.
 *
 */
 
#ifndef BLE_RPC_DEFINES_H__
#define BLE_RPC_DEFINES_H__

#define RPC_CMD_OP_CODE_POS                     0                      /**< Position of the Op Code in the command buffer.*/
#define RPC_CMD_DATA_POS                        1                      /**< Position of the data in the command buffer.*/

#define RPC_CMD_RESP_PKT_TYPE_POS               0                      /**< Position of Packet type  in the command response buffer.*/
#define RPC_CMD_RESP_OP_CODE_POS                1                      /**< Position of the Op Code in the command response buffer.*/
#define RPC_CMD_RESP_STATUS_POS                 2                      /**< Position of the status field in the command response buffer.*/

#define RPC_BLE_FIELD_LEN                       1                      /**< Optional field length size in bytes. */
#define RPC_BLE_FIELD_PRESENT                   0x01                   /**< Value to indicate that an optional field is encoded in the serialized packet, e.g. white list. */
#define RPC_BLE_FIELD_NOT_PRESENT               0x00                   /**< Value to indicate that an optional field is not encoded in the serialized packet. */

#define RPC_ERR_CODE_SIZE                       4                      /**< BLE API err_code size in bytes. */
#define BLE_PKT_TYPE_SIZE                       1                      /**< Packet type (@ref ble_rpc_pkt_type_t) field size in bytes. */
#define BLE_OP_CODE_SIZE                        1                      /**< Operation code field size in bytes. */

#define RPC_BLE_CMD_RESP_PKT_MIN_SIZE           6                      /**< Minimum length of a command response. */
#define RPC_BLE_PKT_MAX_SIZE                    596                    /**< Maximum size for a BLE packet on the HCI Transport layer. This value is the hci_mem_pool buffer size minus the HCI Transport size. @note This value must be aligned with TX_BUF_SIZE in hci_mem_pool_internal.h. */

/**@brief The types of packets. */
typedef enum
{
    BLE_RPC_PKT_CMD,                                                   /**< Command packet type. */
    BLE_RPC_PKT_RESP,                                                  /**< Command Response packet type. */
    BLE_RPC_PKT_EVT,                                                   /**< Event packet type. */
    BLE_RPC_PKT_TYPE_MAX                                               /**< Upper bound. */
} ble_rpc_pkt_type_t;

#endif // BLE_RPC_DEFINES_H__

/** @} */
