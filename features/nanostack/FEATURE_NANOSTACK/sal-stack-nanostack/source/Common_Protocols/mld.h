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

#ifndef MLD_H_
#define MLD_H_

/* MLDv2 Record Types */
#define MLD_MODE_IS_INCLUDE         1
#define MLD_MODE_IS_EXCLUDE         2
#define MLD_CHANGE_TO_INCLUDE_MODE  3
#define MLD_CHANGE_TO_EXCLUDE_MODE  4
#define MLD_ALLOW_NEW_SOURCES       5
#define MLD_BLOCK_OLD_SOURCES       6

struct protocol_interface_info_entry;
struct if_group_entry;
struct buffer;

struct buffer *mld_build(struct protocol_interface_info_entry *cur, uint8_t type, uint16_t max_response_delay, const uint8_t address[static 16]);

struct buffer *mld_report_handler(struct buffer *buf, struct protocol_interface_info_entry *cur);
struct buffer *mld_query_handler(struct buffer *buf, struct protocol_interface_info_entry *cur);

void mld_start_listening(struct protocol_interface_info_entry *interface, struct if_group_entry *entry);
void mld_stop_listening(struct protocol_interface_info_entry *interface, struct if_group_entry *entry);

void mld_slow_timer(struct protocol_interface_info_entry *interface, uint_fast16_t seconds);
void mld_fast_timer(struct protocol_interface_info_entry *interface, uint_fast16_t ticks);

#endif /* MLD_H_ */
