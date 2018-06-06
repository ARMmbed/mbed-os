/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * \file etx.h
 * \brief Expected transmission count (ETX metric), module
 *
 *
 */

#ifndef ETX_H_
#define ETX_H_

#include "NWK_INTERFACE/Include/protocol_abstract.h"

struct mle_neigh_table_entry_t;
/* Fraction that is used when calculating moving average
   e.g. ETX = 7/8 * current ETX + 1/8 * new ETX sample
   Range for value can be from 1 to 11 */
#define ETX_MOVING_AVERAGE_FRACTION      3     // n >> 3, 1/8

/**
 * \brief A function to update ETX value based on transmission attempts
 *
 *  Update is made based on failed and successful message sending
 *  attempts for a message.
 *
 * \param interface_id Interface identifier
 * \param attempts number of attempts to send message
 * \param success was message sending successful
 * \param addr_type address type, ADDR_802_15_4_SHORT or ADDR_802_15_4_LONG
 * \param addr_ptr PAN ID with 802.15.4 address
 */
void etx_transm_attempts_update(int8_t interface_id, uint8_t attempts, bool success, addrtype_t addr_type, const uint8_t *addr_ptr);

/**
 * \brief A function to update ETX value based on remote incoming IDR
 *
 *  Update is made based on remote incoming IDR received from
 *  neighbor.
 *
 * \param interface_id Interface identifier
 * \param remote_incoming_idr Remote incoming IDR
 * \param mac64_addr_ptr long MAC address
 */
void etx_remote_incoming_idr_update(int8_t interface_id, uint8_t remote_incoming_idr, struct mle_neigh_table_entry_t *neigh_table_ptr);

/**
 * \brief A function to read ETX value
 *
 *  Returns ETX value for an address
 *
 * \param interface_id network interface id
 * \param addr_type address type, ADDR_802_15_4_SHORT or ADDR_802_15_4_LONG
 * \param addr_ptr PAN ID with 802.15.4 address
 *
 * \return 0x0100 to 0xFFFF ETX value (8 bit fraction)
 * \return 0xFFFF address not associated
 * \return 0x0000 address unknown or other error
 * \return 0x0001 no ETX statistics on this interface
 */
uint16_t etx_read(int8_t interface_id, addrtype_t addr_type, const uint8_t *addr_ptr);

/**
 * \brief A function to read local incoming IDR value
 *
 *  Returns local incoming IDR value for an address
 *
 * \param mac64_addr_ptr long MAC address
 *
 * \return 0x0100 to 0xFFFF incoming IDR value (8 bit fraction)
 * \return 0x0000 address unknown
 */
uint16_t etx_local_incoming_idr_read(int8_t interface_id, struct mle_neigh_table_entry_t *neigh_table_ptr);

/**
 * \brief A function to update ETX value based on LQI and dBm
 *
 *  Update is made based on dBM and LQI of received message.
 *
 * \param lqi link quality indicator
 * \param dbm measured dBm
 * \param mac64_addr_ptr long MAC address
 *
 * \return 0x0100 to 0xFFFF local incoming IDR value (8 bit fraction)
 */
uint16_t etx_lqi_dbm_update(int8_t interface_id, uint8_t lqi, int8_t dbm, struct mle_neigh_table_entry_t *neigh_table_ptr);

/**
 * \brief A function callback that indicates ETX value change
 *
 *  Callback indicates when ETX value has changed more or equal to
 *  hysteresis value.
 *
 * \param nwk_interface_id network interface id
 * \param previous_etx ETX value to what the current ETX was compared (8 bit fraction)
 * \param current_etx current ETX value (8 bit fraction)
 * \param mac64_addr_ptr long MAC address
 * \param mac16_addr short MAC address or 0xffff address is not set
 *
 */
typedef void (etx_value_change_handler_t)(int8_t nwk_id, uint16_t previous_etx, uint16_t current_etx, const uint8_t *mac64_addr_ptr, uint16_t mac16_addr);

/**
 * \brief A function callback that indicates the number of accumulated TX failures
 *
 * Callback indicates when number of accumulated failures is more or equal to threshold value.
 *
 * \param interface_id interface ID
 * \param accumulated_failures number of accumulated failures
 * \param neigh_table_ptr the neighbor node in question
 *
 */
typedef void (etx_accum_failures_handler_t)(int8_t interface_id, uint8_t accumulated_failures, struct mle_neigh_table_entry_t *neigh_table_ptr);

/**
 * \brief A function to register ETX value change callback
 *
 *  When ETX value has changed more or equal to hysteresis value ETX
 *  module calls ETX value change callback.
 *
 * \param nwk_interface_id network interface id
 * \param hysteresis hysteresis value (8 bit fraction)
 * \param callback_ptr callback function pointer
 *
 * \return 0 not 6LowPAN interface
 * \return 1 success
 */
uint8_t etx_value_change_callback_register(nwk_interface_id nwk_id,int8_t interface_id, uint16_t hysteresis, etx_value_change_handler_t *callback_ptr);

/**
 * \brief A function to register accumulated failures callback
 *
 *  When the number of accumulated failures has reached the threshold
 *  value, the ETX module calls the accumulated failures callback on
 *  every transmission failure.
 *
 * \param nwk_id network ID (6LoWPAN)
 * \param interface_id interface ID
 * \param threshold threshold value for accumulated failures
 * \param callback_ptr callback function pointer
 *
 * \return 0 not 6LowPAN interface
 * \return 1 success
 */
uint8_t etx_accum_failures_callback_register(nwk_interface_id nwk_id, int8_t interface_id, uint8_t threshold, etx_accum_failures_handler_t *callback_ptr);

/**
 * \brief A function to remove ETX neighbor
 *
 *  Notifies ETX module that neighbor has been removed. Calls ETX value change callback
 *  if that is set.
 *
 * \param mac64_addr_ptr long MAC address
 *
 */
void etx_neighbor_remove(int8_t interface_id, struct mle_neigh_table_entry_t *neigh_table_ptr);

/**
 * \brief A function to add ETX neighbor
 *
 *  Notifies ETX module that neighbor has been added. Calls ETX value change callback
 *  if that is set.
 *
 * \param mac64_addr_ptr long MAC address
 *
 */
void etx_neighbor_add(int8_t interface_id, struct mle_neigh_table_entry_t *neigh_table_ptr);

#endif /* ETX_H_ */
