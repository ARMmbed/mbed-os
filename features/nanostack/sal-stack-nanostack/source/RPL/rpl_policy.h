/*
 * Copyright (c) 2015-2017, 2019, Arm Limited and affiliates.
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

#ifndef RPL_POLICY_H_
#define RPL_POLICY_H_

#include "Core/include/ns_address_internal.h"
#include "rpl_control.h"

bool rpl_policy_join_instance(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid);
bool rpl_policy_join_dodag(rpl_domain_t *domain, uint8_t g_mop_prf, uint8_t instance_id, const uint8_t *dodagid);
bool rpl_policy_join_config(rpl_domain_t *domain, const rpl_dodag_conf_t *conf, bool *leaf_only);

bool rpl_policy_request_dao_acks(const rpl_domain_t *domain, uint8_t mop);
uint16_t rpl_policy_initial_dao_ack_wait(const rpl_domain_t *domain, uint8_t mop);
void rpl_policy_set_initial_dao_ack_wait(uint16_t timeout_in_ms);

void rpl_policy_set_dao_retry_count(uint8_t count);
int8_t rpl_policy_dao_retry_count();

int8_t rpl_policy_srh_next_hop_interface(rpl_domain_t *domain, int8_t if_id, const uint8_t *next_hop);
uint16_t rpl_policy_modify_downward_cost_to_root_neighbour(rpl_domain_t *domain, int8_t if_id, const uint8_t *next_hop, uint16_t cost);

uint16_t rpl_policy_parent_selection_period(rpl_domain_t *domain);
uint16_t rpl_policy_etx_hysteresis(rpl_domain_t *domain);
//Return Fixed point multiple which base 1.0 is 0x0100
uint16_t rpl_policy_dio_validity_period(rpl_domain_t *domain);
//Fixed point must 1.0 is 0x0100
void rpl_policy_set_dio_validity_period(rpl_domain_t *domain, uint16_t fixed_point);
uint16_t rpl_policy_etx_change_parent_selection_delay(rpl_domain_t *domain);
uint16_t rpl_policy_dio_parent_selection_delay(rpl_domain_t *domain);

uint16_t rpl_policy_repair_initial_dis_delay(rpl_domain_t *domain);
uint16_t rpl_policy_repair_maximum_dis_interval(rpl_domain_t *domain);
uint_fast8_t rpl_policy_repair_dis_count(rpl_domain_t *domain);
uint_fast8_t rpl_policy_repair_poison_count(rpl_domain_t *domain);

bool rpl_policy_dao_trigger_after_srh_error(rpl_domain_t *domain, uint32_t seconds_since_last_dao_trigger, uint16_t errors_since_last_dao_trigger, uint_fast16_t targets);

bool rpl_policy_target_descriptor_for_own_address(rpl_domain_t *domain, const uint8_t addr[16], if_address_source_t source, void *data, uint32_t *descriptor_out);
bool rpl_policy_target_descriptor_for_host_address(rpl_domain_t *domain, const uint8_t addr[16], uint32_t *descriptor_out);

uint16_t rpl_policy_of0_stretch_of_rank(const rpl_domain_t *domain);
uint_fast8_t rpl_policy_of0_rank_factor(const rpl_domain_t *domain);
bool rpl_policy_of0_dodag_preference_supersedes_grounded(const rpl_domain_t *domain);
uint_fast8_t rpl_policy_of0_max_backup_successors(const rpl_domain_t *domain);

uint_fast8_t rpl_policy_mrhof_parent_set_size(const rpl_domain_t *domain);
uint16_t rpl_policy_mrhof_max_link_metric(const rpl_domain_t *domain);
uint16_t rpl_policy_mrhof_parent_switch_threshold(const rpl_domain_t *domain);
uint16_t rpl_policy_mrhof_max_rank_stretch_for_extra_parents(const rpl_domain_t *domain);
bool rpl_policy_parent_confirmation_requested(void);
void rpl_policy_set_parent_confirmation_request(bool confirmation_requested);
uint8_t rpl_policy_dio_multicast_config_advertisment_min_count(void);
void rpl_policy_set_dio_multicast_config_advertisment_min_count(uint8_t min_count);

#endif /* RPL_POLICY_H_ */
