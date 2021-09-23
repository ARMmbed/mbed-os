/*
 * Copyright (c) 2015, 2017, Pelion and affiliates.
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
 * \file border_router.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef BORDER_ROUTER_H_
#define BORDER_ROUTER_H_



#ifdef HAVE_6LOWPAN_BORDER_ROUTER
int8_t arm_nwk_6lowpan_borderrouter_init(struct protocol_interface_info_entry *cur);
void arm_nwk_6lowpan_borderrouter_data_free(struct protocol_interface_info_entry *cur);
void arm_border_router_ready(protocol_interface_info_entry_t *cur);
void nd_border_router_setup_refresh(nwk_interface_id id, bool fresh_abro);
void border_router_start(protocol_interface_info_entry_t *cur, bool warm_link_restart);
#else
#define arm_nwk_6lowpan_borderrouter_data_free(cur) ((void)0)
#define arm_nwk_6lowpan_borderrouter_init(cur) -1
#define nd_border_router_setup_refresh(id, fresh_abro) ((void)0)
#define arm_border_router_ready(cur) ((void)0)
#define border_router_start(cur, warm_link_restart) ((void)0)
#endif
#endif /* BORDER_ROUTER_H_ */
