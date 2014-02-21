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
 * @defgroup ble_rpc_evt_decoder Event Decoder
 * @{
 * @ingroup ble_sdk_lib_serialization
 *
 * @brief   Decoder for serialized events from nRF51822.
 *
 * @details This file contains the declaration of the function that initializes the event decoder
 *          module and processes received events.
 */

#ifndef BLE_RPC_EVT_DECODER_H__
#define BLE_RPC_EVT_DECODER_H__

#include <stdint.h>

#ifndef SVCALL_AS_NORMAL_FUNCTION
#error "The compiler define SVCALL_AS_NORMAL_FUNCTION is not defined."
#endif


/**@brief Function for pushing an encoded packet in the event decoder.
 *
 * @warning This function is not reentrant safe and should always be called from the same interrupt
 *          context.
 *
 * @param[in] p_event_packet       Pointer to the encoded event received.
 * @param[in] event_packet_length  Length of received packet.
 *
 * @retval NRF_SUCCESS       Upon success.
 * @retval NRF_ERROR_NO_MEM  Upon receive queue full.
 */
uint32_t ble_rpc_event_pkt_received(uint8_t * p_event_packet, uint16_t event_packet_length);

#endif // BLE_RPC_EVT_DECODER_H__

/** @} **/
