/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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

#ifndef WHITEBOARD_H_
#define WHITEBOARD_H_

#include "whiteboard_api.h"
#ifdef WHITEBOARD
extern void whiteboard_init(int8_t id);
extern void whiteboard_init_by_prefix(int8_t id, const uint8_t address[static 8]);
void whiteboard_rm_entry(int8_t id, const uint8_t address[static 16]);
extern whiteboard_entry_t *whiteboard_table_update(const uint8_t address[static 16], const uint8_t eui64[static 8], uint8_t *status);
extern whiteboard_entry_t *whiteboard_table_check_address(const uint8_t address[static 16]);
extern int8_t whiteboard_interface_unregister_all_address(int8_t nwk_id);
extern int8_t whiteboard_interface_register(const uint8_t address[static 16], int8_t nwk_id);
extern void whiteboard_ttl_update(uint16_t ttl_time);
extern bool whiteboard_interface_address_cmp(const uint8_t address[static 16]);
extern uint16_t whiteboard_size_get(void);
#else
#define whiteboard_init(id)
#define whiteboard_init_by_prefix(id, address)
#define whiteboard_ttl_update(ttl_time)
#define whiteboard_rm_entry(id, address)
#define whiteboard_table_check_address(address) NULL
#define whiteboard_table_update(address, eui64, status) NULL
#define whiteboard_interface_unregister_all_address(nwk_id) -1
#define whiteboard_interface_address_cmp(address) false
#endif

#endif /* WHITEBOARD_H_ */
