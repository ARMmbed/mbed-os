/*
 * Copyright (c) 2015, 2017, Arm Limited and affiliates.
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
 * \file protocol_6lowpan_interface.h
 *
 */

#ifndef PROTOCOL_6LOWPAN_INTERFACE_H_
#define PROTOCOL_6LOWPAN_INTERFACE_H_
/**
 * 6LoWPAN interface activate
 */
extern int8_t nwk_6lowpan_up(struct protocol_interface_info_entry *cur);
/**
 * 6LoWPAN interface deactivate
 */
extern int8_t nwk_6lowpan_down(struct protocol_interface_info_entry *cur);

extern void protocol_mac_reset(struct protocol_interface_info_entry *cur);


#endif /* PROTOCOL_6LOWPAN_INTERFACE_H_ */
