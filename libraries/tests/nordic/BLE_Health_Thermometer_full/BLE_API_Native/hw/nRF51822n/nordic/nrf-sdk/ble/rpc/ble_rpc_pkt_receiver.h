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
 * @defgroup rpc_pkt_receiver Packet Receiver
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief   This module is used for processing the Command Response packets and Event packets.
 */

#ifndef BLE_RPC_PKT_RECEIVER_H__
#define BLE_RPC_PKT_RECEIVER_H__

#include <stdint.h>

/**@brief Function for initializing the BLE S110 RPC Packet Receiver module.
 *
 * @return NRF_SUCCESS upon success, any other upon failure.
 */
uint32_t ble_rpc_pkt_receiver_init(void);

#endif

/** @} **/
