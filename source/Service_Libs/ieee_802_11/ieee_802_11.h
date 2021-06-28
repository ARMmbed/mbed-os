/*
 * Copyright (c) 2016-2018, Pelion and affiliates.
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

#ifndef IEEE_802_11_
#define IEEE_802_11_

typedef struct {
    const uint8_t *key;   /**< Key string */
    uint8_t *string;      /**< Data string (A string + Y + B string + X) */
    uint16_t bits;        /**< Number of PRF bits */
    uint16_t key_len;     /**< Key string length */
    uint16_t a_len;       /**< A string length */
    uint16_t b_len;       /**< B string length */
} ieee_802_11_prf_t;

/*
    From IEEE 802.11 chapter 11.6.1.2 PRF:

    In the following, K is a key; A is a unique label for each different purpose
    of the PRF; B is a variable-length string; Y is a single octet containing 0;
    X is a single octet containing the loop parameter i; and || denotes concatenation:

    H-SHA-1(K, A, B, X) ← HMAC-SHA-1(K, A || Y || B || X)

    PRF(K, A, B, Len)
        for i ← 0 to (Len+159)/160 do
            R ← R || H-SHA-1(K, A, B, i)
        return L(R, 0, Len)

    PRF-128(K, A, B) = PRF(K, A, B, 128)
    PRF-192(K, A, B) = PRF(K, A, B, 192)
    PRF-256(K, A, B) = PRF(K, A, B, 256)
    PRF-384(K, A, B) = PRF(K, A, B, 384)
    PRF-512(K, A, B) = PRF(K, A, B, 512)
*/

/**
 * \brief Setup IEEE 802.11 PRF
 *
 *  Update is made based on failed and successful message sending
 *  attempts for a message.
 *
 * \param prf pointer to PRF data
 * \param bits number of bits
 * \param a_len A string length
 * \param b_len B string length
 *
 * \return length of the combined string that is input parameter for further functions
 *         (this is length of the A string + Y + B string + X)
 */
uint16_t ieee_802_11_prf_setup(ieee_802_11_prf_t *prf, uint16_t bits, uint16_t a_len, uint16_t b_len);

/**
 * \brief Get A string pointer
 *
 *  Get a pointer to A string start that is used to write A string contents
 *
 * \param prf pointer to PRF data
 * \param string pointer to input string
 *
 * \return pointer to A string part of the input string
 */
uint8_t *ieee_802_11_prf_get_a_string(ieee_802_11_prf_t *prf, uint8_t *string);

/**
 * \brief Get B string pointer
 *
 *  Get a pointer to B string start that is used to write B string contents
 *
 * \param prf pointer to PRF data
 * \param string pointer to input string
 *
 * \return pointer to B string part of the input string
 */
uint8_t *ieee_802_11_prf_get_b_string(ieee_802_11_prf_t *prf, uint8_t *string);

/**
 * \brief Start PRF process
 *
 *  Start PRF process
 *
 * \param prf pointer to PRF data
 * \param key key
 * \param key_len key length
 *
 * \return length of the return string
 */
uint16_t ieee_802_11_prf_starts(ieee_802_11_prf_t *prf, const uint8_t *key, uint16_t key_len);

/**
 * \brief Update PRF process
 *
 *  Update PRF process
 *
 * \param prf pointer to PRF data
 * \param string pointer to input string
 *
 */
void ieee_802_11_prf_update(ieee_802_11_prf_t *prf, uint8_t *string);

/**
 * \brief Finish PRF process
 *
 *  Finish PRF process
 *
 * \param prf pointer to PRF data
 * \param result pointer to result string
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ieee_802_11_prf_finish(ieee_802_11_prf_t *prf, uint8_t *result);

#endif /* IEEE_802_11_*/
