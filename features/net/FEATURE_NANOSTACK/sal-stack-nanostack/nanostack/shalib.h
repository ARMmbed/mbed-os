/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */
/**
 * \file shalib.h
 * \brief SHA256 Library API.
 *
 *  \section sha256-api SHA256 Library API:
 *   There are two ways to calculate SHA256:
 *   1. Calc by given 1 data and length pointer
 *      - SHALIB_SHA256_HASH(), A function to calculate SHA256 for given data.
 *   2. Calc by different data pointer sequence given separately
 *      - SHALIB_init_sha256(), Init SHA registers
 *      - SHALIB_push_data_sha256(), Give data sectors(s) one by one
 *      - **** Give data
 *      - SHALIB_push_data_sha256(), Give last data sequence
 *      - SHALIB_finish_sha256(), Finish SHA256 by given data to given buffer
 *
 *   \section sha256res-api SHA256 register resume and save library API:
 *   SHA256 Operation dataflow can come in many different timeslots or packets and between them, the application needs
 *   to calculated more SHA256 then SAVE and Resume operation SHA registers is usefully.
 *   -sha_resume_regs(), Load SHA registers from old HASH sessions
 *   -sha_save_regs(), Save SHA registers from current HASH sessions
 *
 *  \section hmac256-inctuction HMAC256 process sequence:
 *  1. SHALIB_init_HMAC(), Init HMAC IN process by given security signature material
 *  2. SHALIB_push_data_sha256(), Give data sectors(s) one by one
 *  3. SHALIB_finish_HMAC(), Finish HMAC and save SHA256 hash to given buffer
 *
 *  \section prf256-inctuction PRF256 process sequence:
 *  1. shalib_prf_param_get(), Init PRF and get configure structure
 *  2. Set the following parameters to configure structure:
 *      - HMAC security signature pointer and length
 *      - Label text and length
 *      - Seed data and length
 *      - PRF result pointer
 *  3. shalib_prf_calc(), Calc PRF256 HASH
 *
 */

#ifndef SHALIB_H_
#define SHALIB_H_

#include "ns_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/** Do Not change. */
#define SHALIB_RING_BUFFER_SIZE 64

/*!
 * \struct prf_sec_param_t
 * \brief PRF 256 stucture
 * This structure is used to configure PRF calc operation: secret, label, seed and buffer before call shalib_prf_calc().
 */
typedef struct {
    const uint8_t *secret;  /**< HMAC security signature pointer. */
    uint8_t sec_len;        /**< HMAC security signature length. */
    uint8_t label[25];      /**< Label text. */
    uint8_t lablen;         /**< Label text length. */
    const uint8_t *seed;    /**< Seed data. */
    uint8_t seedlen;        /**< Seed data length. */
    uint8_t *buffer;        /**< Buffer for saving 256-BIT hash. */
} prf_sec_param_t;

/*!
 * \struct sha256_temp_t
 * \brief SHA temporary buffer database for saving current hash operation or resume saved.
 */
typedef struct {
    uint8_t  m_Data[SHALIB_RING_BUFFER_SIZE];   /**< 64-bytes ring buffer for SHA256 operation. */
    uint8_t  m_Read;                            /**< Read pointer to ring buffer. */
    uint8_t  m_Write;                           /**< Write pointer to ring buffer. */
    uint32_t SHALIB_pushed_bytes;           /**< Hash total byte coun. t*/
    uint8_t SHALIB_buffered_bytes;          /**< Ring buffer data in. */
    uint32_t areg_temps[8];                 /**< Shalib operation 8 HREG. */
} sha256_temp_t;

// Cumulative static version using a static ring buffer object
//=============================================================================
/**
 * \brief Init SHA registers.
 */
void SHALIB_init_sha256(void);                                  // Call this first...
/**
 * \brief Push data SHALIB.
 *
 * \param data A pointer to data.
 * \param len Length of data.
 */
void SHALIB_push_data_sha256(const uint8_t *data, uint16_t len);      // ... add data ...
/**
 * \brief Finish SHA-256 operation and get result to given buffer by given length.
 *
 * The first `len` words of the SHA-256 are output to buffer.
 *
 * \param buffer A pointer to result buffer.
 * \param len Length of 32-bit register to save to buffer (8= 256 bit and 4= 128-bit).
 */
void SHALIB_finish_sha256(uint8_t *buffer, uint8_t len);        // ... get the sha256 digest.
/**
 * \brief Calc SHA-256 by 1 function call.
 *
 * \param data_ptr A pointer to data.
 * \param data_len Length of data.
 * \param buffer A pointer to 256-bit buffer!!
 */
void SHALIB_SHA256_HASH(const uint8_t *data_ptr, uint16_t data_len, uint8_t *buffer); // ... get the sha256 digest.

/* Shalib registers resume and save API */
/**
 * \brief Resume old SHA-256 registers.
 *
 * \param ptr A pointer to saved session.
 */
void sha_resume_regs(const sha256_temp_t *ptr);
/**
 * \brief Save SHA-256 registers.
 *
 * \param ptr A pointer to buffer.
 */
void sha_save_regs(sha256_temp_t *ptr);

// Use these for cumulativec HMAC
/**
 * \brief Init HMAC256 operation by given security material.
 *
 * \param secret A pointer to security material.
 * \param sec_len Length of security material.
 */
void SHALIB_init_HMAC(const uint8_t *secret, uint8_t sec_len);        // Call this first...
// ... add data ... by SHALIB_push_data_sha256()
/**
 * \brief Finish HMAC256 operation and save result in given buffer.
 *
 * \param buffer A pointer to result buffer.
 * \param len Length of 32-bit register to save to buffer (8= 256 bit and 4= 128-bit).
 */
void SHALIB_finish_HMAC(uint8_t *buffer, uint8_t len); // ... get the HMAC digest.


/** PRF API */
/**
 * \brief Init PRF library and SHA registers.
 * This function returns configure structure where the user needs to set the following items:
 *  -Security material and length
 *  -Label text and length
 *  -Seed data and length
 *  -Buffer for 256 Result
 *
 * \return A pointer to PRF configure structure.

 */
prf_sec_param_t *shalib_prf_param_get(void);  // GET PRF structure
/* SET secret, label, seed & buffer to 256 PRF  */
/**
 * \brief Finish PRF256 operation and save result in given buffer.
 *
 */
void shalib_prf_calc(void);// GET 256 PRF
#ifdef __cplusplus
}
#endif
#endif /* SHALIB_H_ */
