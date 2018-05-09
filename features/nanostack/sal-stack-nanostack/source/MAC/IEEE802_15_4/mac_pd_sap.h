/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
 * \file mac_pd_sap.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef MAC_PD_SAP_H_
#define MAC_PD_SAP_H_

struct protocol_interface_rf_mac_setup;
struct arm_phy_sap_msg_s;


/**
 * Send a buffer to the MAC.
 * Used by the protocol core.
 *
 * \param buf pointer to buffer
 *
 */
int8_t mac_pd_sap_req(struct protocol_interface_rf_mac_setup *rf_mac_setup);

void mac_pd_sap_rf_low_level_function_set(void *mac_ptr, void *driver);

int8_t mac_pd_sap_data_cb(void *identifier, struct arm_phy_sap_msg_s *message);
/**
 * Run Mac data interface state Machine.
 *
 */
void mac_pd_sap_state_machine(struct protocol_interface_rf_mac_setup *rf_mac_setup);

#endif /* MAC_PD_SAP_H_ */
