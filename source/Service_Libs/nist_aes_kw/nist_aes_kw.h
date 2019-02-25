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

#ifndef NIST_AES_KW_
#define NIST_AES_KW_

/**
 * \brief NIST AES Key Wrap encode
 *
 *  NIST AES Key Wrap encode
 *
 * \param key pointer to key
 * \param key_bits key length in bits
 * \param input pointer to input data
 * \param input_len input data length
 * \param output pointer to output data, storage must be at least 8 bytes longer than input_len
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t nist_aes_key_wrap(uint8_t is_wrap, const uint8_t *key, int16_t key_bits, const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len);

#endif /* NIST_AES_KW_ */
