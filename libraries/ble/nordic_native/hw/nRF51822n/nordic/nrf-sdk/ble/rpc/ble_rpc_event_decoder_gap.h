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
 * @defgroup ble_rpc_evt_decoder_gap GAP Event Decoder
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief   Decoder for serialized GAP events from nRF51822.
 *
 * @details This file contains declarations of functions used for decoding GAP event packets
 *          received from the Connectivity Chip.
 *
 */

#ifndef BLE_RPC_EVENT_DECODER_GAP_H__
#define BLE_RPC_EVENT_DECODER_GAP_H__

#include <stdint.h>
#include "ble.h"

/** @brief Function for decoding the length of a BLE GAP event.
 *
 * @param[in]   event_id        Event Id for the event to length decode.
 * @param[out]  p_event_length  The pointer for storing the decoded event length.
 */
void ble_rpc_gap_evt_length_decode(uint8_t event_id, uint16_t * p_event_length);

/** @brief Function for decoding a BLE GAP event.
 *
 * @param[out]  p_ble_evt       The pointer for storing the decoded event.
 * @param[in]   p_packet        The pointer to the encoded event.
 */
void ble_rpc_gap_evt_packet_decode(ble_evt_t *           p_ble_evt,
                                   uint8_t const * const p_packet);

#endif // BLE_RPC_EVENT_DECODER_GAP_H__

/** @} **/
