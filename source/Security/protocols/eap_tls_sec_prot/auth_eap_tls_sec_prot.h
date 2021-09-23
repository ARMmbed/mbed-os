/*
 * Copyright (c) 2019, Pelion and affiliates.
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

#ifndef AUTH_EAP_TLS_SEC_PROT_H_
#define AUTH_EAP_TLS_SEC_PROT_H_

/*
 * Authenticator EAP-TLS security protocol. Specified in RFC 5216.
 *
 */

/**
 * auth_eap_tls_sec_prot_register register authenticator EAP-TLS protocol to KMP service
 *
 * \param service KMP service
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t auth_eap_tls_sec_prot_register(kmp_service_t *service);

/**
 * auth_eap_tls_sec_prot_timing_adjust Adjust retries and timings of the EAP-TLS protocol
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
int8_t auth_eap_tls_sec_prot_timing_adjust(uint8_t timing);

#endif /* AUTH_EAP_TLS_SEC_PROT_H_ */
