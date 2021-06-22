/*
 * Copyright (c) 2014-2017, Pelion and affiliates.
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

#ifndef MAC_TIMER_H_
#define MAC_TIMER_H_

#include "MAC/IEEE802_15_4/mac_defines.h"

extern void timer_mac_start(struct protocol_interface_rf_mac_setup *rf_ptr, mac_event_t event, uint16_t slots);

extern void timer_mac_stop(struct protocol_interface_rf_mac_setup *rf_ptr);

void timer_mac_interrupt(int8_t timer_id, uint16_t slots);

#endif /* MAC_TIMER_H_ */
