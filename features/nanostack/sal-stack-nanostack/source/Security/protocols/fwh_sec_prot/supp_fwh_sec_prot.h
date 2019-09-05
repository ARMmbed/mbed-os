/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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

#ifndef SUPP_FWH_SEC_PROT_H_
#define SUPP_FWH_SEC_PROT_H_

/*
 * Supplicant Four Way Handshake (4WH) security protocol. 4WH protocol is
 * specified in IEEE 802.11 and Wi-SUN FANWG-FANTPS.
 *
 */

/**
 * supp_fwh_sec_prot_register register supplicant 4WH protocol to KMP service
 *
 * \param service KMP service
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t supp_fwh_sec_prot_register(kmp_service_t *service);

/**
 * supp_fwh_sec_prot_timing_adjust Adjust retries and timings of the 4WH protocol
 *
 * Timing value is a generic number between 0 to 32 that goes from fast and
 * reactive network to low bandwidth and long latency.
 *
 * example value definitions:
 * 0-8 very fast network
 * 9-16 medium network
 * 16-24 slow network
 * 25-32 extremely slow network
 *
 * There is no need to have lots variations in every layer if protocol is not very active in any case.
 *
 * \param timing Timing value.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t supp_fwh_sec_prot_timing_adjust(uint8_t timing);

#endif /* SUPP_FWH_SEC_PROT_H_ */
