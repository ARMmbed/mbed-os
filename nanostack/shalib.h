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
/**
 * \file shalib.h
 * \brief SHA256 Library API.
 *
 *  \section hmac256-inctuction HMAC256 process sequence:
 *  1. SHALIB_init_HMAC(), Init HMAC IN process by given security signature material
 *  2. SHALIB_push_data_HMAC(), Give data sectors(s) one by one
 *  3. SHALIB_finish_HMAC(), Finish HMAC and save SHA256 hash to given buffer
 *
 *  \section prf256-inctuction PRF256 process sequence:
 *  1. shalib_prf_param_get(), Init PRF and get configure structure
 *  2. Set the following parameters to configure structure:
 *      - HMAC security signature pointer and length
 *      - Label text
 *      - Seed data and length
 *  3. shalib_prf_calc(), Calc PRF256 HASH
 *
 */

#ifndef SHALIB_H_
#define SHALIB_H_

#include "ns_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct prf_sec_param_t
 * \brief PRF 256 stucture
 * This structure is used to configure PRF calc operation: secret, label, seed and buffer before call shalib_prf_calc().
 */
typedef struct {
    const uint8_t *secret;  /**< HMAC security signature pointer. */
    uint8_t sec_len;        /**< HMAC security signature length. */
    const char *label;      /**< PRF label text. */
    const uint8_t *seed;    /**< PRF Seed data. */
    uint8_t seedlen;        /**< PRF Seed data length. */
} prf_sec_param_t;


// Use these for cumulative HMAC
/**
 * \brief Init HMAC256 operation by given security material.
 *
 * \param secret A pointer to security material.
 * \param sec_len Length of security material.
 */
void SHALIB_init_HMAC(const uint8_t *secret, uint8_t sec_len);        // Call this first...
/**
 * \brief Push data for HMAC
 *
 * \param data A pointer to data.
 * \param len Length of data.
 */
void SHALIB_push_data_HMAC(const void *data, uint16_t len);           // ... add data ...
/**
 * \brief Finish HMAC256 operation and save result in given buffer.
 *
 * \param buffer A pointer to result buffer.
 * \param nwords Length of 32-bit register to save to buffer (8= 256 bit and 4= 128-bit).
 */
void SHALIB_finish_HMAC(void *buffer, uint8_t nwords);                // ... get the HMAC digest.


/** PRF API */
/**
 * \brief Init PRF library and SHA registers.
 * This function returns configure structure where the user needs to set the following items:
 *  -Security material and length
 *  -Label text and length
 *  -Seed data and length
 *
 * \return A pointer to PRF configure structure.

 */
prf_sec_param_t *shalib_prf_param_get(void);  // GET PRF structure
/* SET secret, label, seed & buffer to 256 PRF  */
/**
 * \brief Finish PRF256 operation and save result in given buffer.
 */
void shalib_prf_calc(void *output, uint_fast16_t nwords);// GET 256 PRF
#ifdef __cplusplus
}
#endif
#endif /* SHALIB_H_ */
