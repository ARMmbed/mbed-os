/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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

/* Fraction that is used when calculating moving average
   e.g. ETX = 7/8 * current ETX + 1/8 * new ETX sample
   Range for value can be from 1 to 11 */
#define ETX_MOVING_AVERAGE_FRACTION      3     // n >> 3, 1/8

/* ETX cache configuration definitions */

/* Amount of accelerated ETX samples
 * After this value is received the ETX calculation starts to follow the
 * slower ETX cache configuration values.
 *
 * Maximum value is 6
 */
#define ETX_ACCELERATED_SAMPLE_COUNT      3

/* Amount of samples before updating the ETX value when in accelerated mode.
 *
 * if set to 1 every sample updates an ETX value. if set to 2 every second
 * ETX sample updates the ETX value.
 *
 * This value should be smaller than ETX_ACCELERATED_SAMPLE_COUNT
 */
#define ETX_ACCELERATED_INTERVAL          2

typedef struct etx_storage_s {
    uint16_t        etx;                       /*!< 12 bits fraction */
    uint16_t        stored_diff_etx;           /*!< 12 bits fraction */
    uint8_t         remote_incoming_idr;       /*!< 5 bits fraction */
    unsigned        accumulated_failures: 5;
    unsigned        tmp_etx: 1;
    unsigned        linkIdr: 4;
    unsigned        etx_samples: 3;
} etx_storage_t;

typedef struct etx_sample_storage_s {
    uint16_t           attempts_count;         /*!< TX attempt count */
    uint8_t            etx_timer;              /*!< Count down from configured value 0 means that ETX Update is possible done again*/
    uint8_t            received_acks;          /*!< Received ACK's */
    uint8_t            sample_count;           /*!< Finished TX count */
} etx_sample_storage_t;

/**
 * \brief A function to update ETX value based on transmission attempts
 *
 *  Update is made based on failed and successful message sending
 *  attempts for a message.
 *
 * \param interface_id Interface identifier
 * \param attempts number of attempts to send message
 * \param success was message sending successful
 * \param attribute_index Neighbour attribute index
 */
void etx_transm_attempts_update(int8_t interface_id, uint8_t attempts, bool success, uint8_t attribute_index);

/**
 * \brief A function to update ETX value based on remote incoming IDR
 *
 *  Update is made based on remote incoming IDR received from
 *  neighbor.
 *
 * \param interface_id Interface identifier
 * \param remote_incoming_idr Remote incoming IDR
 * \param attribute_index Neighbour attribute index
 */
void etx_remote_incoming_idr_update(int8_t interface_id, uint8_t remote_incoming_idr, uint8_t attribute_index);

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
 *  Returns local incoming IDR value for an neighbour
 *
 * \param attribute_index Neighbour attribute index
 *
 * \return 0x0100 to 0xFFFF incoming IDR value (8 bit fraction)
 * \return 0x0000 address unknown
 */
uint16_t etx_local_incoming_idr_read(int8_t interface_id, uint8_t attribute_index);

/**
 * \brief A function to read local ETXvalue
 *
 *  Returns local ETX value for an address
 *
 * \param mac64_addr_ptr long MAC address
 *
 * \return 0x0100 to 0xFFFF ETX value (8 bit fraction)
 * \return 0x0000 address unknown
 */
uint16_t etx_local_etx_read(int8_t interface_id, uint8_t attribute_index);

/**
 * \brief A function to update ETX value based on LQI and dBm
 *
 *  Update is made based on dBM and LQI of received message.
 *
 * \param lqi link quality indicator
 * \param dbm measured dBm
 * \param attribute_index Neighbour attribute index
 *
 * \return 0x0100 to 0xFFFF local incoming IDR value (8 bit fraction)
 */
uint16_t etx_lqi_dbm_update(int8_t interface_id, uint8_t lqi, int8_t dbm, uint8_t attribute_index);

/**
 * \brief A function callback that indicates ETX value change
 *
 *  Callback indicates when ETX value has changed more or equal to
 *  hysteresis value.
 *
 * \param nwk_interface_id network interface id
 * \param previous_etx ETX value to what the current ETX was compared (8 bit fraction)
 * \param current_etx current ETX value (8 bit fraction)
 * \param attribute_index Neighbour attribute index
 *
 */
typedef void (etx_value_change_handler_t)(int8_t nwk_id, uint16_t previous_etx, uint16_t current_etx, uint8_t attribute_index);

/**
 * \brief A function callback that indicates the number of accumulated TX failures
 *
 * Callback indicates when number of accumulated failures is more or equal to threshold value.
 *
 * \param interface_id interface ID
 * \param accumulated_failures number of accumulated failures
 * \param attribute_index Neighbour attribute index
 *
 */
typedef void (etx_accum_failures_handler_t)(int8_t interface_id, uint8_t accumulated_failures, uint8_t attribute_index);

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
uint8_t etx_value_change_callback_register(nwk_interface_id nwk_id, int8_t interface_id, uint16_t hysteresis, etx_value_change_handler_t *callback_ptr);

/**
 * \brief A function to allocte ETX storage list
 *
 * \param interface_id interface id
 * \param etx_storage_size Size of storage. 0 will free allocate data
 *
 * \return false Allocate fail
 * \return true Allocate OK
 */
bool etx_storage_list_allocate(int8_t interface_id, uint8_t etx_storage_size);

/**
 * \brief A function to read ETX storage for defined neighbour
 *
 * \param interface_id interface id
 * \param attribute_index Neighbour attribute index
 *
 * \return Pointer to ETX storage
 * \return NULL When unknow interface or attribute
 */
etx_storage_t *etx_storage_entry_get(int8_t interface_id, uint8_t attribute_index);


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
 * \param attribute_index Neighbour attribute index
 *
 */
void etx_neighbor_remove(int8_t interface_id, uint8_t attribute_index);

/**
 * \brief A function to add ETX neighbor
 *
 *  Notifies ETX module that neighbor has been added. Calls ETX value change callback
 *  if that is set.
 *
 * \param attribute_index Neighbour attribute index
 *
 */
void etx_neighbor_add(int8_t interface_id, uint8_t attribute_index);

/**
 * \brief A function for update cached ETX calculation
 *
 *  Shuold be call second intevall
 *
 * \param interface_id Interface ID
 * \param seconds_update Seconds Update
 *
 */
void etx_cache_timer(int8_t interface_id, uint16_t seconds_update);

/**
 * \brief A function for enable cached ETX mode and parametrs
 *
 *  Default values for enabled Cached mode is wait time 60 seconds, etx_max_update is 0 (disabled) and etx_min_sample_count is 4.
 *  ETX update will happen when min wait time is reached and also reached min etx sample count.
 *
 * \param min_wait_time how many seconds must wait before do new ETX
 * \param etx_min_sample_count define how many completed TX process must be done for new ETX. Min accepted value is 4.
 *
 * \return true Enable is OK
 * \return false Memory allocation fail
 *
 */
bool etx_cached_etx_parameter_set(uint8_t min_wait_time, uint8_t etx_min_sample_count);


/**
 * \brief A function for set Maxium ETX update
 *
 * ETX RFC define that that Max value for update is 0xffff but this API cuold make that Poor link start go down slowly.
 *
 * \param etx_max_update 0 No limit for Update higher value means. This pameter will change normal ETX which could be 0xffff.
 *
 */
void etx_max_update_set(uint16_t etx_max_update);

#endif /* ETX_H_ */
