/*
 * Copyright (c) 2014-2017, 2020, Pelion and affiliates.
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
 * \file mac_filter.h
 * \brief Private functions for filter API.
 *
 */

#ifndef MAC_FILTER_H_
#define MAC_FILTER_H_

struct mac_fcf_sequence_s;
struct arm_pd_sap_generic_ind_s;

/**
 * Modify the link quality values.
 * This function modifies the link qualities based on the filter settings. This is used from the MAC layer
 *
 * if lqi result is less than 0 this packet is dropped and function returns 1
 * if dbm result is less than -90dbm this packet is dropped and function returns 1
 * if packet can be prosessed function returns 0 with modified link quality values.
 *
 * \param interface_id Network Interface.
 * \param mac_frame pointer to pre parsed mac frame
 *
 * return modified link quality.
 * return >0 Packet is ignored.
 * return 0 Packet is not dropped.
 */
int_fast8_t mac_filter_modify_link_quality(int8_t interface_id, struct mac_fcf_sequence_s *fcf, struct arm_pd_sap_generic_ind_s *mac_frame);

#endif /* MAC_FILTER_H_ */
