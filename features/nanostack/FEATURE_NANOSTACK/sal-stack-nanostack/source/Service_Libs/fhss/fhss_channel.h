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
#ifndef FHSS_CHANNEL_H_
#define FHSS_CHANNEL_H_

bool fhss_change_to_next_channel(fhss_structure_t *fhss_structure);
uint8_t fhss_get_offset(fhss_structure_t *fhss_structure, const uint8_t *ptr);
bool fhss_is_current_channel_broadcast(fhss_structure_t *fhss_structure);
int fhss_change_to_tx_channel(fhss_structure_t *fhss_structure, uint8_t *destination_address);
int fhss_change_to_parent_channel(fhss_structure_t *fhss_structure);
int fhss_change_to_rx_channel(fhss_structure_t *fhss_structure);

#endif /*FHSS_CHANNEL_H_*/
