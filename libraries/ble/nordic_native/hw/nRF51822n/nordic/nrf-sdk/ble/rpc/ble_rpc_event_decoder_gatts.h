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
 * @defgroup ble_rpc_evt_decoder_gatts GATTS Event Decoder
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief   Decoder for serialized GATT Server events from nRF51822.
 *
 * @details This file contains declarations of functions used for decoding GATTS event packets
 *          received from the Connectivity Chip.
 *
 */

#ifndef BLE_RPC_EVENT_DECODER_GATTS_H__
#define BLE_RPC_EVENT_DECODER_GATTS_H__

#include <stdint.h>
#include "ble.h"
#include "ble_gatts.h"

/** @brief Function for decoding the length of a BLE GATTS event. The decoded BLE GATTS event
 *         length will be returned in p_event_length.
 *
 * @param[in]   event_id        Event Id of the event, whose length is to be decoded.
 * @param[out]  p_event_length  The pointer for storing the decoded event length.
 * @param[in]   p_packet        The pointer to the encoded event.
 */
void ble_rpc_gatts_evt_length_decode(uint8_t event_id, 
                                     uint16_t * p_event_length,
                                     uint8_t const * const p_packet);

/** @brief Function for decoding a BLE GATTS event. The decoded BLE GATTS event will be returned in
 *         the memory pointed to by p_ble_evt.
 *
 * @param[out]  p_ble_evt       The pointer for storing the decoded event.
 * @param[in]   p_packet        The pointer to the encoded event.
 */
void ble_rpc_gatts_evt_packet_decode(ble_evt_t * p_ble_evt,
                                     uint8_t const * const p_packet);

#endif // BLE_RPC_EVENT_DECODER_GATTS_H__

/** @} **/
