/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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
 * \file protocol_6lowpan_bootstrap.h
 *
 */

#ifndef PROTOCOL_6LOWPAN_BOOTSTRAP_H_
#define PROTOCOL_6LOWPAN_BOOTSTRAP_H_
#include "net_interface.h"
#include "Service_Libs/mle_service/mle_service_api.h"

struct protocol_interface_info_entry;
struct nd_router;

#define MLE_NEIGHBOR_PURGE_NBR                3
#define MLE_NEIGHBOR_PURGE_TIMER_TIMEOUT      4       // Times advertisement timeout

#define MLE_ROUTER_DEFAULT_LIFETIME           128
#define MLE_ROUTER_HOST_LIFETIME              400

#define MLE_NBR_OF_NEIGHBOR_MAX_LIMIT         0
#define MLE_NBR_OF_NEIGHBOR_LOWER_THRESHOLD   0
#define MLE_NBR_OF_NEIGHBOR_UPPER_THRESHOLD   0
#define MLE_TOKEN_BUCKET_SIZE                 0
#define MLE_TOKEN_BUCKET_RATE                 0
#define MLE_TOKEN_BUCKET_COUNT                0

#define MLE_LINK_REQ_TOKEN_BUCKET_SIZE        3

#define LOWPAN_MAX_FRAME_RETRIES 4

// Waiting Scan confirm from MAC (ms)
#define BOOTSTRAP_SCAN_TIMEOUT  30000
// Waiting Start confirm from MAC (ms)
#define BOOTSTRAP_START_TIMEOUT  10000

typedef struct mle_6lowpan_data {
    uint16_t router_lifetime;               // default lifetime for router neighbor
    uint16_t host_lifetime;                 // default lifetime for host neighbor

    uint16_t nbr_of_neigh_lower_threshold;  // start to limit addition of neighbors to neighbor list
    uint16_t nbr_of_neigh_upper_threshold;  // only priority neighbors are added to neighbor list
    uint16_t nbr_of_neigh_max;              // maximum number of neighbors in neighbor list

    uint8_t token_bucket_size;              // MLE token bucket size
    uint8_t token_bucket_rate;              // Rate in which tokens are added to bucket (0.1 seconds * rate)
    uint8_t token_bucket_count;             // Count of tokens added to bucket at a time

    uint8_t link_req_token_bucket;          // Token bucket for MLE link request with non-valid security counter
} mle_6lowpan_data_t;

#ifdef PANA
extern void nwk_6lowpan_bootsrap_pana_authentication_cb(bool processSuccesfully, struct protocol_interface_info_entry *cur);
#endif
#ifdef HAVE_6LOWPAN_ND
uint8_t *protocol_6lowpan_mle_service_security_notify_cb(int8_t interface_id, mle_security_event_t event, uint8_t keyId);
void arm_6lowpan_bootstrap_init(struct protocol_interface_info_entry *cur);
uint8_t *protocol_6lowpan_nd_border_router_address_get(nwk_interface_id nwk_id);
uint8_t protocol_6lowpan_rf_link_scalability_from_lqi(uint8_t lqi);
void protocol_6lowpan_bootstrap_re_start(struct protocol_interface_info_entry *interface);
void protocol_6lowpan_link_advertise_handle(struct nd_router *cur, struct protocol_interface_info_entry *cur_interface, uint16_t tick);
void protocol_6lowpan_bootstrap_nd_ready(struct protocol_interface_info_entry *cur_interface);
void protocol_6lowpan_nd_borderrouter_connection_down(struct protocol_interface_info_entry *interface);
int8_t arm_6lowpan_mle_service_ready_for_security_init(protocol_interface_info_entry_t *cur);
int protocol_6lowpan_router_synch_to_new_router(struct protocol_interface_info_entry *cur, uint8_t *ll64, uint8_t incoming_idr, bool retrans);
void protocol_6lowpan_bootstrap(struct protocol_interface_info_entry *cur);
int protocol_6lowpan_del_ll16(struct protocol_interface_info_entry *cur, uint16_t mac_short_address);
int protocol_6lowpan_set_ll16(struct protocol_interface_info_entry *cur, uint16_t mac_short_address);
int8_t arm_6lowpan_bootstarp_bootstrap_set(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode, net_6lowpan_mode_extension_e net_6lowpan_mode_extension);
struct mle_6lowpan_data *protocol_6lowpan_mle_data_get(void);
bool lowpan_neighbour_data_clean(int8_t interface_id, const uint8_t *link_local_address);
void arm_6lowpan_security_init_ifup(protocol_interface_info_entry_t *cur);
void protocol_6lowpan_mle_timer(uint16_t ticks_update);
#else
#define protocol_6lowpan_nd_border_router_address_get(nwk_id) NULL
#define arm_6lowpan_mle_service_ready_for_security_init(cur) -1
#define protocol_6lowpan_bootstrap(cur) ((void)0)
#define protocol_6lowpan_del_ll16(cur, mac_short_address) -1
#define protocol_6lowpan_set_ll16(cur, mac_short_address) -1
#define arm_6lowpan_bootstarp_bootstrap_set(interface_id, bootstrap_mode, net_6lowpan_mode_extension) -1
#define protocol_6lowpan_mle_data_get() NULL
#define arm_6lowpan_security_init_ifup(cur) ((void)0)
#define protocol_6lowpan_mle_timer(ticks_update) ((void)0)
#endif
void bootstrap_timer_handle(uint16_t ticks);

#endif /* PROTOCOL_6LOWPAN_BOOTSTRAP_H_ */
