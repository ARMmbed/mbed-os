/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#ifndef BEACON_HANDLER_H
#define BEACON_HANDLER_H

#include <inttypes.h>
#include "net_interface.h"

// 1 byte for protocol id, 1 byte for accept join, 16 bytes for network id
#define PLAIN_BEACON_PAYLOAD_SIZE   (1 + 1 + sizeof((border_router_setup_s *)0)->network_id)

#define BEACON_OPTION_END_DELIMITER            0x00
#define BEACON_OPTION_JOIN_PRIORITY_TYPE       0x0c
#define BEACON_OPTION_JOIN_PRIORITY_VAL_LEN    0x01
#define BEACON_OPTION_JOIN_PRIORITY_LEN        0x02
#define BEACON_OPTION_JOIN_PRIORITY_TYPE_LEN   0xc1

struct mlme_beacon_ind_s;

void beacon_received(int8_t if_id, const struct mlme_beacon_ind_s *data);

void beacon_optional_tlv_fields_skip(uint16_t *len, uint8_t **ptr, uint8_t offset);

/* Beacon */
int8_t mac_beacon_link_beacon_compare_rx_callback_set(int8_t interface_id,
    beacon_compare_rx_cb *beacon_compare_rx_cb_ptr);
int8_t mac_beacon_link_beacon_join_priority_tx_callback_set(int8_t interface_id,
    beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr);

void beacon_join_priority_update(int8_t interface_id);

//TODO: beacon storage here if needed by 6loWPAN?

#endif // BEACON_HANDLER_H
