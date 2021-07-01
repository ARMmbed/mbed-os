/*
 * Copyright (c) 2020, Pelion and affiliates.
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

#ifndef RADIUS_CLIENT_SEC_PROT_H_
#define RADIUS_CLIENT_SEC_PROT_H_

/*
 * RADIUS client security protocol
 *
 */

/**
 * radius_client_sec_prot_register register RADIUS client protocol to KMP service
 *
 * \param service KMP service
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t radius_client_sec_prot_register(kmp_service_t *service);

#endif /* RADIUS_CLIENT_SEC_PROT_H_ */
