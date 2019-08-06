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

#ifndef RPL_DOWNWARD_H_
#define RPL_DOWNWARD_H_

struct rpl_instance;
struct rpl_dodag;
struct rpl_neighbour;
struct rpl_dao_target;
struct rpl_dao_root_transit;

void rpl_downward_dao_slow_timer(struct rpl_instance *instance, uint16_t seconds);
void rpl_downward_dao_timer(struct rpl_instance *instance, uint16_t ticks);
void rpl_downward_print_instance(struct rpl_instance *instance, route_print_fn_t *print_fn);

void rpl_downward_convert_dodag_preferences_to_dao_path_control(struct rpl_dodag *dodag);
void rpl_downward_process_dao_parent_changes(struct rpl_instance *instance);
void rpl_downward_neighbour_gone(struct rpl_instance *instance, struct rpl_neighbour *neighbour);

void rpl_instance_publish_dao_target(struct rpl_instance *instance, const uint8_t *prefix, uint8_t prefix_len, uint32_t valid_lifetime, bool own, bool want_descriptor, uint32_t descriptor);
void rpl_delete_dao_target(struct rpl_instance *instance, struct rpl_dao_target *target);
void rpl_instance_delete_published_dao_target(struct rpl_instance *instance, const uint8_t *prefix, uint8_t prefix_len);
struct rpl_dao_target *rpl_instance_match_dao_target(struct rpl_instance *instance, const uint8_t *prefix, uint8_t prefix_len);

void rpl_instance_dao_request(struct rpl_instance *instance, struct rpl_neighbour *neighbour);
void rpl_instance_dao_trigger(struct rpl_instance *instance, uint16_t delay);
void rpl_instance_dao_acked(struct rpl_instance *instance, const uint8_t src[16], int8_t interface_id, uint8_t dao_sequence, uint8_t status);
void rpl_instance_parent_address_reg_timer_update(struct rpl_instance *instance, uint16_t seconds);
void rpl_instance_send_address_registration(rpl_instance_t *instance, const uint8_t addr[16]);
void rpl_instance_address_registration_done(protocol_interface_info_entry_t *interface, rpl_instance_t *instance, rpl_neighbour_t *neighbour, uint8_t status);
struct rpl_dao_target *rpl_instance_get_active_target_confirmation(struct rpl_instance *instance);

#ifdef HAVE_RPL_DAO_HANDLING
bool rpl_instance_dao_received(struct rpl_instance *instance, const uint8_t src[16], int8_t interface_id, bool multicast, const uint8_t *opts, uint16_t opts_len, uint8_t *status_out);
#endif

#ifdef HAVE_RPL_ROOT
void rpl_downward_transit_error(rpl_instance_t *instance, const uint8_t *target_addr, const uint8_t *transit_addr);
void rpl_downward_compute_paths(rpl_instance_t *instance);
void rpl_downward_paths_invalidate(rpl_instance_t *instance);
#else
#define rpl_downward_compute_paths(instance) ((void) 0)
#define rpl_downward_paths_invalidate(instance) ((void) 0)
#endif

#endif /* RPL_DOWNWARD_H_ */
