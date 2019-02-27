/*
 * Copyright (c) 2017-2018, Arm Limited and affiliates.
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

#ifndef TLS_CCM_CRYPT_H_
#define TLS_CCM_CRYPT_H_

int8_t tls_ccm_data_encrypt(uint8_t *data_ptr, uint16_t data_length, const uint8_t *key_expansion, const uint8_t *nonce, uint8_t type, bool server);
int8_t tls_ccm_data_decrypt(uint8_t *data_ptr, uint16_t data_length, const uint8_t *key_expansion, uint8_t type, bool server);

#endif /* TLS_CCM_CRYPT_H_ */
