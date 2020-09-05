/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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

#ifndef HMAC_SHA1_
#define HMAC_SHA1_

/**
 * \brief Calculate HMAC-SHA1-160
 *
 *  Calculate HMAC-SHA1-160
 *
 * \param key pointer to key
 * \param key_len key length
 * \param data pointer to data
 * \param data_len data length
 * \param result pointer to result
 * \param result_len in bytes, maximum is 20 bytes
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t hmac_sha1_calc(const uint8_t *key, uint16_t key_len, const uint8_t *data, uint16_t data_len, uint8_t *result, uint8_t result_len);

#endif /* HMAC_SHA1_ */
