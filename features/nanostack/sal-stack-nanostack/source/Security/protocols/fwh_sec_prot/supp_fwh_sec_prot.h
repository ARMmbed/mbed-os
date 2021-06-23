/*
 * Copyright (c) 2016-2020, Pelion and affiliates.
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

#endif /* SUPP_FWH_SEC_PROT_H_ */
