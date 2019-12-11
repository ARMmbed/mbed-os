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

#ifndef KEY_SEC_PROT_H_
#define KEY_SEC_PROT_H_

/*
 * EAPOL-Key security protocol. Protocol is used for sending and receiving
 * initial EAPOL-Key message that is used to start the supplicant
 * authentication. Specified in Wi-SUN FANWG-FANTPS.
 *
 */

/**
 * supp_key_sec_prot_register register supplicant EAPOL-Key protocol to KMP service
 *
 * \param service KMP service
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t supp_key_sec_prot_register(kmp_service_t *service);

/**
 * auth_key_sec_prot_register register authenticator EAPOL-Key protocol to KMP service
 *
 * \param service KMP service
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t auth_key_sec_prot_register(kmp_service_t *service);

#endif /* KEY_SEC_PROT_H_ */
